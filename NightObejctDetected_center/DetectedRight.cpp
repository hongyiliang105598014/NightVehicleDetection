#include "DetectedRight.h"
DetectedRight::DetectedRight(string path) :DetectedPosition(path)
{
	position= Rect(0, videoSize.height / 2, videoSize.width / 2, videoSize.height / 2);
}

DetectedRight::~DetectedRight()
{
}

void DetectedRight::SetImageProcessor(ImageProcessor *imageProcessor)
{
	processor = imageProcessor;
}

Mat DetectedRight::getResult()
{
	return src;
}

#include <fstream>
void DetectedRight::run()
{
	CBrightObjectSegment brightObjectSegment(0.985);
	CBrightObjectSegment brightObjectSegment2(0.985);
	
	char filename[] = "FPS4.csv";
	fstream fp;
	fp.open(filename, ios::app);//開啟檔案
	if (!fp) {//如果開啟檔案失敗，fp為0；成功，fp為非0
		cout << "Fail to open file: " << filename << endl;
	}


	while (true)
	{
		
		clock_t begin = clock();

		capture >> src;
		if (src.empty())
		{
			break;
		}
		//Dividing the video to left and right part and changing image from color to gray		
		src = src(position);
		cvtColor(src, Gray, CV_BGR2GRAY);

		//right part
		Rect ROI = Rect(0, Gray.rows * 28 / 100, Gray.cols * 17 / 20, Gray.rows * 5 / 12);
		rectangle(src, ROI, Scalar(0, 255, 255), 1, 8, 0); // draw ROI
		GrayRect = Gray(ROI);
		GrayRectTemp = GrayRect.clone();

		const int FrontRectX = 0;
		const int FrontRectY = Gray.rows * 28 / 100;
		const int FrontRectWidth = Gray.cols * 2 / 5;
		const int FrontRectHeight = Gray.rows * 5 / 12;
		Rect FrontRect = Rect(FrontRectX, FrontRectY, FrontRectWidth, FrontRectHeight);
		rectangle(src, FrontRect, Scalar(0, 0, 255), 1, 8, 0); // draw Front ROI

		//imageProcessor.extractEfficientImage(rightGrayRectTemp);

		Rect FrontROI = Rect(0, 0, FrontRectWidth, FrontRectHeight);
		Rect MiddleROI = Rect(FrontRectWidth, 0, Gray.cols * 17 / 20 - FrontRectWidth, FrontRectHeight);

		ROIs.push_back(FrontRect); //detect and track single light 
		ROIs.push_back(MiddleROI);


		Mat Front = GrayRectTemp(FrontROI);
		Mat Middle = GrayRectTemp(MiddleROI);

		brightObjectSegment.getBinaryImage(Front);
		brightObjectSegment2.getBinaryImage(Middle);

		processor->removeNoice(GrayRectTemp, 5, 5, 7, 7);
		processor->detectLight(src, GrayRectTemp, 0, Gray.rows * 28 / 100, ROIs);

		clock_t end = clock();
		double elapsedSecs = double(end - begin) / CLOCKS_PER_SEC;
		stringstream ss, ss1;
		ss << "FPS: " << int(1 / elapsedSecs);
		ss1 << int(1 / elapsedSecs);
		putText(src, ss.str(), Point(50, 50), 0, 1, Scalar(0, 0, 255), 3);
		fp << ss1.str() << endl;

		imshow("Right Result", src);
		imshow("Right Binary Result", Front);
		imshow("Right Binary Result2", Middle);

		videoWriter << src;
		switch (1) {
		case 1:
			waitKey(1);
			break;
		case 0:
			int key = waitKey(-1);

			if (key == 120)
			{

				frame++;
				cout << "frame:" << frame << endl;
				continue;
			}
			else if (key == 122)
			{
				frame--;
				if (frame < 0)
				{
					frame = 0;
				}
				else
				{
					capture.set(CV_CAP_PROP_POS_FRAMES, frame);
					cout << "frame:" << frame << endl;
				}
			}
			break;
		}
	}

	fp.close()
		; }
