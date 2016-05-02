//
//  main.cpp
//  PhotoCapture
//
//  Created by Marcelo Cobias on 4/9/16.
//  Copyright © 2016 Marcelo Cobias. All rights reserved.
//

#include "capturecamera.h"

void killCamera();
void captureCamera();
Mat drawLines(Mat image);
void printImage(Mat Image, string nameWindow, int PosX, int PosY);


int main(int argc, char *argv[])
{
    Mat imageCapture;
    int count = 1, zoom = 0, contzoom = 0;
    CAPTURECAMERA cam;
    string name, year, sex;
    
    GtkFileChooser *chooser;
    GtkWidget *dialog;
    gint res;

    killCamera();
    
    try
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
                
                int c = waitKey(10);

                if(c == 'p')
                {
                    if(count < 2)
                    {
                        cam.setConfigureCameraAperture(&cam, 48);
                        cam.setConfigureCameraImageSize(&cam, 3);
                        cam.setConfigureCameraFlashMode(&cam, 2);
                    }
                    
                    cam.capturePhoto(imageCapture, 1000, 640, 480);
                    transpose(imageCapture, imageCapture);
                    flip(imageCapture, imageCapture, 0);
                    printImage(imageCapture, "Captured", 600, 100);
                    
                    gtk_init(&argc, &argv);
                    dialog = gtk_file_chooser_dialog_new ("Salve capture", NULL, GTK_FILE_CHOOSER_ACTION_SAVE, "Cancel", GTK_RESPONSE_CANCEL, "Save", GTK_RESPONSE_ACCEPT, NULL);
                    chooser = GTK_FILE_CHOOSER (dialog);
                    gtk_file_chooser_set_do_overwrite_confirmation (chooser, TRUE);
                    gtk_file_chooser_set_current_name (chooser, "Untitled photo");
                    
                    res = gtk_dialog_run (GTK_DIALOG (dialog));
                    
                    if (res == GTK_RESPONSE_ACCEPT)
                    {
                        string path;
                        path = gtk_file_chooser_get_filename (chooser);
                        imwrite(path + ".jpg", imageCapture);
                    }
                    gtk_widget_destroy (dialog);
                   
                    
                    cam.removeImage();
                    c = 0;
                    count ++;
                }
                else if(c == 27)
                    break;
            }
        }
    }
    catch(string param)
    {
        gtk_init(0, 0);
        dialog = gtk_message_dialog_new (NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "Camera error, disconnect camera!");
        gtk_dialog_run (GTK_DIALOG (dialog));
        gtk_widget_destroy (dialog);
    }
    
    destroyAllWindows();
    return 0;
}

void killCamera()
{
    system("killall gvfsd-gphoto2");
}

void printImage(Mat Image, string nameWindow, int PosX, int PosY)
{
    imshow(nameWindow, Image);
    moveWindow(nameWindow, PosX, PosY);
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
