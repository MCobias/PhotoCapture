//
//  main.cpp
//  PhotoCapture
//
//  Created by Marcelo Cobias on 4/9/16.
//  Copyright © 2016 Marcelo Cobias. All rights reserved.
//

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "capturecamera.h"

using namespace std;
using namespace cv;


static int _lookup_widget(CameraWidget*widget, const char *key, CameraWidget **child)
{
    int ret;
    ret = gp_widget_get_child_by_name (widget, key, child);
    if (ret < GP_OK)
        ret = gp_widget_get_child_by_label (widget, key, child);
    return ret;
    
}

void CAPTURECAMERA::setConfigureCameraFlashMode(void *ref)
{
    int retval;
    char			*mval;
    //int choices;
    printf("Get root configm flash mode .\n");
    CameraWidget *rootconfig;
    CameraWidgetType	type;
    CameraWidget *actualrootconfig;
    CameraWidget *child;
    
    retval = gp_camera_get_config(cameranow, &rootconfig, context);
    actualrootconfig = rootconfig;
    
    retval = gp_widget_get_child_by_name(rootconfig, "main", &child);
    
    rootconfig = child;
    retval = gp_widget_get_child_by_name(rootconfig, "capturesettings", &child);
    
    rootconfig = child;
    retval = gp_widget_get_child_by_name(rootconfig, "flashmode", &child);
    
    int value = 1;
    CameraWidget *capture = child;
    const char *widgetinfo;
    
    gp_widget_get_name(capture, &widgetinfo);
    gp_widget_set_value(capture, &value);
    
    const char *widgetlabel;
    gp_widget_get_label(capture, &widgetlabel);
    
    int widgetid;
    gp_widget_get_id(capture, &widgetid);
    
    CameraWidgetType widgettype;
    gp_widget_get_type(capture, &widgettype);
    gp_widget_get_type(capture, &type);
    
    //choices = gp_widget_count_choices (capture);
    //value = gp_widget_get_value (capture, &mval);
    value = gp_widget_get_choice (capture, 2, (const char**)&mval);
    value = gp_widget_set_value (child, mval);
    
    retval = gp_camera_set_config(cameranow, actualrootconfig, context);
    printf("Sucess: %d\n", retval);
    
    if (retval < 0)
    {
        closeCamera();
        openCamera(this);
    }
    
    gp_widget_free(actualrootconfig);
}


void CAPTURECAMERA::setConfigureCameraZoom(void *ref, int x)
{
    int retval, value;
    //int choices;
    float			rval, min, max;
    printf("Get root config Zoom.\n");
    CameraWidget *rootconfig;
    CameraWidgetType	type;
    CameraWidget *actualrootconfig;
    CameraWidget *child;
    
    retval = gp_camera_get_config(cameranow, &rootconfig, context);
    actualrootconfig = rootconfig;
    
    retval = gp_widget_get_child_by_name(rootconfig, "main", &child);
    
    rootconfig = child;
    retval = gp_widget_get_child_by_name(rootconfig, "capturesettings", &child);

    rootconfig = child;
    retval = gp_widget_get_child_by_name(rootconfig, "zoom", &child);
    
    CameraWidget *capture = child;
    const char *widgetinfo;
    
    gp_widget_get_name(capture, &widgetinfo);
    gp_widget_set_value(capture, &value);
    
    const char *widgetlabel;
    gp_widget_get_label(capture, &widgetlabel);
    
    int widgetid;
    gp_widget_get_id(capture, &widgetid);
    
    CameraWidgetType widgettype;
    gp_widget_get_type(capture, &widgettype);
    gp_widget_get_type(capture, &type);
    
    
    //choices = gp_widget_count_choices (capture);
    value = gp_widget_get_value (capture, &rval);
    value = gp_widget_get_range(capture, &min, &max, &rval);

    rval = (float) x;
    value = gp_widget_set_value(child, &rval);
    
    retval = gp_camera_set_config(cameranow, actualrootconfig, context);
    printf("Sucess: %d\n", retval);
    
    if (retval < 0)
    {
        closeCamera();
        openCamera(this);
    }
    
    gp_widget_free(actualrootconfig);
}

