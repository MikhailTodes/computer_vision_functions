//compile code -> g++ histo.cpp -o histo.e `pkg-config --cflags --libs opencv`

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp> 
#include <iostream>
#include <ctype.h>

using namespace std;
using namespace cv;

Mat HistoEqualisation (Mat input);

int main()
{       




    //read the image
    Mat image = imread("moon.bmp");
    
    imshow("Image", image);
    
    Mat output = HistoEqualisation(image);       

    imshow("Histo Equal", output);
imshow("Image", image);
    imwrite("HistoEqualisation.bmp", output);
           
    while (true){//stay open till any key is pressed
      if(waitKey()>0){break;}
    }
  
  return 0;
}

Mat HistoEqualisation (Mat input){

  Mat output = input.clone();//create second image (same size) for the applied morph_ops

  int freq [256] = {0};
  
  //***************Populating the freq array**********
  for( int y = 0; y < input.rows; y++ ) {
    for( int x = 0; x < input.cols; x++ ) {      
      freq[input.at<Vec3b>(y,x)[0]]++;//Add one to the freq of pixel values for the read pixel
    }
  }
  //************************************************

  //***********Add the cumulative probability to create new spread out pixels******************
  double probTot = 0;
  int newPixVal [256] = {0};

  for (int i = 0; i < 256; i++){
    probTot += freq[i];
    float temp = (probTot*255)/(input.rows*input.cols);
    newPixVal[i]=temp;
  }
  //******************************************************************************************

  
  //***********SETTING OUTPUT IMAGE TO NEW VALUES****************
  for( int y = 0; y < input.rows; y++ ) {
    for( int x = 0; x < input.cols; x++ ) { 
      int col = newPixVal[input.at<Vec3b>(y,x)[0]];
      output.at<Vec3b>(y,x) = Vec3b(col,col,col);//Set the scaled value
    }
  }
  //************************************************************

  return output;
}
