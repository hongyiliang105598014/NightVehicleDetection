#include <string>  
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;


void detectLight(Mat binaryImg) {
	int threshold = 100;
	Mat labelImg, stats, centroids;
	int nLabels = connectedComponentsWithStats(binaryImg, labelImg, stats, centroids, 8, CV_32S);



	std::vector<cv::Vec3b> colors(nLabels);
	colors[0] = cv::Vec3b(0, 0, 0);
	std::cout << "Number of connected components = " << nLabels << std::endl << std::endl;

	for (int label = 1; label < nLabels; ++label) {
		colors[label] = cv::Vec3b((std::rand() & 255), (std::rand() & 255), (std::rand() & 255));
		//std::cout << "Component " << label << std::endl;
		//std::cout << "CC_STAT_LEFT   = " << stats.at<int>(label, cv::CC_STAT_LEFT) << std::endl;
		//std::cout << "CC_STAT_TOP    = " << stats.at<int>(label, cv::CC_STAT_TOP) << std::endl;
		//std::cout << "CC_STAT_WIDTH  = " << stats.at<int>(label, cv::CC_STAT_WIDTH) << std::endl;
		//std::cout << "CC_STAT_HEIGHT = " << stats.at<int>(label, cv::CC_STAT_HEIGHT) << std::endl;
		std::cout << "CC_STAT_AREA   = " << stats.at<int>(label, cv::CC_STAT_AREA) << std::endl;
		//std::cout << "CENTER = (" << centroids.at<double>(label, 0) << "," << centroids.at<double>(label, 1) << ")" << std::endl << std::endl;
	}

	cv::Mat dst(binaryImg.size(), CV_8UC3);
	for (int r = 0; r < dst.rows; ++r) {
		for (int c = 0; c < dst.cols; ++c) {
			int label = labelImg.at<int>(r, c);
			cv::Vec3b &pixel = dst.at<cv::Vec3b>(r, c);
			pixel = colors[label];
		}
	}

	imshow("Labeling", dst);

}


int main() {
	Mat src;
	Mat leftSrc, rightSrc;
	Mat leftROI, rightROI;
	Mat leftGrey, rightGrey;
	Mat leftHist, rightHist;
	string path = "video/AV1-20170510_213352.avi";
	VideoCapture capture(path);
	if (!capture.isOpened()) {
		cout << "Cannot open video" << endl;
		return -1;
	}

	Size videoSize = Size((int)capture.get(CV_CAP_PROP_FRAME_WIDTH), (int)capture.get(CV_CAP_PROP_FRAME_HEIGHT));
	//namedWindow("src", CV_WINDOW_AUTOSIZE);

	while (true) {
		capture >> src;
		if (src.empty()) {
			break;
		}

		leftSrc = src(Rect(0, videoSize.height / 2, videoSize.width / 2, videoSize.height / 2));
		cvtColor(leftSrc, leftGrey, CV_BGR2GRAY);
		
		leftROI = leftGrey(Rect(10, leftSrc.rows / 7 * 2, leftSrc.cols / 8 * 7, leftSrc.rows / 4 * 2)).clone();
		//adaptiveThreshold(leftROI, leftROI, 0, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 11, 5);
		//equalizeHist(leftROI, leftROI);
		threshold(leftROI, leftROI, 220, 255, THRESH_BINARY); //OTSU is not necessary to set thres
		detectLight(leftROI);
		

		//rightSrc = src(Rect(videoSize.width / 2, videoSize.height / 2, videoSize.width / 2, videoSize.height / 2));
		//cvtColor(rightSrc, rightGrey, CV_BGR2GRAY);
		
		
		imshow("Left Src", leftSrc);
		imshow("Left Grey", leftGrey);
		imshow("Left ROI", leftROI);
		//imshow("Left Hist", leftHist);
		
		//imshow("src", src);
		//imshow("Right Src", rightSrc);
		//imshow("Right Grey", rightGrey);
		
		waitKey(1);
	}



}