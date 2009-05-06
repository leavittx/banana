/* Lev Panov, 11-3, 2009 */

#include "tex.h"

byte Buf[10000];

void CheckerTexSet( int tex )
{
  float checker[2][2][3] = {
    {{0, 0, 0}, {0, 1, 0}},
    {{0, 1, 0}, {0, 0, 1}} };

    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, 2, 2, 0, GL_BGR_EXT, GL_FLOAT, checker);
}

void G24_Tex_Load( char *FileName, int tex )
{
  FILE *F;
  void *mem;
  int W = 0, H = 0;
  
  if ((F = fopen(FileName, "rb")) == NULL)
    exit(0);

  if (fread(&W, 2, 1, F) != 1)
      printf("Something wrong with G24 image\n");
  if (fread(&H, 2, 1, F) != 1)
      printf("Something wrong with G24 image\n");

  if ((mem = malloc(3 * W * H)) == NULL)
    exit(EXIT_SUCCESS);
  if (fread(mem, 3, W * H, F) != W * H)
      printf("Something wrong with G24 image\n");

  fclose(F);
  
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glTexImage2D(GL_TEXTURE_2D, 0, 3, W, H, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, mem);
}

void BMP_Tex_Load( char *FileName, int tex )
{
  FILE *F;
//  void *mem;
  int x, y, v, c, r;
  BMP bmp;
  static byte pal[256][4];
  int bpl, cols;
  RGB image[129][129];

  F = fopen(FileName, "rb");
  if (!F)
  {
    printf("Error while opening file \"%s\"!\n", FileName);
    exit(EXIT_SUCCESS);
  }

  if (fread(&bmp, sizeof(BMP), 1, F) != 1)
      printf("Something wrong with BMP image\n");
  if (bmp.Planes != 1 || bmp.Reserved ||
    bmp.Size != 40 || bmp.Sign != ('M' << 8) + 'B')
  {
    printf("\"%s\" isn't valid BMP file!", FileName);
    exit(EXIT_SUCCESS);
  }

  //mem = malloc(bmp.W * bmp.H * sizeof(RGB *));
  //image = malloc(bmp.W * bmp.H * sizeof(RGB *));
  //if (image == NULL)
  //  exit(EXIT_SUCCESS);

  switch (bmp.Bits)
  {
  case 1:
    bpl = ((((int)bmp.W + 7) / 8) + 3) / 4 * 4;
    cols = 2;
    break;
  case 4:
    bpl = (((int)bmp.W + 1) / 2 + 3) / 4 * 4;
    cols = 16;
    break;
  case 8:
    bpl = ((int)bmp.W + 3) / 4 * 4;
    cols = 256;
    break;
  case 24:
    bpl = ((int)bmp.W * 3 + 3) / 4 * 4;
    cols = 0;
    break;
  default:
    cols = (int)bmp.ColorUsed;
    break;
  }

  if (cols)
    if (fread(pal, 4, cols, F) != cols)
        printf("Something wrong with BMP image\n");

  for (y = (int)bmp.H; y >= 0; y --)
  {
    if (fread(Buf, bpl, 1, F) != 1)
        printf("Something wrong with BMP image\n");
    for (x = 0; x < bmp.W; x ++)
    {
      switch (bmp.Bits)
      {
      case 1:
        v = (Buf[x / 8] >> (7 - x % 8)) & 1;
        break;
      case 4:
        v = (Buf[x / 2] >> (4 * (1 - x % 2))) & 15;
        break;
      case 8:
        v = Buf[x];
        break;
      case 24:
        for (c = 0; c < 3; c ++)
          //pic->Rows[y][x][c] = Buf[x * 3 + c];
          image[y][x][c] = Buf[x * 3 + c];
        break;
      default:
        break;
      }
      if (bmp.Bits != 24)
        for (c = 0; c < 3; c ++)
          image[y][x][c] = pal[v][c];
    }
  }
  
  fclose(F);

  glBindTexture(GL_TEXTURE_2D, tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glPixelStorei(GL_PACK_ALIGNMENT, 4);
  glTexImage2D(GL_TEXTURE_2D, 0, 3, bmp.W, bmp.H, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, image);
}

/* Todo: fix bmp image loading */
#if 0
void BMP_Tex_Load( char *FileName, int tex )
{
  FILE *F;
  byte *mem;
  int x, y, v, c, r;
  BMP bmp;
  static byte pal[256][4];
  int bpl, cols;
//  RGB image[129][129];

  F = fopen(FileName, "rb");
  if (!F)
  {
    printf("Error while opening file \"%s\"!\n", FileName);
    exit(EXIT_SUCCESS);
  }

  fread(&bmp, sizeof(BMP), 1, F);
  if (bmp.Planes != 1 || bmp.Reserved ||
    bmp.Size != 40 || bmp.Sign != ('M' << 8) + 'B')
  {
    printf("\"%s\" isn't valid BMP file!", FileName);
    return;
    //exit(EXIT_SUCCESS);
  }

  mem = malloc(bmp.W * bmp.H * sizeof(byte) * 3);
  if (mem == NULL)
    exit(EXIT_SUCCESS);

  switch (bmp.Bits)
  {
  case 1:
    bpl = ((((int)bmp.W + 7) / 8) + 3) / 4 * 4;
    cols = 2;
    break;
  case 4:
    bpl = (((int)bmp.W + 1) / 2 + 3) / 4 * 4;
    cols = 16;
    break;
  case 8:
    bpl = ((int)bmp.W + 3) / 4 * 4;
    cols = 256;
    break;
  case 24:
    bpl = ((int)bmp.W * 3 + 3) / 4 * 4;
    cols = 0;
    break;
  default:
    cols = (int)bmp.ColorUsed;
    break;
  }

  if (cols)
    fread(pal, 4, cols, F);

  for (y = (int)bmp.H - 1; y >= 0; y --)
  {
    fread(Buf, bpl, 1, F);
    for (x = 0; x < bmp.W; x ++)
    {
      switch (bmp.Bits)
      {
      case 1:
        v = (Buf[x / 8] >> (7 - x % 8)) & 1;
        break;
      case 4:
        v = (Buf[x / 2] >> (4 * (1 - x % 2))) & 15;
        break;
      case 8:
        v = Buf[x];
        break; 
      case 24:
        for (c = 0; c < 3; c ++)
          //image[y][x][c] = Buf[x * 3 + c];
          *(mem + bmp.W * y + x + c) = Buf[x * 3 + c];
        break;
      default:
        break; 
      }
      if (bmp.Bits != 24)
        for (c = 0; c < 3; c ++)
          //image[y][x][c] = pal[v][c];
          *(mem + bmp.W * y + x + c) = pal[v][c];
    }
  }
  fclose(F);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  glTexImage2D(GL_TEXTURE_2D, 0, 3, bmp.W, bmp.H, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, mem);
  free(mem);
  /*
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(GL_TEXTURE_2D, 0, 3, bmp.W, bmp.H, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, image);
  */
}
#endif