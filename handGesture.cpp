#include "handGesture.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <fstream>

using namespace cv;
using namespace std;

HandGesture::HandGesture(){
	frameNumber=0;
	nrNoFinger=0;
	fontFace = FONT_HERSHEY_PLAIN;
}

void HandGesture::initVectors(){
	hullI=vector<vector<int> >(contours.size());
	hullP=vector<vector<Point> >(contours.size());
	defects=vector<vector<Vec4i> > (contours.size());
}

void HandGesture::analyzeContours(){
	bRect_height=bRect.height;
	bRect_width=bRect.width;
}

string HandGesture::bool2string(bool tf){
	if(tf)
		return "true";
	else
		return "false";
}

string HandGesture::intToString(int number){
		stringstream ss;
		ss << number;
		string str = ss.str();
		return str;
}



void HandGesture::printGestureInfo(Mat src){
	int fontFace = FONT_HERSHEY_PLAIN;
	Scalar fColor(245,200,200);
	int xpos=src.cols/1.5;
	int ypos=src.rows/1.6;
	float fontSize=0.7f;
	int lineChange=14;
	string info= "Figure info:";

//putText(src,info,Point(ypos,xpos),fontFace,fontSize,fColor);
	xpos+=lineChange;
	info=string("Number of defects: ") + string(intToString(nrOfDefects)) ;
	//putText(src,info,Point(ypos,xpos),fontFace,fontSize  ,fColor);
	xpos+=lineChange;
	info=string("bounding box height, width ") + string(intToString(bRect_height)) + string(" , ") +  string(intToString(bRect_width)) ;
	//putText(src,info,Point(ypos,xpos),fontFace,fontSize ,fColor);
	xpos+=lineChange;
	info=string("Is hand: ") + string(bool2string(isHand));
	//putText(src,info,Point(ypos,xpos),fontFace,fontSize  ,fColor);
}

