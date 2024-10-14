#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>

#include "e2util.h"


// Switch all of the values in the superblock structure from ext2 little-endian
// to the host's byte order.
void byteswap_superblock(struct superblock *sb)
{
	SWAP_LE(sb->s_inodes_count, 32);
	SWAP_LE(sb->s_blocks_count, 32);
	SWAP_LE(sb->s_first_data_block, 32);
	SWAP_LE(sb->s_log_block_size, 32);
	SWAP_LE(sb->s_blocks_per_group, 32);
	SWAP_LE(sb->s_inodes_per_group, 32);
	SWAP_LE(sb->s_state, 16);
}

// Display a formatted output of the superblock parameters.
void print_superblock(struct superblock *sb)
{
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

// Switch all of the values in the inode structure from ext2 little-endian to
// the host's byte order.
void byteswap_bgdesc(struct bgdesc *bg)
{
	SWAP_LE(bg->bg_block_bitmap, 32);
	SWAP_LE(bg->bg_inode_bitmap, 32);
	SWAP_LE(bg->bg_inode_table, 32);
}

// Switch all of the values in the inode structure from ext2 little-endian to
// the host's byte order.
void byteswap_inode(struct inode *i)
{
	int j;

	SWAP_LE(i->i_mode, 16);
	SWAP_LE(i->i_uid, 16);
	SWAP_LE(i->i_size, 32);
	SWAP_LE(i->i_atime, 32);
	SWAP_LE(i->i_ctime, 32);
	SWAP_LE(i->i_mtime, 32);
	SWAP_LE(i->i_dtime, 32);
	for (j = 0; j < 12; j++)
		SWAP_LE(i->i_block_d[j], 32);
	SWAP_LE(i->i_block_1i, 32);
	SWAP_LE(i->i_block_2i, 32);
	SWAP_LE(i->i_block_3i, 32);
}

// Display a formatted output of the inode parameters.
void print_inode(struct inode *i)
{
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

// Print out all the data in the file represented by a certain inode.
// Return 0 on success, 1 on error.
int print_inode_data(struct superblock *sb, struct inode *i)
{
	int fullblocks = i->i_size / blocksize(sb);
	int j;
	char *block;


	block = malloc(blocksize(sb));
	if (block == NULL)
		return 1;

	for (j = 0; j < fullblocks; j++) {
		if (get_inode_block(sb, i, j, block))
			return 1;
		if (fwrite(block, blocksize(sb), 1, stdout) != 1)
			return 1;
	}
	if (i->i_size % blocksize(sb)) {
		if (get_inode_block(sb, i, j, block))
			return 1;
		if (fwrite(block, i->i_size % blocksize(sb), 1, stdout) != 1)
			return 1;
	}

	free(block);
	return 0;
}

// Switch all of the values in an indirect block from ext2 little-endian to the
// host's byte order.
void byteswap_iblock(struct superblock *sb, char *block)
{
	int i;
	uint32_t *entry = (uint32_t *) block;
	for (i = 0; i < blocksize(sb) / 4; i++)
		SWAP_LE(entry[i], 32);
}

// Returns the block size of the filesystem
int blocksize(struct superblock *sb)
{
	return 1024 << sb->s_log_block_size;
}

// --- end provided code --- //


// Retrieve the interesting parts of the superblock and store it in the struct.
// Return 0 on success, 1 on error.
int get_superblock(FILE *f, struct superblock *out)
{

  // Save the file so other functions can use it
  out->file = f;
  //Total number of inodes

  //out->s_inodes_count = fread(buff, 4, 1, in);
  fseek(f ,1024L, SEEK_SET);
  fread(&out->s_inodes_count, 4, 1, f);
  //Total number of blocks
  fread(&out->s_blocks_count, 4, 1, f);
  // First block that belongs to the filesystem (i.e. the main superblock)
  fseek(f ,12L, SEEK_CUR);
  fread(&out->s_first_data_block, 4, 1, f);
  // Block size = 1024 << s_log_block_size
  fread(&out->s_log_block_size, 4, 1, f);
  // Blocks in each block group (starting with s_first_data_block) 
  fseek(f, 4L, SEEK_CUR);
  fread(&out->s_blocks_per_group, 4, 1, f);
  // Inodes preallocated in each block group
  fseek(f, 4L, SEEK_CUR);
  fread(&out->s_inodes_per_group, 4, 1, f);
  // Filesystem state: s_state == 1 if filesystem was cleanly unmounted
  fseek(f, 14L, SEEK_CUR);
  fread(&out->s_state, 2, 1, f);

  // Convert the superblock from little-endian format to whatever the
  // host system is.  Leave this at the end of get_superblock.
  byteswap_superblock(out);

  return 0;
}

// Fetch the data from the specified block into the provided buffer.
// Return 0 on success, 1 on error.
int get_block_data(struct superblock *sb, int blk, char *out)
{
  int bs = blocksize(sb);
  fseek(sb->file, (bs*blk), SEEK_SET);
  fread(out, bs, 1, sb->file);
  return 0;
}

// Write the data from the specified block to standard output.
// Return 0 on success, 1 on error.
int print_block_data(struct superblock *sb, int blk)
{
  char out[blocksize(sb)];
  get_block_data(sb, blk, out);
  fwrite(out, 1, sizeof(out), stdout);
  return 0;
}

// Return the number of the block group that a certain block belongs to.
int bg_from_blk(struct superblock *sb, int blk)
{
  return blk/sb->s_blocks_per_group;
}

// Return the index of a block within its block group.
int blk_within_bg(struct superblock *sb, int blk)
{
  return  blk%sb->s_blocks_per_group;
}

// Return the number of the block group that a certain inode belongs to.
int bg_from_ino(struct superblock *sb, int ino)
{
  return (ino - 1)/sb->s_inodes_per_group;
}

// Return the index of an inode within its block group
int ino_within_bg(struct superblock *sb, int ino)
{
  return (ino - 1)%sb->s_inodes_per_group;
}

// Retrieve information from the block group descriptor table.
// Return 0 on success, 1 on error.
int get_bgdesc(struct superblock *sb, int bg, struct bgdesc *out)
{
  int index = sb->s_first_data_block + 1;
  
  fseek(sb->file, index * blocksize(sb) + (bg * 32), SEEK_SET);
  // Block numbers for block bitmap, inode bitmap, and inode table
  fread(&out->bg_block_bitmap, 4, 1, sb->file);
  fread(&out->bg_inode_bitmap, 4, 1, sb->file);
  fread(&out->bg_inode_table, 4, 1, sb->file);

  // Convert the block info from little-endian format to whatever the
  // host system is.  Leave this at the end of get_bgdesc.
  byteswap_bgdesc(out);

  return 0;
}

// Retrieve information from an inode (file control block).
// Return 0 on success, 1 on error.
int get_inode(struct superblock *sb, int ino, struct inode *out)
{
  int inodeLocation;
  int bg = bg_from_ino(sb, ino);
  int index = ino_within_bg(sb, ino);
  
  // We should be at the correct block's inode table
  struct bgdesc temp;
  get_bgdesc(sb, bg, &temp); 

  // Block # for the starting block of the inode table for this block group
  uint32_t bgINtable = temp.bg_inode_table;

  // The index of the inode that we are getting
  inodeLocation = (blocksize(sb) * bgINtable) +  (index * 128);

  fseek(sb->file, (long) inodeLocation, SEEK_SET);

  // Mode (including the `is a directory' bit)
  fread(&out->i_mode, 2, 1, sb->file);

  // Owner
  fread(&out->i_uid, 2, 1, sb->file);

  // File size
  fread(&out->i_size, 4, 1, sb->file);

  // Access, change, modification, and deletion timestamps
  fread(&out->i_atime, 4, 1, sb->file);
  fread(&out->i_ctime, 4, 1, sb->file);
  fread(&out->i_mtime, 4, 1, sb->file);
  fread(&out->i_dtime, 4, 1, sb->file);

  // Blocks of the file: direct...
  fseek(sb->file ,16, SEEK_CUR);
  int i;
  for (i = 0; i < 12; i++) {
    fread(&out->i_block_d[i], 4, 1, sb->file);
  }
  // indirect...
  fread(&out->i_block_1i, 4, 1, sb->file);
  // doubly indirect...
  fread(&out->i_block_2i, 4, 1, sb->file);
  // and triply indirect.
  fread(&out->i_block_3i, 4, 1, sb->file);

  // Convert the inode from little-endian format to whatever the host
  // system is.  Leave this at the end of get_inode.
  byteswap_inode(out);
  return 0;
}

// Retrieves the data from the nth data block of a certain inode.
// Return 0 on success, 1 on error.
int get_inode_block(struct superblock *sb, struct inode *i, uint32_t n, char *out)
{
  int direct = 12;
  int indirect = (blocksize(sb) / 4);
  int doubly = indirect * indirect;
  int triply = ((blocksize(sb) / 4) * (blocksize(sb) / 4) 
				   * (blocksize(sb) / 4));
  char buffOne[blocksize(sb)];
  char buffTwo[blocksize(sb)];
  char buffThree[blocksize(sb)];

  int pointersPerBlock = blocksize(sb) / 4;  //The enteries in an indirect block

  //Direct Block
  if (n < 12) {
    get_block_data(sb, i->i_block_d[n], out);
    byteswap_iblock(sb, out);
  }
  //Indirect Block
  else if (n < indirect + direct) {
    get_block_data(sb, i->i_block_1i, buffOne);
    byteswap_iblock(sb, buffOne);
    get_block_data(sb, ((uint32_t*) buffOne)[n-direct], out);
    }
  //Doubly Indirect Block
  else if (n < doubly + indirect + direct) {
    get_block_data(sb, i->i_block_2i, buffOne);
    byteswap_iblock(sb, buffOne);
    get_block_data(sb, ((uint32_t*) buffOne)[(n-direct-indirect)/indirect], buffTwo);
    byteswap_iblock(sb, buffTwo);
    get_block_data(sb, ((uint32_t*) buffTwo)[(n-direct-indirect)%indirect], out);  

  }
  //Triply Indirect Block
  else if (n < triply + doubly + indirect + direct) {
    get_block_data(sb, i->i_block_3i, buffOne);
    byteswap_iblock(sb, buffOne);
    get_block_data(sb, buffOne[n-(doubly+pointersPerBlock)/pointersPerBlock], buffTwo);
    byteswap_iblock(sb, buffTwo);
    get_block_data(sb, buffTwo[n-(doubly+pointersPerBlock)/pointersPerBlock], buffThree);
    byteswap_iblock(sb, buffThree);
    get_block_data(sb, buffThree[n-(doubly+pointersPerBlock)%pointersPerBlock], out);   
  }
  byteswap_iblock(sb, out);
  return 0;
}

// Return 1 if a block is free, 0 if it is not, and -1 on error
int is_block_free(struct superblock *sb, int blk)
{
  int bg_num = bg_from_blk(sb, blk);
  int index = blk_within_bg(sb, blk);
  struct bgdesc temp;
  get_bgdesc(sb, bg_num, &temp);
  uint32_t byte = 0;
  int bit_offset = index/8;

  fseek(sb->file, blocksize(sb) * temp.bg_block_bitmap + (index%8), SEEK_SET);
  fread(&byte, 1, 1, sb->file);

  //return ((byte >> bit_offset) & 1) == 0;
  return 1;
}

// Return 1 if a block appears to be an indirect block, 0 if it does not, and
// -1 on error.
int looks_indirect(struct superblock *sb, char *block)
{
  uint32_t *arr;
  arr = (uint32_t*) block;
  int zeroFlag = 0;
  int i;

  for ( i = 0; i < (blocksize(sb)/4); i++) {
    if (arr[i] >= sb->s_blocks_count || (arr[i] > 0 && zeroFlag))
      return 0;
    if (arr[i] == 0)
      zeroFlag = 1;
	}
  return 1;
}

// Return 1 if a block appears to be a doubly-indirect block, 0 if it does not,
// and -1 on error.
int looks_2indirect(struct superblock *sb, char *block)
{
  uint32_t *arr;
  arr = (uint32_t*) block;
  char buff[blocksize(sb)];

  if (looks_indirect(sb, block) == 1) {
    int i;
    for(i = 0; i < (blocksize(sb)/4); i++) {
      get_block_data(sb, arr[i], buff);   
      if (looks_indirect(sb, buff) == 0)
	return 0;
	}
    return 1;
  }
  return 0;
}