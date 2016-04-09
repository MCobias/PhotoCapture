//
//  main.cpp
//  PhotoCapture
//
//  Created by Marcelo Cobias on 4/9/16.
//  Copyright © 2016 Marcelo Cobias. All rights reserved.
//


#include <sys/stat.h>
#include <iostream>
#include "capturecamera.h"

void killCamera();
bool isDirExist(string path);
bool makePath(string path);
void printImage(Mat Image, string nameWindow, int TamW, int TamH, int PosX, int PosY);

int main(int argc, char *argv[])
{
    string name;
    CAPTURECAMERA cam;
    Mat imageCapture;
    
    cout << "Person name capture: ";
    cin >> name;
    
    if(cam.openCamera(&cam))
    {
        cam.setConfigureCamera(1, (char*) "flasmode");
        cam.setConfigureCamera(1, (char*) "resolution");
        
        while(waitKey(33) != 13)
        {
            cam.processFrame(imageCapture);
            printImage(cam.getCurrentImage(), "Camera Preview - Take a photo press ENTER", 200, 200, 100, 100);
            cam.removeImage();
        }
        cam.capturePhoto(imageCapture, 2000);
        if(cam.isCaptured())
        {
            if(isDirExist("/Users/mcobias/Desktop/DataSet/"))
            {
                imwrite("/Users/mcobias/Desktop/DataSet/ImageDataSet_" + name, cam.getCurrentImage());
                printImage(cam.getCurrentImage(), "Image captured camera....", 620, 480, 300, 100);
            }
        }
        else
        {
            cout << "Photo not captured, try again!";
        }
    }
    
    cam.removeImage();
    destroyAllWindows();
    return 0;
}

void printImage(Mat Image, string nameWindow, int TamW, int TamH, int PosX, int PosY)
{
    resize(Image, Image, Size(), TamW, TamH);
    imshow(nameWindow, Image);
    moveWindow(nameWindow, PosX, PosY);
}

void killCamera()
{
    system("killall PTPCamera");
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