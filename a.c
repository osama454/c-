#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>

#include "e2util.h"

void byteswap_superblock(struct superblock *sb) {
    SWAP_LE(sb->s_inodes_count, 32);
    SWAP_LE(sb->s_blocks_count, 32);
    SWAP_LE(sb->s_first_data_block, 32);
    SWAP_LE(sb->s_log_block_size, 32);
    SWAP_LE(sb->s_blocks_per_group, 32);
    SWAP_LE(sb->s_inodes_per_group, 32);
    SWAP_LE(sb->s_state, 16);
}

void print_superblock(struct superblock *sb) {
    printf("Inodes: %u\n"
           "Blocks: %u\n"
           "First data block: %u\n"
           "Block size: %u\n"
           "Blocks/group: %u\n"
           "Inodes/group: %u\n"
           "State: %s\n",
           sb->s_inodes_count, sb->s_blocks_count,
           sb->s_first_data_block, blocksize(sb),
           sb->s_blocks_per_group, sb->s_inodes_per_group,
           sb->s_state == 1 ? "Clean" : "Dirty");
}

void byteswap_bgdesc(struct bgdesc *bg) {
    SWAP_LE(bg->bg_block_bitmap, 32);
    SWAP_LE(bg->bg_inode_bitmap, 32);
    SWAP_LE(bg->bg_inode_table, 32);
}

void byteswap_inode(struct inode *i) {
    SWAP_LE(i->i_mode, 16);
    SWAP_LE(i->i_uid, 16);
    SWAP_LE(i->i_size, 32);
    SWAP_LE(i->i_atime, 32);
    SWAP_LE(i->i_ctime, 32);
    SWAP_LE(i->i_mtime, 32);
    SWAP_LE(i->i_dtime, 32);
    for (int j = 0; j < 12; j++)
        SWAP_LE(i->i_block_d[j], 32);
    SWAP_LE(i->i_block_1i, 32);
    SWAP_LE(i->i_block_2i, 32);
    SWAP_LE(i->i_block_3i, 32);
}

void print_inode(struct inode *i) {
    time_t t;
    printf("Mode: %o\n"
           "User ID: %u\n"
           "Size: %u\n",
           i->i_mode, i->i_uid, i->i_size);
    t = i->i_atime; printf("Access time: %s", ctime(&t));
    t = i->i_ctime; printf("Change time: %s", ctime(&t));
    t = i->i_mtime; printf("Modification time: %s", ctime(&t));
    t = i->i_dtime; printf("Deletion time: %s", ctime(&t));
    printf("First direct block: %u\n", i->i_block_d[0]);
}

int print_inode_data(struct superblock *sb, struct inode *i) {
    int bs = blocksize(sb);
    char *block = malloc(bs);
    if (!block) return 1;

    for (int j = 0; j < i->i_size / bs; j++) {
        if (get_inode_block(sb, i, j, block) || fwrite(block, bs, 1, stdout) != 1) {
            free(block);
            return 1;
        }
    }
    int remaining = i->i_size % bs;
    if (remaining) {
        if (get_inode_block(sb, i, i->i_size / bs, block) || fwrite(block, remaining, 1, stdout) != 1) {
            free(block);
            return 1;
        }
    }
    free(block);
    return 0;
}


void byteswap_iblock(struct superblock *sb, char *block) {
    for (uint32_t *entry = (uint32_t *)block, *end = entry + blocksize(sb) / 4; entry < end; entry++)
        SWAP_LE(*entry, 32);
}

int blocksize(struct superblock *sb) {
    return 1024 << sb->s_log_block_size;
}

int get_superblock(FILE *f, struct superblock *out) {
    out->file = f;
    fseek(f, 1024, SEEK_SET);
    fread(&out->s_inodes_count, 4, 1, f);
    fread(&out->s_blocks_count, 4, 1, f);
    fseek(f, 4, SEEK_CUR);
    fread(&out->s_first_data_block, 4, 1, f);
    fread(&out->s_log_block_size, 4, 1, f);
    fseek(f, 4, SEEK_CUR);
    fread(&out->s_blocks_per_group, 4, 1, f);
    fseek(f, 4, SEEK_CUR);
    fread(&out->s_inodes_per_group, 4, 1, f);
    fseek(f, 14, SEEK_CUR);
    fread(&out->s_state, 2, 1, f);
    byteswap_superblock(out);
    return 0;
}

int get_block_data(struct superblock *sb, int blk, char *out) {
    fseek(sb->file, (long)blocksize(sb) * blk, SEEK_SET);
    return fread(out, blocksize(sb), 1, sb->file) != 1;
}

int print_block_data(struct superblock *sb, int blk) {
    char out[blocksize(sb)];
    return get_block_data(sb, blk, out) || fwrite(out, blocksize(sb), 1, stdout) != 1;
}

int bg_from_blk(struct superblock *sb, int blk) { return blk / sb->s_blocks_per_group; }
int blk_within_bg(struct superblock *sb, int blk) { return blk % sb->s_blocks_per_group; }
int bg_from_ino(struct superblock *sb, int ino) { return (ino - 1) / sb->s_inodes_per_group; }
int ino_within_bg(struct superblock *sb, int ino) { return (ino - 1) % sb->s_inodes_per_group; }

