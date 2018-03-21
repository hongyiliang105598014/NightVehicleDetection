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
	CBrightObjectSegment brightObjectSegment(0.99);
	CBrightObjectSegment brightObjectSegment2(0.995);

	char filename[] = "FPS4.csv";
	fstream fp;
	fp.open(filename, ios::app);//開啟檔案
	if (!fp) {//如果開啟檔案失敗，fp為0；成功，fp為非0
		cout << "Fail to open file: " << filename << endl;
	}

	capture.set(CV_CAP_PROP_POS_FRAMES, 460);

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
		const int frontRectX = 0;
		const int frontRectY = Gray.rows * 28 / 100;
		const int frontRectWidth = Gray.cols * 16 / 20;
		const int frontRectHeight = Gray.rows * 5 / 12;

		Rect frontROI = Rect(frontRectX, frontRectY, frontRectWidth, frontRectHeight);
		rectangle(src, frontROI, Scalar(0, 255, 255), 1, 8, 0); // draw ROI
		GrayRect = Gray(frontROI);
		GrayRectTemp = GrayRect.clone();

		const int nearRectX = 0;
		const int nearRectY = Gray.rows * 28 / 100;
		const int nearRectWidth = Gray.cols * 2 / 5;
		const int nearRectHeight = Gray.rows * 5 / 12;

		Rect nearRect = Rect(nearRectX, nearRectY, nearRectWidth, nearRectHeight);
		rectangle(src, nearRect, Scalar(0, 0, 255), 1, 8, 0); // draw Front ROI
		Rect nearROI = Rect(nearRectX, nearRectY, nearRectWidth, nearRectHeight);

		ROIs.push_back(nearROI); //detect and track single light 
		ROIs.push_back(frontROI);

		brightObjectSegment.getBinaryImage(GrayRectTemp);

		processor->removeNoice(GrayRectTemp, 5, 5, 7, 7);
		processor->detectLight(src, GrayRectTemp, 0, Gray.rows * 28 / 100, ROIs);

		clock_t end = clock();
		double elapsedSecs = double(end - begin) / CLOCKS_PER_SEC;
		stringstream ss, ss1;
		ss << "FPS: " << int(1 / elapsedSecs);
		ss1 << int(1 / elapsedSecs);
		//putText(src, ss.str(), Point(50, 50), 0, 1, Scalar(0, 0, 255), 3);
		fp << ss1.str() << endl;

		imshow("Right Result", src);
		imshow("Right Binary Result", GrayRectTemp);
		//imshow("Right Binary Result2", near);


		imshow("Left gray", Gray);

		ThresholdSet thres = brightObjectSegment.GetThresholdSet();

		for (int i = 0; i < thres.size(); i++)
		{
			cout << "Thres:" << thres[0] << endl;
			for (int j = 0; j < Gray.cols; j++)
			{
				for (int k = 0; k < Gray.rows; k++)
				{
					//cout << "pixel: " << (int)gray.at<uchar>(k, j) << endl;
					if ((int)Gray.at<uchar>(k, j) <= thres[thres.size() - 4])
					{
						Gray.at<uchar>(k, j) = 0;
					}
					else if ((int)Gray.at<uchar>(k, j) >= thres[thres.size() - 4] && (int)Gray.at<uchar>(k, j) < thres[thres.size() - 3])
					{
						Gray.at<uchar>(k, j) = 32;
					}
					else if ((int)Gray.at<uchar>(k, j) >= thres[thres.size() - 3] && (int)Gray.at<uchar>(k, j) < thres[thres.size() - 2])
					{
						Gray.at<uchar>(k, j) = 64;
					}
					else if ((int)Gray.at<uchar>(k, j) >= thres[thres.size() - 2] && (int)Gray.at<uchar>(k, j) < thres[thres.size() - 1])
					{
						Gray.at<uchar>(k, j) = 128;
					}
					else if ((int)Gray.at<uchar>(k, j) >= thres[thres.size() - 1])
					{
						Gray.at<uchar>(k, j) = 255;
					}
				}
			}
		}

		imshow("Left gray thres", Gray);

		videoWriter << src;
		switch (0) {
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

	fp.close();
}
