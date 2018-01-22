#pragma once
#include "DetectedPosition.h"
class DetectedPano : public DetectedPosition
{
public:
	DetectedPano(string path);
	void SetImageProcessor(ImageProcessor *imageProcessor);
	~DetectedPano();
	void run();
private:
	Rect backPosition;
	Rect rightPosition;
	Rect leftPosition;
	ImageProcessor *processor;
};
