#include "utils.h"
unsigned char * LoadFileContent(const char *path, int &filesize) {
	filesize = 0;
	unsigned char *filecontent = nullptr;
	FILE *pFile = fopen(path, "rb");
	if (pFile){
		fseek(pFile, 0, SEEK_END);
		int nLen = ftell(pFile);
		if (nLen>0){
			rewind(pFile);
			filecontent = new unsigned char[nLen+1];
			fread(filecontent, sizeof(unsigned char), nLen, pFile);
			filecontent[nLen] = '\0';
			filesize = nLen;
		}
		fclose(pFile);
	}
	return filecontent;
}
unsigned char *DecodeBMP(unsigned char*filecontent, int&width, int&height) {
	if (0x4D42==*((unsigned short*)filecontent)){
		int pixelOffset = *((int*)(filecontent + 10));
		width = *((int*)(filecontent + 18));
		height = *((int*)(filecontent + 22));
		unsigned char*pixelData = filecontent + pixelOffset;
		for (int i = 0; i < width*height * 3;i+=3) {
			unsigned char temp = pixelData[i];
			pixelData[i] = pixelData[i+2];
			pixelData[i + 2] = temp;
		}
		return pixelData;
	}
	return nullptr;
}