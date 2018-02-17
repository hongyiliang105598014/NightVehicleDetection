#pragma once
#include "DetectedPosition.h"
class DetectedPano : public DetectedPosition
{
public:
	DetectedPano(string path);
	~DetectedPano();
	void SetImageProcessor(ImageProcessor *imageProcessor);
	void checkOverlappingRegion(Mat right, Mat back, Mat left);
	void run();
	static bool sortByRightPosition(const ObjectTracker &a, const ObjectTracker &b);
	static bool sortByLeftPosition(const ObjectTracker &a, const ObjectTracker &b);
	static bool sortByBackPosition(const ObjectTracker &a, const ObjectTracker &b);
private:
	Mat gray;
	Mat backResult;
	Mat rightResult;
	Rect backPosition;
	Rect rightPosition;
	Rect leftPosition;
	ImageProcessor *backImageProcessor;
	ImageProcessor *rightImageProcessor;
	ImageProcessor *leftImageProcessor;
	vector<Rect> backROIs;
	vector<Rect> rightROIs;
	vector<Rect> leftROIs;
};
