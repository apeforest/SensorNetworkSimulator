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
//#define MAIN_ALGORITHMMODE       1
//#define MAIN_ALGORITHMTYPE       1
#define MAIN_CENMINVALUE         0
//#define MAIN_CENMAXVALUE         3
#define MAIN_LOCMINVALUE         0
//#define MAIN_LOCMAXVALUE         0
#define MAIN_MINSENSORS          1
#define MAIN_OUTPUTFILENAME      "OutFile.txt"
#define MAIN_PAUSETIME           750
#define MAIN_MAXSENSORS          32767
//#define MAIN_NUMSENSORS          15
#define MAIN_COLOROBJECT         0
#define MAIN_DRAW                1
#define MAIN_SIZE                0.025f
#define MAIN_MINRADIUS           0.0f
#define MAIN_MAXRADIUS           1.414213f
//#define MAIN_COMMUNICATIONRADIUS 0.35f
//#define MAIN_STARTX              0.25f
//#define MAIN_STARTY              0.25f
//#define MAIN_ENDX                0.75f
//#define MAIN_ENDY                0.75f
#define MAIN_MINLENGTH           0.0f
#define MAIN_MAXLENGTH           1000.0f
//#define MAIN_LENGTH              0.85f
#define MAIN_GRIDH               15
#define MAIN_GRIDV               15
#define MAIN_RED                 ((float) MAIN_COLOR_BKG_R)
#define MAIN_GREEN               ((float) MAIN_COLOR_BKG_G)
#define MAIN_BLUE                ((float) MAIN_COLOR_BKG_B)

// Miscellaneous
#define MAIN_WINDOWNAME                "Ellipse Cover"
#define MAIN_CONTROLPANELNAME          "Control Panel"
#define MAIN_ANGLEDELTA                3.0

// Lin's definition
#define MAIN_NUMSENSORS          200
#define MAIN_ALGORITHMMODE       GVEXPOSURESENSORNETWORK2D_ALGORITHM_LOCAL
#define MAIN_ALGORITHMTYPE       MAIN_TYPE_GPSR
#define MAIN_LOCMAXVALUE         GVEXPOSURESENSORNETWORK2D_LOCMAXEXPOSUREMODE_GPSR
#define MAIN_CENMAXVALUE         GVEXPOSURESENSORNETWORK2D_CENMAXEXPOSUREMODE_SHORTEST
#define MAIN_STARTX              0.25f
#define MAIN_STARTY              0.25f
#define MAIN_ENDX                0.75f
#define MAIN_ENDY                0.75f
#define MAIN_LENGTH              1.85f
#define MAIN_COMMUNICATIONRADIUS 0.13f
#define FILENAMELEN				 20
#define MAIN_NUMEVENTS			 5
#define MAIN_SPACE               "   "
// Global Variables ////////////////////////////////////////////////////////////
// Algorithm Objects
GVExposureSensorNetwork2D SensorNetwork;           // Exposure Sensor Network

// Window Live Variables
int NumberOfSensors = MAIN_NUMSENSORS;                    // Number of Sensors
int NumberOfEvent = MAIN_NUMEVENTS;
int NumberOfObstacles = 1;

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
int CheckNetworkConnectivity(void);

// Main ////////////////////////////////////////////////////////////////////////
int __cdecl main(int argc, char** argv)
/*
  main() initializes the environment and calls the glutMainLoop() for OpenGL
  drawing.
*/
{ // main
 int i;
 int j;
 if(argc>1)
 {
	 NumberOfSensors = atoi(argv[1]);
	 if(argc > 2)
		seed = atoi(argv[2]);
 }

 srand(seed);
 OnNew();
 /*
 //if(CheckNetworkConnectivity())
// {
//	 cout << "Connect" <<endl;
//	 for(i=0; i< NumberOfSensors; i++)
	 {
		cout<< SensorNetwork.GetX(i) << MAIN_SPACE
			<< SensorNetwork.GetY(i) << endl;
	 }
 }
 else
	 cout << "disconnected" << endl;
	 */
 RunTraffic();
 return 0;
} // main

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
{ 
  SensorNetwork.New(NumberOfSensors, CommunicationRadius); 
} // OnNew

int CheckNetworkConnectivity(void)
{
	int i = 0;
	int ReturnVal = 1;
	int SourceNodeIndex = 0;
	int DestinationNodeIndex = 0;

	for(i=1; i<NumberOfSensors; i++)
	{
		DestinationNodeIndex = i;
		SensorNetwork.SetPathParameters(SensorNetwork.GetX(SourceNodeIndex), SensorNetwork.GetY(SourceNodeIndex), SensorNetwork.GetX(DestinationNodeIndex), SensorNetwork.GetY(DestinationNodeIndex));			
		RunRouting();
		if(SensorNetwork.GetShortestPathLength() == 0)
		{
			ReturnVal = 0;
			break;
		}
	}
	return ReturnVal;
}

void RunTraffic(void)
{
	int i = 0; // index variable
	int j = 0; // index variable
	int SourceNodeIndex;
	int DestinationNodeIndex;
	int NumMessages;
	
	i = 0;
	while(i<10000)
	{
		SourceNodeIndex = rand()%(NumberOfSensors);
		DestinationNodeIndex = rand()%(NumberOfSensors);
		if(SourceNodeIndex != DestinationNodeIndex)
		{
			SensorNetwork.SetPathParameters(SensorNetwork.GetX(SourceNodeIndex), SensorNetwork.GetY(SourceNodeIndex), SensorNetwork.GetX(DestinationNodeIndex), SensorNetwork.GetY(DestinationNodeIndex));
			//if(RunRouting())
			RunRouting();
			if(SensorNetwork.GetShortestPathLength() > 0)
			{
				
				RoutingResult();
				i++;
			}
		}	
	}
		
	/*
	for(i=0; i<NumberOfSensors; i++)
	{
		for(j=i+1; j<NumberOfSensors; j++)
		{
			SourceNodeIndex = i;
			DestinationNodeIndex = j;
			SensorNetwork.SetPathParameters(SensorNetwork.GetX(SourceNodeIndex), SensorNetwork.GetY(SourceNodeIndex), SensorNetwork.GetX(DestinationNodeIndex), SensorNetwork.GetY(DestinationNodeIndex));
			RunRouting();
			RoutingResult();
		}
	}
	*/
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

void DebugNetwork(void)
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