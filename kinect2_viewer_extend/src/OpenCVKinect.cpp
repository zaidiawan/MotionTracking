 
//#include "OpenCVKinect.h"

//OpenCVKinect::OpenCVKinect(void)
//{
//	m_depthTimeStamp = 0;
//	m_colorTimeStamp = 0;

//	m_alignedStreamStatus = false;
//	m_colorStreamStatus = false;
//	m_depthStreamStatus = false;

//	/*threshNear = 60;
//	threshFar = 100;
//	dilateAmt = 1;
//	erodeAmt = 1;
//	blurAmt = 1;
//	blurPre = 1;

	
//    namedWindow("depth map");
//    createTrackbar( "amount dilate", "depth map", &dilateAmt,16);
//    createTrackbar( "amount erode", "depth map", &erodeAmt,16);
//    createTrackbar( "amount blur", "depth map", &blurAmt,16);
//    createTrackbar( "blur pre", "depth map", &blurPre,1);
//    createTrackbar( "threshold near", "depth map", &threshNear,255);
//    createTrackbar( "threshold far", "depth map", &threshFar,255);*/

//}

//bool OpenCVKinect::setMode(int inMode)
//{
//	if((inMode&C_MODE_COLOR) == C_MODE_COLOR && (inMode&C_MODE_DEPTH) == C_MODE_DEPTH )
//	{
//		std::cout << "COLOR + DEPTH" << std::endl;
//		m_colorStreamStatus = true;
//		m_depthStreamStatus = true;


//		if((inMode&C_MODE_ALIGNED) == C_MODE_ALIGNED)
//		{
//			std::cout << "+ ALIGNED" << std::endl;
//			m_alignedStreamStatus = true;
//		}

//		return true;

//	}
//	else
//	{
//		std::cout << "NOTHING" << std::endl;
//		return false;

//	}
//	return false;
//}

//bool OpenCVKinect::init()

//{
//	setMode(C_MODE_DEPTH | C_MODE_COLOR | C_MODE_ALIGNED);
//	m_status = openni::STATUS_OK;
//	//const char* deviceURI = "C:/Users/Abhijit/Desktop/OniFiles/circle.oni";
//	const char* deviceURI = openni::ANY_DEVICE;
//	m_status = openni::OpenNI::initialize();

//	std::cout << "After initialization: " << std::endl;
//	std::cout << openni::OpenNI::getExtendedError() << std::endl;

//	// open the device
//	m_status = m_device.open(deviceURI);
//	if(m_status != openni::STATUS_OK)
//	{
//		std::cout << "OpenCVKinect: Device open failseed: " << std::endl;
//		std::cout << openni::OpenNI::getExtendedError() << std::endl;
//		openni::OpenNI::shutdown();
//		return false;
//	}

//	if(m_depthStreamStatus)
//	{
//		// create a depth object
//		m_status = m_depth.create(m_device, openni::SENSOR_DEPTH);
//		if(m_status == openni::STATUS_OK)
//		{
//			m_status = m_depth.start();
//			if(m_status != openni::STATUS_OK)
//			{
//				std::cout << "OpenCVKinect: Couldn't start depth stream: " << std::endl;
//				std::cout << openni::OpenNI::getExtendedError() << std::endl;
//				m_depth.destroy();
//				return false;
//			}
//		}
//		else
//		{
//			std::cout << "OpenCVKinect: Couldn't find depth stream: " << std::endl;
//			std::cout << openni::OpenNI::getExtendedError() << std::endl;
//			return false;
//		}
//	}

//	if(m_colorStreamStatus)
//	{
//		// create a color object
//		m_status = m_color.create(m_device, openni::SENSOR_COLOR);
//		if(m_status == openni::STATUS_OK)
//		{
//			m_status = m_color.start();
//			if(m_status != openni::STATUS_OK)
//			{

//				std::cout << "OpenCVKinect: Couldn't start color stream: " << std::endl;
//				std::cout << openni::OpenNI::getExtendedError() << std::endl;
//				m_color.destroy();
//				return false;
//			}
//		}
//		else
//		{
//			std::cout << "OpenCVKinect: Couldn't find color stream: " << std::endl;
//			std::cout << openni::OpenNI::getExtendedError() << std::endl;
//			return false;
//		}

//	}

//	if(m_alignedStreamStatus)
//	{
//		m_device.setImageRegistrationMode(openni::IMAGE_REGISTRATION_DEPTH_TO_COLOR);
//	}


//	if(!m_depth.isValid() && !m_color.isValid())
//	{
//		std::cout << "OpenCVKinect: No valid streams. Exiting" << std::endl;
//		openni::OpenNI::shutdown();
//		return false;
//	}

//	this->m_streams = new openni::VideoStream*[C_NUM_STREAMS];
//	m_streams[C_DEPTH_STREAM] = &m_depth;
//	m_streams[C_COLOR_STREAM] = &m_color;
//	/*const char* recorderURI = "C:/Users/Abhijit/Downloads/Test1.oni";
//	recorder.create(recorderURI);
//	recorder.attach(m_depth);
//	recorder.attach(m_color);
//	recorder.start();*/
//	return true;
//}


