//compile code -> g++ morph_ops.cpp -o morph_ops.e `pkg-config --cflags --libs opencv`

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp> 
#include <iostream>
#include <ctype.h>

using namespace std;
using namespace cv;

Mat Dilation (Mat input, int SE);
Mat Erosion (Mat input, int SE);
Mat Opening (Mat input, int SE);
Mat Closing (Mat input, int SE);
Mat Boundary (Mat input);

int main()
{       
    //read the image
    Mat image = imread("gun.bmp");
    

    imshow("Image", image);
    
    Mat bound = Boundary(image);
    Mat E = Erosion(image, 2);
    Mat D = Dilation(image, 8);
    Mat O = Opening(image, 2);
    Mat C = Closing(image, 8);
       

    imwrite("BoundaryGun.bmp", bound);
    imwrite("ErosionGun.bmp", E);
    imwrite("DilationGun.bmp", D);
    imwrite("OpeninGun.bmp", O);
    imwrite("ClosingGun.bmp", C);
       
    while (true){//stay open till any key is pressed
      if(waitKey()>0){break;}
    }
  
  return 0;
}

Mat Dilation (Mat input, int SE){

  Mat output = input.clone();//create second image (same size) for the applied morph_ops
  int white_count = 0;

  for( int y = 0; y < input.rows; y++ ) {
    for( int x = 0; x < input.cols; x++ ) {
      if ( input.at<Vec3b>(y,x) == Vec3b(255,255,255) ) {//if pixel is white
	if(y>(SE/2) && y<(input.rows-SE/2) && x>(SE/2) && x<(input.cols-SE/2)){//Dealing with border security
	  
	  //*****************SIZE FILTERING*********************
	  for( int a = (y-(4)); a < (y+(4)); a++){//Dilate
	    for ( int b = (x-(4)); b < (x+(4)); b++){
	      if ( input.at<Vec3b>(a,b) == Vec3b(255,255,255) ) {//if pixel is white
		white_count++;
	      }
	    }
	  }
	  if (white_count<(9)){
	    for( int a = (y-(4)); a < (y+(4)); a++){
	      for ( int b = (x-(4)); b < (x+(4)); b++){
		output.at<Vec3b>(a,b) = Vec3b(0,0,0);
	      }
	    }
	  }else{
	  //****************************************************
	  
	  //******************DILATION**************************
	    for( int a = (y-(SE/2)); a < (y+(SE/2)); a++){
	      for ( int b = (x-(SE/2)); b < (x+(SE/2)); b++){
		output.at<Vec3b>(a,b) = Vec3b(255,255,255);
	      }
	    }
	  }
	  //****************************************************
	  white_count = 0;
	}
      }
    }
  }
  return output;
}

Mat Erosion (Mat input, int SE){

  Mat output = input.clone();//create second image (same size) for the applied morph_ops

  for( int y = 0; y < input.rows; y++ ) {
    for( int x = 0; x < input.cols; x++ ) {
      if ( input.at<Vec3b>(y,x) == Vec3b(0,0,0) ) {//if pixel is black
	if(y>(SE/2) && y<(input.rows-SE/2) && x>(SE/2) && x<(input.cols-SE/2)){//Dealing with border security

	  //*****************EROSION**************************
	  for( int a = (y-(SE/2)); a < (y+(SE/2)); a++){//Erode
	    for ( int b = (x-(SE/2)); b < (x+(SE/2)); b++){
	      output.at<Vec3b>(a,b) = Vec3b(0,0,0);
	    }
	  }
	  //*************************************************
	}
      }
    }
  }
  return output;
}

Mat Opening (Mat input, int SE){//Erosion then Dilation
  return Dilation(Erosion(input, SE), SE+2);
}

Mat Closing (Mat input, int SE){//Dilation then Erosion
  return Erosion(Dilation(input, SE+2), SE);
}

Mat Boundary (Mat input){

  Mat output  = Dilation(input, 8);
  
  Mat temp = Erosion(output.clone(), 4);
  
  for( int y = 0; y < input.rows; y++ ) {
    for( int x = 0; x < input.cols; x++ ) {
      if ( temp.at<Vec3b>(y,x) == Vec3b(255,255,255) ) {//if pixel is white
	output.at<Vec3b>(y,x) = Vec3b(0,0,0);
      }      
    }
  }  
  return output;
}
