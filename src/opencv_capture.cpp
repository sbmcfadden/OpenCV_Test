#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace std;
using namespace cv;

// bug in OpenCV 3.1.0 on Mac OS X
#define APPLE_OPENCV3_BUG       \
    (__APPLE__              && \
    CV_VERSION_MAJOR == 3   && \
    CV_VERSION_MINOR == 1   && \
    CV_VERSION_REVISION == 0)

void printInfoAboutAppleOpenCV3Bug(void) {
    cout << "There is a bug in OpenCV 3.1.0 when using VideoCapture" << endl;
    cout << "and waitKey() on OS X 10.11" << endl;
    cout << "A fix has been submitted on GitHub but there is not yet" << endl;
    cout << "a stable release with this fix" << endl;
    cout << "(as of March 4, 2016)" << endl;
}

int captureVideo() {
    VideoCapture cap(0);
    if(!cap.isOpened()) {
        cout << "Failed to open Video Capture." << endl;
        return -1;
    }
    Mat frame;
    const char* windowName = "capWindow | Press any key to quit";
    namedWindow(windowName);
    while(cap.read(frame)) {
        imshow(windowName, frame);
        
        if(waitKey(16) >= 0) {
            cout << "User exit." << endl;
            break;
        }
    }
    cap.release();
    
    return 0;
}

int main(int argc, char *argv[]) {
    int ret_val = 0;
    try {
        ret_val = captureVideo();
    } catch (...) {
        cout << "Unhandled exception caught." << endl;
#if defined(APPLE_OPENCV3_BUG)
        printInfoAboutAppleOpenCV3Bug();
#endif
        return -1;
    }
    
    return ret_val;
}