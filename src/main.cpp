#include "convolution.hpp"
#include "kernel.hpp"
#include "race.hpp"

#include <algorithm>
#include <iostream>
#include <opencv2/core/hal/interface.h>
#include <opencv2/opencv.hpp>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int opt;
  bool is_directedly_show = true;
  char *output_path;
  char *kernel_type;
  if (argc <= 1) {
    std::cerr << "please offer a picture" << std::endl;
    exit(-1);
  }
  while ((opt = getopt(argc, argv, "o:t:")) != -1) {
    switch (opt) {
    case 'o': {
      std::cerr << "the output picture is " << optarg << std::endl;
      is_directedly_show = false;
      output_path = optarg;
      break;
    }
    case 't': {
      kernel_type = optarg;
      std::cerr << "kernel type is " << optarg << std::endl;
    }
    default: {
      break;
    }
    }
  }
  auto path = argv[optind];
  std::cerr << "picture path is " << path << std::endl;
  auto image = imread(path, IMREAD_GRAYSCALE);
  //  picture transfer
  image.convertTo(image, CV_32FC1);
  image /= 255.0;
  // generate the kernel
  auto kernel = GenerateKernel(kernel_type);
  // do the convolution
  convolution(image, kernel);

  TEST(convolution_seq(image, kernel));
  
  // denormalize
  image *= 255.0;
  image.convertTo(image, CV_8UC1);
  // generate output
  if (is_directedly_show) {
    imshow("output", image);
  } else {
    imwrite(output_path, image);
  }
  return 0;
}