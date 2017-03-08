#include "pdi.hpp"

Matx33f rgb2yiq_mat (0.299f,  0.587f,  0.114f,
					 0.596f, -0.274f, -0.322f,
					 0.211f, -0.523f,  0.312f);

Matx33f yiq2rgb_mat(1.0f,  0.956f,  0.621f,
					1.0f, -0.272f, -0.647f,
					1.0f, -1.106f,  1.703f);

// 1.1
Mat rgb2yiq2rgb (const Mat &img){

	return yiq2rgb(rgb2yiq(img));
}

Mat rgb2yiq (const Mat &img){

	Mat img_out(img.size(), img.type()); 
	img_out = img.clone();
	img_out.convertTo(img_out, CV_32F);

	for(int j = 0; j < img.rows; j++){
		for(int i = 0; i < img.cols * 3; i += 3){

			Vec3f pixel(img_out.at<float>(j, i+2), img_out.at<float>(j, i+1), img_out.at<float>(j, i));
			pixel = rgb2yiq_mat * pixel;

			
			for(int k = 0; k < 3; k++){
				// if(pixel[k] > 255.0f) pixel[k] = 255.0f;
				// else if(pixel[k] < 0.0f) pixel[k] = 0.0f;

				img_out.at<float>(j, i+2-k) = pixel[k];
			}
		}
	}

	// img_out.convertTo(img_out, CV_8UC3);

	return img_out;
}

Mat yiq2rgb (const Mat &img){

	Mat img_out(img.size(), img.type()); 
	img_out = img;
	img_out.convertTo(img_out, CV_32F);

	for(int j = 0; j < img.rows; j++){
		for(int i = 0; i < img.cols * 3; i += 3){

			Vec3f pixel(img_out.at<float>(j, i+2), img_out.at<float>(j, i+1), img_out.at<float>(j, i));
			pixel = yiq2rgb_mat * pixel;


			 for(int k = 0; k < 3; k++){
				if(pixel[k] > 255.0) pixel[k] = 255.0;
				else if(pixel[k] < 0.0) pixel[k] = 0.0;

				img_out.at<float>(j, i+2-k) = pixel[k];
			}
		}
	}

	img_out.convertTo(img_out, CV_8UC3);

	return img_out;
}

// 2.1
Mat split (const Mat &img, int channel, bool mono){
	
	int type;

	if(mono)
		type = CV_8UC1;
	else
		type = CV_8UC3;

	Mat img_out(img.size(), type);

	for(int j = 0; j < img.rows; j++){
		for(int i = 0; i < img.cols; i++){

			img_out.at<uchar>(j, i+2-channel + !mono*2*i + mono*(channel-2)) = img.at<uchar>(j, 3*i + 2-channel);
		}
	}

	return img_out;
}

Mat merge (const Mat &img_R, const Mat &img_G, const Mat &img_B){
	
	Mat img_out(img_R.size(), CV_8UC3);

	for(int j = 0; j < img_R.rows; j++){
		for(int i = 0; i < img_R.cols; i++){
			img_out.at<uchar>(j, 3*i+2) = img_R.at<uchar>(j, i);
			img_out.at<uchar>(j, 3*i+1) = img_G.at<uchar>(j, i);
			img_out.at<uchar>(j, 3*i) = img_B.at<uchar>(j, i);
		}
	}

	return img_out;
}

// 1.3
Mat negative (const Mat &img, bool rgb){

	if(rgb)
		return negative_rgb(img);
	else
		return negative_y(img);
}

// 1.3a
Mat negative_rgb(const Mat &img){

	bool color = true;
	if(img.type() == CV_32FC1 || img.type() == CV_8UC1)
		color = false;
		
	Mat img_out(img.size(), img.type());

	for(int j = 0; j < img.rows; j++){
		for(int i = 0; i < img.cols + color*img.cols*2; i++){

			img_out.at<uchar>(j, i) = 255 - img.at<uchar>(j, i);
		}
	}

	return img_out;
}

