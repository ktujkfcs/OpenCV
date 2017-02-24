#pragma once

#include <stdlib.h>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2\opencv.hpp>
#include <iostream>
#include <time.h>
#include <opencv\cv.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <math.h>



namespace Project2 {
	using namespace System::Runtime::InteropServices;
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace std;
	using namespace cv;


	/** Function Headers */
	void detectAndDisplay(Mat frame);
	//Каскады Каара
	CascadeClassifier face_cascade;
	CascadeClassifier eyes_cascade;
	CascadeClassifier month_cascade;
	CascadeClassifier noce_cascade;
	string window_name = "Capture - Face detection";

	//Поворот изображения
	Mat rotateImage(const Mat& source, double angle)
	{
		Point2f src_center(source.cols / 2.0F, source.rows / 2.0F);
		Mat rot_mat = getRotationMatrix2D(src_center, angle, 1.0);
		Mat dst;
		warpAffine(source, dst, rot_mat, source.size());
		return dst;
	}
	//convert IplImage* to Mat
	Mat IplToMat(IplImage* image){
		Mat mat1 = image;
		return mat1;
	}
	//convert Mat to IplImage*
	IplImage* MatToIpl(Mat image){

		IplImage* image1 = cvCloneImage(&(IplImage)image);

		return image1;
	}

	//вычисление коэфициента яркости
	double kof_brightness(IplImage* image){
		Mat img = IplToMat(image).clone();
		long long ss2 = 0;
		for (int y(0); y < img.rows; ++y)
		{
			for (int x(0); x < img.cols; ++x)
				ss2 += img.at<unsigned char>(y, x);
		}
		double ddd = ss2 / (img.rows*img.cols);
		if (ddd < 100)return 1.5;
		else if (ddd>170)return 0.7;
		else return((ddd / 120.0) / 2.0);

	}
	//настройка резкости

