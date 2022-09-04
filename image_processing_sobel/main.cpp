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

double sobel_x_kernel[3][3] = {
    {-1.0, 0.0, 1.0},
    {-2.0, 0.0, 2.0},
    {-1.0, 0.0, 1.0}};

double sobel_y_kernel[3][3] = {
    {1.0, 2.0, 1.0},
    {0.0, 0.0, 0.0},
    {-1.0, -2.0, -1.0}};

/*
double** gaussian_filter_creation(double sigma, int dimension) {
  double **matriz_kernel;
  int x_d, y_d;
  x_d = y_d = -(dimension / 2);

  double r, sum = 0.0, s = 2.0 * sigma * sigma;

  matriz_kernel = new double *[dimension];

  for(int i = 0; i < dimension; i++) matriz_kernel[i] = new double [dimension];

  for (int x = x_d; x <= (x_d * -1); x++) {
    for (int y = y_d; y <= (y_d * -1); y++) {
      r = sqrt(x * x + y * y);
      matriz_kernel[x + (x_d * -1)][y + (y_d * -1)] = (exp(-(r * r) / s)) / (M_PI * s);
      sum += matriz_kernel[x + (x_d * -1)][y + (y_d * -1)];
    }
  }

  for(int i = 0; i < dimension; i++) {
    for(int j = 0; j < dimension; j++) {
      matriz_kernel[i][j] /= sum;
    }
  }

  return matriz_kernel;
}
*/

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

  return pixel;
}

RGB **matrix_gaussian_filter(RGB **matrix_image, int h, int w, int d)
{
  for (int i = 0; i < h; i++)
  {
    for (int j = 0; j < w; j++)
    {
      matrix_image[i][j] = convolution(i, j, d, matrix_image, gaussian_kernel, w, h);
    }
  }

  return matrix_image;
}

RGB **matrix_sobelx_filter(RGB **matrix_image, RGB **matrix_sobel_x, int h, int w, int d)
{
  // copy image
  for (int i = 0; i < h; i++)
  {
    for (int j = 0; j < w; j++)
    {
      matrix_sobel_x[i][j].r = matrix_image[i][j].r;
      matrix_sobel_x[i][j].g = matrix_image[i][j].g;
      matrix_sobel_x[i][j].b = matrix_image[i][j].b;
    }
  }

  for (int i = 0; i < h; i++)
  {
    for (int j = 0; j < w; j++)
    {
      matrix_sobel_x[i][j] = convolution(i, j, d, matrix_image, sobel_x_kernel, w, h);
    }
  }

  return matrix_sobel_x;
}

RGB **matrix_sobely_filter(RGB **matrix_image, RGB **matrix_sobel_y, int h, int w, int d)
{
  // copy image
  for (int i = 0; i < h; i++)
  {
    for (int j = 0; j < w; j++)
    {
      matrix_sobel_y[i][j].r = matrix_image[i][j].r;
      matrix_sobel_y[i][j].g = matrix_image[i][j].g;
      matrix_sobel_y[i][j].b = matrix_image[i][j].b;
    }
  }

  for (int i = 0; i < h; i++)
  {
    for (int j = 0; j < w; j++)
    {
      matrix_sobel_y[i][j] = convolution(i, j, d, matrix_image, sobel_y_kernel, w, h);
    }
  }

  return matrix_sobel_y;
}

RGB **matrix_sobel_x_sqr(RGB **matrix_sobel_x, int h, int w)
{
  for (int i = 0; i < h; i++)
  {
    for (int j = 0; j < w; j++)
    {
      matrix_sobel_x[i][j].r = matrix_sobel_x[i][j].r * matrix_sobel_x[i][j].r;
      matrix_sobel_x[i][j].g = matrix_sobel_x[i][j].g * matrix_sobel_x[i][j].g;
      matrix_sobel_x[i][j].b = matrix_sobel_x[i][j].b * matrix_sobel_x[i][j].b;
    }
  }

  return matrix_sobel_x;
}

RGB **matrix_sobel_y_sqr(RGB **matrix_sobel_y, int h, int w)
{
  for (int i = 0; i < h; i++)
  {
    for (int j = 0; j < w; j++)
    {
      matrix_sobel_y[i][j].r = matrix_sobel_y[i][j].r * matrix_sobel_y[i][j].r;
      matrix_sobel_y[i][j].g = matrix_sobel_y[i][j].g * matrix_sobel_y[i][j].g;
      matrix_sobel_y[i][j].b = matrix_sobel_y[i][j].b * matrix_sobel_y[i][j].b;
    }
  }

  return matrix_sobel_y;
}

