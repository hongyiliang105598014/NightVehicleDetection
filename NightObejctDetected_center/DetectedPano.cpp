#include "DetectedPano.h"
#include "BackImageProcessor.h"
#include "RightImageProcessor.h"
#include "LeftImageProcessor.h"
#include <fstream>
#include <ctime>


DetectedPano::DetectedPano(string path) :DetectedPosition(path)
{
	backPosition = Rect(videoSize.width / 2 + 10, 0, videoSize.width / 2 - 10, videoSize.height / 2);
	rightPosition = Rect(videoSize.width / 2, videoSize.height / 2, videoSize.width / 2 , videoSize.height / 2);
	//rightPosition = Rect(0, videoSize.height / 2, videoSize.width / 2, videoSize.height / 2);
	leftPosition = Rect(0, videoSize.height / 2, videoSize.width / 2, videoSize.height / 2);
	//leftPosition = Rect(0, 0, videoSize.width / 2, videoSize.height / 2);
	//position = backPosition;
}

DetectedPano::~DetectedPano()
{

}

void DetectedPano::SetImageProcessor(ImageProcessor *imageProcessor)
{
	if (BackImageProcessor *backImgPro = dynamic_cast<BackImageProcessor*>(imageProcessor))
	{
		backImageProcessor = backImgPro;
	}
	else if (RightImageProcessor *rightImgPro = dynamic_cast<RightImageProcessor*>(imageProcessor))
	{
		rightImageProcessor = rightImgPro;
	}
	else if (LeftImageProcessor *leftImgPro = dynamic_cast<LeftImageProcessor*>(imageProcessor))
	{
		leftImageProcessor = leftImgPro;
	}
}

