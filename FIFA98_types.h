#pragma once
#include <stdint.h>

#pragma pack(push, 1)
typedef struct
{
	uint8_t data[28];
} fifa98_pc_file_header_t;
#pragma pack(pop)

// Check for correct sizeof(fifa98_pc_file_header_t)
typedef int ERROR_FIFA98_PC_FILE_HEADER_WRONG_SIZE[sizeof(fifa98_pc_file_header_t) == 28 ? 1 : -1];

#pragma pack(push, 1)
typedef struct
{
	uint32_t unk0;
	uint8_t data0[4];
	uint32_t id;				// name of the material/texture from the ".FSH" file (4 chars without '\0')
	uint8_t data1[4];
} fifa98_pc_file_material_t;
#pragma pack(pop)

// Check for correct sizeof(fifa98_pc_file_node_t)
typedef int ERROR_FIFA98_PC_FILE_NODE_WRONG_SIZE[sizeof(fifa98_pc_file_material_t) == 16 ? 1 : -1];

#pragma pack(push, 1)
typedef struct
{
	uint32_t type;			// Data type
	char name[20];			// Part name
	int32_t prev_id;		// Prev part ID
	int32_t data7;
	int32_t next_id;		// Next part ID

	uint32_t data9;			// 65536
	uint32_t data10;		// 65536
	uint32_t data11;		// 65536
	uint32_t data12;		// 0
	uint32_t data13;		// 0
	uint32_t data14;		// 0
	uint32_t data15;		// 0
	int32_t origin[3];
} fifa98_pc_file_data_header_t;
#pragma pack(pop)

// Check for correct sizeof(fifa98_pc_file_data_header_t)
typedef int ERROR_FIFA98_PC_FILE_DATA_HEADER_WRONG_SIZE[sizeof(fifa98_pc_file_data_header_t) == 76 ? 1 : -1];
