#include "pdi.hpp"
#include <string>

int main (int argc, char *argv[]){

	if(argc < 2)
		return 1;

	int *filter_list = new int[argc-1];

	for(int i = 0; i < argc-1; i++)
		filter_list[i] = std::strtol(argv[i], NULL, 10);

	
	//Mat img   = imread("./original/pb.jpg", CV_LOAD_IMAGE_GRAYSCALE); 
	Mat img   = imread("./original/bioshock.jpg", CV_LOAD_IMAGE_COLOR); 
	
	Mat img_out  = img;
	IplImage output = img_out;

	namedWindow("Janela", CV_WINDOW_NORMAL);
	setWindowProperty("Janela", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
 	cvShowImage("Janela", static_cast<CvArr*>(&output));

	waitKey(0);

	return 0;
}