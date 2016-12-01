//compile code -> g++ lightcorrection.cpp -o lightcorrection.e `pkg-config --cflags --libs opencv` 

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp> 
#include <iostream>
#include <ctype.h>

using namespace std;
using namespace cv;

Mat LightCorrection (Mat input);

int main()
{       
  //read the image
  Mat image = imread("HistoEqualisation.bmp");
    
  imshow("Image", image);
    
  Mat output = LightCorrection(image);       

  imshow("Light Corrected", output);
  imwrite("LightCorrected.bmp", output);
           
  while (true){//stay open till any key is pressed
    if(waitKey()>0){break;}
  }
  
  return 0;
}

Mat LightCorrection (Mat input){//Linear Method for light correction

  Mat output = input.clone();//create second image (same size) for the applied morph_ops

  //*****Create a normalised structural element 5x5********
  //*************NEEDS TO BE DIVIDED BY 74*****************
  float SE [5][5] = {{1,2,3,2,1},
  		     {2,3,4,3,2},
  		     {3,4,5,4,3},
   		     {2,3,4,3,2},
   		     {1,2,3,2,1}};

  //float SE [3][3] = {{1,2,1},{2,4,2},{1,2,1}};
  //*******************************************************

  
  //***********SETTING OUTPUT IMAGE WITH NEW VALUES****************
  for( int y = 0; y < input.rows; y++ ) {
    for( int x = 0; x < input.cols; x++ ) { 
      float avg = 0;
          
      int greyCol = input.at<Vec3b>(y,x)[0];

      //******USE THE STRUCT IMAGE TO CORRECT REGION*************
      for (int i = 0; i < 5; i++){
      	for (int j = 0; j < 5; j++){
      	  avg += (greyCol*SE[i][j]/74);
      	}
      }
      //*******************************************************
      int col = avg;
      output.at<Vec3b>(y,x) = Vec3b(col,col,col);//Set the scaled value      
    }
  }
  //***************************************************************

  return output;
}
