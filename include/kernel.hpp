#pragma once

#include <boost/algorithm/string.hpp>
#include <iostream>
#include <map>
#include <opencv2/opencv.hpp>
#include <string>

using namespace boost::algorithm;
using namespace cv;

enum KernelType { GAUSSIAN = 0, LAPLACIAN, AVERAGE };

Mat GenerateKernel(char *type);