//
//  opencv_blending.cpp
//  OpenCV-Test
//

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace std;
using namespace cv;

int main(int argc, char *argv[]) {
    double alpha = 0.5;
    string basePath = "../images/";
    string fname1 = basePath + "starry_night.jpg";
    string fname2 = basePath + "butterfly.jpg";

    // Read first image
    Mat im1 = imread(fname1.c_str());
    if(!im1.data) {
        cout << "Failed to open " << fname1 << endl;
        return -1;
    }
    // Display first image
    namedWindow(fname1.c_str());
    imshow(fname1.c_str(), im1);
    
    // Read second image
    Mat im2 = imread(fname2.c_str());
    if(!im2.data) {
        cout << "Failed to open " << fname2 << endl;
        return -1;
    }
    // Display second image
    namedWindow(fname2.c_str());
    imshow(fname2.c_str(), im2);

    // image sizes must match
    // resize image(s) to dimensions of largest image
    int maxWidth  = (im1.cols > im2.cols) ? im1.cols : im2.cols;
    int maxHeight = (im1.rows > im2.rows) ? im1.rows : im2.rows;

    Mat im1_resized(maxHeight, maxWidth, im1.type());
    // only resize images if dimensions are different
    if(maxWidth != im1.cols && maxHeight != im1.rows) {
        resize(im1, im1_resized, Size(im1_resized.cols,im1_resized.rows), 0.0, 0.0, INTER_CUBIC);
    }
    Mat im2_resized(maxHeight, maxWidth, im2.type());
    if(maxWidth != im2.cols && maxHeight != im2.rows) {
        resize(im2, im2_resized, Size(im2_resized.cols,im2_resized.rows), 0.0, 0.0, INTER_CUBIC);
    }
    
    // Create blended output image
    Mat imOut;
    addWeighted(im1_resized, alpha, im2_resized, 1.0-alpha, 0.0, imOut);
    // Display blended image
    namedWindow("Blended Image");
    imshow("Blended Image", imOut);
    
    // Wait for keypress
    waitKey(0);
    
    return 0;
}
