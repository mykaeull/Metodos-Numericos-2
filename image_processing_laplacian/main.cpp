#include <iostream>
#include <cmath>
#include <iomanip>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"

using namespace std;

struct RGB
{
  int r, g, b;
};

double gaussian_kernel[3][3] = {
    {1 / 16., 2 / 16., 1 / 16.},
    {2 / 16., 4 / 16., 2 / 16.},
    {1 / 16., 2 / 16., 1 / 16.}};

double laplacian_kernel[3][3] = {
    {0., 1., 0.},
    {1., -4., 1.},
    {0., 1., 0.}};

RGB convolution(int i, int j, int d, RGB **src, double kernel[3][3], int w, int h)
{
  RGB pixel = {0, 0, 0};

  for (int y = 0; y < d; y++)
  {
    for (int x = 0; x < d; x++)
    {
      pixel.r += src[(i - (d / 2) + y + h) % h][(j - (d / 2) + x + w) % w].r * kernel[y][x];
      pixel.g += src[(i - (d / 2) + y + h) % h][(j - (d / 2) + x + w) % w].g * kernel[y][x];
      pixel.b += src[(i - (d / 2) + y + h) % h][(j - (d / 2) + x + w) % w].b * kernel[y][x];
    }
  }

  pixel.r = abs(pixel.r);
  pixel.g = abs(pixel.g);
  pixel.b = abs(pixel.b);

  pixel.r = min(pixel.r, 255);
  pixel.g = min(pixel.g, 255);
  pixel.b = min(pixel.b, 255);

  return pixel;
}

void matrix_gaussian_filter(RGB **matrix_image, RGB **matrix_image_nova, int h, int w, int d)
{
  for (int i = 0; i < h; i++)
  {
    for (int j = 0; j < w; j++)
    {
      matrix_image_nova[i][j] = convolution(i, j, d, matrix_image, gaussian_kernel, w, h);
    }
  }
}

void matrix_laplacian(RGB **matrix_image, RGB **matrix_image_nova, int h, int w, int d)
{
  for (int i = 0; i < h; i++)
  {
    for (int j = 0; j < w; j++)
    {
      matrix_image_nova[i][j] = convolution(i, j, d, matrix_image, laplacian_kernel, w, h);
    }
  }

  /*
  for(int i = 0; i < h; i++) {
    for(int j = 0; j < w; j++) {
      matrix_image_nova[i][j].r = matrix_image_nova[i][j].r > 80.0 ? 255 : 0;
      matrix_image_nova[i][j].g = matrix_image_nova[i][j].g > 80.0 ? 255 : 0;
      matrix_image_nova[i][j].b = matrix_image_nova[i][j].b > 80.0 ? 255 : 0;
    }
  }
  */
}

int main()
{
  RGB pixel;

  RGB **matrix_image;
  RGB **matrix_new_image;
  RGB **matrix_new_image2;

  int d = 3; // dimension

  int w, h, chan;

  unsigned char *rgb_image = stbi_load("lena.png", &w, &h, &chan, 3);

  matrix_image = new RGB *[h];
  matrix_new_image = new RGB *[h];
  matrix_new_image2 = new RGB *[h];

  for (int i = 0; i < h; i++)
  {
    matrix_image[i] = new RGB[w];
    matrix_new_image[i] = new RGB[w];
    matrix_new_image2[i] = new RGB[w];
  }

  for (int i = 0; i < h; i++)
  {
    for (int j = 0; j < w; j++)
    {
      matrix_image[i][j].r = (int)rgb_image[j * 3 + w * i * 3];
      matrix_image[i][j].g = (int)rgb_image[j * 3 + w * i * 3 + 1];
      matrix_image[i][j].b = (int)rgb_image[j * 3 + w * i * 3 + 2];
    }
  }

  matrix_gaussian_filter(matrix_image, matrix_new_image, h, w, d);

  matrix_laplacian(matrix_new_image, matrix_new_image2, h, w, d);

  for (int i = 0; i < h; i++)
  {
    for (int j = 0; j < w; j++)
    {
      rgb_image[j * 3 + w * i * 3] = (unsigned char)matrix_new_image2[i][j].r;
      rgb_image[j * 3 + w * i * 3 + 1] = (unsigned char)matrix_new_image2[i][j].g;
      rgb_image[j * 3 + w * i * 3 + 2] = (unsigned char)matrix_new_image2[i][j].b;
    }
  }

  stbi_write_png("lena_teste1.png", w, h, chan, rgb_image, w * 3);

  stbi_image_free(rgb_image);

  return 0;
}