// 1.3b
Mat negative_y(const Mat &img){

	Mat img_out = rgb2yiq(img);

	for(int j = 0; j < img.rows; j++){
		for(int i = 2; i < 3*img.cols; i += 3){

			img_out.at<float>(j, i) = 255 - img_out.at<float>(j, i);
		}
	}

	return yiq2rgb(img_out);
}

// 1.4
Mat addBrightness (const Mat &img, int c){

	bool color = true;
	if(img.type() == CV_32FC1 || img.type() == CV_8UC1)
		color = false;

	Mat img_out(img.size(), img.type());

	for(int j = 0; j < img.rows; j++){
		for(int i = 0; i < img.cols + color*img.cols*2; i++){

			int pixel = img.at<uchar>(j, i) + c;

			if(pixel > 255) pixel = 255;
			else if(pixel < 0) pixel = 0;

			img_out.at<uchar>(j, i) = pixel;
		}
	}

	return img_out;
}

//1.5
Mat mulBrightness (const Mat &img, float c, bool rgb){
	if(rgb)
		return mulBrightness_rgb(img, c);
	else
		return mulBrightness_y(img, c);
}

// 1.5a
Mat mulBrightness_rgb (const Mat &img, float c){

	if(c < 0.0f) return img;

	bool color = true;
	if(img.type() == CV_32FC1 || img.type() == CV_8UC1)
		color = false;

	Mat img_out(img.size(), img.type());

	for(int j = 0; j < img.rows; j++){
		for(int i = 0; i < img.cols + color*img.cols*2; i++){

			float pixel = img.at<uchar>(j, i) * c;

			if(pixel > 255.0f) pixel = 255.0f;

			img_out.at<uchar>(j, i) = pixel;
		}
	}

	return img_out;
}

// 1.5b
Mat mulBrightness_y (const Mat &img, float c){

	if(c < 0.0f) return img;

	Mat img_out = rgb2yiq(img);

	for(int j = 0; j < img.rows; j++){
		for(int i = 2; i < img.cols * 3; i += 3){

			float pixel = img_out.at<float>(j, i) * c;

			if(pixel > 255.0f) pixel = 255.0f;

			img_out.at<float>(j, i) = pixel;
		}
	}

	return yiq2rgb(img_out);
}

// 1.6
Mat thresholding(const Mat &img, uchar threshold, int type, bool mean){

	Mat img_out;
	bool color = true;

	if(img.type() == CV_32FC1 || img.type() == CV_8UC1){
		color = false;
		img_out = img;
		return thresholdingGray(img_out, threshold, type, mean);
	}
	else{
		img_out = rgb2yiq(img);
		return thresholdingYIQ(img_out, float(threshold), type, mean);
	}
}

Mat thresholdingGray(const Mat &img, uchar threshold, int type, bool mean){

	Mat img_out = img;
	if(mean) threshold = mean_y_uchar(img_out);

	for(int j = 0; j < img.rows; j++){
		for(int i = 0; i < img.cols; i ++){
			if(type == 0)
				img_out.at<uchar>(j, i) = img_out.at<uchar>(j, i) > threshold ? 255 : 0;
			else if(type == 1)
				img_out.at<uchar>(j, i) = img_out.at<uchar>(j, i) > threshold ? 255 : img_out.at<uchar>(j, i);
			else
				img_out.at<uchar>(j, i) = img_out.at<uchar>(j, i) > threshold ? threshold : img_out.at<uchar>(j, i);
		}
	} 

	return img_out;
}

Mat thresholdingYIQ(const Mat &img, float threshold, int type, bool mean){

	Mat img_out = img;
	if(mean) threshold = mean_y_float(img_out);

	for(int j = 0; j < img.rows; j++){
		for(int i = 2; i < img.cols*3; i += 3){
			if(type == 0)
				img_out.at<float>(j, i) = img_out.at<float>(j, i) > threshold ? 255 : 0;
			else if(type == 1)
				img_out.at<float>(j, i) = img_out.at<float>(j, i) > threshold ? 255 : img_out.at<float>(j, i);
			else
				img_out.at<float>(j, i) = img_out.at<float>(j, i) > threshold ? threshold : img_out.at<float>(j, i);
		}
	} 

	return yiq2rgb(img_out);
}

