#include<vector>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#pragma warning(disable : 4996)

//#include "main.h"

//#define _CRT_SECURE_NO_WARNINGS

static unsigned short read_u16(FILE* fp);
static unsigned int   read_u32(FILE* fp);
static int            read_s32(FILE* fp);

static unsigned short read_u16(FILE *fp)
{
    unsigned char b0, b1;
 
    b0 = getc(fp);
    b1 = getc(fp);
 
    return ((b1 << 8) | b0);
}
 
 
static unsigned int read_u32(FILE *fp)
{
    unsigned char b0, b1, b2, b3;
 
    b0 = getc(fp);
    b1 = getc(fp);
    b2 = getc(fp);
    b3 = getc(fp);
 
    return ((((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}
 
 
static int read_s32(FILE *fp)
{
    unsigned char b0, b1, b2, b3;
 
    b0 = getc(fp);
    b1 = getc(fp);
    b2 = getc(fp);
    b3 = getc(fp);
 
    return ((int)(((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}

class my_images {
    //std::vector<RGBQUAD**> images;
public:
    RGBQUAD** my_rgb;

    void read_bmp(const char* filename) {

        FILE* pFile = fopen(filename, "rb");

        // ��������� ��������� �����

        BITMAPFILEHEADER header;

        header.bfType = read_u16(pFile);
        header.bfSize = read_u32(pFile);
        header.bfReserved1 = read_u16(pFile);
        header.bfReserved2 = read_u16(pFile);
        header.bfOffBits = read_u32(pFile);

        // ��������� ��������� �����������
        BITMAPINFOHEADER bmiHeader;

        bmiHeader.biSize = read_u32(pFile);
        bmiHeader.biWidth = read_s32(pFile);
        bmiHeader.biHeight = read_s32(pFile);
        bmiHeader.biPlanes = read_u16(pFile);
        bmiHeader.biBitCount = read_u16(pFile);
        bmiHeader.biCompression = read_u32(pFile);
        bmiHeader.biSizeImage = read_u32(pFile);
        bmiHeader.biXPelsPerMeter = read_s32(pFile);
        bmiHeader.biYPelsPerMeter = read_s32(pFile);
        bmiHeader.biClrUsed = read_u32(pFile);
        bmiHeader.biClrImportant = read_u32(pFile);


        RGBQUAD** rgb = new RGBQUAD * [bmiHeader.biWidth];
        for (int i = 0; i < bmiHeader.biWidth; i++) {
            rgb[i] = new RGBQUAD[bmiHeader.biHeight];
        }

        for (int i = 0; i < bmiHeader.biWidth; i++) {
            for (int j = 0; j < bmiHeader.biHeight; j++) {
                rgb[i][j].rgbBlue = getc(pFile);
                rgb[i][j].rgbGreen = getc(pFile);
                rgb[i][j].rgbRed = getc(pFile);
            }

            // ���������� ��������� ���� � ������
            getc(pFile);
        }

        my_rgb = rgb;
    }

};