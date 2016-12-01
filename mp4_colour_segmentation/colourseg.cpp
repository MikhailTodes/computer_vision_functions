//compile code -> g++ colourseg.cpp -o colourseg.e `pkg-config --cflags --libs opencv`

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp> 
#include "opencv/cv.h"
#include <iostream>
#include <string>
#include <ctype.h>

using namespace std;
using namespace cv;

Mat Training (void);
Mat ColourSegment (Mat input, Mat histo);


int main()
{      
  Training();//Call the training method to establish histogram

  Mat input = imread("mikhail.jpg");
  Mat histo = imread("Histogram.bmp");
  Mat output = ColourSegment(input, histo);
  imshow ("Output", output);

  imshow ("Histo", histo);
  imwrite ("CS_gun1.bmp", output);

  while (true){//stay open till any key is pressed
    if(waitKey()>0){break;}
  }
  
  return 0;
}


Mat Training (void){
  int total = 0;
  float hueSatCombo [255][255] = {0, 0};
  
  for (int i = 1; i <= 6; i++){//Loop through the six training images
    //read the image
    ostringstream filename;
    filename << "training_data/sample"<< i << ".bmp";
    string s = filename.str();
    Mat image = imread(s), hsv_image;//Read in image and create and empty hsv Mat    
    cvtColor(image, hsv_image, CV_BGR2HSV);//convert to HSV 

    
    //**********Populating and scaling the histogram matrix************
    for( int y = 0; y < hsv_image.rows; y++ ) {
      for( int x = 0; x < hsv_image.cols; x++ ) {   
	float h = (hsv_image.at<Vec3b>(y,x)[0])*255/180, s = (hsv_image.at<Vec3b>(y,x)[1]); 
	hueSatCombo[(int)h][(int)s] = hueSatCombo[(int)h][(int)s]+1;//update histogram
	total++;	
      }
    }
    //***********************************************************
  }

  Mat out(Size(255, 255), CV_8UC3); 
  //***********SETTING OUTPUT IMAGE TO NEW VALUES****************
  for( int y = 0; y < 255; y++ ) {
    for( int x = 0; x < 255; x++ ) { 
      int col1 = hueSatCombo[y][x];
      out.at<Vec3b>(y,x) = Vec3b(col1,col1,0);//Set the scaled value
    }
  }
  //*************************************************************    
  imwrite("Histogram.bmp", out);  
  return out;
}


Mat ColourSegment (Mat input, Mat histo){

  Mat hsv_image;
  cvtColor(input, hsv_image, CV_BGR2HSV);//convert to HSV
  int threshold = 1;

  //*************Filtering with the inputted histogram*****************
  for( int y = 0; y < hsv_image.rows; y++ ) {
    for( int x = 0; x < hsv_image.cols; x++ ) {   
      float h = (hsv_image.at<Vec3b>(y,x)[0])*255/180, s = (hsv_image.at<Vec3b>(y,x)[1]); 
      if (histo.at<Vec3b>((int)h,(int)s)[0]<threshold){//If outside threshhold 
	input.at<Vec3b>(y,x) = Vec3b(0,0,0);//Set pixels black
      }      	
    }
  }
  //***********************************************************
  
  return input;
}
