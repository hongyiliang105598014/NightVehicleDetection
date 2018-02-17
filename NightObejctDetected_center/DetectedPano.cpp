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

bool DetectedPano::sortByRightPosition(const ObjectTracker &a, const ObjectTracker &b)
{
	return a._headLight.x < b._headLight.x;
}

bool DetectedPano::sortByLeftPosition(const ObjectTracker &a, const ObjectTracker &b)
{
	return a._headLight.x > b._headLight.x;
}

bool DetectedPano::sortByBackPosition(const ObjectTracker &a, const ObjectTracker &b)
{
	return a._headLight.y < b._headLight.y;
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

void DetectedPano::checkOverlappingRegion(Mat right, Mat back, Mat left)
{
	vector<ObjectTracker> panoObjectTracker;
	int carNo = 1;
	int motorNo = 1;
	vector<ObjectTracker> vectorOfRightObjectTracker = rightImageProcessor->getVectorOfObjectTracker();
	sort(vectorOfRightObjectTracker.begin(), vectorOfRightObjectTracker.end(), sortByRightPosition);
	//cout << vectorOfRightObjectTracker.size() << endl;
	
	vector<ObjectTracker> vectorOfBackObjectTracker = backImageProcessor->getVectorOfObjectTracker();
	sort(vectorOfBackObjectTracker.begin(), vectorOfBackObjectTracker.end(), sortByRightPosition);
	//cout << vectorOfBackObjectTracker.size() << endl;

	vector<ObjectTracker> vectorOfLeftObjectTracker = leftImageProcessor->getVectorOfObjectTracker();
	sort(vectorOfLeftObjectTracker.begin(), vectorOfLeftObjectTracker.end(), sortByLeftPosition);
	//cout << vectorOfLeftObjectTracker.size() << endl;
	
	double ly, ry;
	//double ly = -0.624 * lx + 454;
	//double ry = 0.607 * rx + 12;

	

	for (int i = 0; i < vectorOfBackObjectTracker.size(); i++)
	{
		vectorOfBackObjectTracker[i].setID(carNo++);	
	}
	
	for (int i = 0; i < vectorOfRightObjectTracker.size(); i++)
	{

		vectorOfRightObjectTracker[i].setID(carNo++);
	}

	for (int i = 0; i < vectorOfLeftObjectTracker.size(); i++)
	{
		vectorOfLeftObjectTracker[i].setID(carNo++);
	}

	if (vectorOfBackObjectTracker.size() > 0 && vectorOfRightObjectTracker.size() > 0)
	{
		for (int i = 0; i < vectorOfBackObjectTracker.size(); i++)
		{
			int posX = vectorOfBackObjectTracker[i].getCurrentPos().x + vectorOfBackObjectTracker[i].getCurrentPos().width;
			int posY = vectorOfBackObjectTracker[i].getCurrentPos().y + vectorOfBackObjectTracker[i].getCurrentPos().height;

			ly = -0.624 * posX + 454;
			ry = 0.607 * posX + 12;

			//cout << "posX = " << posX << " posY = " << posY << endl;
			//cout << "ly = " << ly;

			if (posX < 300 && posY < ly)
			{
				for (int j = 0; j < vectorOfRightObjectTracker.size(); j++)
				{

					//cout << "Match" << endl;
					vectorOfBackObjectTracker[i].IsOverlapping = true;
					vectorOfRightObjectTracker[j].IsOverlapping = true;

					vectorOfRightObjectTracker[j].setID(vectorOfBackObjectTracker[i].getID());

					break;
				}
			}
			else if (posX > 360 && posY < ry)
			{
				for (int j = 0; j < vectorOfLeftObjectTracker.size(); j++)
				{
					vectorOfBackObjectTracker[i].IsOverlapping = true;
					vectorOfLeftObjectTracker[j].IsOverlapping = true;

					vectorOfLeftObjectTracker[j].setID(vectorOfBackObjectTracker[i].getID());
				}
			}
		}
	}
	
	/*else if (vectorOfRightObjectTracker.size() > 0)
	{
		for (int i = 0; i < vectorOfRightObjectTracker.size(); i++)
		{
			if (vectorOfRightObjectTracker[i].ID == 0)
			{
				vectorOfRightObjectTracker[i].ID = carNo;

				for (int j = 0; j < vectorOfBackObjectTracker.size(); j++)
				{
					int posX = vectorOfBackObjectTracker[j].getCurrentPos().x + vectorOfBackObjectTracker[j].getCurrentPos().width;
					int posY = vectorOfBackObjectTracker[j].getCurrentPos().y + vectorOfBackObjectTracker[j].getCurrentPos().height;

					ly = -0.624 * posX + 454;
					if (posX < 340 && posY < ly)
					{
						//cout << "Match" << endl;
						vectorOfRightObjectTracker[i].IsOverlapping = true;
						vectorOfBackObjectTracker[j].IsOverlapping = true;


						vectorOfBackObjectTracker[j].ID = vectorOfRightObjectTracker[i].ID;

						break;
					}
				}
			}
			carNo++;
		}
	}*/
	
	for(int i = 0; i < vectorOfBackObjectTracker.size(); i++)
	{
		stringstream ss;
		ss << "car" << vectorOfBackObjectTracker[i].getID();
		putText(back, ss.str(), Point(vectorOfBackObjectTracker[i].getCurrentPos().x - 10, vectorOfBackObjectTracker[i].getCurrentPos().y - 10), 0, 1, Scalar(0, 0, 255), 3);
	}

	for (int i = 0; i < vectorOfRightObjectTracker.size(); i++)
	{
		stringstream ss;
		ss << "car" << vectorOfRightObjectTracker[i].getID();
		putText(right, ss.str(), Point(vectorOfRightObjectTracker[i].getCurrentPos().x - 10, vectorOfRightObjectTracker[i].getCurrentPos().y - 10), 0, 1, Scalar(0, 0, 255), 3);
	}

	for (int i = 0; i < vectorOfLeftObjectTracker.size(); i++)
	{
		stringstream ss;
		ss << "car" << vectorOfLeftObjectTracker[i].getID();
		putText(left, ss.str(), Point(vectorOfLeftObjectTracker[i].getCurrentPos().x - 10, vectorOfLeftObjectTracker[i].getCurrentPos().y - 10), 0, 1, Scalar(0, 0, 255), 3);
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

	capture.set(CV_CAP_PROP_POS_FRAMES, 700);

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
		Rect grayRightROI = Rect(0, grayRight.rows * 28 / 100, grayRight.cols * 16 / 20, grayRight.rows * 5 / 12);
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
		Rect grayLeftROI = Rect(grayLeft.cols * 4 / 20, grayLeft.rows * 28 / 100, grayLeft.cols * 16 / 20, grayLeft.rows * 5 / 12);
		Mat grayLeftFront = grayLeft(grayLeftROI).clone();
		Rect grayLeftNearROI = Rect(grayLeft.cols * 3 / 5, grayLeft.rows * 28 / 100, grayRight.cols * 2 / 5, grayRight.rows * 5 / 12);
		Mat grayLeftNear = grayLeft(grayLeftNearROI).clone();
		leftROIs.push_back(grayLeftNearROI);
		leftROIs.push_back(grayLeftROI);
		leftBrightObjectSegment.getBinaryImage(grayLeftFront);
		leftImageProcessor->removeNoice(grayLeftFront, 5, 5, 7, 7);
		leftImageProcessor->detectLight(left, grayLeftFront, grayLeft.cols * 4 / 20, grayLeft.rows * 28 / 100, leftROIs);
		//imshow("binaryLeftFront", grayLeftFront);
		
		checkOverlappingRegion(right, back, left);

		// show result image
		Mat pano = cv::Mat::ones(videoSize.height / 2, videoSize.width / 2 * 3, CV_8UC3);
		Mat binary = cv::Mat::ones(videoSize.height / 2, videoSize.width / 2 * 3, CV_8UC1);

		rectangle(back, grayCenterROI, Scalar(0, 255, 0), 1, 8, 0); // draw back ROI
		line(back, Point(grayBack.cols / 2, grayBack.rows / 2 - 12), Point(0, right.rows - 30), Scalar(0, 255, 0), 3);
		line(back, Point(grayBack.cols / 2, grayBack.rows / 2 - 12), Point(right.cols, right.rows - 30), Scalar(0, 255, 0), 3);

		//cout << "leftSide: " << "(" << grayBack.cols / 2 << "," << grayBack.rows / 2 - 12 << ")" << " " << "(" << 0 << "," << right.rows - 30 << ")" << endl;
		//cout << "rightSide: " << "(" << grayBack.cols / 2 << "," << grayBack.rows / 2 - 12 << ")" << " " << "(" << right.cols << "," << right.rows - 30 << ")" << endl;

		//rectangle(back, Rect(0, grayBack.rows / 2, 300, grayBack.rows / 2), Scalar(255, 255, 255), 1, 8, 0); // draw back ROI
		//rectangle(back, Rect(400, grayBack.rows / 2, grayBack.cols, grayBack.rows / 2), Scalar(255, 255, 255), 1, 8, 0); // draw back ROI


		rectangle(right, grayRightROI, Scalar(0, 255, 255), 1, 8, 0); // draw right ROI
		rectangle(right, grayRightNearROI, Scalar(0, 0, 255), 1, 8, 0); // draw right near ROI
		line(right, Point(0, right.rows / 2 - 10), Point(500, right.rows / 2 - 10), Scalar(255, 0, 0), 3);
		//line(right, Point(348, 0), Point(348, right.rows), Scalar(255, 0, 0), 3);
		circle(right, Point(350, 60), 20, Scalar(0, 255, 255), -1);


		rectangle(left, grayLeftROI, Scalar(0, 255, 255), 1, 8, 0); // draw Front ROI
		rectangle(left, grayLeftNearROI, Scalar(0, 0, 255), 1, 8, 0); // draw Front ROI
		line(left, Point(100, left.rows / 2 - 50), Point(500, left.rows / 2 - 50), Scalar(255, 0, 0), 3);
		


		//draw pano
		right.copyTo(pano(Rect(0, 0, right.cols, right.rows)));
		back.copyTo(pano(Rect(right.cols, 0, back.cols, back.rows)));
		left.copyTo(pano(Rect(right.cols + back.cols, 0, left.cols, left.rows)));
		
		resize(pano, pano, Size(), 0.6, 0.6, INTER_LINEAR);

		clock_t end = clock();
		double elapsedSecs = double(end - begin) / CLOCKS_PER_SEC;
		stringstream ss, ss1, ssfps;
		ss << "FPS: " << int(1 / elapsedSecs);
		ssfps << int(1 / elapsedSecs);
		ss1 << "result_frame/frame " << framePosition << ".jpg";
		putText(pano, ss.str(), Point(50, 50), 0, 1, Scalar(0, 0, 255), 3);

		//draw binary
		grayRightFront.copyTo(binary(Rect(0, 0, grayRightFront.cols, grayRightFront.rows)));
		line(binary, Point(grayRightFront.cols, 0), Point(grayRightFront.cols, grayRightFront.rows), Scalar(255, 0, 0), 3);
		grayCenter.copyTo(binary(Rect(grayRightFront.cols, 0, grayCenter.cols, grayCenter.rows)));
		line(binary, Point(grayRightFront.cols + grayCenter.cols, 0), Point(grayRightFront.cols + grayCenter.cols, grayCenter.rows), Scalar(255, 0, 0), 3);
		grayLeftFront.copyTo(binary(Rect(grayRightFront.cols + grayCenter.cols, 0, grayLeftFront.cols, grayLeftFront.rows)));
		resize(binary, binary, Size(), 0.8, 0.8, INTER_LINEAR);

		


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
