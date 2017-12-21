//#ifndef CACHE_H
#define CACHE_H

#include <stdio.h>
#include "host.h"
#include "misc.h"
#include "machine.h"
#include "memory.h"
#include "stats.h"




/* highly associative caches are implemented using a hash table lookup to
   speed block access, this macro decides if a cache is "highly associative" */
#define CACHE_HIGHLY_ASSOC(cp)	((cp)->assoc > 4)


//const unsigned int SUPER_SIZE = 4;

/* cache replacement policy */
enum cache_policy {
  LRU,		/* replace least recently used block (perfect LRU) */
  Random,	/* replace a random block */
  FIFO,		/* replace the oldest block in the set */
  LAEE
};



/* parse policy */
enum cache_policy                       /* replacement policy enum */
cache_char2policy(char c);              /* replacement policy as a char */






/* block status values */
#define CACHE_BLK_VALID		0x00000001	/* block in valid, in use */
#define CACHE_BLK_DIRTY		0x00000002	/* dirty block */



struct cache_blk_t
{
  byte_t data[1];		/* actual data block starts here, block size
				   should probably be a multiple of 8 */
};



struct cache_super_t
{
	  unsigned int super_size;
	  struct cache_super_t *way_next;	/* next super block in the ordered way chain, used
					   to order super blocks for replacement */
	  struct cache_super_t *way_prev;	/* previous super block in the order way chain */
	  struct cache_super_t *hash_next;/* next super block in the hash bucket chain, only
					   used in highly-associative caches */
	  /* since hash table lists are typically small, there is no previous
	     pointer, deletion requires a trip through the hash table bucket list */
	  md_addr_t tag;		/* super block tag value */

	  unsigned int status[4];		/* sub block status array, see CACHE_BLK_* defs above */

	  tick_t ready;		/* time when block will be accessible, field
					   is set when a miss fetch is initiated */
	  byte_t *user_data;		/* pointer to user defined data, e.g.,
					   pre-decode data or physical page address */
	  /* DATA should be pointer-aligned due to preceeding field */
	  /* NOTE: this is a variable-size tail array, this must be the LAST field
	     defined in this structure! */


	  struct cache_blk_t *subblocks;		/* actual sub blocks start here*/

};





struct cache_set_t
{
  struct cache_super_t **hash;	/* hash table: for fast access w/assoc, NULL
				   for low-assoc caches */
  struct cache_super_t *way_head;	/* head of way list */
  struct cache_super_t *way_tail;	/* tail pf way list */
  struct cache_super_t *blks;	/* cache blocks, allocated sequentially, so
				   this pointer can also be used for random
				   access to cache blocks */
};












/* cache definition */
struct cache_t
{
  /* parameters */
  char *name;			/* cache name */
  int nsets;			/* number of sets */
  int bsize;			/* block size in bytes */
  int balloc;			/* maintain cache contents? */
  int usize;			/* user allocated data size */
  int assoc;			/* cache associativity */
  enum cache_policy policy;	/* cache replacement policy */
  unsigned int hit_latency;	/* cache hit latency */

  unsigned int	(*blk_access_fn)(enum mem_cmd cmd,		/* block access command */
		     md_addr_t baddr,		/* program address to access */
		     int bsize,			/* size of the cache block */
		     struct cache_blk_t *blk,	/* ptr to cache block struct */
		     tick_t now);		/* when fetch was initiated */

  /* derived data, for fast decoding */
  int hsize;			/* cache set hash table size */
  md_addr_t blk_mask;
  int set_shift;
  md_addr_t set_mask;		/* use *after* shift */
  int tag_shift;
  md_addr_t tag_mask;		/* use *after* shift */
  md_addr_t tagset_mask;	/* used for fast hit detection */

  unsigned int super_blk_size;

  /* bus resource */
  tick_t bus_free;		/* time when bus to next level of cache is
				   free, NOTE: the bus model assumes only a
				   single, fully-pipelined port to the next
 				   level of memory that requires the bus only
 				   one cycle for cache line transfer (the
 				   latency of the access to the lower level
 				   may be more than one cycle, as specified
 				   by the miss handler */

  /* per-cache stats */
  counter_t hits;		/* total number of hits */
  counter_t misses;		/* total number of misses */
  counter_t replacements;	/* total number of replacements at misses */
  counter_t writebacks;		/* total number of writebacks at misses */
  counter_t invalidations;	/* total number of external invalidations */

  /* last block to hit, used to optimize cache hit processing */
  md_addr_t last_tagset;	/* tag of last line accessed */
  struct cache_super_t *last_blk;	/* cache block last accessed */

  /* data blocks */
  byte_t *data;			/* pointer to data blocks allocation */

  /* NOTE: this is a variable-size tail array, this must be the LAST field
     defined in this structure! */
  struct cache_set_t sets[1];	/* each entry is a set */
};





/* create and initialize a general cache structure */
struct cache_t *			/* pointer to cache created */
cache_create(char *name,		/* name of the cache */
	     int nsets,			/* total number of sets in cache */
	     int bsize,			/* block (line) size of cache */
	     int balloc,		/* allocate data space for blocks? */
	     int usize,			/* size of user data to alloc w/blks */
	     int assoc,			/* associativity of cache */
	     enum cache_policy policy,	/* replacement policy w/in sets */
	     /* block access function, see description w/in struct cache def */
	     unsigned int (*blk_access_fn)(enum mem_cmd cmd,
					   md_addr_t baddr, int bsize,
					   struct cache_blk_t *blk,
					   tick_t now),
	     unsigned int hit_latency,
	     unsigned int super_blk_size	
	     );/* latency in cycles for a hit */









