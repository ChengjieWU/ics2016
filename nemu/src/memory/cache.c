#include "common.h"

#define BLOCK_BIT 6
#define BLOCK_SIZE 64
#define GROUP_BIT 7
#define GROUP_NUM 128
#define WAY_NUM 8
#define TAG_BIT (32 - BLOCK_BIT - GROUP_BIT)

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

typedef struct {
	bool valid;
	union {
		struct {
			uint32_t offset : BLOCK_BIT;
			uint32_t group : GROUP_BIT;
			uint32_t tag : TAG_BIT;
		};
		uint32_t addr;
	};
	uint8_t data[BLOCK_SIZE];
} Cache_1;


typedef struct {
	Cache_1 cache[WAY_NUM];
} Cache_1_group;


Cache_1_group L1[GROUP_NUM];


void init_cache1()
{
	int i, j;
	memset(L1, 0, sizeof L1);
	for (i = 0; i < GROUP_NUM; i++)
	{
		for (j = 0; j < WAY_NUM; j++)
		{
			L1[i].cache[j].group = (uint32_t)j;
		}
	}
}



inline static void memcpy_cache (void *dest, void *src, size_t len)
{
	int i;
	for (i = 0; i < len; i++)
	{
		((uint8_t*)dest)[i] = ((uint8_t*)src)[i];
	}
}


uint32_t Cache_1_read(hwaddr_t addr, size_t len) 
{
	Cache_1 mirror;
	mirror.addr = addr;
	uint32_t offset = mirror.offset;
	uint32_t group = mirror.group;
	uint32_t tag = mirror.tag;
	hwaddr_t addr_block = addr & (~0u << BLOCK_BIT);
	uint8_t temp[2 * BLOCK_SIZE];

	int Hit = 0, x = 0;
	int i = 0;
	for (i = 0; i < WAY_NUM; i++)
		if (L1[group].cache[i].valid && L1[group].cache[i].tag == tag)
		{
			Hit = 1;
			x = i;
			break;
		}

	if (!Hit)
	{
		//find a cache to use
		for (i = 0; i < WAY_NUM; i++)
			if (!L1[group].cache[i].valid)
			{
				x = i;
				break;
			}
		L1[group].cache[x].valid = true;
		L1[group].cache[x].tag = tag;
		for (i = 0; i < BLOCK_SIZE; i++) 
			L1[group].cache[x].data[i] = dram_read(addr_block + i, 1) & (~0u >> ((4 - 1) << 3));
	}
	
	memcpy_cache (temp, L1[group].cache[x].data, BLOCK_SIZE);
	if (offset + len > BLOCK_SIZE) *(uint32_t*)(temp + BLOCK_SIZE) = Cache_1_read(addr_block + BLOCK_SIZE, len);

	return unalign_rw(temp + offset, 4);
}



void Cache_1_write(hwaddr_t addr, size_t len, uint32_t data) 
{
	
}