	IplImage* Calibration(IplImage* image, int i)
	{
		int w = i;
		IplImage* dst = cvCloneImage(image);
		float kernel[9];
		kernel[0] = -0.1;
		kernel[1] = -0.1;
		kernel[2] = -0.1;

		kernel[3] = -0.1;
		kernel[4] = 2;
		kernel[5] = -0.1;

		kernel[6] = -0.1;
		kernel[7] = -0.1;
		kernel[8] = -0.1;
		// матрица
		CvMat kernel_matrix = cvMat(3, 3, CV_32FC1, kernel);
		// накладываем фильтр
		Mat f;
		cvFilter2D(image, dst, &kernel_matrix, cvPoint(-1, -1));
		return dst;

	}
	double eyeLeftX, eyeLeftY, eyeRightX, eyeRightY, noceY, noceX, monthX, monthY;
	//detect eye and darving
	void Eye_draving(Mat src, Mat frame, int i, int face_x, int face_y, int face_w, int face_h){
		eyeLeftX = 0;
		eyeLeftY = 0;
		eyeRightX = 0; eyeRightY = 0; noceY = 0; noceX = 0; monthX = 0; monthY=0;
		double kof_w = (face_w) / 400.0;
		double kof_h = (face_h) / 400.0;
		std::vector<Rect> eyes;
		//eyes_cascade.detectMultiScale(src, eyes, 1.1, 3, 0 | CV_HAAR_SCALE_IMAGE, Size(70, 30));
		eyes_cascade.detectMultiScale(src, eyes, 1.1, 1, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(20, 10));
		if (eyes.size() == 2){
			if ((face_x + (eyes[0].x + eyes[0].width*0.5)*kof_w, face_y) < (face_x + (eyes[1].x + eyes[1].width*0.5)*kof_w, face_y)){
				eyeLeftX = face_x + (eyes[0].x + eyes[0].width*0.5)*kof_w;
				eyeLeftY = face_y + (eyes[0].y + eyes[0].height*0.5)*kof_h;
				eyeRightX = face_x+ (eyes[1].x + eyes[1].width*0.5)*kof_w;
				eyeRightY = face_y+ (eyes[1].y + eyes[1].height*0.5)*kof_h;
						}
			else {
				eyeLeftX = face_x + (eyes[1].x + eyes[1].width*0.5)*kof_w;
				eyeLeftY = face_y + (eyes[1].y + eyes[1].height*0.5)*kof_h;
				eyeRightX = face_x+ (eyes[0].x + eyes[0].width*0.5)*kof_w;
				eyeRightY = face_y+ (eyes[0].y + eyes[0].height*0.5)*kof_h;
			}
		
		}else{
			eyeLeftX = 0; eyeLeftY = 0; 
		}
		for (size_t j = 0; j < eyes.size(); j++)
		{
			double d23 = (eyes[j].y)*kof_h;
			double d_top = face_h / 6.0;
			double d_bootom = face_h / 2.0;
			if ((d23>d_bootom) || (d23 < d_top))continue;

			cv::Point center2(face_x + (eyes[j].x + eyes[j].width*0.0)*kof_w, face_y + (eyes[j].y + eyes[j].height*0.5)*kof_h);
			cv::Point center3(face_x + (eyes[j].x + eyes[j].width*1.0)*kof_w, face_y + (eyes[j].y + eyes[j].height*0.5)*kof_h);
			//Mat image_12 = rotateImage(src, 30);
			int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
			circle(frame, center2, 2, Scalar(255, 0, 0), 2, 8, 0);
			circle(frame, center3, 2, Scalar(255, 0, 0), 2, 8, 0);

		//	cv::Point center4(face_x + (eyes[j].x + eyes[j].width*0.5)*kof_w, face_y + (eyes[j].y + eyes[j].height*0.5)*kof_h);
			//circle(frame, center4, 2, Scalar(255, 255,255), 2, 8, 0);


		}
		stringstream ss;
		srand(time(NULL));
		int dd = i;
		ss << dd;
		string str = ss.str();
		//src = rotateImage(src, 2 * 30);
		//	imshow(str, src);
	}
	//detect mouth and darving
	void mouth_draving(Mat src, Mat frame, int i, int face_x, int face_y, int face_w, int face_h){
		double kof_w = (face_w) / 400.0;
		double kof_h = (face_h) / 400.0;

		std::vector<Rect> eyes;
		month_cascade.detectMultiScale(src, eyes, 1.1, 3, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(70, 30));
		if (eyes.size() >= 1){
			monthX = face_x + (eyes[0].x + eyes[0].width*0.5)*kof_w;
			monthY = face_y + (eyes[0].y + eyes[0].height*0.5)*kof_h;

		}
		else{ 
				eyeLeftX = 0; eyeLeftY = 0; 
		}

		for (size_t j = 0; j < eyes.size(); j++)
		{

			double d23 = (eyes[j].y)*kof_h;
			double d223 = face_h / 1.6;
			if (d23 < d223)continue;
			cv::Point center2(face_x + (eyes[j].x + eyes[j].width*0.0)*kof_w, face_y + (eyes[j].y + eyes[j].height*0.5)*kof_h);
			cv::Point center3(face_x + (eyes[j].x + eyes[j].width*1.0)*kof_w, face_y + (eyes[j].y + eyes[j].height*0.5)*kof_h);
			int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
			circle(frame, center2, 2, Scalar(255, 222, 0), 2, 8, 0);
			circle(frame, center3, 2, Scalar(255, 222, 0), 2, 8, 0);
			
			//cv::Point center4(face_x + (eyes[j].x + eyes[j].width*0.5)*kof_w, face_y + (eyes[j].y + eyes[j].height*0.5)*kof_h);
		//	circle(frame, center4, 2, Scalar(255, 255, 255), 2, 8, 0);

			break;
		}
		stringstream ss;

		int dd = i;
		ss << dd;
		string str = ss.str();
		//src = rotateImage(src, 2 * 30);
		//imshow(str, src);
	}
	void noce_draving(Mat src, Mat frame, int i, int face_x, int face_y, int face_w, int face_h){
		double kof_w = (face_w) / 400.0;
		double kof_h = (face_h) / 400.0;
		std::vector<Rect> eyes;
		noce_cascade.detectMultiScale(src, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
		if (eyes.size() == 1){
			noceX = face_x + (eyes[0].x + eyes[0].width*0.5)*kof_w;
				noceY = face_y + (eyes[0].y + eyes[0].height*0.5)*kof_h;

		}
		else{ 
			eyeLeftX = 0; eyeLeftY = 0;
		}

		for (size_t j = 0; j < eyes.size(); j++)
		{

			double d23 = (eyes[j].y)*kof_h;
			double d_x = (eyes[j].x)*kof_h;
			double d223 = 2 * face_h / 3;
			double x_l = face_w / 4;
			double x_r = face_w - face_w / 4;

			if ((d23>d223) || (d23 < (face_h / 3.0)))continue;
			if ((d_x<x_l) || (d_x>x_r))continue;

			cv::Point center2(face_x + (eyes[j].x + eyes[j].width*0.5)*kof_w, face_y + (eyes[j].y + eyes[j].height*0.5)*kof_h);
		
			circle(frame, center2, 2, Scalar(0, 255, 0), 2, 8, 0);

			break;
		}
		stringstream ss;
		srand(time(NULL));
		int dd = i;
		ss << dd;
		string str = ss.str();
		//src = rotateImage(src, 2 * 30);
		//	imshow(str, src);

	}


	void detect(Mat faceROI, Mat frame2, int i, int  x, int y, int width, int height){
		Mat src;
		Mat faceROI2 = faceROI.clone();
		resize(faceROI2, src, cv::Size(400, 400));
		src *= kof_brightness(MatToIpl(src));
		src = MatToIpl(Calibration(MatToIpl(src), i));
		Eye_draving(src, frame2, i, x, y, width, height);
		mouth_draving(src, frame2, i, x, y, width, height);
		noce_draving(src, frame2, i, x, y, width, height);
		//vec_draving(src, frame2, i, x, y, width, height);


	}
	void detectAndDisplay(Mat frame)
	{
		std::vector<Rect> faces;
		Mat frame_gray;
		cvtColor(frame, frame_gray, CV_BGR2GRAY);
		equalizeHist(frame_gray, frame_gray);
		//-- Detect faces
		face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
		for (size_t i = 0; i < faces.size(); i++)
		{
			cv::Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
			ellipse(frame, center, cv::Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar(255, 0, 255), 1, 8, 0);
			Mat faceROI = frame_gray(faces[i]);
			int wid = faces[i].width;
			int hight = faces[i].height;
			detect(faceROI.clone(), frame, i, faces[i].x, faces[i].y, faces[i].width, faces[i].height);
		}
		//-- Show what you got
		imshow(window_name, frame);
	}

	
	
	void detectAndDisplay1(Mat frame, int count)
	{
		std::vector<Rect> faces;
		Mat frame_gray;

		cvtColor(frame, frame_gray, CV_BGR2GRAY);
		equalizeHist(frame_gray, frame_gray);

		//-- Detect faces
		face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));

