// VoxLib.cpp: 定义应用程序的入口点。
//

#define OGT_VOX_IMPLEMENTATION
#include "VoxLib.h"

using namespace std;
#include <iostream>
#include <fstream>
#include "stdio.h"
#include <vector>

void ReadVoxFileHeader(uint8_t* fileBuffer, uint32_t bufSize, int* sizes)
{
	const ogt_vox_scene* scene = ogt_vox_read_scene(fileBuffer, bufSize);
	
	if (scene->num_models < 1 || scene->num_models > 1)
	{
		return;
	}

	auto model = scene->models[0];

	sizes[0] = model->size_x;
	sizes[1] = model->size_z;
	sizes[2] = model->size_y;

	return;
}

bool ReadVoxFileSingle(uint8_t* fileBuffer, uint32_t bufSize, uint32_t* buffer, int sizeX, int sizeY, int sizeZ)
{
	// sizeY is z axis in magicavoxel (sky / ground)
	// sizeZ is y axis in magicavoxel (forward / backward)

	const ogt_vox_scene* scene = ogt_vox_read_scene(fileBuffer, bufSize);

	if (scene -> num_models < 1 || scene -> num_models > 1)
	{
		return false;
	}

	auto model = scene->models[0];
	if (model->size_x != sizeX || model->size_y != sizeZ || model->size_z != sizeY)
	{
		return false;
	}

	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
		{
			for (int z = 0; z < sizeZ; z++)
			{
				auto index = model->voxel_data[y * sizeZ * sizeX + z * sizeX + x];
				if (index > 0)
				{
					auto color = scene->palette.color[index];
					buffer[x * sizeY * sizeZ + y * sizeZ + z] = (
						(((uint32_t)color.r) << 24U) +
						(((uint32_t)color.g) << 16U) +
						(((uint32_t)color.b) << 8U ) +
						((uint32_t)color.a)
						);

					//printf("%3u, %3u, %3u, %3u ->", color.r, color.g, color.b, color.a);
					//printf(" %8x\n", buffer[x * sizeY * sizeZ + y * sizeZ + z]);
					//printf(" %8x\n", buffer[x * sizeY * sizeZ + y * sizeZ + z]);
				}
				else
				{
					buffer[x * sizeY * sizeZ + y * sizeZ + z] = 0;
				}
			}
		}
	}

	return true;
}

std::vector<uint8_t> read_vector_from_disk(std::string file_path)
{
    std::ifstream instream(file_path, std::ios::in | std::ios::binary);
    std::vector<uint8_t> data((std::istreambuf_iterator<char>(instream)), std::istreambuf_iterator<char>());
    return data;
}

int main()
{
	char* filePth = "D:\\RP_Games\\UnityProj\\WGRDemo\\MagicaVoxelFiles\\colorfulFlowers.vox";
	cout << "Opening file " << filePth << endl;

	uint32_t arr[512];

	auto data = read_vector_from_disk(filePth);
	if (!ReadVoxFileSingle(data.data(), data.size(), &arr[0], 8, 8, 8))
	{
		printf("Error while loading file\n");
		return 1;
	}
	else
	{
		printf("Loaded\n");
	}

	for (int y = 0; y < 8; y++)
	{
		for (int z = 7; z >= 0; z--)
		{
			for (int x = 0; x < 8; x++)
			{
				printf("%2x ", (arr[x * 64 + y * 8 + z] >> 24) & 0xFF);
				//printf("%3x ", arr[x * 64 + y * 8 + z]);
			}
			printf("\n");
		}
		printf("\n***********************************************************\n");
	}

	return 0;
}