bool HandGesture::detectIfHand(){
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

float HandGesture::distanceP2P(Point a, Point b){
	float d= sqrt(fabs( pow(a.x-b.x,2) + pow(a.y-b.y,2) )) ;
	return d;
}

// remove fingertips that are too close to
// eachother
void HandGesture::removeRedundantFingerTips(){
	vector<Point> newFingers;
	for(int i=0;i<fingerTips.size();i++){
		for(int j=i;j<fingerTips.size();j++){
			if(distanceP2P(fingerTips[i],fingerTips[j])<10 && i!=j){
			}else{
				newFingers.push_back(fingerTips[i]);
				break;
			}
		}
	}
	fingerTips.swap(newFingers);
}

/********************* finger detect method */
void HandGesture::computeFingerNumber(){
	std::sort(fingerNumbers.begin(), fingerNumbers.end());
	int frequentNr;
	int thisNumberFreq=1;
	int highestFreq=1;
	frequentNr=fingerNumbers[0];
	for(int i=1;i<fingerNumbers.size(); i++){
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

	/***********************  code added */

	ofstream jinxd;

	cout<<endl;
	cout<<"Displaying finger count ";
	cout<<mostFrequentFingerNumber<<endl;

	switch(mostFrequentFingerNumber){

    case 0:
            break;

	case 1:
        cout<<"TV on"<<endl;
        cout<<"Please wait for 2 sec before next action"<<endl;
        cout<<endl;
        cout<<"Executing mute";
        usleep(10000000);
        system("irsend -#20  SEND_START AC KEY_MUTE; sleep 0.8");
      //  system("cd  /home/ajinkya/Documents/codes/1.led_blink/ && sudo avrdude -p m16 -c usbasp flash:r:main.hex");
     //   system("sudo avrdude -p m16 -c usbasp flash:r:main.hex");
		//system("cd  /home/ajinkya/Documents/codes/1.led_blink/ && sudo avrdude -p m16 -c usbasp -U flash:w:main.hex");
        usleep(2000000);
        break;

    case 2:
        cout<<"channel forward"<<endl;
        cout<<"Please wait for 2 sec before next action"<<endl;
        cout<<endl;

        cout<<"Executing 0";
        usleep(10000000);
        system("irsend -#20  SEND_START AC KEY_0; sleep 0.8");
        //system("cd /home/ajinkya/Documents/codes/2.lcd_test/ && sudo avrdude -p m16 -c usbasp flash:r:main.hex");
        //system("sudo avrdude -p m16 -c usbasp flash:r:main.hex");
		//system("cd /home/ajinkya/Documents/codes/2.lcd_test/ && sudo avrdude -p m16 -c usbasp -U flash:w:main.hex");
        usleep(2000000);
        break;

    case 3:
        cout<<"channel backward"<<endl;
        cout<<"Please wait for 2 sec before next action"<<endl;
        cout<<endl;

        cout<<"Executing 1";
        usleep(10000000);
        system("irsend -#20  SEND_START AC KEY_1; sleep 0.8");
        usleep(2000000);
        break;

    case 4:
        cout<<"volume increase"<<endl;
        cout<<"Please wait for 2 sec before next action"<<endl;
        cout<<endl;

        cout<<"Executing 2";
        usleep(10000000);
        system("irsend -#20  SEND_START AC KEY_2; sleep 0.8");
        usleep(2000000);
        break;


    case 5:
        cout<<"volume decrease"<<endl;
        cout<<"Please wait for 2 sec before next action"<<endl;
        cout<<endl;

        cout<<"Executing mute";
        usleep(10000000);
        system("irsend -#20  SEND_START AC KEY_MUTE; sleep 0.8");
        usleep(2000000);
        break;

    default:
        cout<<"Please input a gesture from 1 to 5"<<endl;
        cout<<"Please wait for 2 sec before next action";
        cout<<endl;
        usleep(2000000);

        break;



	}

	cout<<endl;

	/************** added code ends */
}

void HandGesture::addFingerNumberToVector(){
	int i=fingerTips.size();
	fingerNumbers.push_back(i);
}

// add the calculated number of fingers to image m->src
void HandGesture::addNumberToImg(MyImage *m){
	int xPos=10;
	int yPos=10;
	int offset=30;
	float fontSize=1.5f;
	int fontFace = FONT_HERSHEY_PLAIN;
	for(int i=0;i<numbers2Display.size();i++){
		rectangle(m->src,Point(xPos,yPos),Point(xPos+offset,yPos+offset),numberColor, 2);
		putText(m->src, intToString(numbers2Display[i]),Point(xPos+7,yPos+offset-3),fontFace,fontSize,numberColor);
		xPos+=40;
		if(xPos>(m->src.cols-m->src.cols/3.2)){
			yPos+=40;
			xPos=10;
		}
	}
}

// calculate most frequent numbers of fingers
// over 20 frames
void HandGesture::getFingerNumber(MyImage *m){
	removeRedundantFingerTips();
	if(bRect.height > m->src.rows/2 && nrNoFinger>12 && isHand ){
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
	addNumberToImg(m);
}

float HandGesture::getAngle(Point s, Point f, Point e){
	float l1 = distanceP2P(f,s);
	float l2 = distanceP2P(f,e);
	float dot=(s.x-f.x)*(e.x-f.x) + (s.y-f.y)*(e.y-f.y);
	float angle = acos(dot/(l1*l2));
	angle=angle*180/PI;
	return angle;
}

void HandGesture::eleminateDefects(MyImage *m){
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
	removeRedundantEndPoints(defects[cIdx], m);
}

// remove endpoint of convexity defects if they are at the same fingertip
void HandGesture::removeRedundantEndPoints(vector<Vec4i> newDefects,MyImage *m){
	Vec4i temp;
	float avgX, avgY;
	float tolerance=bRect_width/6;
	int startidx, endidx, faridx;
	int startidx2, endidx2;
	for(int i=0;i<newDefects.size();i++){
		for(int j=i;j<newDefects.size();j++){
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
void HandGesture::checkForOneFinger(MyImage *m){
	int yTol=bRect.height/6;
	Point highestP;
	highestP.y=m->src.rows;
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
			cout<<"x " << v.x << " y "<<  v.y << " highestpY " << highestP.y<< "ytol "<<yTol<<endl;
		if(v.y<highestP.y+yTol && v.y!=highestP.y && v.x!=highestP.x){
			n++;
		}
		d++;
	}if(n==0){
		fingerTips.push_back(highestP);
	}
}

void HandGesture::drawFingerTips(MyImage *m){
	Point p;
	int k=0;
	for(int i=0;i<fingerTips.size();i++){
		p=fingerTips[i];
		putText(m->src,intToString(i),p-Point(0,30),fontFace, 1.2f,Scalar(200,200,200),2);
   		circle( m->src,p,   5, Scalar(100,255,100), 4 );
   	 }
}

void HandGesture::getFingerTips(MyImage *m){
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
		checkForOneFinger(m);
	}
}
