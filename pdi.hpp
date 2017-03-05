#ifndef PDI_H
#define PDI_H

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <algorithm>

using namespace cv;

Mat filter (const Mat &img, const Mat &mask);
Mat filterRGB (const Mat &img, const Mat &mask);
Mat filterGray (const Mat &img, const Mat &mask);

Mat rgb2yiq2rgb (const Mat &img);
Mat rgb2yiq (const Mat &img);
Mat yiq2rgb (const Mat &img);
Mat split (const Mat &img, int channel, bool mono = false);
Mat merge (const Mat &img_R, const Mat &img_G, const Mat &img_B);
uchar mean_y(const Mat &img);

Mat negative (const Mat &img, bool rgb = true);
Mat negative_rgb (const Mat &img);
Mat negative_y (const Mat &img);
Mat addBrightness (const Mat &img, int c);
Mat mulBrightness (const Mat &img, float c, bool rgb = true);
Mat mulBrightness_rgb (const Mat &img, float c);
Mat mulBrightness_y (const Mat &img, float c);
Mat thresholding(const Mat &img, uchar threshold, int type, bool mean = true);

Mat meanFilter(const Mat &img, uint size);
Mat medianFilter(const Mat &img, uint size);
Mat medianFilterGray(const Mat &img, uint size);
Mat medianFilterRGB	(const Mat &img, uint size);
Mat sobelFilter(const Mat &img, int type);
Mat laplacianFilter(const Mat &img, int type);


#endif