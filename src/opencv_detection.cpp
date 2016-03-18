//  opencv_detection.cpp
//  OpenCV-Test
//
// Simple OpenCV object/scene match using:
// SURF (Speeded-Up Robust Features) feature detection and description
//  (moved to opencv_contrib module in OpenCV 3.0)
// FLANN (Fast Library for Approximation of Nearest Neighbours) feature matching
//
// Requires following OpenCV libraries:
//  opencv_core, opencv_features2d, opencv_flann,
//  opencv_highgui, opencv_imgcodecs, opencv_xfeatures2d
//  opencv_imgproc, opencv_calib3d

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// requires opencv_contrib module
#include <opencv2/xfeatures2d.hpp>

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

int main(int argc, char *argv[]) {
    
    // Allow use of custom images but use demo images as default
    string fnameObj, fnameScene;
    if (argc != 3) {
        // use demo images
        cout << "Usage: " << argv[0] << " [object_image] [scene_image]" << endl;
        cout << "Using demo images" << endl;
        
        string basePath = "./images/";
        fnameObj = basePath + "box.png";
        fnameScene = basePath + "box_in_scene.png";
    } else {
        // use image files provided by user
        fnameObj   = argv[1];
        fnameScene = argv[2];
    }

    Mat imObj = imread(fnameObj, IMREAD_GRAYSCALE);
    Mat imScene = imread(fnameScene, IMREAD_GRAYSCALE);
    
    if(imObj.empty() || imScene.empty()) {
        cout << "Failed to load images" << endl;
        return -1;
    }
    
    // Show object and scene
    namedWindow("Object");
    imshow("Object", imObj);
    namedWindow("Scene");
    imshow("Scene", imScene);
    
    // Detect keypoints and compute their descriptors
    Ptr<Feature2D> surfDetector = SURF::create(); // using defaults
    vector<KeyPoint> objPoints, scenePoints;
    Mat objDescriptor, sceneDescriptor;
    surfDetector->detectAndCompute(imObj, Mat(), objPoints, objDescriptor);
    surfDetector->detectAndCompute(imScene, Mat(), scenePoints, sceneDescriptor);
    
    // Match features between images
    FlannBasedMatcher matcher;
    vector<DMatch> matches;
    matcher.match(objDescriptor, sceneDescriptor, matches);
    
#if defined(DEBUG_SHOW_ALL_MATCHES)
    // Illustrate _all_ matches (regardless of how "good")
    // (use 'NOT_DRAW_SINGLE_POINTS' flag to ignore unmatched keypoints)
    Mat imAllMatch;
    drawMatches(imObj, objPoints, imScene, scenePoints, matches, imAllMatch,
                Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
    namedWindow("All Matches");
    imshow("All Matches", imAllMatch);
#endif // DEBUG_SHOW_ALL_MATCHES
    
    // Extract "good" match points
    // Determine minimum distance
    double minDist = 100;
    for (int i=0; i<matches.size(); ++i) {
        if (matches[i].distance < minDist) {
            minDist = matches[i].distance;
        }
    }
    // populate vector of good matches
    // defining 'good match' as <= 2.5*minDist
    // (recommended factor is 2-3)
    vector<DMatch> goodMatches;
    for (int i=0;i<matches.size();++i) {
        if(matches[i].distance < 3*minDist) {
            goodMatches.push_back(matches[i]);
        }
    }
    
    // Illustrate "Good" Matches
    // (use 'NOT_DRAW_SINGLE_POINTS' flag to ignore unmatched keypoints)
    Mat imGoodMatch;
    drawMatches(imObj, objPoints, imScene, scenePoints, goodMatches, imGoodMatch,
                Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
    namedWindow("Good Matches");
    imshow("Good Matches", imGoodMatch);
    
    // determine transformation from object points to scene points
    vector<Point2f> objGoodPoints, sceneGoodPoints;
    for(int i=0;i<goodMatches.size();++i) {

        objGoodPoints.push_back(objPoints[goodMatches[i].queryIdx].pt);
        sceneGoodPoints.push_back(scenePoints[goodMatches[i].trainIdx].pt);
    }
    Mat tform = findHomography(objGoodPoints, sceneGoodPoints, CV_RANSAC);

    // Draw polygon around detected object in scene
    vector<Point2f> objCorners(4), sceneCorners(4);
    // Object corners
    objCorners[0] = Point2f(0,0);                           // top-left corner
    objCorners[1] = Point2f(0, imObj.rows-1);               // bottom-left corner
    objCorners[2] = Point2f(imObj.cols-1, imObj.rows-1);    // bottom-right corner
    objCorners[3] = Point2f(imObj.cols-1, 0);               // top-right corner
    // Convert object corners to scene corners
    perspectiveTransform(objCorners, sceneCorners, tform);
    // Draw polygon in scene; remember to account for offset
    Point2f offset = Point2f(imObj.cols,0); // offset to account for object image width
    line(imGoodMatch, sceneCorners[0]+offset, sceneCorners[1]+offset, Scalar(255,0,0),2);
    line(imGoodMatch, sceneCorners[1]+offset, sceneCorners[2]+offset, Scalar(255,0,0),2);
    line(imGoodMatch, sceneCorners[2]+offset, sceneCorners[3]+offset, Scalar(255,0,0),2);
    line(imGoodMatch, sceneCorners[3]+offset, sceneCorners[0]+offset, Scalar(255,0,0),2);
    
    imshow("Good Matches", imGoodMatch);
    
    waitKey(0);
    
    return 0;
}
