#pragma once
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <vector>
#include <algorithm>
#include "HeadLightManager.h"
#include "ObjectDetected.h"
#include "ImageProcessor.h"

using namespace cv;
using namespace std;

class BackImageProcessor : public ImageProcessor
{
public:
	BackImageProcessor();
	~BackImageProcessor();
	void threshold_hist(Mat& src);
	void removeNoice(Mat &src, int Eheight, int Ewidth, int Dheight, int Dwidth);
	void detectLight(Mat& srcImg, Mat binaryImg, int offsetX, int offsetY, vector<Rect> ROIs);
	int thresholdValue(Mat& src);
	void extractEfficientImage(Mat& src);
	void setHeadLightManager(HeadLightManager headLightManager);
	vector<ObjectDetected> getObjectDetectedVector();
	bool isCarLightHeightDiffYCorrect(int diffY, int distance);
	vector<ObjectTracker> getVectorOfObjectTracker();
	void calcDistance(Mat& srcImg);
	int vehicleID = 1;
	double getMinDistance();
private:
	vector<ObjectDetected> ObjectDetectedVector;
	static bool compareDistance(const ObjectDetected &a, const ObjectDetected &b);
	int previousThresholdValue = 0;
	HeadLightManager _headLightManager;
	vector<double> distanceList[5];
	double avgDistance[5] = {0};
	double minDistance = 999;
};