void CAPTURECAMERA::setConfigureCameraImageSize(void *ref)
{
    int retval;
    char			*mval;
    //int choices;
    printf("Get root config Image Size.\n");
    CameraWidget *rootconfig;
    CameraWidgetType	type;
    CameraWidget *actualrootconfig;
    CameraWidget *child;
    
    retval = gp_camera_get_config(cameranow, &rootconfig, context);
    actualrootconfig = rootconfig;
    
    retval = gp_widget_get_child_by_name(rootconfig, "main", &child);
    
    rootconfig = child;
    retval = gp_widget_get_child_by_name(rootconfig, "imgsettings", &child);
    
    rootconfig = child;
    retval = gp_widget_get_child_by_name(rootconfig, "imagesize", &child);
    
    int value = 1;
    CameraWidget *capture = child;
    const char *widgetinfo;
    
    gp_widget_get_name(capture, &widgetinfo);
    gp_widget_set_value(capture, &value);
    
    const char *widgetlabel;
    gp_widget_get_label(capture, &widgetlabel);
    
    int widgetid;
    gp_widget_get_id(capture, &widgetid);
    
    CameraWidgetType widgettype;
    gp_widget_get_type(capture, &widgettype);
    gp_widget_get_type(capture, &type);
    
    //choices = gp_widget_count_choices (capture);
    //value = gp_widget_get_value (capture, &mval);
    value = gp_widget_get_choice (capture, 3, (const char**)&mval);
    value = gp_widget_set_value (child, mval);
    
    retval = gp_camera_set_config(cameranow, actualrootconfig, context);
    printf("Sucess: %d\n", retval);
    
    if (retval < 0)
    {
        closeCamera();
        openCamera(this);
    }
    
    gp_widget_free(actualrootconfig);
}

void CAPTURECAMERA::canon_enable_capture(Camera *camera, GPContext *context, CameraWidget *config)
{
    int onoff = 1;
    CameraWidgetType type;
    CameraWidget *child;
    int ret;
    
    ret = _lookup_widget (config, "capture", &child);
    if (ret < GP_OK)
    {
        //fprintf (stderr, "lookup widget failed: %d\n", ret);
        goto out;
    }
    
    ret = gp_widget_get_type (child, &type);
    if (ret < GP_OK)
    {
        //fprintf (stderr, "widget get type failed: %d\n", ret);
        goto out;
    }
    switch (type)
    {
        case GP_WIDGET_TOGGLE:
            break;
        default:
            fprintf (stderr, "widget has bad type %d\n", type);
            ret = GP_ERROR_BAD_PARAMETERS;
            goto out;
    }
    ret = gp_widget_set_value (child, &onoff);
    if (ret < GP_OK)
    {
        //fprintf (stderr, "toggling Canon capture to %d failed with %d\n", onoff, ret);
        goto out;
    }
    ret = gp_camera_set_config (camera, config, context);
    if (ret < GP_OK)
    {
        //fprintf (stderr, "camera_set_config failed: %d\n", ret);
        return;
    }
    
out:
    child=NULL;
    return;
}


bool CAPTURECAMERA::openCamera(void *ref)
{
    gp_camera_new (&cameranow);
    context = gp_context_new();
    gp_context_set_error_func(context,  CAPTURECAMERA::cp_error_func, this);
    gp_context_set_message_func(context, CAPTURECAMERA::cp_message_func, this);

    int ret = gp_camera_init (cameranow, context);
    canon_enable_capture(cameranow, context, NULL);
    
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
    Mat imgbuf;
    CameraFile *cf;
    gp_file_new(&cf);
    
    gp_camera_capture_preview (cameranow, cf, context);

    unsigned long datasize = 0;
    const char *data = NULL;

    gp_file_get_data_and_size(cf, &data, &datasize);

    if (datasize == 0 )
        return;

    imgbuf = Mat(240, 320, CV_8U,(char*)data);
    currentFrame = imdecode(imgbuf, CV_LOAD_IMAGE_COLOR);
    Image = currentFrame;
    
    gp_file_free(cf);
}

bool CAPTURECAMERA::capturePhoto(Mat &frame, int waittime)
{
    Mat imgbuf;
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

    imgbuf = Mat(480, 640, CV_8U,(char*)data);
    frame = imdecode(imgbuf, CV_LOAD_IMAGE_COLOR);
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