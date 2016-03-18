CC	= g++
CFLAGS	= -I /usr/local/Cellar/opencv3/3.1.0_1/include/
LDFLAGS	= -L /usr/local/Cellar/opencv3/3.1.0_1/lib/ -lopencv_core -lopencv_features2d -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc 

all: blend cap detect clean

# opencv_blending
blend: opencv_blending.o
	$(CC) -o $@ $^ $(LDFLAGS)

opencv_blending.o: src/opencv_blending.cpp
	$(CC) -c $(CFLAGS) $<

# opencv_capture
cap: opencv_capture.o
	$(CC) -o $@ $^ $(LDFLAGS) -lopencv_videoio

opencv_capture.o: src/opencv_capture.cpp
	$(CC) -c $(CFLAGS) $<

# opencv_detect
detect: opencv_detection.o
	$(CC) -o $@ $^ $(LDFLAGS) -lopencv_calib3d -lopencv_flann -lopencv_xfeatures2d

opencv_detection.o: src/opencv_detection.cpp
	$(CC) -c $(CFLAGS) $<


.PHONY: clean cleanest

clean:
	rm *.o

cleanest:
	rm blend cap detect
