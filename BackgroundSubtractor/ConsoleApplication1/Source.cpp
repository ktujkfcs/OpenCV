#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <iostream>
#include <sstream>
#include <stdio.h>

using namespace cv;
using namespace std;

Mat frame, frameNew;
int key;
Ptr<BackgroundSubtractor> substr;// subtractor

int main(int argc, char* argv[])
{
	namedWindow("delta");
	namedWindow("orig");
	substr = createBackgroundSubtractorMOG2(); 
		VideoCapture cap("2.mp4");
		if (!cap.isOpened()) {
			cerr << "Error opening file";
			return 0;
		}
		else
		while ((char) key != 27) {
			if (!cap.read(frame)) {
				cerr << "не получен следующий кадр, возможно закончено видео" << endl;
				waitKey(3000);
				return 0;
			}
			substr->apply(frame, frameNew);
			imshow("orig", frame);
			imshow("delta", frameNew);
				key = waitKey(33);
		}
		cap.release();
		//освобождение памяти для всех окон
	destroyAllWindows();
	return 0;
}
