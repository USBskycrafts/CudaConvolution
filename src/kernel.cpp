#include "kernel.hpp"
#include <opencv2/core/hal/interface.h>

static Mat GenerateLaplacian() {
  static float data[]{0, -1, 0, -1, 4, -1, 0, -1, 0};
  return Mat(3, 3, CV_64FC1, data);
}

static Mat GenerateGaussian() {
  static float data[]{1.0 / 16, 2.0 / 16, 1.0 / 16, 2.0 / 16, 4.0 / 16,
                      2.0 / 16, 1.0 / 16, 2.0 / 16, 1.0 / 16};
  return Mat(3, 3, CV_64FC1, data);
}

static Mat GenerateAverage() {
  static float data[]{1.0 / 9, 1.0 / 9, 1.0 / 9, 1.0 / 9, 1.0 / 9,
                      1.0 / 9, 1.0 / 9, 1.0 / 9, 1.0 / 9};
  return Mat(3, 3, CV_64FC1, data);
}

Mat GenerateKernel(char *type) {
  std::string type_(type);
  to_lower(type_);
  if (type_ == "laplacian") {
    return GenerateLaplacian();
  } else if (type_ == "gaussian") {
    return GenerateGaussian();
  } else if (type_ == "average") {
    return GenerateAverage();
  } else {
    std::cerr << "invalid type of kernel, valid types are gaussian, laplacian, "
                 "average"
              << std::endl;
    exit(-1);
  }
}
