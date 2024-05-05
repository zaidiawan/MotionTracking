#ifndef _HelperFunctions_
#define _HelperFunctions_
 

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/video/tracking.hpp>
 

#include <iostream>
#include <ctype.h>


#include "Kalman.h"
using namespace std;
using namespace cv;
 
 
class HelperFunctions{


	int MINNOOFPIXELS ;
 
	/*
	
	bool showHist;
	
	
	int vmin , vmax , smin ;

	int hueMin1 ;
	int hueMax1 ;
	int hueMin2 ;
	int hueMax2 ;

	int staticHueMin1 ;
	int staticHueMax1 ;
	int staticHueMin2 ;
	int staticHueMax2 ;

	int satMin ;
	int satMax ;
	int valMin ;
	int valMax ;
 
	int medianBlurCnt ;
	int erodeAmt ;
	int dilateAmt ;
	int backGroundSubtractionFlag ;

	int flagForImage ;
	int flagForKalman ;
	int globalCounter ;
	int NOOFFRAMES ;
	
	vector<Mat> colorFramesForStdDeviation;
	vector<Mat> binMaskFramesForStdDeviation;
  

	Rect trackWindow;
	RotatedRect trackBox;
	int hsize ;
	float* hranges ;
	const float* phranges ; 
	*/
	public:

		
/*
	Mat hsv, hue,sat,val, mask, hist, histimg, backproj;
	Mat trackWindowImage;

	// kalman stuff
	cv::Point center;
	
	//BackGroundSubtraction stuff 
	BackGroundSubtraction* backGroundSubtraction; 
	 
	Kalman* kal;
	*/
	 

	
	

	
	HelperFunctions();
	void preFiltering(cv::Mat& srcMat);
	void methodForCOunterDetectionAndThresholding(cv::Mat& srcColorMat,cv::Mat& returnGrayMat);
	 
	int findBiggestContour(vector<vector<Point> > contours);
	 
	void stdDeviationModified(vector<Mat>& mask,vector<Mat>& filteredMatVec,int& retHueMin1,int& retHueMax1);
	void indAvgOfIntVector(vector<int>& srcVec,int& avg);
	void calHist(cv::Mat& srcMat,cv::Mat& mask);
	void calAndDrawHist(cv::Mat& srcMat,cv::Mat& mask);
	float findBiggestValueInHist(cv::Mat& srcMat);
	
	
	
	void mainFunction(cv::Mat& image, KalmanFilter& KF);
	 
	void createTrackbar();
};
#endif
