#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

int main()
{
	int ww = 200;
	
	double aa, cc;
	

			int pos = 0;
			int neg = 0;
			waitKey(1000);
			for (int k = 1; k < 81; k++){
				Mat image;
				char b[254];
				string st = "./1/";
					st+= itoa(k, b, 10);
				st += ".jpg";
				image = imread(st, CV_LOAD_IMAGE_COLOR);
				namedWindow("window1", 1);  
				imshow("window1", image);

				// Load Face cascade (.xml file)
				CascadeClassifier face_cascade;
				face_cascade.load("cascade6.xml");

				// Detect faces
				std::vector<Rect> faces;
				//face_cascade.detectMultiScale(image, faces, 1.7, 1, 0 | CV_HAAR_SCALE_IMAGE, Size(10, 10));
				  face_cascade.detectMultiScale(image, faces, 1.7, 3, 0 | CV_HAAR_SCALE_IMAGE, Size(10, 10));
			//	face_cascade.detectMultiScale(image, faces, 1.7, 5, 0 | CV_HAAR_SCALE_IMAGE, Size(200, 200), Size(500, 500));
				String text;
				  if ((faces.size() > 0) && (k<41)) {
					neg++;
				}
				  if ((faces.size() == 0) && (k > 40)) {
					pos++;
				}
				// Draw circles on the detected faces
				  
				  text = "men";
				for (int i = 0; i < faces.size(); i++)
				{
					text = "female";
					
					
					//Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
				//	ellipse(image, center, Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0, 340, Scalar(255, 0, 255), 1, 8, 0);
				}
				int baseline = 0; int fontFace = FONT_HERSHEY_SCRIPT_SIMPLEX;
				double fontScale = 2;
				int thickness = 3;

				Size textSize = getTextSize(text, fontFace,
					fontScale, thickness, &baseline);
				baseline += thickness;

				// center the text
				Point textOrg((image.cols - textSize.width) / 7,
					(image.rows + textSize.height) / 7);

		
				putText(image, text, textOrg, fontFace, fontScale,
					Scalar::all(0), thickness, 8);
imshow("Detected Face", image);
				cout << "pos: " << pos << "\tneg: " << neg << endl;
waitKey(300);

				
			}
			pos = (int)(((double)pos) / 40.0*100.0);
			neg = (int)(((double)neg) / 40.0*100.0);
			cout << "Fem: " << pos << "\tMen: " << neg << endl;
	
	int is = 5;
	while (1==1)waitKey(0);
	return 0;
}