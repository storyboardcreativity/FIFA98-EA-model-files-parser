#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <string>

#include "FIFA98_types.h"

const double fifa98_model_multiplier0 = 1.0 / 65536; //0.0000152587890625;
const float fifa98_model_multiplier1 = 0.01; //0.0099999998;

// ---

typedef struct point_s
{
	float x;
	float y;
	float z;
} point_t;

typedef struct texcoord_s
{
	float u;
	float v;
} texcoord_t;

typedef struct strip_s
{
	std::vector<std::shared_ptr<point_t>> points;
	std::vector<texcoord_t> texcoords;

	uint32_t material_index;
} strip_t;

typedef struct part_s
{
	std::string name;
	int next_id;
	int prev_id;

	std::vector<std::shared_ptr<point_t>> points;
	point_t origin;
} part_t;

typedef struct shape_s
{
	std::string name;
	std::vector<strip_t> strips;							// Triangles
} shape_t;

typedef struct model_s
{
	std::vector<std::shared_ptr<shape_t>> shapes;			// Nodes by ID
} model_t;

typedef struct material_s
{
	uint32_t unk0;
	uint32_t unk_data0_value;
	std::string id;				// name of the material/texture from the ".FSH" file (4 chars without '\0')
	uint8_t data1[4];
} material_t;

// ---