uchar mean_y_uchar(const Mat &img){

	long mean = 0;

	for(int j = 0; j < img.rows; j++)
		for(int i = 0; i < img.cols; i ++)
			mean += img.at<uchar>(j, i);

	mean = mean / (img.rows * img.cols);
	return mean;
}

float mean_y_float(const Mat &img){

	float mean = 0;

	for(int j = 0; j < img.rows; j++)
		for(int i = 2; i < img.cols*3; i +=3)
			mean += img.at<float>(j, i);

	mean = mean / (img.rows * img.cols);
	return mean;
}

Mat meanFilter(const Mat &img, uint size){

	Mat mask = Mat::zeros(size, size, CV_32F);

	float den = 1 / float(size*size);

	for(int i = 0; i < size; i++)
		for(int j = 0; j < size; j++)
			mask.at<float>(i, j) = den;

	return filter(img, mask);
}

Mat medianFilterGray(const Mat &img, uint size){
	Mat img_out(img.size(), img.type()); 
	int raio = (size - 1)/2;
	int med = int(size)*int(size)/2;

	Mat img_pad;
	copyMakeBorder(img, img_pad, raio, raio, raio, raio, BORDER_REFLECT101);

	for(int j = raio; j < img.rows + raio; j++){

		for(int i = raio; i < img.cols + raio; i++){

			Rect region(i-raio, j-raio, size, size);
			Mat img_region = img_pad(region);

			std::vector<uchar> array(size*size);

			for(int k = 0; k < size; k++)
				for(int m = 0; m < size; m++)
					array[k*size + m] = img_region.at<uchar>(k, m);

			std::sort(array.begin(), array.end());

			img_out.at<uchar>(j-raio,i-raio) = array[med]; 
		}
	}

	return img_out;
}

Mat medianFilterRGB(const Mat &img, uint size){
	
	Mat channel[3];

	for(int i = 0; i < 3; i++){
		channel[i] = medianFilterGray(split(img, i, true), size);
	}

	return merge(channel[0], channel[1], channel[2]);
}

Mat medianFilter(const Mat &img, uint size){

	if(img.type() == CV_32FC3 || img.type() == CV_8UC3)
		return medianFilterRGB(img, size);

	if(img.type() == CV_32FC1 || img.type() == CV_8UC1)
		return medianFilterGray(img, size);
}

Mat filter(const Mat &img, const Mat &mask){

	if(img.type() == CV_32FC3 || img.type() == CV_8UC3)
		return filterRGB(img, mask);

	if(img.type() == CV_32FC1 || img.type() == CV_8UC1)
		return filterGray(img, mask);	
}

Mat filterGray (const Mat &img, const Mat &mask){

	Mat img_out(img.size(), img.type()); 
	int raio = (mask.rows - 1)/2;

	Mat img_pad;
	copyMakeBorder(img, img_pad, raio, raio, raio, raio, BORDER_REFLECT101);

	for(int j = raio; j < img.rows + raio; j++){

		for(int i = raio; i < img.cols + raio; i++){

			Rect region(i-raio, j-raio, mask.rows, mask.cols);
			Mat img_region = img_pad(region);

			img_region.convertTo(img_region, CV_32F);

			img_region = img_region.mul(mask);

			float pixel = sum(img_region)[0];

			if(pixel > 255){ pixel = 255;}
			if(pixel < 0){ pixel = 0;}

			img_out.at<uchar>(j-raio,i-raio) = pixel; 
		}
	}

	return img_out;
}

Mat filterRGB(const Mat &img, const Mat &mask){

	Mat channel[3];

	for(int i = 0; i < 3; i++){
		channel[i] = filterGray(split(img, i, true), mask);
	}

	return merge(channel[0], channel[1], channel[2]);
}

