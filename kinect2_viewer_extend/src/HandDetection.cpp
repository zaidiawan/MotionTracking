#include "HandDetection.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

HandDetection::HandDetection(){
	frameNumber=0;
	nrNoFinger=0;
	fontFace = FONT_HERSHEY_PLAIN;
}

void HandDetection::initVectors(){
	hullI=vector<vector<int> >(contours.size());
	hullP=vector<vector<Point> >(contours.size());
	defects=vector<vector<Vec4i> > (contours.size());	
}

void HandDetection::analyzeContours(){
	bRect_height=bRect.height;
	bRect_width=bRect.width;
}

string HandDetection::bool2string(bool tf){
	if(tf)
		return "true";
	else
		return "false";
}

string HandDetection::intToString(int number){
		stringstream ss;
		ss << number;
		string str = ss.str();
		return str;
}

void HandDetection::printGestureInfo(Mat src){
	int fontFace = FONT_HERSHEY_PLAIN;
	Scalar fColor(245,200,200);
	int xpos=src.cols/1.5;
	int ypos=src.rows/1.6;
	float fontSize=0.7f;
	int lineChange=14;		
	string info= "Figure info:";
	putText(src,info,Point(ypos,xpos),fontFace,fontSize,fColor);
	xpos+=lineChange;
	info=string("Number of defects: ") + string(intToString(nrOfDefects)) ;
	putText(src,info,Point(ypos,xpos),fontFace,fontSize  ,fColor);
	xpos+=lineChange;
	info=string("bounding box height, width ") + string(intToString(bRect_height)) + string(" , ") +  string(intToString(bRect_width)) ;
	putText(src,info,Point(ypos,xpos),fontFace,fontSize ,fColor);
	xpos+=lineChange;
	info=string("Is hand: ") + string(bool2string(isHand));
	putText(src,info,Point(ypos,xpos),fontFace,fontSize  ,fColor);
}

bool HandDetection::detectIfHand(){
	analyzeContours();
	double h = bRect_height; 
	double w = bRect_width;
	isHand=true;
	if(fingerTips.size() > 5 ){
		isHand=false;
	}else if(h==0 || w == 0){
		isHand=false;
	}else if(h/w > 4 || w/h >4){
		isHand=false;	
	}else if(bRect.x<20){
		isHand=false;	
	}	
	return isHand;
}

float HandDetection::distanceP2P(Point a, Point b){
	float d= sqrt(fabs((double) pow((double)(a.x-b.x),2) + (double)pow((double)(a.y-b.y),2) )) ;  
	return d;
}

// remove fingertips that are too close to 
// eachother
void HandDetection::removeRedundantFingerTips(){
	vector<Point> newFingers;
    int maxLimit1 = fingerTips.size();
    for(int i=0;i< maxLimit1 ;i++){
        for(int j=i;j<maxLimit1;j++){
			if(distanceP2P(fingerTips[i],fingerTips[j])<10 && i!=j){
			}else{
				newFingers.push_back(fingerTips[i]);	
				break;
			}	
		}	
	}
	fingerTips.swap(newFingers);
}

void HandDetection::computeFingerNumber(){
	std::sort(fingerNumbers.begin(), fingerNumbers.end());
	int frequentNr;	
	int thisNumberFreq=1;
	int highestFreq=1;
	frequentNr=fingerNumbers[0];
    int maxLimit1 = fingerNumbers.size();
    for(int i=1;i<maxLimit1; i++){
		if(fingerNumbers[i-1]!=fingerNumbers[i]){
			if(thisNumberFreq>highestFreq){
				frequentNr=fingerNumbers[i-1];	
				highestFreq=thisNumberFreq;
			}
			thisNumberFreq=0;	
		}
		thisNumberFreq++;	
	}
	if(thisNumberFreq>highestFreq){
		frequentNr=fingerNumbers[fingerNumbers.size()-1];	
	}
	mostFrequentFingerNumber=frequentNr;	
}

void HandDetection::addFingerNumberToVector(){
	int i=fingerTips.size();	
	fingerNumbers.push_back(i);
}

// add the calculated number of fingers to image m->src
void HandDetection::addNumberToImg(Mat& srcMat){
	int xPos=10;
	int yPos=10;
	int offset=30;
	float fontSize=1.5f;
	int fontFace = FONT_HERSHEY_PLAIN;
    int maxLimit1 = numbers2Display.size();
    for(int i=0;i<maxLimit1;i++){
		rectangle(srcMat,Point(xPos,yPos),Point(xPos+offset,yPos+offset),numberColor, 2);	
		putText(srcMat, intToString(numbers2Display[i]),Point(xPos+7,yPos+offset-3),fontFace,fontSize,numberColor);
		xPos+=40;
		if(xPos>(srcMat.cols-srcMat.cols/3.2)){
			yPos+=40;
			xPos=10;
		}
	}
}

// calculate most frequent numbers of fingers 
// over 20 frames
void HandDetection::getFingerNumber(Mat& srcMat){
	removeRedundantFingerTips();
	if(bRect.height > srcMat.rows/2 && nrNoFinger>12 && isHand ){
		numberColor=Scalar(0,200,0);
		addFingerNumberToVector();
		if(frameNumber>12){
			nrNoFinger=0;
			frameNumber=0;	
			computeFingerNumber();	
			numbers2Display.push_back(mostFrequentFingerNumber);
			fingerNumbers.clear();
		}else{
			frameNumber++;
		}
	}else{
		nrNoFinger++;
		numberColor=Scalar(200,200,200);
	}
	addNumberToImg(srcMat);
}

