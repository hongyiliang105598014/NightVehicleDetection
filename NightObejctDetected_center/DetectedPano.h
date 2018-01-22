#pragma once
#include "DetectedPosition.h"
class DetectedPano : public DetectedPosition
{
public:
	DetectedPano(string path);
	//void SetBackImageProcessor(ImageProcessor *imageProcessor);
	//void SetSideImageProcessor(ImageProcessor *imageProcessor);
	void SetImageProcessor(ImageProcessor *imageProcessor);
	~DetectedPano();
	void run();
	void setBackImageProcessorResult(Mat res);
	void setRightImageProcessorResult(Mat res);
private:
	Mat gray;
	Mat backResult;
	Mat rightResult;
	Rect backPosition;
	Rect rightPosition;
	Rect leftPosition;
	ImageProcessor *backImageProcessor;
	ImageProcessor *rightImageProcessor;
	vector<Rect> backROIs;
	vector<Rect> rightROIs;
};
