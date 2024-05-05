#include "HelperFunctions.h"

HelperFunctions::HelperFunctions(){
	 MINNOOFPIXELS = 200;
	 
}



 

float HelperFunctions::findBiggestValueInHist(cv::Mat& srcMat){

	float biggestValue = 0;
	 
	
		for(int j = 0; j< 255; j++) {
			
			if(biggestValue <= srcMat.at<float>(j)){
			
				biggestValue = srcMat.at<float>(j);
			}
		 
	}

	return biggestValue;
}
	
 
void HelperFunctions::calHist(cv::Mat& srcMat,cv::Mat& mask){

	//convert srcMat fromm BGR to HSV
	cvtColor(srcMat,srcMat,CV_BGR2HSV);
	/// Separate the image in 3 places ( B, G and R )
  vector<Mat> bgr_planes;
  split( srcMat, bgr_planes );

  /// Establish the number of bins
  int histSize = 256;

  /// Set the ranges ( for B,G,R) )
  float range[] = { 0, 256 } ;
  const float* histRange = { range };

  bool uniform = true; bool accumulate = false;

  Mat b_hist, g_hist, r_hist;

  /// Compute the histograms:
  calcHist( &bgr_planes[0], 1, 0, mask, b_hist, 1, &histSize, &histRange, uniform, accumulate );
  calcHist( &bgr_planes[1], 1, 0, mask, g_hist, 1, &histSize, &histRange, uniform, accumulate );
  calcHist( &bgr_planes[2], 1, 0, mask, r_hist, 1, &histSize, &histRange, uniform, accumulate );

  /*cv::Mat otsuMat;
  cv::threshold(bgr_planes[0], otsuMat, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);

  cv::imshow("otsuMat", otsuMat);*/

  /*
  float hueBig = findBiggestValueInHist(b_hist);
  float satBig = findBiggestValueInHist(g_hist);
  float valBig = findBiggestValueInHist(r_hist);
  */
  // Draw the histograms for B, G and R
  int hist_w = 512; int hist_h = 255;
  int bin_w = cvRound( (double) hist_w/histSize );

  Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

  /// Normalize the result to [ 0, histImage.rows ]
  normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
  normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
  normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
  /*
  float hueBig1 = findBiggestValueInHist(b_hist);
  float satBig1 = findBiggestValueInHist(g_hist);
  float valBig1 = findBiggestValueInHist(r_hist);
   */
  
  /// Draw for each channel
  for( int i = 1; i < histSize; i++ )
  {
      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                       Scalar( 255, 0, 0), 2, 8, 0  );
      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
                       Scalar( 0, 255, 0), 2, 8, 0  );
      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                       Scalar( 0, 0, 255), 2, 8, 0  );
  }

  /// Display
  namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE );
  imshow("calcHist Demo", histImage );

}

void HelperFunctions::calAndDrawHist(cv::Mat& srcMatHSV,cv::Mat& mask){

		int ch[] = {0, 0};
		int hsize = 16;
		float hranges[] = {0,180};
	    const float* phranges = hranges;

        cv::Mat hueTemp;
		hueTemp.create(srcMatHSV.size(), srcMatHSV.depth());
        mixChannels(&srcMatHSV, 1, &hueTemp, 1, ch, 1);	
		
		cv::Mat histTemp,histimgTemp = Mat::zeros(200, 320, CV_8UC3);
		
		calcHist(&hueTemp, 1, 0, mask, histTemp, 1, &hsize, &phranges);
                    normalize(histTemp, histTemp, 0, 255, CV_MINMAX);
                     
					
                    histimgTemp = Scalar::all(0);
                    int binWTemp = histimgTemp.cols / hsize;
                    Mat bufTemp(1, hsize, CV_8UC3);
                    for( int i = 0; i < hsize; i++ )
                        bufTemp.at<Vec3b>(i) = Vec3b(saturate_cast<uchar>(i*180./hsize), 255, 255);
                    cvtColor(bufTemp, bufTemp, CV_HSV2BGR);
                        
                    for( int i = 0; i < hsize; i++ )
                    {
                        int val = saturate_cast<int>(histTemp.at<float>(i)*histimgTemp.rows/255);
                        rectangle( histimgTemp, Point(i*binWTemp,histimgTemp.rows),
                                   Point((i+1)*binWTemp,histimgTemp.rows - val),
                                   Scalar(bufTemp.at<Vec3b>(i)), -1, 8 );
                    }

					imshow("histimgTemp",histimgTemp);
}

 
void HelperFunctions::indAvgOfIntVector(vector<int>& srcVec,int& avg){

	 
	int sum = 0;
    int maxLimit1 = srcVec.size();
    for(int i = 0; i < maxLimit1;i++){
		sum += srcVec[i];
	}

	if(!srcVec.empty()){
	avg = sum/srcVec.size();
	}
}