float HandDetection::getAngle(Point s, Point f, Point e){
	float l1 = distanceP2P(f,s);
	float l2 = distanceP2P(f,e);
	float dot=(s.x-f.x)*(e.x-f.x) + (s.y-f.y)*(e.y-f.y);
	float angle = acos(dot/(l1*l2));
    angle=angle*180/PI_1;
	return angle;
}

void HandDetection::eleminateDefects(){
	int tolerance =  bRect_height/5;
	float angleTol=95;
	vector<Vec4i> newDefects;
	int startidx, endidx, faridx;
	vector<Vec4i>::iterator d=defects[cIdx].begin();
	while( d!=defects[cIdx].end() ) {
   	    Vec4i& v=(*d);
	    startidx=v[0]; Point ptStart(contours[cIdx][startidx] );
   		endidx=v[1]; Point ptEnd(contours[cIdx][endidx] );
  	    faridx=v[2]; Point ptFar(contours[cIdx][faridx] );
		if(distanceP2P(ptStart, ptFar) > tolerance && distanceP2P(ptEnd, ptFar) > tolerance && getAngle(ptStart, ptFar, ptEnd  ) < angleTol ){
			if( ptEnd.y > (bRect.y + bRect.height -bRect.height/4 ) ){
			}else if( ptStart.y > (bRect.y + bRect.height -bRect.height/4 ) ){
			}else {
				newDefects.push_back(v);		
			}
		}	
		d++;
	}
	nrOfDefects=newDefects.size();
	defects[cIdx].swap(newDefects);
	removeRedundantEndPoints(defects[cIdx]);
}

// remove endpoint of convexity defects if they are at the same fingertip
void HandDetection::removeRedundantEndPoints(vector<Vec4i> newDefects){
	Vec4i temp;
    //float avgX, avgY;
	float tolerance=bRect_width/6;
    int startidx, endidx ;
	int startidx2, endidx2;
    int maxLimit1 = newDefects.size();
    for(int i=0;i<maxLimit1;i++){
        for(int j=i;j<maxLimit1;j++){
	    	startidx=newDefects[i][0]; Point ptStart(contours[cIdx][startidx] );
	   		endidx=newDefects[i][1]; Point ptEnd(contours[cIdx][endidx] );
	    	startidx2=newDefects[j][0]; Point ptStart2(contours[cIdx][startidx2] );
	   		endidx2=newDefects[j][1]; Point ptEnd2(contours[cIdx][endidx2] );
			if(distanceP2P(ptStart,ptEnd2) < tolerance ){
				contours[cIdx][startidx]=ptEnd2;
				break;
			}if(distanceP2P(ptEnd,ptStart2) < tolerance ){
				contours[cIdx][startidx2]=ptEnd;
			}
		}
	}
}

// convexity defects does not check for one finger
// so another method has to check when there are no
// convexity defects
void HandDetection::checkForOneFinger(Mat& srcMat){
	int yTol=bRect.height/6;
	Point highestP;
	highestP.y=srcMat.rows;
	vector<Point>::iterator d=contours[cIdx].begin();
	while( d!=contours[cIdx].end() ) {
   	    Point v=(*d);
		if(v.y<highestP.y){
			highestP=v;
			cout<<highestP.y<<endl;
		}
		d++;	
	}int n=0;
	d=hullP[cIdx].begin();
	while( d!=hullP[cIdx].end() ) {
   	    Point v=(*d);
			//cout<<"x " << v.x << " y "<<  v.y << " highestpY " << highestP.y<< "ytol "<<yTol<<endl;
		if(v.y<highestP.y+yTol && v.y!=highestP.y && v.x!=highestP.x){
			n++;
		}
		d++;	
	}if(n==0){
		fingerTips.push_back(highestP);
	}
}

void HandDetection::drawFingerTips(Mat& srcMat){
	Point p;
    //int k=0;
    int maxLimit1 = fingerTips.size();
    for(int i=0;i<maxLimit1;i++){
		p=fingerTips[i];
		putText(srcMat,intToString(i),p-Point(0,30),1, 1.2f,Scalar(200,200,200),2);
   		circle( srcMat,p,   5, Scalar(100,255,100), 2 );
   	 }
}

void HandDetection::getFingerTips(Mat& srcMat){
	fingerTips.clear();
	int i=0;
	vector<Vec4i>::iterator d=defects[cIdx].begin();
	while( d!=defects[cIdx].end() ) {
   	    Vec4i& v=(*d);
	    int startidx=v[0]; Point ptStart(contours[cIdx][startidx] );
   		int endidx=v[1]; Point ptEnd(contours[cIdx][endidx] );
  	    int faridx=v[2]; Point ptFar(contours[cIdx][faridx] );
		if(i==0){
			fingerTips.push_back(ptStart);
			i++;
		}
		fingerTips.push_back(ptEnd);
		d++;
		i++;
   	}
	if(fingerTips.size()==0){
		checkForOneFinger(srcMat);
	}
}
