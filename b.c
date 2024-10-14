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
    t = i->i_atime;
    printf("Access time: %s", ctime(&t));
    t = i->i_ctime;
    printf("Change time: %s", ctime(&t));
    t = i->i_mtime;
    printf("Modification time: %s", ctime(&t));
    t = i->i_dtime;
    printf("Deletion time: %s", ctime(&t));
    printf("First direct block: %u\n", i->i_block_d[0]);
}

int print_inode_data(struct superblock *sb, struct inode *i) {
    int bs = blocksize(sb);
    char *block = malloc(bs);
    if (!block) return 1; // Memory allocation failed

    int num_full_blocks = i->i_size / bs;
    int last_block_size = i->i_size % bs;

    for (int j = 0; j < num_full_blocks; j++) {
        if (get_inode_block(sb, i, j, block)) {
            free(block);
            return 1; // Failed to get the inode block
        }

        if (fwrite(block, bs, 1, stdout) != 1) {
            free(block);
            return 1; // Failed to write full block
        }
    }

    // Handle the last partial block
    if (last_block_size > 0) {
        if (get_inode_block(sb, i, num_full_blocks, block)) {
            free(block);
            return 1; // Failed to get the last inode block
        }

        // Write the last partial block
        if (fwrite(block, last_block_size, 1, stdout) != 1) {
            free(block);
            return 1; // Failed to write last partial block
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


static int safe_fread(void *ptr, size_t size, size_t count, FILE *stream) {
    return fread(ptr, size, count, stream) == count;
}

static int safe_fseek(FILE *stream, long offset, int whence) {
    return fseek(stream, offset, whence) == 0;
}


int get_superblock(FILE *f, struct superblock *out) {
    out->file = f;
    safe_fseek(f, 1024, SEEK_SET);
    safe_fread(&out->s_inodes_count, 4, 1, f);
    safe_fread(&out->s_blocks_count, 4, 1, f);
    safe_fseek(f, 12, SEEK_CUR);
    safe_fread(&out->s_first_data_block, 4, 1, f);
    safe_fread(&out->s_log_block_size, 4, 1, f);
    safe_fseek(f, 4, SEEK_CUR);
    safe_fread(&out->s_blocks_per_group, 4, 1, f);
    safe_fseek(f, 4, SEEK_CUR);
    safe_fread(&out->s_inodes_per_group, 4, 1, f);
    safe_fseek(f, 14, SEEK_CUR);
    safe_fread(&out->s_state, 2, 1, f);
    byteswap_superblock(out);
    return 0;
}

int get_block_data(struct superblock *sb, int blk, char *out) {
    int bs = blocksize(sb);
    safe_fseek(sb->file, (long)bs * blk, SEEK_SET);
    safe_fread(out, bs, 1, sb->file);
    return 0;
}

int print_block_data(struct superblock *sb, int blk) {
    char out[blocksize(sb)];
    return get_block_data(sb, blk, out) || fwrite(out, 1, sizeof(out), stdout) != sizeof(out);
}

int bg_from_blk(struct superblock *sb, int blk) {
    return blk / sb->s_blocks_per_group;
}

int blk_within_bg(struct superblock *sb, int blk) {
    return blk % sb->s_blocks_per_group;
}

int bg_from_ino(struct superblock *sb, int ino) {
    return (ino - 1) / sb->s_inodes_per_group;
}

int ino_within_bg(struct superblock *sb, int ino) {
    return (ino - 1) % sb->s_inodes_per_group;
}

int get_bgdesc(struct superblock *sb, int bg, struct bgdesc *out) {
    int bs = blocksize(sb);
    safe_fseek(sb->file, (long)bs * (sb->s_first_data_block + 1) + bg * 32, SEEK_SET);
    safe_fread(&out->bg_block_bitmap, 4, 1, sb->file);
    safe_fread(&out->bg_inode_bitmap, 4, 1, sb->file);
    safe_fread(&out->bg_inode_table, 4, 1, sb->file);
    byteswap_bgdesc(out);
    return 0;
}


int get_inode(struct superblock *sb, int ino, struct inode *out) {
    int bs = blocksize(sb);
    struct bgdesc bgd;
    get_bgdesc(sb, bg_from_ino(sb, ino), &bgd);
    safe_fseek(sb->file, (long)bs * bgd.bg_inode_table + (long)ino_within_bg(sb, ino) * 128, SEEK_SET);

    safe_fread(&out->i_mode, 2, 1, sb->file);
    safe_fread(&out->i_uid, 2, 1, sb->file);
    safe_fread(&out->i_size, 4, 1, sb->file);
    safe_fread(&out->i_atime, 4, 1, sb->file);
    safe_fread(&out->i_ctime, 4, 1, sb->file);
    safe_fread(&out->i_mtime, 4, 1, sb->file);
    safe_fread(&out->i_dtime, 4, 1, sb->file);
    safe_fseek(sb->file, 16, SEEK_CUR);
    for (int i = 0; i < 12; i++)
        safe_fread(&out->i_block_d[i], 4, 1, sb->file);
    safe_fread(&out->i_block_1i, 4, 1, sb->file);
    safe_fread(&out->i_block_2i, 4, 1, sb->file);
    safe_fread(&out->i_block_3i, 4, 1, sb->file);

    byteswap_inode(out);
    return 0;
}


int get_inode_block(struct superblock *sb, struct inode *i, uint32_t n, char *out) {
    int bs = blocksize(sb), indirect = bs / 4;
    char block[bs];

    if (n < 12) {
        get_block_data(sb, i->i_block_d[n], out);
    } else if (n < 12 + indirect) {
        get_block_data(sb, i->i_block_1i, block);
        byteswap_iblock(sb, block);
        get_block_data(sb, ((uint32_t *)block)[n - 12], out);
    } else if (n < 12 + indirect + indirect * indirect) {
        int n2 = n - 12 - indirect;
        get_block_data(sb, i->i_block_2i, block);
        byteswap_iblock(sb, block);
        get_block_data(sb, ((uint32_t *)block)[n2 / indirect], block);
        byteswap_iblock(sb, block);
        get_block_data(sb, ((uint32_t *)block)[n2 % indirect], out);
    } else {
        // Triply indirect access
        int n2 = n - 12 - indirect - indirect * indirect;
        get_block_data(sb, i->i_block_3i, block);
        byteswap_iblock(sb, block);
        get_block_data(sb, ((uint32_t *)block)[n2 / (indirect * indirect)], block);
        byteswap_iblock(sb, block);
        get_block_data(sb, ((uint32_t *)block)[n2 % (indirect * indirect)], out);
    }
    byteswap_iblock(sb, out);
    return 0;
}


int is_block_free(struct superblock *sb, int blk) {
    struct bgdesc bgd;
    get_bgdesc(sb, bg_from_blk(sb, blk), &bgd);
    int bs = blocksize(sb);
    int index = blk_within_bg(sb, blk);
    char byte;
    
    safe_fseek(sb->file, (long)bs * bgd.bg_block_bitmap + index / 8, SEEK_SET);
    safe_fread(&byte, 1, 1, sb->file);
    
    // Check if the block is free
    return ((byte >> (index % 8)) & 1) == 0; // Return 1 if free, 0 if not
}



int looks_indirect(struct superblock *sb, char *block) {
    uint32_t *arr = (uint32_t *)block;
    int zeroFlag = 0;
    for (int i = 0; i < blocksize(sb) / 4; i++) {
        if (arr[i] >= sb->s_blocks_count || (arr[i] > 0 && zeroFlag)) return 0;
        if (arr[i] == 0) zeroFlag = 1;
    }
    return 1;
}


int looks_2indirect(struct superblock *sb, char *block) {
    if (!looks_indirect(sb, block)) return 0;
    char buff[blocksize(sb)];
    uint32_t *arr = (uint32_t *)block;
    for (int i = 0; i < blocksize(sb) / 4; i++) {
        get_block_data(sb, arr[i], buff);
        if (!looks_indirect(sb, buff)) return 0;
    }
    return 1;
}