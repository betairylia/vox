#pragma once

#include "ogt_vox.h"

extern "C"
{
	__declspec(dllexport) bool ReadVoxFileSingle(uint8_t* fileBuffer, uint32_t bufSize, uint32_t* buffer, int sizeX, int sizeY, int sizeZ);
	__declspec(dllexport) void ReadVoxFileHeader(uint8_t* fileBuffer, uint32_t bufSize, int* sizes);
}