void DetectedPano::checkOverlappingRegion(Mat right, Mat back, Mat left, Mat pano)
{

	vector<ObjectTracker> rightObjectTracker = rightImageProcessor->getVectorOfObjectTracker();
	cout << rightObjectTracker.size() << endl;

	vector<ObjectTracker> backObjectTracker = backImageProcessor->getVectorOfObjectTracker();
	cout << backObjectTracker.size() << endl;

	vector<ObjectTracker> leftObjectTracker = leftImageProcessor->getVectorOfObjectTracker();
	cout << leftObjectTracker.size() << endl;


	

	for (int i = 0; i < rightObjectTracker.size(); i++)
	{
		cout << "right : ( " << rightObjectTracker[0].getCurrentPos().x << ":" << rightObjectTracker[0].getCurrentPos().y << ")" << endl;
	}

	for (int i = 0; i < backObjectTracker.size(); i++)
	{
		cout << right.cols << endl;
		backObjectTracker[i].setCurrentPosOffsetForPano(right.cols, 0);
		cout << "back : ( " << backObjectTracker[0].getCurrentPos().x << ":" << backObjectTracker[0].getCurrentPos().y << ")" << endl;


	}

	for (int i = 0; i < leftObjectTracker.size(); i++)
	{
		leftObjectTracker[i].setCurrentPosOffsetForPano(right.cols + back.cols, 0);
		cout << "left : ( " << leftObjectTracker[0].getCurrentPos().x << ":" << leftObjectTracker[0].getCurrentPos().y << ")" << endl;
	}


	for (int i = 0; i < backObjectTracker.size(); i++)
	{
		for (int j = 0; j < rightObjectTracker.size(); j++)
		{
			backObjectTracker[i].IsOverlapping = false;
			rightObjectTracker[j].IsOverlapping = false;

			if (backObjectTracker[i].getCurrentPos().x + backObjectTracker[i].getCurrentPos().width <= right.cols + 300 &&
				rightObjectTracker[j].getCurrentPos().x + rightObjectTracker[j].getCurrentPos().width >= 450)
			{
				backObjectTracker[i].IsOverlapping = true;
				rightObjectTracker[j].IsOverlapping = true;

				stringstream ss;
				ss << "car" << i;
				//putText(back, ss.str(), Point(backObjectTracker[i].getCurrentPos().x - 10, backObjectTracker[i].getCurrentPos().y - 10), 0, 1, Scalar(0, 0, 255), 3);
				//putText(right, ss.str(), Point(rightObjectTracker[j].getCurrentPos().x - 10, rightObjectTracker[j].getCurrentPos().y - 10), 0, 1, Scalar(0, 0, 255), 3);
			}
		}
	}

	for (int i = 0; i < backObjectTracker.size(); i++)
	{
		for (int j = 0; j < leftObjectTracker.size(); j++)
		{
			backObjectTracker[i].IsOverlapping = false;
			leftObjectTracker[j].IsOverlapping = false;

			if (backObjectTracker[i].getCurrentPos().x >= right.cols + 400 &&
				leftObjectTracker[j].getCurrentPos().x + leftObjectTracker[j].getCurrentPos().width <= right.cols + back.cols + 450)
			{
				backObjectTracker[i].IsOverlapping = true;
				leftObjectTracker[j].IsOverlapping = true;

				stringstream ss;
				ss << "car" << i;
				//putText(back, ss.str(), Point(backObjectTracker[i].getCurrentPos().x - 10, backObjectTracker[i].getCurrentPos().y - 10), 0, 1, Scalar(0, 0, 255), 3);
				//putText(left, ss.str(), Point(leftObjectTracker[j].getCurrentPos().x - 10, leftObjectTracker[j].getCurrentPos().y - 10), 0, 1, Scalar(0, 0, 255), 3);
			}
		}
	}

	vector<ObjectTracker> panoObjectTracker;
	int carNo = 1;
	int motorNo = 1;

	panoObjectTracker.insert(panoObjectTracker.end(), rightObjectTracker.begin(), rightObjectTracker.end());
	panoObjectTracker.insert(panoObjectTracker.end(), backObjectTracker.begin(), backObjectTracker.end());
	panoObjectTracker.insert(panoObjectTracker.end(), leftObjectTracker.begin(), leftObjectTracker.end());


	const double lambda = 170.73;
	for (int i = 0; i < panoObjectTracker.size(); i++)
	{
		cout << "pano : " << "(" << panoObjectTracker[i].getCurrentPos().x << "," << panoObjectTracker[i].getCurrentPos().y << ") "  << panoObjectTracker[i].IsOverlapping << endl;
		
		//if (panoObjectTracker[i].getNumberOfObjectContain() == 1)
		//{
		//	stringstream ss;
		//	ss << "motor " << motorNo++;
		//	putText(pano, ss.str(), Point(panoObjectTracker[i].getCurrentPos().x - 10, panoObjectTracker[i].getCurrentPos().y - 10), 0, 1, Scalar(0, 0, 255), 3);
		//}
		if (panoObjectTracker[i].getNumberOfObjectContain() == 2)
		{
			cout << "pano width:" << pano.cols << ", " << "pano height:" << pano.rows << endl;
			if (panoObjectTracker[i].IsOverlapping)
			{
				stringstream ss;
				ss << "car" << carNo;
				putText(pano, ss.str(), Point(panoObjectTracker[i].getCurrentPos().x - 10, panoObjectTracker[i].getCurrentPos().y - 10), 0, 1, Scalar(0, 0, 255), 3);
			}
			else if (!panoObjectTracker[i].IsOverlapping)
			{
				stringstream ss;
				ss << "car" << carNo++;
				putText(pano, ss.str(), Point(panoObjectTracker[i].getCurrentPos().x - 10, panoObjectTracker[i].getCurrentPos().y - 10), 0, 1, Scalar(0, 0, 255), 3);
			}
		}
	}
}

