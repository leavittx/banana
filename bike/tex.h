/* Lev Panov, 11-3, 2009 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <GL/glut.h>

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned long dword;

#pragma pack(push, 2)
typedef struct
{
  word Sign;
  dword Fsize;
  dword Reserved;
  dword Offset;
  dword Size;
  dword W, H;
  word Planes;
  word Bits;
  dword Compr;
  dword Isize;
  dword XRes, YRes;
  dword ColorUsed, ColorImp;
} BMP;
#pragma pack(pop)

typedef byte RGB[3];

void CheckerTexSet( int tex );
void G24_Tex_Load( char *FileName, int tex );
void BMP_Tex_Load( char *FileName, int tex );
