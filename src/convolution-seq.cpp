#include "convolution.hpp"
#include <opencv2/core/hal/interface.h>

void convolution_seq(Mat &image, Mat &kernel) {
  Mat output(image.rows, image.cols, CV_32FC1);
  for (int y = 0; y < image.rows; y++) {
    for (int x = 0; x < image.cols; x++) {
      auto radio = kernel.rows / 2;
      assert(kernel.rows == kernel.cols && kernel.rows % 2);
      for (int i = 0; i < kernel.rows; i++) {
        for (int j = 0; j < kernel.cols; j++) {
          output.at<float>(i, j) = 0.0;
          if (y + (i - radio) >= 0 && y + (i - radio) < image.rows &&
              x + (j - radio) >= 0 && x + (j - radio) < image.cols) {
            output.at<float>(i, j) +=
                kernel.at<float>(i, j) *
                image.at<float>(y + i - radio, x + i - radio);
          }
        }
      }
    }
  }
  image = output;
}