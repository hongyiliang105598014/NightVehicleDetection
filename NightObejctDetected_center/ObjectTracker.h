#pragma once
#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <cstring>

using namespace cv;
using namespace std;
class ObjectTracker
{
public:
	ObjectTracker();
	~ObjectTracker();
	void initialize(Rect2d startPos, Mat& srcImg);
	void update(Mat& srcImg);
	Rect2d getCurrentPos();
	Rect2d _headLight;
	void setCurrentPosOffsetForPano(int offsetX, int offsetY);
	void addObjectContain();
	int getNumberOfObjectContain();
	void clearObjectContain();
	void setFrameCount(int count);
	int getFrameCount();
	bool IsOverlapping = false;
	void setID(int id);
	int getID();
	double Distance = -1;
private:
	string _trackingAlg = "KCF";
	Ptr<Tracker> _tracker;
	int _objectContainCount = 0;
	int _frameCount = 0;
	int _id = 0;
	
};