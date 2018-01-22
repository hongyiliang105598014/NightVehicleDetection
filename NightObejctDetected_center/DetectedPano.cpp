#include "DetectedPano.h"
#include <fstream>

DetectedPano::DetectedPano(string path) :DetectedPosition(path)
{
	backPosition = Rect(videoSize.width / 2 + 10, 0, videoSize.width / 2 - 10, videoSize.height / 2);
	rightPosition = Rect(videoSize.width / 2, videoSize.height / 2, videoSize.width / 2 , videoSize.height / 2);
	leftPosition = Rect(0, videoSize.height / 2, videoSize.width / 2, videoSize.height / 2);
	//position = backPosition;
}

DetectedPano::~DetectedPano()
{

}

void DetectedPano::SetImageProcessor(ImageProcessor *imageProcessor)
{
	processor = imageProcessor;
}

void DetectedPano::run()
{

	CBrightObjectSegment brightObjectSegment(0.99);
	char filename[] = "FPS.csv";
	fstream fp;
	fp.open(filename, ios::app);//開啟檔案
	if (!fp) {//如果開啟檔案失敗，fp為0；成功，fp為非0
		cout << "Fail to open file: " << filename << endl;
	}


	capture.set(CV_CAP_PROP_POS_FRAMES, 8000);

	for (;;)
	{

		capture >> src;
		if (src.empty())
		{
			break;
		}

		Mat back = src(backPosition);
		Mat right = src(rightPosition);
		Mat left = src(leftPosition);

		Mat pano = cv::Mat::ones(videoSize.height / 2 , videoSize.width / 2 * 3, CV_8UC3);

		right.copyTo(pano(Rect(0, 0, right.cols, right.rows)));
		back.copyTo(pano(Rect(right.cols, 0, back.cols, back.rows)));
		left.copyTo(pano(Rect(right.cols + back.cols, 0, left.cols, left.rows)));

		//pano.push_back(back);
		//pano.push_back(right);
		//pano.push_back(left);


		//imshow("back", back);
		resize(pano, pano, Size(), 0.8,  0.8, INTER_LINEAR);
		imshow("right", pano);
		//imshow("left", left);
		waitKey(1);

	}
}