void DetectedPano::run()
{

	CBrightObjectSegment backBrightObjectSegment(0.995);
	CBrightObjectSegment rightBrightObjectSegment(0.995);
	CBrightObjectSegment leftBrightObjectSegment(0.995);
	char filename[] = "road1_FPS.csv";
	fstream fp;
	fp.open(filename, ios::app);//開啟檔案
	if (!fp) {//如果開啟檔案失敗，fp為0；成功，fp為非0
		cout << "Fail to open file: " << filename << endl;
	}

	VideoWriter writer, writer2;
	writer.open("test.avi", CV_FOURCC('M', 'P', '4', '2'), capture.get(CV_CAP_PROP_FPS), Size(1747, 387), true);
	if (!writer.isOpened())
	{
		cout << "Cannot write video" << endl;
		system("pause");
	}

	capture.set(CV_CAP_PROP_POS_FRAMES, 0);

	for (;;)
	{
		int framePosition = capture.get(CV_CAP_PROP_POS_FRAMES);

		capture >> src;
		
		/*if (framePosition == 2800)
		{
			break;
		}*/

		if (src.empty())
		{
			break;
		}

		clock_t begin = clock();

		cvtColor(src, gray, CV_BGR2GRAY);
		Mat back = src(backPosition);
		Mat right = src(rightPosition);
		Mat left = src(leftPosition);

		//stringstream ss2;
		//Mat srcPano = cv::Mat::ones(videoSize.height / 2, videoSize.width / 2 * 3, CV_8UC3);
		//right.copyTo(srcPano(Rect(0, 0, right.cols, right.rows)));
		//back.copyTo(srcPano(Rect(right.cols, 0, back.cols, back.rows)));
		//left.copyTo(srcPano(Rect(right.cols + back.cols, 0, left.cols, left.rows)));
		//ss2 << "frame/frame " << framePosition << ".jpg";
		//cout << ss1.str() << endl;

		//putText(srcPano, ss2.str(), Point(50, 50), 0, 1, Scalar(0, 0, 255), 3);
		//imwrite(ss2.str(), srcPano);


		Mat grayBack = gray(backPosition);
		Rect grayCenterROI = Rect(0, grayBack.rows / 2 - 12, grayBack.cols, grayBack.rows / 2 + 12);
		Mat grayCenter = grayBack(grayCenterROI).clone();
		backROIs.push_back(grayCenterROI);
		backROIs.push_back(grayCenterROI);
		backROIs.push_back(grayCenterROI);
		backBrightObjectSegment.getBinaryImage(grayCenter);
		backImageProcessor->removeNoice(grayCenter, 5, 5, 7, 7);
		backImageProcessor->detectLight(back, grayCenter, 0, grayBack.rows / 2 - 12, backROIs);
		//imshow("binaryCenter", grayCenter);

		Mat grayRight = gray(rightPosition);
		Rect grayRightROI = Rect(0, grayRight.rows * 28 / 100, grayRight.cols * 17 / 20, grayRight.rows * 5 / 12);
		Mat grayRightFront = grayRight(grayRightROI).clone();
		Rect grayRightNearROI = Rect(0, grayRight.rows * 28 / 100, grayRight.cols * 2 / 5, grayRight.rows * 5 / 12);
		Mat grayRightNear = grayRight(grayRightNearROI).clone();
		
		rightROIs.push_back(grayRightNearROI);
		rightROIs.push_back(grayRightROI);
		rightBrightObjectSegment.getBinaryImage(grayRightFront);
		rightImageProcessor->removeNoice(grayRightFront, 5, 5, 7, 7);
		rightImageProcessor->detectLight(right, grayRightFront, 0, grayRight.rows * 28 / 100, rightROIs);
		//imshow("binaryRightFront", grayRightFront);

		Mat grayLeft = gray(leftPosition);
		Rect grayLeftROI = Rect(grayLeft.cols * 2 / 20, grayLeft.rows * 28 / 100, grayLeft.cols * 18 / 20, grayLeft.rows * 5 / 12);
		Mat grayLeftFront = grayLeft(grayLeftROI).clone();
		Rect grayLeftNearROI = Rect(grayLeft.cols * 3 / 5, grayLeft.rows * 28 / 100, grayRight.cols * 2 / 5, grayRight.rows * 5 / 12);
		Mat grayLeftNear = grayLeft(grayLeftNearROI).clone();
		leftROIs.push_back(grayLeftNearROI);
		leftROIs.push_back(grayLeftROI);
		leftBrightObjectSegment.getBinaryImage(grayLeftFront);
		leftImageProcessor->removeNoice(grayLeftFront, 5, 5, 7, 7);
		leftImageProcessor->detectLight(left, grayLeftFront, grayLeft.cols * 2 / 20, grayLeft.rows * 28 / 100, leftROIs);
		//imshow("binaryLeftFront", grayLeftFront);
		

		// show result image
		Mat pano = cv::Mat::ones(videoSize.height / 2, videoSize.width / 2 * 3, CV_8UC3);
		Mat binary = cv::Mat::ones(videoSize.height / 2, videoSize.width / 2 * 3, CV_8UC1);

		rectangle(back, grayCenterROI, Scalar(0, 255, 0), 1, 8, 0); // draw back ROI
		//rectangle(back, Rect(0, grayBack.rows / 2, 300, grayBack.rows / 2), Scalar(255, 255, 255), 1, 8, 0); // draw back ROI
		//rectangle(back, Rect(400, grayBack.rows / 2, grayBack.cols, grayBack.rows / 2), Scalar(255, 255, 255), 1, 8, 0); // draw back ROI


		rectangle(right, grayRightROI, Scalar(0, 255, 255), 1, 8, 0); // draw right ROI
		rectangle(right, grayRightNearROI, Scalar(0, 0, 255), 1, 8, 0); // draw right near ROI
		//line(right, Point(0, right.rows / 2 - 30), Point(500, right.rows / 2 - 30), Scalar(255, 0, 0), 3);
		//line(right, Point(348, 0), Point(348, right.rows), Scalar(255, 0, 0), 3);
		circle(right, Point(270, 50), 20, Scalar(0, 255, 255), -1);


		rectangle(left, grayLeftROI, Scalar(0, 255, 255), 1, 8, 0); // draw Front ROI
		rectangle(left, grayLeftNearROI, Scalar(0, 0, 255), 1, 8, 0); // draw Front ROI
		//line(left, Point(100, left.rows / 2 - 100), Point(500, left.rows / 2 - 100), Scalar(255, 0, 0), 3);
		
		//stringstream s1;
		//s1 << framePosition;

		//putText(right, s1.str(), Point(500, 60), 0, 1, Scalar(0, 255, 0), 2);

		//draw pano

		right.copyTo(pano(Rect(0, 0, right.cols, right.rows)));
		back.copyTo(pano(Rect(right.cols, 0, back.cols, back.rows)));
		left.copyTo(pano(Rect(right.cols + back.cols, 0, left.cols, left.rows)));
		checkOverlappingRegion(right, back, left, pano);
		resize(pano, pano, Size(), 0.8, 0.8, INTER_LINEAR);

		

		//draw binary
		grayRightFront.copyTo(binary(Rect(0, 0, grayRightFront.cols, grayRightFront.rows)));
		line(binary, Point(grayRightFront.cols, 0), Point(grayRightFront.cols, grayRightFront.rows), Scalar(255, 0, 0), 3);
		grayCenter.copyTo(binary(Rect(grayRightFront.cols, 0, grayCenter.cols, grayCenter.rows)));
		line(binary, Point(grayRightFront.cols + grayCenter.cols, 0), Point(grayRightFront.cols + grayCenter.cols, grayCenter.rows), Scalar(255, 0, 0), 3);
		grayLeftFront.copyTo(binary(Rect(grayRightFront.cols + grayCenter.cols, 0, grayLeftFront.cols, grayLeftFront.rows)));
		resize(binary, binary, Size(), 0.8, 0.8, INTER_LINEAR);

		

		clock_t end = clock();
		double elapsedSecs = double(end - begin) / CLOCKS_PER_SEC;
		stringstream ss, ss1, ssfps;
		ss << "FPS: " << int(1 / elapsedSecs);
		ssfps << int(1 / elapsedSecs);
		ss1 << "result_frame/frame " << framePosition << ".jpg";
		//cout << ss1.str() << endl;

		putText(pano, ss.str(), Point(50, 50), 0, 1, Scalar(0, 0, 255), 3);
		fp << ssfps.str() << endl;
		imwrite(ss1.str(), pano);
		imshow("pano", pano);
		imshow("binary", binary);
		
		
		//cout << "VW: " << videoSize.width / 2 * 3 << "H: " << videoSize.height / 2 << endl;
		//cout << "PW: " << pano.cols << "H: " << pano.rows << endl;
		
		writer << pano;
		waitKey(1);
	}
}