Mat sobelFilterHor(const Mat &img){

	Mat	mask  = (Mat_<float>(3, 3)<<
				-1,  0,  1,
				-2,  0,  2,
				-1,  0,  1);

	return filter(img, mask);
}

Mat sobelFilterVer(const Mat &img){

	Mat	mask  = (Mat_<float>(3, 3)<<
				 1,  2,  1,
				 0,  0,  0,
				-1, -2, -1);

	return filter(img, mask);
}

Mat sobelFilter(const Mat &img){

	Mat img_out = sobelFilterVer(img) + sobelFilterHor(img);

	bool color = true;
	if(img_out.type() == CV_32FC1 || img_out.type() == CV_8UC1)
		color = false;

	for(int j = 0; j < img_out.rows; j++){
		for(int i = 0; i < img_out.cols + color*img.cols*2; i++){

			uchar pixel = img_out.at<uchar>(j, i);

			if(pixel > 255) pixel = 255;
			else if(pixel < 0) pixel = 0;

			img_out.at<uchar>(j, i) = pixel;
		}
	}

	return img_out;
}

Mat laplacianFilterHor(const Mat &img){

	Mat	mask  = (Mat_<float>(3, 3)<<
				 0,  1,  0,
				 1, -4,  1,
				 0,  1,  0);

	return filter(img, mask);
}

Mat laplacianFilterVer(const Mat &img){

	Mat	mask  = (Mat_<float>(3, 3)<<
				 1,  1,  1,
				 1, -8,  1,
				 1,  1,  1);

	return filter(img, mask);
}

Mat laplacianFilter(const Mat &img){

	// Mat img_out = laplacianFilterVer(img) + laplacianFilterHor(img);

	// bool color = true;
	// if(img_out.type() == CV_32FC1 || img_out.type() == CV_8UC1)
	// 	color = false;

	// for(int j = 0; j < img_out.rows; j++){
	// 	for(int i = 0; i < img_out.cols + color*img.cols*2; i++){

	// 		uchar pixel = img_out.at<uchar>(j, i);

	// 		if(pixel > 255) pixel = 255;
	// 		else if(pixel < 0) pixel = 0;

	// 		img_out.at<uchar>(j, i) = pixel;
	// 	}
	// }

	// return img_out;
	
	Mat	mask  = (Mat_<float>(3, 3)<<
				 0,  1,  0,
				 1, -4,  1,
				 0,  1,  0);

	return filter(img, mask);
}

Mat noiseSaltPepperMono(const Mat &img, float density){

	// Mat black(img.size(), IPL_DEPTH_1U);
	// Mat white(img.size(), IPL_DEPTH_1U);
	uchar pixel;
	Mat out_img(img.size(), img.type());

	for(int j = 0; j < img.rows; j++){
		for(int i = 0; i < img.cols; i++){

			pixel = uchar(rand()%256);			
			out_img.at<uchar>(j, i) = pixel < uint(127*density) ? 0 : img.at<uchar>(j, i);
			out_img.at<uchar>(j, i) = pixel > uint(127*(2.0f-density)) ? 255 : out_img.at<uchar>(j, i);
		}
	}

	//randu(saltpepper_noise, 0, 255);

	// Mat black = saltpepper_noise < uint(127*density);
	// Mat white = saltpepper_noise > uint(127*(2.0-density));

	// Mat saltpepper_img = img.clone();

	// saltpepper_img.setTo(255, white);
	// saltpepper_img.setTo(0, black);

	return out_img;
}

Mat noiseSaltPepperRGB(const Mat &img, float density){

	Mat channel[3];

	for(int i = 0; i < 3; i++){
		channel[i] = noiseSaltPepperMono(split(img, i, true), density);
	}

	return merge(channel[0], channel[1], channel[2]);
	
}

Mat noiseSaltPepper(const Mat &img, float density){

	if(img.type() == CV_32FC3 || img.type() == CV_8UC3)
		return noiseSaltPepperRGB(img, density);

	if(img.type() == CV_32FC1 || img.type() == CV_8UC1)
		return noiseSaltPepperMono(img, density);
}