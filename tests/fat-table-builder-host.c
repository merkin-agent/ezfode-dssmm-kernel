#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "fat_table_builder.h"

static void check(int condition, const char *message)
{
	if(!condition)
	{
		fprintf(stderr, "FAIL: %s\n", message);
		exit(1);
	}
}

static void test_region(int game_rom, size_t start_word, size_t pair_capacity)
{
	uint32_t table[FAT_TABLE_SIZE_BYTES / sizeof(uint32_t) + 1u];
	uint32_t *cursor;
	uint32_t *start = table + start_word;
	uint32_t *limit = FatTableLimit(table, game_rom);
	size_t i;

	for(i = 0; i < sizeof(table) / sizeof(table[0]); i++)
		table[i] = 0xA5A5A5A5u;

	cursor = start;
	check((size_t)(limit - start) == pair_capacity * 2u, "region capacity");

	for(i = 0; i < pair_capacity; i++)
		check(FatTableAppendPair(&cursor, limit, (uint32_t)i, (uint32_t)(i + 1u)),
			"pair fits before the region limit");

	check(cursor == limit, "exact-capacity writes end at the region limit");
	check(!FatTableAppendPair(&cursor, limit, 0xDEADBEEFu, 0xCAFEBABEu),
		"overflowing pair is rejected");
	check(cursor == limit, "rejected pair leaves the cursor unchanged");
	check(*limit == 0xA5A5A5A5u, "rejected pair does not overwrite the next region");
	check(table[FAT_TABLE_SIZE_BYTES / sizeof(uint32_t)] == 0xA5A5A5A5u,
		"no write crosses the complete table boundary");
}

int main(void)
{
	test_region(1, 0, FAT_TABLE_GAME_PARAMETER_OFFSET_BYTES / sizeof(uint32_t) / 2u);
	test_region(0, FAT_TABLE_RTS_OFFSET_BYTES / sizeof(uint32_t),
		(FAT_TABLE_SIZE_BYTES - FAT_TABLE_RTS_OFFSET_BYTES) / sizeof(uint32_t) / 2u);
	puts("FAT table builder host test passed.");
	return 0;
}
