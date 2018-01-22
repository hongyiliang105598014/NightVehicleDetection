#include "DetectedPano.h"
#include "BackImageProcessor.h"
#include "RightImageProcessor.h"
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
	cout << "123" << endl;
	if (BackImageProcessor *backImgPro = dynamic_cast<BackImageProcessor*>(imageProcessor))
	{
		backImageProcessor = backImgPro;
	}
	else if (RightImageProcessor *rightImgPro = dynamic_cast<RightImageProcessor*>(imageProcessor))
	{
		rightImageProcessor = rightImgPro;
	}
}

void DetectedPano::setBackImageProcessorResult(Mat res)
{
	backResult = res;
}

void DetectedPano::setRightImageProcessorResult(Mat res)
{
	rightResult = res;
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


	capture.set(CV_CAP_PROP_POS_FRAMES, 1410);

	for (;;)
	{

		capture >> src;
		if (src.empty())
		{
			break;
		}

		cvtColor(src, gray, CV_BGR2GRAY);
		Mat back = src(backPosition);
		Mat right = src(rightPosition);
		Mat left = src(leftPosition);

		Mat grayBack = gray(backPosition);
		Rect grayCenterROI = Rect(0, grayBack.rows / 2, grayBack.cols, grayBack.rows / 2);
		Mat grayCenter = grayBack(grayCenterROI);
		backROIs.push_back(grayCenterROI);
		backROIs.push_back(grayCenterROI);
		backROIs.push_back(grayCenterROI);
		brightObjectSegment.getBinaryImage(grayCenter);
		//backImageProcessor->removeNoice(grayCenter, 8, 8, 5, 5);
		backImageProcessor->detectLight(back, grayCenter, 0, grayBack.rows / 2, backROIs);
		imshow("binaryCenter", grayCenter);


		Mat grayRight = gray(rightPosition);
		Rect grayRightROI = Rect(0, grayRight.rows * 28 / 100, grayRight.cols * 17 / 20, grayRight.rows * 5 / 12);
		Mat grayRightFront = grayRight(grayRightROI);
		rightROIs.push_back(grayRightROI);
		rightROIs.push_back(grayRightROI);
		//rightROIs.push_back(grayRightROI);
		brightObjectSegment.getBinaryImage(grayRightFront);
		rightImageProcessor->removeNoice(grayRightFront, 8, 8, 5, 5);
		rightImageProcessor->detectLight(right, grayRightFront, 0, grayRight.rows * 28 / 100, rightROIs);
		imshow("binaryRightFront", grayRightFront);



		Mat grayLeft = gray(leftPosition);
		
		
		

		






		// show result image

		Mat pano = cv::Mat::ones(videoSize.height / 2, videoSize.width / 2 * 3, CV_8UC3);

		rectangle(back, grayCenterROI, Scalar(0, 255, 0), 1, 8, 0); // draw Front ROI
		rectangle(right, grayRightROI, Scalar(0, 255, 0), 1, 8, 0); // draw Front ROI

		right.copyTo(pano(Rect(0, 0, right.cols, right.rows)));
		back.copyTo(pano(Rect(right.cols, 0, back.cols, back.rows)));
		left.copyTo(pano(Rect(right.cols + back.cols, 0, left.cols, left.rows)));
		resize(pano, pano, Size(), 0.8, 0.8, INTER_LINEAR);

		imshow("right", right);
		imshow("back", back);

		imshow("pano", pano);
		
		waitKey(1);
	}
}
