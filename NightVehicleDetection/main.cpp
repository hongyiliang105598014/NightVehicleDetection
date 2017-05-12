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


void removeNoice(Mat binaryImg) {
	Mat erodeStruct = getStructuringElement(MORPH_RECT, Size(5, 5));
	erode(binaryImg, binaryImg, erodeStruct);

	imshow("Erode", binaryImg);

}


void detectLight(Mat srcImg, Mat binaryImg, int offsetX, int offsetY) {
	int threshold = 100;
	Mat labelImg, stats, centroids;
	int nLabels = connectedComponentsWithStats(binaryImg, labelImg, stats, centroids, 8, CV_32S);


	std::vector<cv::Vec3b> colors(nLabels);
	colors[0] = cv::Vec3b(0, 0, 0);
	//std::cout << "Number of connected components = " << nLabels << std::endl << std::endl;

	for (int label = 1; label < nLabels; ++label) {
		colors[label] = cv::Vec3b((std::rand() & 255), (std::rand() & 255), (std::rand() & 255));

		int left = stats.at<int>(label, CC_STAT_LEFT) + offsetX;
		int top = stats.at<int>(label, CC_STAT_TOP) + offsetY;
		int width = stats.at<int>(label, CC_STAT_WIDTH);
		int height = stats.at<int>(label, CC_STAT_HEIGHT);
		int area = stats.at<int>(label, CC_STAT_AREA);
		Point centroid = Point(centroids.at<double>(label, 0), centroids.at<double>(label, 1));

		if (area > 100 && abs(width / height) < 2) {
			rectangle(srcImg, Rect(left, top, width, height), Scalar(0, 0, 255), 2);

		}


	}

	//show labeling color
	Mat dst(binaryImg.size(), CV_8UC3);
	for (int r = 0; r < dst.rows; ++r) {
		for (int c = 0; c < dst.cols; ++c) {
			int label = labelImg.at<int>(r, c);
			Vec3b &pixel = dst.at<Vec3b>(r, c); //get the index of the pixel
			pixel = colors[label]; //set new pixel
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

		//leftSrc = src(Rect(0, videoSize.height / 2, videoSize.width / 2, videoSize.height / 2));
		//cvtColor(leftSrc, leftGrey, CV_BGR2GRAY);
		//
		//rectangle(leftSrc, Rect(10, leftSrc.rows / 7 * 2, leftSrc.cols / 7 * 6, leftSrc.rows / 4 * 2), Scalar(0, 255, 0), 2); // detect ROI
		//leftROI = leftGrey(Rect(10, leftSrc.rows / 7 * 2, leftSrc.cols / 7 * 6, leftSrc.rows / 4 * 2));
		//
		////adaptiveThreshold(leftROI, leftROI, 0, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 11, 5);
		////equalizeHist(leftROI, leftROI);
		//threshold(leftROI, leftROI, 220, 255, THRESH_BINARY); //OTSU is not necessary to set thres
		//removeNoice(leftROI);
		//detectLight(leftSrc, leftROI, 10, leftSrc.rows / 7 * 2);
		

		rightSrc = src(Rect(videoSize.width / 2, videoSize.height / 2, videoSize.width / 2, videoSize.height / 2));
		cvtColor(rightSrc, rightGrey, CV_BGR2GRAY);
		rectangle(rightSrc, Rect(100, rightSrc.rows / 7 * 2, rightSrc.cols / 7 * 6, rightSrc.rows / 4 * 2), Scalar(0, 255, 0), 2); // detect ROI
		rightROI = rightGrey(Rect(100, rightSrc.rows / 7 * 2, rightSrc.cols / 7 * 6, rightSrc.rows / 4 * 2));
		threshold(rightROI, rightROI, 220, 255, THRESH_BINARY); //OTSU is not necessary to set thres
		removeNoice(rightROI);
		detectLight(rightSrc, rightROI, 100, rightSrc.rows / 7 * 2);
		
		//imshow("Left Src", leftSrc);
		//imshow("Left Grey", leftGrey);
		//imshow("Left ROI", leftROI);
		
		imshow("Right Src", rightSrc);
		imshow("Right Grey", rightGrey);
		imshow("Right ROI", rightROI);
		
		waitKey(1);
	}



}