#include <fstream>
#include <iostream>

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
    uint8_t data0[8];
    uint32_t id;
    uint8_t data1[4];
} fifa98_pc_file_node_t;
#pragma pack(pop)

// Check for correct sizeof(fifa98_pc_file_header_t)
typedef int ERROR_FIFA98_PC_FILE_NODE_WRONG_SIZE[sizeof(fifa98_pc_file_node_t) == 16 ? 1 : -1];

#pragma pack(push, 1)
typedef struct
{
    uint32_t type;
    char name[20];

    uint32_t data6;
    uint32_t data7;
    uint32_t data8;

    uint32_t data9;
    uint32_t data10;
    uint32_t data11;
    uint32_t data12;
    uint32_t data13;
    uint32_t data14;
    uint32_t data15;
    uint32_t data16;
    uint32_t data17;
    uint32_t data18;
} fifa98_pc_file_data_header_t;
#pragma pack(pop)

// Check for correct sizeof(fifa98_pc_file_header_t)
typedef int ERROR_FIFA98_PC_FILE_DATA_HEADER_WRONG_SIZE[sizeof(fifa98_pc_file_data_header_t) == 76 ? 1 : -1];

void process_pc_file(std::istream& f_in)
{
    // Read header
    fifa98_pc_file_header_t header;
    f_in.read((char*)&header, sizeof(header));

    // Read 00 00 00 01
    uint32_t __unk0;
    f_in.read((char*)&__unk0, sizeof(__unk0));

    // Read 'MATR'
    uint32_t __unk1;
    f_in.read((char*)&__unk1, sizeof(__unk1));

    // Read nodes count
    uint32_t nodes_count;
    f_in.read((char*)&nodes_count, sizeof(nodes_count));
    std::cout << "Nodes count: " << nodes_count << std::endl;

    // Read each node info
    for (uint32_t i = 0; i < nodes_count; ++i)
    {
        // Read node
        fifa98_pc_file_node_t node;
        f_in.read((char*)&node, sizeof(node));
        std::cout << "Node #" << i << " id = 0x" << std::hex << node.id << std::dec << std::endl;
    }

    // Read 'NODE'
    uint32_t __unk2;
    f_in.read((char*)&__unk2, sizeof(__unk2));

    // Read data blocks count
    uint32_t data_count;
    f_in.read((char*)&data_count, sizeof(data_count));
    std::cout << "Data blocks count: " << data_count << std::endl;

    // Read each data block
    for (uint32_t i = 0; i < data_count; ++i)
    {
        // Read data header
        fifa98_pc_file_data_header_t data_header;
        f_in.read((char*)&data_header, sizeof(data_header));
        std::cout << "Data block type: " << data_header.type << std::endl;
        std::cout << "Data block name: " << data_header.name << std::endl;
        
        //data_header.type = __unk0 ? 4 : 3;

        switch(data_header.type)
        {
        case 1:
        case 3:
            {
                if (data_header.type != 3)
                {
                    if (__unk0)
                    {
                        uint32_t unk_count_0;
                        f_in.read((char*)&unk_count_0, sizeof(unk_count_0));
                        std::cout << "Reading x3 data blocks. Count = " << unk_count_0 << std::endl;

                        for (int j = 0; j < 3 * unk_count_0; ++j)
                        {
                            uint32_t unk_data0;
                            f_in.read((char*)&unk_data0, sizeof(unk_data0));
                        }

                        std::cout << "Finished reading x3 data blocks" << std::endl;
                    }
                    else
                    {
                        uint32_t unk_value_0;
                        f_in.read((char*)&unk_value_0, sizeof(unk_value_0));
                    }

                    uint32_t unk_value_1;
                    f_in.read((char*)&unk_value_1, sizeof(unk_value_1));
                    std::cout << "Reading subblocks. Count = " << unk_value_1 << std::endl;

                    for (int j = 0; j < unk_value_1; ++j)
                    {
                        uint32_t unk_a;
                        f_in.read((char*)&unk_a, sizeof(unk_a));
                        std::cout << "unk_a = " << unk_a << std::endl;

                        uint32_t unk_b;
                        f_in.read((char*)&unk_b, sizeof(unk_b));
                        std::cout << "unk_b = " << unk_b << std::endl;

                        uint32_t unk_c;
                        f_in.read((char*)&unk_c, sizeof(unk_c));
                        std::cout << "unk_c = " << unk_c << std::endl;

                        uint32_t unk_d;
                        f_in.read((char*)&unk_d, sizeof(unk_d));
                        std::cout << "unk_d = " << unk_d << std::endl;

                        for (int p = 0; p < unk_d; ++p)
                        {
                            uint32_t unk_a_a;
                            f_in.read((char*)&unk_a_a, sizeof(unk_a_a));
                            std::cout << "unk_a_a = " << unk_a_a << std::endl;

                            uint32_t unk_a_b;
                            f_in.read((char*)&unk_a_b, sizeof(unk_a_b));
                            std::cout << "unk_a_b = " << unk_a_b << std::endl;

                            uint32_t unk_a_c;
                            f_in.read((char*)&unk_a_c, sizeof(unk_a_c));
                            std::cout << "unk_a_c = " << unk_a_c << std::endl;

                            uint32_t unk_a_d;
                            f_in.read((char*)&unk_a_d, sizeof(unk_a_d));
                            std::cout << "unk_a_d = " << unk_a_d << std::endl;
                        }
                    }
                }
            }
            break;
        case 2:
        case 4:
            {
                if (data_header.type != 4)
                {
                    uint32_t unk_count0;
                    f_in.read((char*)&unk_count0, sizeof(unk_count0));

                    for (int j = 0; j < unk_count0; ++j)
                    {
                        uint32_t values_1[3];
                        f_in.read((char*)&(values_1[0]), sizeof(values_1[0]));
                        f_in.read((char*)&(values_1[1]), sizeof(values_1[1]));
                        f_in.read((char*)&(values_1[2]), sizeof(values_1[2]));

                        uint32_t values_2[3];
                        f_in.read((char*)&(values_2[0]), sizeof(values_2[0]));
                        f_in.read((char*)&(values_2[1]), sizeof(values_2[1]));
                        f_in.read((char*)&(values_2[2]), sizeof(values_2[2]));
                    }
                }
            }
            break;
        default:
            continue;
        }
    }
}

std::streampos get_file_size(const char* filePath)
{
    std::streampos fsize = 0;
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open())
        return 0;

    fsize = file.tellg();
    file.seekg(0, std::ios::end);
    fsize = file.tellg() - fsize;
    file.close();

    return fsize;
}

int main()
{
    const char* path = "examples/lowpoly.pc";

    // Get file size
    auto size = get_file_size(path);

    std::fstream in_file(path, std::ios::in | std::ios::binary);
    if (!in_file.is_open())
    {
        std::cout << "Could not open '.pc' file!" << std::endl;
        return -1;
    }

    process_pc_file(in_file);

    if (in_file.tellg() != size)
    {
        std::cout << "Parsing failed!" << std::endl << \
            "Expected size : " << size << std::endl << \
            "Actual parsed size : " << in_file.tellg() << std::endl;
    }
    else
    {
        std::cout << "Parsing success!" << std::endl;
    }
    

    in_file.close();

    return 0;
}