		for (size_t i = 0; i < faces.size(); i++)
		{
			cv::Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
			ellipse(frame, center, cv::Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);

			Mat faceROI = frame_gray(faces[i]);
			std::vector<Rect> eyes;
			
			//-- In each face, detect eyes
			face_cascade.detectMultiScale(faceROI, eyes, 1.05, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));

			for (size_t j = 0; j < eyes.size(); j++)
			{
				cv::Point center1(faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5);
				int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
				circle(frame, center1, radius, Scalar(255, 0, 0), 4, 8, 0);
				cv::Point center2(center1.x + 4 * (center1.x - center.x), center1.y + 4 * (center1.y - center.y));
				line(frame, center, center2,Scalar(0, 0, 255),2);
			}
		}
		
			imshow(window_name, frame);
		
	}
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown1;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private: System::Windows::Forms::Button^  button4;
	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->numericUpDown1 = (gcnew System::Windows::Forms::NumericUpDown());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->button4 = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown1))->BeginInit();
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(137, 9);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(96, 23);
			this->button1->TabIndex = 0;
			this->button1->Text = L"Веб камера";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(9, 14);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(58, 13);
			this->label1->TabIndex = 2;
			this->label1->Text = L"webcamID";
			// 
			// numericUpDown1
			// 
			this->numericUpDown1->Location = System::Drawing::Point(73, 9);
			this->numericUpDown1->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10, 0, 0, 0 });
			this->numericUpDown1->Name = L"numericUpDown1";
			this->numericUpDown1->Size = System::Drawing::Size(47, 20);
			this->numericUpDown1->TabIndex = 3;
			this->numericUpDown1->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(137, 38);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(96, 23);
			this->button2->TabIndex = 4;
			this->button2->Text = L"Изображение";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(137, 67);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(96, 23);
			this->button3->TabIndex = 5;
			this->button3->Text = L"Видео";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &MyForm::button3_Click);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// button4
			// 
			this->button4->Location = System::Drawing::Point(12, 67);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(74, 23);
			this->button4->TabIndex = 6;
			this->button4->Text = L"Об авторае";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &MyForm::button4_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(268, 97);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->numericUpDown1);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->button1);
			this->Name = L"MyForm";
			this->Text = L"Detect";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}




