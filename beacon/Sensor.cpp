// Sensor Interface File ///////////////////////////////////////////////////////
// NOTE: You Must Include "Sensor.h" BEFORE Including This File

#ifndef SENSOR_CPP
#define SENSOR_CPP

#include <fstream.h>
#include "StdAfx.h"



int InputSensors(char* FileName, int &NumSensors, Sensor** SensorArray)
{ // InputSensors
  ifstream InputFile; // Input File Stream
  int i = 0;          // Index Variable for Inputting Sensors
  int Success = 1;    // Was the Input a Success? 1: Yes, 0: No

  // Open the File for Input
  InputFile.open(FileName, ios::nocreate);
  // If the File Opened Correctly
  if (!InputFile.fail())
  {
    // Delete the Previous Array (if Any)
    if ((*SensorArray) != NULL)
    {
      delete[] (*SensorArray);
    }
    // Input the Number of Sensors
    InputFile >> NumSensors;
    // Allocate a New Array
    (*SensorArray) = new Sensor[NumSensors];
    // Input Sensors While Not EOF and Less Than NumSensors
    while ((i < NumSensors) && (!InputFile.eof()))
    {
      // Input the Sensor Data
      InputFile >> (*SensorArray)[i].x;
      InputFile >> (*SensorArray)[i].y;
      InputFile >> (*SensorArray)[i].Beacon;
      InputFile >> (*SensorArray)[i].dx;
      InputFile >> (*SensorArray)[i].dy;
      // Go to the Next Sensor
      ++i;
    }
    // Close the Sensor File
    InputFile.close();
  }
  else
  {
    // Clear the Error Flag
    InputFile.clear();
    // Display an Error Message
    // MessageBox("Input File Not Found!", "File Not Found");
    // Indicate Failure
    Success = 0;
  }
  // Return the Success Value
  return (Success);
} // InputSensors



void DrawSensor(CClientDC* PicWindow, int Width, int Height, double X, double Y, int Type, int BorderSize = 2)
{ // DrawSensor
  int R; // Red Color Value
  int G; // Green Color Value
  int B; // Blue Color Value

  // Determine Color Values
  switch(Type)
  {
    default: // (255, 0, 0) Standard Sensor (Red)
    case 1:  // (255, 0, 0)
      R = 255;
      G = 0;
      B = 0;
      break;
    case 2:  // (255, 255, 0) Initial Beacon (Yellow)
      R = 255;
      G = 255;
      B = 0;
      break;
    case 3:  // (0, 192, 0) Calculated Beacon (Green)
      R = 0;
      G = 192;
      B = 0;
      break;
    case 4:  // (0, 0, 255); Error Estimated Sensor (Blue)
      R = 0;
      G = 0;
      B = 255;
      break;
    case 5: // (255, 128, 128); Estimated Beacon (Light Red)
      R = 255;
      G = 128;
      B = 128;
      break;
    case 6: // (128, 128, 255); Error Estimated Beacon (Light Blue)
      R = 128;
      G = 128;
      B = 255;
      break;
  }

  if (!((int (Width * X + BorderSize) < BorderSize) || (int (Width * X + BorderSize) > Width + BorderSize) || (int (Height * Y + BorderSize) < BorderSize) || (int (Height * Y + BorderSize) > Height + BorderSize)))
  {
    PicWindow->SetPixel(int (Width * X) + BorderSize, int (Height * Y) + BorderSize, RGB(R, G, B));
    PicWindow->SetPixel(int (Width * X) + BorderSize + 1, int (Height * Y) + BorderSize, RGB(R, G, B));
    PicWindow->SetPixel(int (Width * X) + BorderSize - 1, int (Height * Y) + BorderSize, RGB(R, G, B));
    PicWindow->SetPixel(int (Width * X) + BorderSize, int (Height * Y) + BorderSize + 1, RGB(R, G, B));
    PicWindow->SetPixel(int (Width * X) + BorderSize, int (Height * Y) + BorderSize - 1, RGB(R, G, B));
  }
} // DrawSensor

#endif