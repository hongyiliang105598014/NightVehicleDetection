#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>
#include "RightImageProcessor.h"
#include "BackImageProcessor.h"
#include "LeftImageProcessor.h"
#include "DetectedRight.h"
#include "DetectedBack.h"
#include "DetectedPano.h"

using namespace std;
using namespace cv;

int main() {
	int mode = 3;
	string carBackPath1 = "D:/UserFolder/Desktop/haitec/video/car_back/華創鏡頭/car-str.mp4";
	string carBackPath2 = "D:/UserFolder/Desktop/haitec/video/car_back/華創鏡頭/car-curve.mp4";
	string carBackPath3 = "D:/UserFolder/Desktop/haitec/video/car_back/華創鏡頭/car-curve2.mp4";
	string carBackPath4 = "D:/UserFolder/Desktop/haitec/video/car_back/華創鏡頭/motor-str.mp4";
	string carBackPath5 = "D:/UserFolder/Desktop/haitec/video/car_back/華創鏡頭/motor-curve.mp4";
	string carBackPath6 = "D:/UserFolder/Desktop/haitec/video/car_back/華創鏡頭/distance-10-5.mp4";
	string carBackPath7 = "D:/UserFolder/Desktop/haitec/video/car_back/華創鏡頭/SJCAM-car-distance40-30.mp4";
	string carBackPath8 = "D:/UserFolder/Desktop/haitec/video/car_back/華創鏡頭/SJCAM-car-distance20-10-5.mp4";
	string carBackPath9 = "D:/UserFolder/Desktop/haitec/video/car_back/華創鏡頭/FILE0268.mp4";
	string carRightPath1 = "D:/UserFolder/Desktop/haitec/video/cut/highway1.avi";
	string carRightPath2 = "D:/UserFolder/Desktop/haitec/video/cut/highway2.avi";
	string carRightPath3 = "D:/UserFolder/Desktop/haitec/video/cut/lowbrightness1.avi";
	string carRightPath4 = "D:/UserFolder/Desktop/haitec/video/cut/lowbrightness2.avi";

	string carPanoPath1 = "../car_back_side_cut/cut_sunny/highway1.avi"; //frame 0 ~ 1000 city
	string carPanoPath2 = "../car_back_side_cut/cut_sunny/highway2.avi"; //frame 1410 highway
	string carPanoPath3 = "../car_back_side_cut/cut_sunny/highway3.avi"; //frame 1410 highway
	string carPanoPath4 = "../car_back_side_cut/cut_sunny/road1.avi";
	string carPanoPath5 = "../car_back_side_cut/dazi/9m.avi";
	string carPanoPath6 = "../car_back_side_cut/dazi/car1_rainy.avi";
	string carPanoPath7 = "../car_back_side_cut/dazi/motor1_rainy.avi";
	string carPanoPath8 = "../car_back_side_cut/distance_test/left.avi";
	string carPanoPath9 = "../car_back_side_cut/distance_test/right1.avi";

	string carPanoPath10 = "../car_back_side_cut/cut_sunny/neighu2_cut.mp4";
	string carPanoPath11 = "../car_back_side_cut/cut_sunny/neighu3_cut.mp4";
	string carPanoPath12 = "../car_back_side_cut/cut_sunny/neighu4_cut.mp4";

	if (mode == 0)
	{
		DetectedPosition *detectedPosition = new DetectedBack(carBackPath1);
		BackImageProcessor *backImageProcessor = new BackImageProcessor();
		detectedPosition->SetImageProcessor(backImageProcessor);
		detectedPosition->run();
	}
	else if (mode == 1)
	{
		DetectedPosition *detectedPosition = new DetectedRight(carPanoPath10);
		RightImageProcessor *rightImageProcessor = new RightImageProcessor();
		detectedPosition->SetImageProcessor(rightImageProcessor);
		detectedPosition->run();
	}
	/*else if (mode == 2)
	{
		DetectedPosition *detectedPosition = new DetectedLeft(carPanoPath12);
		LeftImageProcessor *leftImageProcessor = new LeftImageProcessor();
		detectedPosition->SetImageProcessor(leftImageProcessor);
		detectedPosition->run();
	}*/
	else if (mode == 3)
	{
		DetectedPosition *detectedPosition = new DetectedPano(carPanoPath11);
		ImageProcessor *backImageProcessor = new BackImageProcessor();
		ImageProcessor *rightImageProcessor = new RightImageProcessor();
		ImageProcessor *leftImageProcessor = new LeftImageProcessor();
		detectedPosition->SetImageProcessor(backImageProcessor);
		detectedPosition->SetImageProcessor(rightImageProcessor);
		detectedPosition->SetImageProcessor(leftImageProcessor);
		detectedPosition->run();
	}
	return 0;
}