RGB **sum_matrix(RGB **matrix_image, RGB **matrix_sobel_x, RGB **matrix_sobel_y, int h, int w)
{
  for (int i = 0; i < h; i++)
  {
    for (int j = 0; j < w; j++)
    {
      matrix_image[i][j].r = matrix_sobel_x[i][j].r + matrix_sobel_y[i][j].r;
      matrix_image[i][j].g = matrix_sobel_x[i][j].g + matrix_sobel_y[i][j].g;
      matrix_image[i][j].b = matrix_sobel_x[i][j].b + matrix_sobel_y[i][j].b;
    }
  }

  return matrix_image;
}

RGB **sqrt_matrix(RGB **matrix_image, int h, int w)
{
  for (int i = 0; i < h; i++)
  {
    for (int j = 0; j < w; j++)
    {
      matrix_image[i][j].r = sqrt(matrix_image[i][j].r);
      matrix_image[i][j].g = sqrt(matrix_image[i][j].g);
      matrix_image[i][j].b = sqrt(matrix_image[i][j].b);
    }
  }

  return matrix_image;
}

RGB **threshold_matrix_image(RGB **matrix_image, int h, int w, double threshold)
{
  for (int i = 0; i < h; i++)
  {
    for (int j = 0; j < w; j++)
    {
      matrix_image[i][j].r = matrix_image[i][j].r > threshold ? 255 : 0;
      matrix_image[i][j].g = matrix_image[i][j].g > threshold ? 255 : 0;
      matrix_image[i][j].b = matrix_image[i][j].b > threshold ? 255 : 0;
    }
  }

  return matrix_image;
}

int main()
{
  // double **gaussian_filter;

  RGB **matrix_image;
  RGB **matrix_sobel_x;
  RGB **matrix_sobel_y;

  int d = 3; // dimension

  double threshold = 80.0;

  int w, h, chan;

  unsigned char *rgb_image = stbi_load("lena.png", &w, &h, &chan, 3);

  // gaussian_filter = gaussian_filter_creation(1.0, d);

  matrix_image = new RGB *[h];
  matrix_sobel_x = new RGB *[h];
  matrix_sobel_y = new RGB *[h];

  for (int i = 0; i < h; i++)
  {
    matrix_image[i] = new RGB[w];
    matrix_sobel_x[i] = new RGB[w];
    matrix_sobel_y[i] = new RGB[w];
  }

  for (int i = 0; i < h; i++)
  {
    for (int j = 0; j < w; j++)
    {
      matrix_image[i][j].r = rgb_image[j * 3 + w * i * 3];
      matrix_image[i][j].g = rgb_image[j * 3 + w * i * 3 + 1];
      matrix_image[i][j].b = rgb_image[j * 3 + w * i * 3 + 2];
    }
  }

  matrix_image = matrix_gaussian_filter(matrix_image, h, w, d);

  matrix_sobel_x = matrix_sobelx_filter(matrix_image, matrix_sobel_x, h, w, d);
  matrix_sobel_y = matrix_sobely_filter(matrix_image, matrix_sobel_y, h, w, d);

  matrix_sobel_x = matrix_sobel_x_sqr(matrix_sobel_x, h, w);
  matrix_sobel_y = matrix_sobel_y_sqr(matrix_sobel_y, h, w);

  matrix_image = sum_matrix(matrix_image, matrix_sobel_x, matrix_sobel_y, h, w);
  matrix_image = sqrt_matrix(matrix_image, h, w);

  matrix_image = threshold_matrix_image(matrix_image, h, w, threshold);

  for (int i = 0; i < h; i++)
  {
    for (int j = 0; j < w; j++)
    {
      rgb_image[j * 3 + w * i * 3] = matrix_image[i][j].r;
      rgb_image[j * 3 + w * i * 3 + 1] = matrix_image[i][j].g;
      rgb_image[j * 3 + w * i * 3 + 2] = matrix_image[i][j].b;
    }
  }

  stbi_write_png("lena_teste2.png", w, h, chan, rgb_image, w * 3);

  stbi_image_free(rgb_image);

  return 0;
}