int get_bgdesc(struct superblock *sb, int bg, struct bgdesc *out) {
    fseek(sb->file, (long)blocksize(sb) * (sb->s_first_data_block + 1) + bg * 32, SEEK_SET);
    fread(&out->bg_block_bitmap, 4, 1, sb->file);
    fread(&out->bg_inode_bitmap, 4, 1, sb->file);
    fread(&out->bg_inode_table, 4, 1, sb->file);
    byteswap_bgdesc(out);
    return 0;
}


int get_inode(struct superblock *sb, int ino, struct inode *out) {
    struct bgdesc bgd;
    get_bgdesc(sb, bg_from_ino(sb, ino), &bgd);
    long offset = (long)blocksize(sb) * bgd.bg_inode_table + (long)ino_within_bg(sb, ino) * 128;

    fseek(sb->file, offset, SEEK_SET);
    fread(&out->i_mode, 2, 1, sb->file);
    fread(&out->i_uid, 2, 1, sb->file);
    fread(&out->i_size, 4, 1, sb->file);
    fread(&out->i_atime, 4, 1, sb->file);
    fread(&out->i_ctime, 4, 1, sb->file);
    fread(&out->i_mtime, 4, 1, sb->file);
    fread(&out->i_dtime, 4, 1, sb->file);
    fseek(sb->file, 16, SEEK_CUR);
    for (int i = 0; i < 12; i++) fread(&out->i_block_d[i], 4, 1, sb->file);
    fread(&out->i_block_1i, 4, 1, sb->file);
    fread(&out->i_block_2i, 4, 1, sb->file);
    fread(&out->i_block_3i, 4, 1, sb->file);
    byteswap_inode(out);
    return 0;
}


int get_inode_block(struct superblock *sb, struct inode *i, uint32_t n, char *out) {
    int bs = blocksize(sb), indir_per_block = bs / 4;
    uint32_t block_num = 0;
    char indirect_block[bs];

    if (n < 12) {
      block_num= i->i_block_d[n];
    } else if (n < 12 + indir_per_block) {
        get_block_data(sb, i->i_block_1i, indirect_block);
        byteswap_iblock(sb, indirect_block);
        block_num = ((uint32_t*)indirect_block)[n - 12];
    } else if (n < 12 + indir_per_block + indir_per_block * indir_per_block) {
        get_block_data(sb, i->i_block_2i, indirect_block);
        byteswap_iblock(sb, indirect_block);
        uint32_t index1 = (n - 12 - indir_per_block) / indir_per_block;
        get_block_data(sb, ((uint32_t*)indirect_block)[index1], indirect_block);
        byteswap_iblock(sb, indirect_block);
        uint32_t index2 = (n - 12 - indir_per_block) % indir_per_block;     
        block_num = ((uint32_t*)indirect_block)[index2];
    } else {
         //Triply Indirect Block - Left as before as given instructions are don't edit something if unsure.
        char buffTwo[blocksize(sb)];
        char buffThree[blocksize(sb)];

        get_block_data(sb, i->i_block_3i, indirect_block);
        byteswap_iblock(sb, indirect_block);
        get_block_data(sb, indirect_block[n-(indir_per_block + indir_per_block* indir_per_block)/indir_per_block], buffTwo);
        byteswap_iblock(sb, buffTwo);
        get_block_data(sb, buffTwo[n-(indir_per_block + indir_per_block* indir_per_block)/indir_per_block], buffThree);
        byteswap_iblock(sb, buffThree);
        block_num = buffThree[n-(indir_per_block+ indir_per_block * indir_per_block)%indir_per_block];
    }
    return get_block_data(sb, block_num, out);
}

int is_block_free(struct superblock *sb, int blk) {
    struct bgdesc bgd;
    get_bgdesc(sb, bg_from_blk(sb, blk), &bgd);
    char byte;
    fseek(sb->file, (long)blocksize(sb) * bgd.bg_block_bitmap + blk_within_bg(sb, blk) / 8, SEEK_SET);
    fread(&byte, 1, 1, sb->file);
    return (byte >> (blk_within_bg(sb, blk) % 8)) & 1; 
}


int looks_indirect(struct superblock *sb, char *block) {
    int bs = blocksize(sb);
    uint32_t *arr = (uint32_t *)block;
    int zeroFlag = 0;
    for (int i = 0; i < bs / 4; i++) {
        if (arr[i] >= sb->s_blocks_count || (arr[i] > 0 && zeroFlag)) return 0;
        if (arr[i] == 0) zeroFlag = 1;
    }
    return 1;
}

int looks_2indirect(struct superblock *sb, char *block) {
    if (!looks_indirect(sb, block)) return 0;

    int bs = blocksize(sb);
    char buff[bs];
    uint32_t *arr = (uint32_t*)block;

    for (int i = 0; i < bs / 4; i++) {
        get_block_data(sb, arr[i], buff);
        if (!looks_indirect(sb, buff)) return 0;
    }
    return 1;

}