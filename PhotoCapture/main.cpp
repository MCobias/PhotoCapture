//
//  main.cpp
//  PhotoCapture
//
//  Created by Marcelo Cobias on 4/9/16.
//  Copyright © 2016 Marcelo Cobias. All rights reserved.
//

#include "capturecamera.h"


void killCamera();
bool isDirExist(string path);
bool makePath(string path);
void captureCamera();
Mat drawLines(Mat image);
void printImage(Mat Image, string nameWindow, int PosX, int PosY);

int main(int argc, char *argv[])
{
    int count = 1, zoom = 0, contzoom = 0;
    CAPTURECAMERA cam;
    string name, year, sex;
    Mat imageCapture;
    
    cout << "Name Image: ";
    cin >> name;
    
    cout << "Age: ";
    cin >> year;
    
    cout << "Sex: ";
    cin >> sex;

    killCamera();
    
    if(isDirExist("/Users/mcobias/Desktop/DataSet/"))
    {
        if(cam.openCamera(&cam))
        {
            while(1)
            {
                cam.capturePreview(imageCapture);
                transpose(imageCapture, imageCapture);
                flip(imageCapture, imageCapture, 0);
                resize(imageCapture, imageCapture, Size(480, 640));
                drawLines(imageCapture);
                printImage(imageCapture, "Press ESC exit or P take a photo", 100, 100);
                createTrackbar("Zoom", "Press ESC exit or P take a photo", &zoom, 12);
                
                if(zoom != contzoom)
                {
                    cam.setConfigureCameraZoom(&cam, zoom);
                    contzoom = zoom;
                }
                
                int c = cvWaitKey(10);
                if(c == 'p')
                {
                    if(count < 2)
                    {
                        cam.setConfigureCameraAperture(&cam);
                        cam.setConfigureCameraImageSize(&cam);
                        cam.setConfigureCameraFlashMode(&cam);
                    }
                    
                    cam.capturePhoto(imageCapture, 1000);
                    transpose(imageCapture, imageCapture);
                    flip(imageCapture, imageCapture, 0);
                    imwrite("/Users/mcobias/Desktop/DataSet/ImageDataSet_" + name + "_" + year + "_" + sex + "_" + to_string(count) + ".jpg", imageCapture);
                    printImage(imageCapture, "Captured", 600, 100);
                    cam.removeImage();
                    c = 0;
                    count ++;
                }
                else if(c == 27)
                    break;
            }
        }
    }
    
    destroyAllWindows();
    return 0;
}

void killCamera()
{
    system("killall PTPCamera");
}

void printImage(Mat Image, string nameWindow, int PosX, int PosY)
{
    imshow(nameWindow, Image);
    moveWindow(nameWindow, PosX, PosY);
}

bool isDirExist(string path)
{
    struct stat info;
    if (stat(path.c_str(), &info) != 0)
    {
        makePath(path);
        return true;
    }
    return (info.st_mode & S_IFDIR) != 0;
}

bool makePath(string path)
{
    mode_t mode = 0755;
    int ret = mkdir(path.c_str(), mode);
    if (ret == 0)
        return true;
    
    switch (errno)
    {
        case ENOENT:
        {
            int pos = path.find_last_of('/');
            if (pos == std::string::npos)
                return false;
            if (!makePath( path.substr(0, pos) ))
                return false;
        }
            
            return 0 == mkdir(path.c_str(), mode);
            
        case EEXIST:
            // done!
            return isDirExist(path);
            
        default:
            return false;
    }
}

Mat drawLines(Mat image)
{
    int midX = image.cols/2;
    int eyeR = midX - (image.cols * 0.125);
    int eyeL = midX + (image.cols * 0.125); //distância com +- 60 pixeis para resoluções 640x480
    int eyeHeigh = image.rows*0.334375; //Distância de 260 pixeis para imagens 640x480
    
    //image = imread("teste.ppm");
    LineIterator it(image, Point(midX, 0), Point(midX, image.rows), 8);
    // get a line iterator
    for (int i = 0; i < it.count; i++, it++)
        if (i % 5 != 0) {
            (*it)[2] = 255;
        }         // every 5'th pixel gets dropped, blue stipple line
    
    LineIterator it2(image, Point(0, eyeHeigh), Point(image.cols, eyeHeigh), 8);
    
    for (int i = 0; i < it2.count; i++, it2++)
        if (i % 5 != 0) {
            (*it2)[2] = 255;
        }
    
    line(image, Point(eyeR + 4, eyeHeigh), Point(eyeR - 4, eyeHeigh), Scalar(0, 255, 255), 3, 8);
    line(image, Point(eyeL + 4, eyeHeigh), Point(eyeL - 4, eyeHeigh), Scalar(0, 255, 255), 3, 8);
    
    return image;
}