//cv::vector<cv::Mat> OpenCVKinect::getData()
//{
//	cv::vector<cv::Mat> returnVec;
//	cv::Mat bufferImage;
//	bool depthCaptured = false, colorCaptured = false;
//	Mat xCoOrdinateMat,yCoOrdinateMat,zCoOrdinateMat;
//	xCoOrdinateMat.create(480, 640, CV_16UC1);
//	yCoOrdinateMat.create(480, 640, CV_16UC1);
//	zCoOrdinateMat.create(480, 640, CV_16UC1);

//	if(this->m_colorStreamStatus & this->m_depthStreamStatus)
//	{
//		while( !depthCaptured || !colorCaptured || m_depthTimeStamp != m_colorTimeStamp)
//		{
//			m_status = openni::OpenNI::waitForAnyStream(m_streams, C_NUM_STREAMS, &m_currentStream, C_STREAM_TIMEOUT);
//			if(m_status != openni::STATUS_OK)
//			{
//				std::cout << "OpenCVKinect: Unable to wait for streams. Exiting" << std::endl;
//				exit(EXIT_FAILURE);
//			}

//			switch(m_currentStream)
//			{
//			case C_DEPTH_STREAM:
//				m_depth.readFrame(&m_depthFrame);
//				m_depthImage.create(m_depthFrame.getHeight(), m_depthFrame.getWidth(), CV_16UC1);
//				m_depthImage.data = (uchar*)m_depthFrame.getData();
//				this->m_depthTimeStamp = m_depthFrame.getTimestamp() >> 16;
////				std::cout << "Depth Timestamp: " << this->m_depthTimeStamp << std::endl;
//				depthCaptured = true;

				
//				break;
//			case C_COLOR_STREAM:
//				m_color.readFrame(&m_colorFrame);
//				m_colorImage.create(m_colorFrame.getHeight(), m_colorFrame.getWidth(), CV_8UC3);
//				bufferImage.create(m_colorFrame.getHeight(), m_colorFrame.getWidth(), CV_8UC3);
//				bufferImage.data = (uchar*)m_colorFrame.getData();
//				this->m_colorTimeStamp = m_colorFrame.getTimestamp() >> 16;
////				std::cout << "Color Timestamp: " << m_colorTimeStamp << std::endl;
//				colorCaptured = true;
//				cv::cvtColor(bufferImage, m_colorImage, CV_BGR2RGB);
//				break;
//			default:
//				break;
//			}

//		}
//	} else {
//		m_colorImage = cv::Mat::zeros(10, 10, CV_8UC1);
//		m_depthImage = cv::Mat::zeros(10, 10, CV_8UC1);

//	}


	
//	/*char stop;
//	cin>> stop;

//	if(stop == 'x' || stop == 'X'){
//		recorder.stop();
//		recorder.destroy();
//	}*/
//	returnVec.push_back(m_depthImage);
	 
//	returnVec.push_back(m_colorImage);

	
//	bufferImage.release();
//	return returnVec;
//}

//void OpenCVKinect::calculateWorldCoOrdinates(float x,float y,float depth,float& xW,float& yW,float& zW){
//	//for world coordinates..
	 
	 
//			float px, py, pz;
//			openni::CoordinateConverter::convertDepthToWorld(m_depth,x,y,depth,&px,&py,&pz);
			 
//			xW = px;
//			yW = py;
//			zW = pz;
			 
	 
//}
//void OpenCVKinect::registerDepthAndImage()
//{
//	m_device.setImageRegistrationMode(openni::IMAGE_REGISTRATION_DEPTH_TO_COLOR);
//}

//cv::Mat OpenCVKinect::getColor()
//{
//	return m_colorImage;
//}

//cv::Mat OpenCVKinect::getDepth()
//{
//	return m_depthImage;
//}

//OpenCVKinect::~OpenCVKinect(void)
//{
//	this->m_depthFrame.release();
//	this->m_colorFrame.release();
//	this->m_depth.stop();
//	this->m_color.stop();
//	openni::OpenNI::shutdown();
//	this->m_device.close();

//	recorder.stop();
//	recorder.destroy();
//}
 

