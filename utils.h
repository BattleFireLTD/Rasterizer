#pragma once
#include <stdio.h>
unsigned char *LoadFileContent(const char *path, int &filesize);
unsigned char *DecodeBMP(unsigned char*filecontent, int&width, int&height);