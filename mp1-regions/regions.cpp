//compile code -> g++ regions.cpp -o regions.e `pkg-config --cflags --libs opencv`

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp> 
#include <stdio.h>   

using namespace cv;

int main()
{        
  int count_white = 0;

    //read the image
    Mat image= imread("face.bmp");
    Mat imageInv = imread("face.bmp");//create second image (same size) for coloured regions

    namedWindow("Image");
    imshow("Image", image);

    //************E_TABLE*******************
    /*This serves as a table to record the region of each pixel. It is initialised here to equal zero for all pixels*/
    int pixelsRegion[image.rows][image.cols];
    for( int y = 0; y < image.rows; y++ ) {
      for( int x = 0; x < image.cols; x++ ) {
	pixelsRegion[y][x]=0;
      }
    }
    //**************************************

    int regionCount = 0;
    char up = 0, left = 0;
    for (int recursion = 0; recursion<image.cols; recursion++){
    regionCount =0;
    count_white= 0;
      for( int y = 0; y < image.rows; y++ ) {
	for( int x = 0; x < image.cols; x++ ) {

	  if ( image.at<Vec3b>(y,x) == Vec3b(255,255,255) ) {//if pixel is white
	    count_white++;//counting the total number of white pixels

	    //*****CHECKING ABOVE AND LEFT OF PIXEL****************
	    try{
	    if (pixelsRegion[y-1][x]>0){//if pixel above is already part of a region
	      up = 1;
	    }
	    if (pixelsRegion[y][x-1]>0){//if pixel left is already part of a region
	      left = 1;
	    }
	    }
	    catch(int e){
	      printf("Boundary Conditions");
	    }
	    //*****************************************************

	    //********ACTIONS IN DIFFERENT CASES**************
	    if (up == 0 && left == 0){
	      regionCount++;
	      pixelsRegion[y][x]=regionCount;
	    }
	    if (up==1){
	      pixelsRegion[y][x]=pixelsRegion[y-1][x];
	    }
	    if (left==1){
	      pixelsRegion[y][x]=pixelsRegion[y][x-1];
	    }
	    if (up==1 && left ==1){
	      pixelsRegion[y][x]=pixelsRegion[y-1][x];
	      pixelsRegion[y][x-1]=pixelsRegion[y-1][x];
	    }
	    //---------------CORRECTING THE E_TABLE WITH UPDATES------------------
	    if(pixelsRegion[y+1][x]>0&&pixelsRegion[y+1][x]!=pixelsRegion[y][x]){pixelsRegion[y][x]=pixelsRegion[y+1][x];}
	    if(pixelsRegion[y][x+1]>0&&pixelsRegion[y][x+1]!=pixelsRegion[y][x]){pixelsRegion[y][x]=pixelsRegion[y][x+1];}
	    //--------------------------------------------------------------------
	    //***************************************************** 
	   } 
	  up=0;//reset check variables
	  left=0;//reset check variable
	}
      }
    }

     
    //********SETTING COLOURS OF DIFFERENT REGIONS************
    for( int y = 0; y < image.rows; y++ ) {
      for( int x = 0; x < image.cols; x++ ) {
    	if(pixelsRegion[y][x]>0){
    	  int b=((pixelsRegion[y][x]*222)),g=((pixelsRegion[y][x]*22)),r((pixelsRegion[y][x]*100));
    	    imageInv.at<Vec3b>(y,x) = Vec3b(b,g,r);
    	}
      }
    }
    //********************************************************
 
    namedWindow("Region Coloured");
    imshow("Region Coloured", imageInv);
    imwrite("Face Region Coloured.bmp", imageInv);

    while (true){//stay open till any key is pressed
      if(waitKey()>0){break;}
    }

  printf ("White pixels =  %d\n",count_white);
  printf ("%d-rows, %d-columns\n", image.rows, image.cols);
  
  return 0;
}