#pragma endregion
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->Visible = false;
				 IplImage* image, *imgout;
				 imgout = 0;
				 CvCapture* capture;
				 if ((capture = cvCreateCameraCapture((int)(this->numericUpDown1->Value))) == 0){
					 cout << "ERROR: NO CAMERAS DETECTED." << endl;
					 // return ;
				 }
				 while (1){
					 if ((image = cvQueryFrame(capture)) == 0){
						 cout << "ERROR: BAD FRAME!" << endl;
						 //	 return 0;
					 }
					 if (imgout) {
						 cvReleaseImage(&imgout);
					 }
				 detectAndDisplay(IplToMat(image));
				
					 char key = cvWaitKey(5);
					 if (key == 27) {
						 cout << "Exiting on KEY: " << key << endl;
						 this->Visible = true;
						 break;
					 }
				 }
				 if (imgout) {
					 cvReleaseImage(&imgout);
				 }
				 cvReleaseCapture(&capture);
				 cvDestroyWindow("Example");

	}
			 char * and_SysStringToChar(System::String^ string)
			 {
				 return (char*)(void*)Marshal::StringToHGlobalAnsi(string);
			 }
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
				 if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK){
					 char *str1 = and_SysStringToChar(openFileDialog1->FileName);
					 IplImage* image = cvLoadImage(str1, 1);
					 //detectAndDisplay(IplToMat(image));
					 
					 detectAndDisplay(IplToMat(image));
					 detectAndDisplay1(IplToMat(image), 0);
					 char key = cvWaitKey(0);
					 this->Visible = true;
					 if (image) {
						 cvReleaseImage(&image);
					 }
					 cvDestroyWindow("Example");
				 }
	}
	private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
				 if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK){
					 this->Visible = false;
					 char key;
					 IplImage* image;

					 char *str1 = and_SysStringToChar(openFileDialog1->FileName);
					 CvCapture* capture = cvCreateFileCapture(str1);

					 while (1){
						 if ((image = cvQueryFrame(capture)) == 0){
							 cout << "ERROR: BAD FRAME!" << endl;
							 //	 return 0;
						 }
				 detectAndDisplay(IplToMat(image));
				
						 key = cvWaitKey(33);
						 if (key == 27) {
							 cout << "Exiting on KEY: " << key << endl;
							 this->Visible = true;
							 break;
						 }
					 }
					 if (image) {
						 cvReleaseImage(&image);
					 }
					 cvReleaseCapture(&capture);
					 cvDestroyWindow("Example");
				 }
	}
	private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) {
				 MessageBox::Show("Автором проекта является Андрей Калоша", "Об авторе");
	}
	private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e) {
				 //load cascades
				 button1->Enabled = true; button2->Enabled = true; button3->Enabled = true;
				 if (!face_cascade.load("haarcascade_frontalface_alt.xml")){ printf("--(!)Error loading\n"); button1->Enabled = true; button2->Enabled = true; button3->Enabled = true; MessageBox::Show("Не удалось загрузить каскады face"); };
				 if (!eyes_cascade.load("haarcascade_eye.xml")){ printf("--(!)Error loading\n"); button1->Enabled = true; button2->Enabled = true; button3->Enabled = true; MessageBox::Show("Не удалось загрузить каскады eye"); };
				 if (!month_cascade.load("haarcascade_mcs_mouth.xml")){ printf("--(!)Error loading\n"); button1->Enabled = true; button2->Enabled = true; button3->Enabled = true; MessageBox::Show("Не удалось загрузить каскады mouth"); };
				 if (!noce_cascade.load("haarcascade_mcs_nose.xml")){ printf("--(!)Error loading\n"); button1->Enabled = true; button2->Enabled = true; button3->Enabled = true; MessageBox::Show("Не удалось загрузить каскады nose"); };
	}
	};
}
