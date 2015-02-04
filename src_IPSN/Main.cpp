////////////////////////////////////////////////////////////////////////////////
// Main.cpp                                                                   //
//                                                                            //
// GLUI application to test ellipse-covering algorithm.                       //
////////////////////////////////////////////////////////////////////////////////



// Includes ////////////////////////////////////////////////////////////////////
#include <iostream.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <GL\glut.h>
#include <GL\glui.h>
#include "GVUtil.h"
#include "GVExposureSensorNetwork2D.h"

// Window Live Variable Initial Value Defines
// somelines are commented out and redefined in Lin's definitions
#define MAIN_OVERLAYEXPOSURE     1
#define MAIN_CENMINVALUE         0
#define MAIN_LOCMINVALUE         0
#define MAIN_MINSENSORS          1
#define MAIN_OUTPUTFILENAME      "OutFile.txt"
#define MAIN_PAUSETIME           750
#define MAIN_MAXSENSORS          32767
#define MAIN_COLOROBJECT         0
#define MAIN_DRAW                1
#define MAIN_SIZE                0.025f
#define MAIN_MINRADIUS           0.0f
#define MAIN_MAXRADIUS           1.0f
#define MAIN_MINLENGTH           0.0f
#define MAIN_MAXLENGTH           1000.0f
#define MAIN_GRIDH               15
#define MAIN_GRIDV               15

// Miscellaneous
#define MAIN_WINDOWNAME                "Ellipse Cover"
#define MAIN_CONTROLPANELNAME          "Control Panel"
#define MAIN_ANGLEDELTA                3.0

// Lin's definition
#define MAIN_NUMSENSORS          5
#define MAIN_ALGORITHMMODE       GVEXPOSURESENSORNETWORK2D_ALGORITHM_LOCAL
#define MAIN_ALGORITHMTYPE       MAIN_TYPE_GPSR
#define MAIN_LOCMAXVALUE         GVEXPOSURESENSORNETWORK2D_LOCMAXEXPOSUREMODE_GPSR
#define MAIN_CENMAXVALUE         GVEXPOSURESENSORNETWORK2D_CENMAXEXPOSUREMODE_SHORTEST
#define MAIN_STARTX              0.25f
#define MAIN_STARTY              0.25f
#define MAIN_ENDX                0.75f
#define MAIN_ENDY                0.75f
#define MAIN_LENGTH              1.85f
#define MAIN_COMMUNICATIONRADIUS 0.45f
#define FILENAMELEN				 20
#define MAIN_NUMEVENTS			 5
#define MAIN_SPACE               "   "
// Global Variables ////////////////////////////////////////////////////////////
// Algorithm Objects
GVExposureSensorNetwork2D SensorNetwork;           // Exposure Sensor Network

// Window Live Variables
int NumberOfSensors = MAIN_NUMSENSORS;                    // Number of Sensors
int NumberOfEvent = MAIN_NUMEVENTS;

char OutputFileName[FILENAMELEN] = MAIN_OUTPUTFILENAME; // Output File Name
float CommunicationRadius = MAIN_COMMUNICATIONRADIUS;     // Communication Radius
float GLUIWindow_StartingPointX = MAIN_STARTX;                       // Starting Point X Coordinate
float GLUIWindow_StartingPointY = MAIN_STARTY;                       // Starting Point Y Coordinate
float GLUIWindow_EndingPointX = MAIN_ENDY;                           // Ending Point X Coordinate
float GLUIWindow_EndingPointY = MAIN_ENDY;                           // Ending Point Y Coordinate

int seed;
// Prototypes //////////////////////////////////////////////////////////////////
void Pause(int Milliseconds);
void OnNew(void);
int RunRouting(void);
void RunTraffic(void);
void RoutingResult(void);

// Lin's Functions
int RunDetection(void);


// Main ////////////////////////////////////////////////////////////////////////
int __cdecl main(int argc, char** argv)
/*
  main() initializes the environment and calls the glutMainLoop() for OpenGL
  drawing.
*/
{ // main
	int j;
	if(argc>1)
	{
		NumberOfSensors = atoi(argv[1]);
		if(argc > 2)
			NumberOfEvent = atoi(argv[2]);
		if(argc > 3)
			seed = atoi(argv[3]);
	}

	seed = time(NULL);
	srand(seed);
  
	j = 0;
	while(j<30)
	{
		OnNew();
		if(RunDetection())
			j++;
		SensorNetwork.Delete();
	}
	return 0;
} // main

void debugNetwork(void)
{
	int SourceNodeIndex;
	int DestinationNodeIndex;
	SourceNodeIndex = rand()%(NumberOfSensors);
	DestinationNodeIndex = rand()%(NumberOfSensors);
	if(SourceNodeIndex != DestinationNodeIndex)
	{
		SensorNetwork.SetPathParameters(SensorNetwork.GetX(SourceNodeIndex), SensorNetwork.GetY(SourceNodeIndex), SensorNetwork.GetX(DestinationNodeIndex), SensorNetwork.GetY(DestinationNodeIndex));
		if(RunRouting())
		{
			RoutingResult();
		}
	}
}
// Functions ///////////////////////////////////////////////////////////////////
void Pause(int Milliseconds)
/*
  Pause() performs a busy-wait for the specified number of milliseconds.
*/
{ // Pause
  int Start = clock(); // Starting Time
  int End = clock();   // Ending Time

  while ((((double) (End - Start)) * 1000.0 / CLOCKS_PER_SEC) < Milliseconds)
  {
    End = clock();
  }
} // Pause

