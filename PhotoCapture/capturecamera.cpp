//
//  main.cpp
//  PhotoCapture
//
//  Created by Marcelo Cobias on 4/9/16.
//  Copyright © 2016 Marcelo Cobias. All rights reserved.
//

#include "capturecamera.h"
#include <fcntl.h>
#include <stdio.h>
#include <cstring>


using namespace std;
using namespace cv;


bool CAPTURECAMERA::openCamera(void *ref)
{
    gp_camera_new (&cameranow);
    context = gp_context_new();
  
    gp_context_set_error_func(context,  CAPTURECAMERA::cp_error_func, this);
    gp_context_set_message_func(context, CAPTURECAMERA::cp_message_func, this);

    int ret = gp_camera_init (cameranow, context);

    if (ret < GP_OK)
    {
        return false;
    }

    CameraAbilities ca;
    ret = gp_camera_get_abilities (cameranow, &ca);

    if (ret < GP_OK)
    {
        return false;
    }
    
    return true;
}

bool CAPTURECAMERA::closeCamera()
{
    gp_camera_exit(cameranow, context);
    gp_camera_unref(cameranow);
    gp_context_unref(context);

    return true;
}

void CAPTURECAMERA::processFrame(Mat &currentFrame)
{
    //Mat imgbuf;
    //vector<char> vec;
    CameraFile *cf;
    gp_file_new(&cf);

    gp_camera_capture_preview (cameranow, cf, context);

    unsigned long datasize = 0;
    const char *data = NULL;

    gp_file_get_data_and_size(cf, &data, &datasize);

    if (datasize == 0 )
        return;

    /*const char* end = data + strlen( data );
    vec.insert( vec.end(), data, end );
    
    imgbuf = Mat(vec);
    currentFrame = imdecode(imgbuf, CV_LOAD_IMAGE_COLOR);*/
    
    QPixmap pm;
    pm.loadFromData( (uchar*) data,  (uint) datasize);
    QImage im = pm.toImage();
    currentFrame = qimage2cvMat(im);
    Image = currentFrame;
    
    gp_file_free(cf);
}

bool CAPTURECAMERA::capturePhoto(Mat &frame, int waittime)
{
    //Mat imgbuf;
    //vector<char> vec;
    CameraFile *cf;
    CameraFilePath camera_file_path;
    int ret = gp_camera_capture(cameranow, GP_CAPTURE_IMAGE, &camera_file_path, context);
    if (ret < GP_OK)
        return false;
    gp_file_new(&cf);

    gp_camera_file_get(cameranow, camera_file_path.folder, camera_file_path.name, GP_FILE_TYPE_NORMAL, cf, context);

    unsigned long datasize = 0;
    const char *data = NULL;

    gp_file_get_data_and_size(cf, &data, &datasize);

    /*const char* end = data + strlen( data );
    vec.insert( vec.end(), data, end );
    
    imgbuf = Mat(vec);
    frame = imdecode(imgbuf, CV_LOAD_IMAGE_COLOR);*/
    
    QPixmap pm;
    pm.loadFromData( (uchar*) data,  (uint) datasize);
    QImage im = pm.toImage();
    frame = qimage2cvMat(im);
    Image = frame;

    gp_camera_file_delete(cameranow, camera_file_path.folder, camera_file_path.name, context);
    gp_file_free(cf);

    frame.copyTo(Image);
    
    CameraEventType type;
    void *datavoid;
    while(1)
    {
        gp_camera_wait_for_event(cameranow, waittime, &type, &datavoid, context);
        if(type == GP_EVENT_TIMEOUT) {
            break;
        }
        else if (type == GP_EVENT_CAPTURE_COMPLETE)
        {
            printf("Capture completea\n");
            waittime = 100;
        }
        else if (type != GP_EVENT_UNKNOWN)
        {
            printf("Unexpected event received from camera: %d\n", (int)type);
        }
    }

    return true;
}

bool CAPTURECAMERA::isCaptured()
{
    if (Image.empty() == true)
        return false;
    return true;
}

void CAPTURECAMERA::removeImage()
{
    this->Image.release();
}

Mat CAPTURECAMERA::getCurrentImage()
{
    return this->Image;
}

void CAPTURECAMERA::cp_error_func(GPContext *context, const char *text, void *data)
{
    fprintf(stderr, "*** Camera error ***\n");
    fprintf(stderr, text, "%s\n");
}

void CAPTURECAMERA::cp_message_func(GPContext *context, const char *text, void *data)
{
    fprintf(stdout, "*** Camera message ***\n");
    fprintf(stdout, text, "%s\n");
}

void CAPTURECAMERA::setConfigureCamera(int value, char* param)
{
    int val;
    CameraWidget *parent, *child;
    gp_camera_get_config(cameranow, &parent, context);
    gp_widget_get_child_by_name(parent, param, &child);
    gp_widget_get_value (child, &val);
    printf("Parametre value: ");
    printf("%d\n", val);
    printf("New value\n");
    val = value;
    printf("%d\n", value);
    printf("%d\n", gp_widget_set_value(child, &val));
    int vret = gp_camera_set_config(cameranow, parent, context);
    
    if (vret < 0)
    {
        closeCamera();
        openCamera(this);
    }
    gp_widget_free (parent);
}

Mat CAPTURECAMERA::qimage2cvMat(QImage &im)
{
    cv::Mat mat = cv::Mat(im.height(), im.width(), CV_8UC4, (uchar*)im.bits(), im.bytesPerLine());
    cv::Mat mat2 = cv::Mat(mat.rows, mat.cols, CV_8UC3 );
    int from_to[] = { 0,0,  1,1,  2,2 };
    cv::mixChannels( &mat, 1, &mat2, 1, from_to, 3 );
    return mat2;
}
