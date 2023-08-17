#pragma once

#include <opencv2/imgcodecs.hpp>

using namespace cv;

void convolution(Mat &image, Mat &kernel);


void convolution_seq(Mat &image, Mat &kernel);