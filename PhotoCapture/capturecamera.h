//
//  main.cpp
//  PhotoCapture
//
//  Created by Marcelo Cobias on 4/9/16.
//  Copyright © 2016 Marcelo Cobias. All rights reserved.
//

#ifndef CAPTURECAMERA_H
#define CAPTURECAMERA_H

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <sys/stat.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <GPhoto2/gphoto2-camera.h>
#include <GPhoto2/gphoto2-setting.h>

using namespace std;
using namespace cv;

class CAPTURECAMERA
{
public:
    
    Camera *cameranow;
    
    GPContext *context;
    
    Mat Image;
    
    bool openCamera(void* ref);

    bool closeCamera();
    
    void capturePreview(Mat &frame);

    bool capturePhoto(Mat &frame, int waittime);
    
    static void cp_error_func(GPContext *context, const char *text, void *data);
    
    static void cp_message_func(GPContext *context, const char *text, void *data);
    
    void canon_enable_capture(Camera *camera, GPContext *context, CameraWidget *config);

    void setConfigureCameraFlashMode(void *ref);
    
    void setConfigureCameraZoom(void *ref, int x);
    
    void setConfigureCameraImageSize(void *ref);
    
    void setConfigureCameraAperture(void *ref);
    
    bool isCaptured();
    
    void removeImage();
    
    Mat getImage();
};

#endif