////
////
/////**
////* this code is from : http://stackoverflow.com/questions/16584036/kinect-depth-image-processing/16590380#16590380
////*
////**/
////void OpenCVKinect::newMethodForTest(cv::Mat& srcMat) {
////
////				const float scaleFactor = 0.05f;
////                Mat show; srcMat.convertTo( show, CV_8UC1, scaleFactor );
////                //threshold
////                Mat tnear,tfar;
////                show.copyTo(tnear);
////                show.copyTo(tfar);
////                threshold(tnear,tnear,threshNear,255,CV_THRESH_TOZERO);
////                threshold(tfar,tfar,threshFar,255,CV_THRESH_TOZERO_INV);
////                show = tnear & tfar;//or cvAnd(tnear,tfar,show,NULL); to join the two thresholded images
////                //filter
////                if(blurPre == 1) blur(show,show,Size(blurAmt+1,blurAmt+1));
////                Mat cntr; show.copyTo(cntr);
////                erode(cntr,cntr,Mat(),Point(-1,-1),erodeAmt);
////                if(blurPre == 0) blur(cntr,cntr,Size(blurAmt+1,blurAmt+1));
////                dilate(cntr,cntr,Mat(),Point(-1,-1),dilateAmt);
////
////                //compute and draw contours
////                findContours(cntr,contours,0,1);
////                drawContours(cntr,contours,-1,Scalar(192,0,0),2,3);
////
////                //optionally compute bounding box and circle to exclude small blobs(non human) or do further filtering,etc.
////               int numContours = contours.size();
////                vector<vector<Point> > contours_poly( numContours );
////                vector<Rect> boundRect( numContours );
////                vector<Point2f> centers( numContours );
////                vector<float> radii(numContours);
////                for(int i = 0; i < numContours; i++ ){
////                    approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
////                    boundRect[i] = boundingRect( Mat(contours_poly[i]) );
////                    minEnclosingCircle(contours_poly[i],centers[i],radii[i]);
////                    rectangle( cntr, boundRect[i].tl(), boundRect[i].br(), Scalar(64), 2, 8, 0 );
////                    circle(cntr,centers[i],radii[i],Scalar(192));
////                 }
////				imshow( "depth map", show );
////                imshow( "contours", cntr );
////
////
////
////
////
////}
////
////
////
/////***********     METHODS TO DETECT THE SYNCHRONIZATION OF BOTH STREAMS... ************************/
////
////
////
////void OpenCVKinect::methodForTwoStreamsCombined(cv::Mat& depthMat,cv::Mat& colorMat) {
////
////	cv::imshow("Depth", depthMat);
////	cv::imshow("Color", colorMat);
////	cv::Mat returnMatForDepthMat;
////	methodForCOunterDetectionAndThresholding(depthMat,returnMatForDepthMat);
////	returnMatForDepthMat;
////
////	Mat gray;
////	cvtColor(colorMat,gray,CV_BGR2GRAY);
////
////	/*FileStorage fs;
////		fs.open("..//1.txt", FileStorage::WRITE);
////		fs <<"returnMatForDepthMat"<< returnMatForDepthMat;
////		fs <<"depthMat"<< depthMat;
////		fs <<"gray"<< gray;
////		fs.release();*/
////
////		cv::Mat andMat = gray & returnMatForDepthMat;
////
////
////cv::imshow("AndMat", andMat);
////cv::imshow("Gray", gray);
////cout<<"checkpoint"<<endl;
////}
////
////
////
////void OpenCVKinect::methodForCOunterDetectionAndThresholding(cv::Mat& srcMat,cv::Mat& returnMat) {
////
////
////	const float scaleFactor = 0.05f;
////                Mat show; srcMat.convertTo( show, CV_8UC1, scaleFactor );
////                //threshold
////                Mat tnear,tfar;
////                show.copyTo(tnear);
////                show.copyTo(tfar);
////                threshold(tnear,tnear,threshNear,255,CV_THRESH_TOZERO);
////                threshold(tfar,tfar,threshFar,255,CV_THRESH_TOZERO_INV);
////                show = tnear & tfar;//or cvAnd(tnear,tfar,show,NULL); to join the two thresholded images
////
////				 //filter
////                if(blurPre == 1) blur(show,show,Size(blurAmt+1,blurAmt+1));
////                Mat cntr; show.copyTo(cntr);
////                erode(cntr,cntr,Mat(),Point(-1,-1),erodeAmt);
////                if(blurPre == 0) blur(cntr,cntr,Size(blurAmt+1,blurAmt+1));
////                dilate(cntr,cntr,Mat(),Point(-1,-1),dilateAmt);
////
////                //compute and draw contours
////                findContours(cntr,contours,0,1);
////                drawContours(cntr,contours,-1,Scalar(192,0,0),2,3);
////
////                //optionally compute bounding box and circle to exclude small blobs(non human) or do further filtering,etc.
////               int numContours = contours.size();
////                vector<vector<Point> > contours_poly( numContours );
////                vector<Rect> boundRect( numContours );
////                vector<Point2f> centers( numContours );
////                vector<float> radii(numContours);
////                for(int i = 0; i < numContours; i++ ){
////                    approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
////                    boundRect[i] = boundingRect( Mat(contours_poly[i]) );
////                    minEnclosingCircle(contours_poly[i],centers[i],radii[i]);
////                    rectangle( cntr, boundRect[i].tl(), boundRect[i].br(), Scalar(64), 2, 8, 0 );
////                    circle(cntr,centers[i],radii[i],Scalar(192));
////                 }
////
////                imshow( "contours", cntr );
////
////				imshow( "depth map", show );
////				returnMat = cntr;
////
////}

 