void process_pc_file(std::istream& f_in)
{
	model_t model;
	std::map<uint32_t, std::shared_ptr<point_t>> points_by_id;
	std::map<std::shared_ptr<point_t>, uint32_t> ids_by_point;
	std::map<uint32_t, std::shared_ptr<part_t>> parts_by_id;
	std::map<std::shared_ptr<point_t>, std::shared_ptr<part_t>> parts_by_point;

	std::vector<material_t> materials;

    // Read header
    fifa98_pc_file_header_t header;
    f_in.read((char*)&header, sizeof(header));

    // Read 00 00 00 01
    uint32_t __unk0;
    f_in.read((char*)&__unk0, sizeof(__unk0));

    // Read 'MATR' (materials)
    uint32_t __unk1;
    f_in.read((char*)&__unk1, sizeof(__unk1));

    // Read materials count
    uint32_t materials_count;
    f_in.read((char*)&materials_count, sizeof(materials_count));
    std::cout << "Textures count: " << materials_count << std::endl;

    // Read each material info
    for (uint32_t i = 0; i < materials_count; ++i)
    {
        // Read material
		fifa98_pc_file_material_t material;
        f_in.read((char*)&material, sizeof(material));

		char node_id_text[5];
		memcpy(node_id_text, (void*)&material.id, 4);
		node_id_text[4] = '\0';

		uint32_t unk_data0_value = 0xFF000000 | (material.data0[0] << 16) | (material.data0[1] << 8) | material.data0[2];

		std::cout << "Material #" << i << " id = 0x" << std::hex << material.id << std::dec << " (\'" << node_id_text << "\') ";
		std::cout << "unk = 0x" << std::hex << unk_data0_value << std::dec << std::endl;

		material_t mat;
		mat.unk0 = material.unk0;
		mat.unk_data0_value = unk_data0_value;
		mat.id = node_id_text;
		memcpy(mat.data1, material.data1, sizeof(mat.data1));

		materials.push_back(mat);
    }

    // Read 'NODE' (nodes)
    uint32_t __unk2;
    f_in.read((char*)&__unk2, sizeof(__unk2));

    // Read data blocks count
    uint32_t data_count;
    f_in.read((char*)&data_count, sizeof(data_count));
    std::cout << "Data blocks count: " << data_count << std::endl;

    // Read each data block
    for (uint32_t i = 0; i < data_count; ++i)
    {
		std::cout << "--- --- ---" << std::endl;

		uint32_t part_id = i;

        // Read data header
        fifa98_pc_file_data_header_t data_header;
        f_in.read((char*)&data_header, sizeof(data_header));
        std::cout << "Block: " << data_header.type << " " << data_header.name << std::endl;
        
        //data_header.type = __unk0 ? 4 : 3;
		std::cout << "(" << data_header.next_id << " " << data_header.data7 << " " << data_header.prev_id;
		std::cout << " " << data_header.data9 << " " << data_header.data10 << " " << data_header.data11;
		std::cout << " " << data_header.data12 << " " << data_header.data13 << " " << data_header.data14 << " " << data_header.data15 << ")" << std::endl;

        switch(data_header.type)
        {
        case 1:
			{
				std::shared_ptr<shape_t> shape(new shape_t);
				shape->name = data_header.name;

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

				uint32_t strips_count;
				f_in.read((char*)&strips_count, sizeof(strips_count));
				std::cout << "Reading strips (triangles). Count = " << strips_count << std::endl;

				for (int j = 0; j < strips_count; ++j)
				{
					uint32_t unk_a;
					f_in.read((char*)&unk_a, sizeof(unk_a));
					uint32_t skipped_b;
					f_in.read((char*)&skipped_b, sizeof(skipped_b));		// Unused
					uint32_t material_id;
					f_in.read((char*)&material_id, sizeof(material_id));
					uint32_t points_in_strip_count;
					f_in.read((char*)&points_in_strip_count, sizeof(points_in_strip_count));

					std::cout << "unk_a = " << unk_a << " material_id = " << material_id << " (\'" << materials[material_id].id << "\')" << " points_in_strip_count = " << points_in_strip_count << std::endl;

					strip_t strip;
					strip.material_index = material_id;
					for (int p = 0; p < points_in_strip_count; ++p)
					{
						uint32_t point_index;
						f_in.read((char*)&point_index, sizeof(point_index));
						//std::cout << "unk_a_a = " << unk_a_a << std::endl;

						uint32_t unk_a_b;
						f_in.read((char*)&unk_a_b, sizeof(unk_a_b));
						//std::cout << "unk_a_b = " << unk_a_b << std::endl;

						uint32_t unk_a_c;
						f_in.read((char*)&unk_a_c, sizeof(unk_a_c));
						//std::cout << "unk_a_c = " << unk_a_c << std::endl;

						uint32_t unk_a_d;
						f_in.read((char*)&unk_a_d, sizeof(unk_a_d));		// Unused

						// Texcoord is cut by the EA engine to [0;1]
						float texcoord[2];
						texcoord[0] = (long double)unk_a_b * fifa98_model_multiplier0;
						texcoord[1] = (long double)unk_a_c * fifa98_model_multiplier0;
						if (texcoord[0] < 0.0f)
							texcoord[0] = 0.0f;
						if (texcoord[0] > 1.0f)
							texcoord[0] = 1.0f;
						if (texcoord[1] < 0.0f)
							texcoord[1] = 0.0f;
						if (texcoord[1] > 1.0f)
							texcoord[1] = 1.0f;

						std::cout << point_index << " (" << parts_by_point[points_by_id[point_index]]->name << ") Texcoord: (" << texcoord[0] << "; " << texcoord[1] << ")" << std::endl;

						auto point = points_by_id[point_index];
						auto part = parts_by_point[point];
						//strip->points.push_back(point);
						//strips[part] = strip;
						strip.points.push_back(point);
					}
					shape->strips.push_back(strip);
				}

				model.shapes.push_back(shape);
			}
        case 3:
            {
            }
            break;
        case 2:
			{
				// Save model part
				std::shared_ptr<part_t> part(new part_t);
				part->name = data_header.name;
				parts_by_id[parts_by_id.size()] = part;

				// Converting origin to float
				float origin[3];
				for (int i = 0; i < 3; ++i)
					origin[i] = (long double)data_header.origin[i] * fifa98_model_multiplier0;

				// Saving origin
				part->origin.x = origin[0];
				part->origin.y = origin[1];
				part->origin.z = origin[2];

				std::cout << "Origin: (" << origin[0] << "; " << origin[1] << "; " << origin[2] << ")" << std::endl;

				// Read part's points
				uint32_t points_count;
				f_in.read((char*)&points_count, sizeof(points_count));
				for (int j = 0; j < points_count; ++j)
				{
					// Reading fixed-point values ([16-bits].[16-bits])
					int32_t values_1[3];
					f_in.read((char*)&(values_1[0]), sizeof(values_1[0]));
					f_in.read((char*)&(values_1[1]), sizeof(values_1[1]));
					f_in.read((char*)&(values_1[2]), sizeof(values_1[2]));

					// Converting to float
					float values_f_1[3];
					for (int i = 0; i < 3; ++i)
						values_f_1[i] = (long double)values_1[i] * fifa98_model_multiplier0;

					std::cout << "Coord: (" << values_f_1[0] << "; " << values_f_1[1] << "; " << values_f_1[2] << ") | ";

					uint32_t values_2[3];
					f_in.read((char*)&(values_2[0]), sizeof(values_2[0]));
					f_in.read((char*)&(values_2[1]), sizeof(values_2[1]));
					f_in.read((char*)&(values_2[2]), sizeof(values_2[2]));

					float values_f_2[3];
					values_f_2[0] = values_2[0];
					values_f_2[1] = values_2[1];
					values_f_2[2] = ((long double)values_2[2] * fifa98_model_multiplier0) * fifa98_model_multiplier1;
					std::cout << "Data: (" << values_2[0] << "; ID = " << values_2[1] << "; " << values_f_2[2] << ")" << std::endl;

					if (values_f_2[2] != 1.0f)
					{
						throw "MUST BE 1! ANOTHER IS NOT IMPLEMENTED YET! WATCH sub_4122C0()!";
					}

					std::shared_ptr<point_t> point(new point_t);
					point->x = values_f_1[0];
					point->y = values_f_1[1];
					point->z = values_f_1[2];

					parts_by_point[point] = part;
					part->points.push_back(point);

					points_by_id[values_2[1]] = point;
					ids_by_point[point] = values_2[1];
				}
			}
        case 4:
            {
            }
            break;
        default:
            continue;
        }

		std::cout << "--- END ---" << std::endl;
    }

	/*
	for (auto it = model.points.begin(); it != model.points.end(); ++it)
	{
		auto point = it->second;
		std::cout << "[" << it->first << "] " << point.x << " " << point.y << " " << point.z << std::endl;
	}

	for (auto it = model.strips.begin(); it != model.strips.end(); ++it)
	{
		std::cout << "[strip]" << std::endl;
		for (auto it1 = it->points.begin(); it1 != it->points.end(); ++it1)
		{
			auto point = model.points[*it1];
			std::cout << "v " << point.x << " " << point.y << " " << point.z << std::endl;
		}
		std::cout << "---" << std::endl;
	}
	*/

	// Change parts positions
	for (auto it_part = parts_by_id.begin(); it_part != parts_by_id.end(); ++it_part)
	{
		auto index = it_part->first;
		auto part = it_part->second;
		auto origin = part->origin;
		auto points = part->points;

		if (part->name == "LowerTorso_1")// || part->name == "UpperTorso")
			continue;

		for (int i = 0; i < points.size(); ++i)
		{
			points[i]->x += 0;// +origin.x;
			points[i]->y += index * 100.0f;;// +origin.y// + index * 100.0f;
			points[i]->z += 0;// +origin.z;
		}
	}

	for (auto it = points_by_id.begin(); it != points_by_id.end(); ++it)
	{
		auto point = it->second;
		std::cout << "v " << point->x << " " << point->y << " " << point->z << std::endl;
	}

	std::cout << std::endl;

	int i = 0;
	for (auto it_shape = model.shapes.begin(); it_shape != model.shapes.end(); ++it_shape)
	{
		auto shape = *it_shape;

		// Strip name
		std::cout << "g " << shape->name << std::endl;

		for (auto it_strip = shape->strips.begin(); it_strip != shape->strips.end(); ++it_strip)
		{
			auto strip = *it_strip;

			//if (strip.points.size() < 3)
			//	continue;

			// Point indexes
			std::cout << "f ";
			for (auto it_point = strip.points.begin(); it_point != strip.points.end(); ++it_point)
				std::cout << ids_by_point[*it_point] + 1 << " ";
			std::cout << std::endl;
		}

		std::cout << std::endl;

		++i;
	}

	std::cout << std::endl;
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

int ___main___()
{
	const char* path = "examples/lowpoly.pc";
	//const char* path = "examples/medpoly.pc";
	//const char* path = "examples/shipoly.pc";

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