/** http://stackoverflow.com/questions/16293268/finding-hsv-thresholds-via-histograms-with-opencv  **/
void HelperFunctions::stdDeviationModified(vector<Mat>& mask,vector<Mat>& filteredMatVec,
	int& retHueMin1,int& retHueMax1){
	
	vector<int> huesMin,huesMax,satsMin,satsMax;

    int maxLimit1 = filteredMatVec.size();
    for(int i =0; i< maxLimit1;i++){
		// STEP 1: learning phase
		Mat hsv, imgThreshed, processed, denoised;
	   
		// must split all channels to get Hue only
		std::vector<cv::Mat> hsvPlanes;
		cv::split(filteredMatVec[i], hsvPlanes);

		// compute statistics for Hue value
		cv::Scalar meanHue, stddevHue;
		int hueMinLocal,hueMaxLocal;  
		cv::meanStdDev(hsvPlanes[0], meanHue, stddevHue,mask[i]);
		// ensure we get 95% of all valid Hue samples (statistics 3*sigma rule)
		hueMinLocal = (meanHue[0] - stddevHue[0]);
		hueMaxLocal = (meanHue[0] + stddevHue[0]);

		// check if hue min is between 0 to 2 then set it to 2
		if(hueMinLocal <= 2 && hueMinLocal >= 0){
			hueMinLocal = 2;
		}

		// check if hue min is -ve then add 180 in it(overflow)
		if(hueMinLocal < 0){
			hueMinLocal = 180 + hueMinLocal ;
		}
		
		//// if hue max is above 15, set it to 3
		//if(hueMinLocal >= 15){
		//	hueMinLocal = 3;
		//}

		//// if hue max is below 10, set it to 30
		//if(hueMaxLocal <= 10){
		//	hueMaxLocal = 30;
		//}
		
		// if hue max is above 180, set it to 180
		if(hueMaxLocal >= 180){
			hueMaxLocal = hueMaxLocal-180;
		}

		huesMin.push_back(hueMinLocal );
		huesMax.push_back(hueMaxLocal );
		 
	}
		indAvgOfIntVector(huesMin,retHueMin1);
		indAvgOfIntVector(huesMax,retHueMax1);


		cout<<"avg hue : "<<retHueMin1<<" , " << retHueMax1<<endl;
}



int HelperFunctions::findBiggestContour(vector<vector<Point> > contours){
    int indexOfBiggestContour = -1;
    int sizeOfBiggestContour = 0;
    int maxLimit1 = contours.size();
    for (int i = 0; i < maxLimit1; i++){
        int limit2 = contours[i].size();
        if(limit2 > sizeOfBiggestContour){
            sizeOfBiggestContour = contours[i].size();
            indexOfBiggestContour = i;
        }
    }
    return indexOfBiggestContour;
}
  
void HelperFunctions::methodForCOunterDetectionAndThresholding(cv::Mat& srcColorMat,cv::Mat& returnGrayMat) {

				cvtColor(srcColorMat,srcColorMat,CV_BGR2GRAY);
				const float scaleFactor = 0.05f;
                Mat show; srcColorMat.convertTo( show, CV_8UC1, scaleFactor );
                
				 //filter
               // if(blurPre == 1) blur(show,show,Size(blurAmt+1,blurAmt+1));
                Mat cntr; show.copyTo(cntr);
                erode(cntr,cntr,Mat(),Point(-1,-1),1);
              //  if(blurPre == 0) blur(cntr,cntr,Size(blurAmt+1,blurAmt+1));
                dilate(cntr,cntr,Mat(),Point(-1,-1),1);

                //compute and draw contours
				vector<vector<Point> > contours;
                findContours(cntr,contours,0,1);
                drawContours(cntr,contours,-1,Scalar(192,0,0),2,3);

                //optionally compute bounding box and circle to exclude small blobs(non human) or do further filtering,etc.
               int numContours = contours.size();
                vector<vector<Point> > contours_poly( numContours );
                vector<Rect> boundRect( numContours );
                vector<Point2f> centers( numContours );
                vector<float> radii(numContours);

				int index = findBiggestContour(contours);

                //for(int i = 0; i < numContours; i++ ){
                    approxPolyDP( Mat(contours[index]), contours_poly[index], 3, true );
                    boundRect[index] = boundingRect( Mat(contours_poly[index]) );
                    minEnclosingCircle(contours_poly[index],centers[index],radii[index]);
                    rectangle( cntr, boundRect[index].tl(), boundRect[index].br(), Scalar(64), 2, 8, 0 );
                    circle(cntr,centers[index],radii[index],Scalar(192));
                 //}
				 
                imshow( "contours", cntr );
				 
				returnGrayMat = cntr;
                
}

//this method converts white pixels to black for further processing.
//since black color represents 0,0,0 HSV value
void HelperFunctions::preFiltering(cv::Mat& srcMat){

	//split color image into 3 channels(B,G,R)
	/*vector<Mat> bgr_planes;
	split( srcMat, bgr_planes );*/

	for(int i = 0 ; i< srcMat.rows; i ++){
		for(int j = 0 ; j< srcMat.cols; j ++){
			
			/*if(bgr_planes[0].at<float>(i,j) == 255 && bgr_planes[1].at<float>(i,j) == 255 && bgr_planes[2].at<float>(i,j) == 255){
				srcMat.at<float>(i,j);
			}*/
			Vec3b intensity = srcMat.at<Vec3b>(i, j);
			if(intensity[0] >= 220 && intensity[1] >= 220 && intensity[2] >= 220){
				intensity[0] = 0;
				intensity[1] = 0;
				intensity[2] = 0;

				srcMat.at<Vec3b>(i,j) = intensity;
			}
		}
	}

	/*Mat tmp,alpha,dst;

	cvtColor(srcMat,tmp,CV_BGR2GRAY);
	threshold(tmp,alpha,100,200,THRESH_BINARY_INV);
	imshow("alpha",alpha);
	  cvtColor(alpha,alpha,CV_GRAY2BGR);
	  srcMat = srcMat & alpha;*/
	 

}





 

 
