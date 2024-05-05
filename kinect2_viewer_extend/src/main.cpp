//#include "opencv2/video/tracking.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include <time.h>
//#include <iostream>
//#include <ctype.h>

//#include "main.h"
//#include "OpenCVKinect.h"
  
//using namespace cv;
//using namespace std;
 
//// Usage: ./app input.jpg
//#include "opencv2/opencv.hpp"
//#include <string>

//using namespace cv;
//using namespace std;

//#include "MainMethodClass.h"
 
//#include "Kalman.hpp"
//#include "GestureRecognizer.h"
 
///** 16/8/15 code **/
 
// //this main is the current one... which has all the complete upto date code....
//int main( int argc, char** argv )
//{
	   
//	//Kinect data
//	OpenCVKinect myDataCap;
	 
//	if(!myDataCap.init())
//	{
//		std::cout << "Error initializing" << std::endl;
//		return 1;
//	}
	 
//	//timer initialization
//	clock_t initial, final, intemediate, intemediate1,frameRateInitial,frameRateInitialFinal;
  
//	// >>>> Kalman Filter
//	int stateSize = 6;
//	int measSize = 4;
//	int contrSize = 0;
 
//	unsigned int type = CV_32F;
//	cv::KalmanFilter kfLeft(stateSize, measSize, contrSize, type);
//	cv::KalmanFilter kfRight(stateSize, measSize, contrSize, type);
//	MainMethodClass mmc(kfLeft,kfRight);
//	mmc.createTrackbars();
//	frameRateInitial = clock();
//	int count = 0;
//	for(;;)
//    {
//		initial = clock();
//	 	dataStream = myDataCap.getData();
//		mmc.run(dataStream[C_COLOR_STREAM],dataStream[C_DEPTH_STREAM],myDataCap,frameRate);
//		final = clock() - initial;

//		if(final != 0)
//		frameRate = 1000 / final;
		 
//		//if(count >= 15){
//		//	//find how much time for 15 frames..
//		//	frameRateInitialFinal = clock() - frameRateInitial;

//		//	//reset initial timer..
//		//	frameRateInitial = clock();

//		//	//do the math.. if 15 frames takes that much time then how much time one frame takes..
//		//	frameRate = frameRateInitialFinal/15;
//		//	count = 0;
//		//}
//		cout<< "final time : "<<(double)final<<endl<<endl;
//        char c = (char)waitKey(1);
       
//    }

	  
//    return 0;
//}




 