void OnNew(void)
/*
	initilize the sensornetwork
*/
{ // OnNew
  //SensorNetwork.New(NumberOfSensors, CommunicationRadius);
  SensorNetwork.New(NumberOfSensors, CommunicationRadius); 

} // OnNew

void RunTraffic(void)
{
	int SourceNodeIndex;
	int DestinationNodeIndex;
	int i = 0;
	while(i<1000)
	{
		SourceNodeIndex = rand()%(NumberOfSensors);
		DestinationNodeIndex = rand()%(NumberOfSensors);
		if(SourceNodeIndex != DestinationNodeIndex)
		{
			SensorNetwork.SetPathParameters(SensorNetwork.GetX(SourceNodeIndex), SensorNetwork.GetY(SourceNodeIndex), SensorNetwork.GetX(DestinationNodeIndex), SensorNetwork.GetY(DestinationNodeIndex));
			if(RunRouting())
			{
				RoutingResult();
			}
			i++;
		}	
	}
}

int RunDetection(void)
{
	int i;
	int j;
	int ReturnVal = 1;
	int CenterNodeIndex = 0;
	int PathLength = 0;
	double TotalEnergy[4] = {0.0};
	double TransmitEnergy[4] = {0.0};
	double ReceiveEnergy[4] = {0.0};
	double ProcessEnergy[4] = {0.0};
	double MonitorEnergy[4] = {0.0};
	double DistanceSquareSum = 0.0;
	double DistanceSum = 0.0;
	double TotalCommDistance = 0.0;
	double TotalCommDistanceSquare = 0.0;

	SensorNetwork.GenerateEvent(NumberOfEvent);

	for(i = 1; i< NumberOfSensors; i++)
	{
		SensorNetwork.SetPathParameters(SensorNetwork.GetX(i), SensorNetwork.GetY(i), SensorNetwork.GetX(CenterNodeIndex), SensorNetwork.GetY(CenterNodeIndex));
		if(RunRouting())
		{
			DistanceSquareSum += SensorNetwork.GetDistanceSquareSum();
			DistanceSum += SensorNetwork.GetDistanceSum();
			PathLength += SensorNetwork.GetGPSRPathLength();
			for(j=0;j<4;j++)
			{
				SensorNetwork.SetDetectionScheme(j);
				SensorNetwork.DetectionInitialize();
				SensorNetwork.DetectEvent();
				TotalEnergy[j] += SensorNetwork.GetTotalEnergy();
				TransmitEnergy[j] += SensorNetwork.GetTransmitEnergy();
				ReceiveEnergy[j]  += SensorNetwork.GetReceiveEnergy();
				ProcessEnergy[j]  += SensorNetwork.GetProcessEnergy();
				MonitorEnergy[j]  += SensorNetwork.GetMonitorEnergy();				
			}
		}
		else
		{
			ReturnVal = 0;
			break;
			//printf("Node %d (%.2f,%.2f) cannot reach the center(%.2f, %.2f) \n",i,SensorNetwork.GetX(i),SensorNetwork.GetY(i),SensorNetwork.GetX(CenterNodeIndex),SensorNetwork.GetY(CenterNodeIndex));
		}			
	}

	if(ReturnVal)
	{
		cout.width(6);
		cout << setiosflags(ios::fixed | ios::showpoint) << setprecision(2);
		
		for(j=0; j<4; j++)
		{
			cout.width(6);
			cout << setiosflags(ios::fixed | ios::showpoint) << setprecision(2);
			cout<< j << MAIN_SPACE 
				<< setw(4) << NumberOfSensors << MAIN_SPACE
				<< setw(8) << PathLength << MAIN_SPACE
				<< setw(8) << DistanceSum << MAIN_SPACE
				<< setw(8) << DistanceSquareSum << MAIN_SPACE
				<< setw(8) << TransmitEnergy[j] << MAIN_SPACE
				<< setw(8) << ReceiveEnergy[j] << MAIN_SPACE
				<< setw(8) << ProcessEnergy[j] << MAIN_SPACE
				<< setw(8) << MonitorEnergy[j] << MAIN_SPACE
				<< setw(8) << TotalEnergy[j] << MAIN_SPACE
			<< endl;
		}
		
	}
	return ReturnVal;
}
  
int RunRouting()
/*
  Run() occurs when the user presses the Run button.
*/
{ // Run
  int KeepGoing = 1; // Keep Running The Algorithm
  int ReturnVal = 0;
  
  while(KeepGoing == 1)
  {
	KeepGoing = SensorNetwork.GPSR_Run();
  }
  if (KeepGoing == 0)
  {
	 ReturnVal = 1;
     //cout << "Routing Finished"<< endl;
  }
  else if(KeepGoing == 2) //failed to deliver
  {
	//cout << "Routing Failed to deliver"<< endl;
  }
	return ReturnVal;  
} // OnRun

void RoutingResult(void)
{
	SensorNetwork.GPSR_OutputResults(OutputFileName);
}

