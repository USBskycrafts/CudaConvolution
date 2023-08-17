#include "convolution.hpp"
#include "race.hpp"
#include <cstdint>
#include <iostream>

#define BLOCK_SIZE 32
#define KERNEL_SIZE 3

static inline void copy_from_image(float *&p_image, Mat &image) {
  // p_image = (float *)malloc(image.rows * image.cols * sizeof(float));
  // for (int i = 0; i < image.rows; i++) {
  //   for (int j = 0; j < image.cols; j++) {
  //     p_image[i * image.cols + j] = image.at<float>(i, j);
  //   }
  // }
  p_image = (float *)image.data;
}

static inline void copy_to_image(Mat &image, float *p_image) {
  // for (int i = 0; i < image.rows; i++) {
  //   for (int j = 0; j < image.cols; j++) {
  //     image.at<float>(i, j) = p_image[i * image.cols + j];
  //   }
  // }
  image.data = (uchar *)p_image;
}

__global__ void convolution_kernel(float *image, uint16_t height,
                                   uint16_t width, float *kernel,
                                   float *o_image) {
  auto index = blockIdx.y * width + blockIdx.x * BLOCK_SIZE + threadIdx.x;
  auto radio = KERNEL_SIZE / 2;
  float sum = 0.0;
  for (int i = 0; i < KERNEL_SIZE; i++) {
    for (int j = 0; j < KERNEL_SIZE; j++) {
      int p = index + (i - radio) * width + (j - radio);
      if (p >= 0 && p < width * height) {
        sum += kernel[i * KERNEL_SIZE + j] * image[p];
      }
    }
  }
  o_image[index] = sum;
}

void convolution(Mat &image, Mat &kernel) {
  float *p_image = nullptr;
  float *p_kernel = nullptr;
  copy_from_image(p_image, image);
  copy_from_image(p_kernel, kernel);
  assert(kernel.cols == kernel.rows && kernel.cols % 2);
  float *k_image = nullptr;
  float *k_kernel = nullptr;
  float *o_image = nullptr;

  // copy to device
  cudaMalloc((void **)&k_image, image.cols * image.rows * sizeof(float));
  cudaMalloc((void **)&k_kernel, kernel.cols * kernel.rows * sizeof(float));
  cudaMalloc((void **)&o_image, image.cols * image.rows * sizeof(float));
  cudaMemcpy(k_image, p_image, image.rows * image.cols * sizeof(float),
             cudaMemcpyHostToDevice);
  cudaMemcpy(k_kernel, p_kernel, kernel.cols * kernel.rows * sizeof(float),
             cudaMemcpyHostToDevice);

  // do convolution
  auto grid_size = dim3(image.cols / BLOCK_SIZE + 1, image.rows);
  int start = clock();
  convolution_kernel<<<grid_size, BLOCK_SIZE>>>(k_image, image.rows, image.cols,
                                                k_kernel, o_image);
  int end = clock();
  std::cerr << "running time of convolution is " << (end - start) << "us" << std::endl;
  cudaDeviceSynchronize();
  std::cerr << cudaGetErrorString(cudaGetLastError()) << std::endl;
  // copy to host
  cudaMemcpy(p_image, o_image, image.rows * image.cols * sizeof(float),
             cudaMemcpyDeviceToHost);
  copy_to_image(image, p_image);
  cudaFree(k_image);
  cudaFree(k_kernel);
  cudaFree(o_image);
}
#undef BLOCK_SIZE