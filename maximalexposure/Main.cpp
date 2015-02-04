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
#include <GL/glut.h>
#include <GL/glui.h>
#include "GVUtil.h"
#include "GVExposureSensorNetwork2D.h"



// Defines /////////////////////////////////////////////////////////////////////
// Experiment Defines
#define MAIN_EXPERIMENT_NUMSENSORS_MIN   1000
#define MAIN_EXPERIMENT_NUMSENSORS_MAX   1050
#define MAIN_EXPERIMENT_NUMSENSORS_DELTA 100
#define MAIN_EXPERIMENT_RADIUS_MIN       0.075
#define MAIN_EXPERIMENT_RADIUS_MAX       0.1625
#define MAIN_EXPERIMENT_RADIUS_DELTA     0.025
#define MAIN_EXPERIMENT_GRIDX_MIN        200
#define MAIN_EXPERIMENT_GRIDX_MAX        201
#define MAIN_EXPERIMENT_GRIDX_DELTA      2
#define MAIN_EXPERIMENT_GRIDY_MIN        200
#define MAIN_EXPERIMENT_GRIDY_MAX        201
#define MAIN_EXPERIMENT_GRIDY_DELTA      2
#define MAIN_EXPERIMENT_STARTX_MIN       0.25
#define MAIN_EXPERIMENT_STARTX_MAX       0.35
#define MAIN_EXPERIMENT_STARTX_DELTA     1.0
#define MAIN_EXPERIMENT_STARTY_MIN       0.25
#define MAIN_EXPERIMENT_STARTY_MAX       0.35
#define MAIN_EXPERIMENT_STARTY_DELTA     1.0
#define MAIN_EXPERIMENT_ENDX_MIN         0.75
#define MAIN_EXPERIMENT_ENDX_MAX         0.85
#define MAIN_EXPERIMENT_ENDX_DELTA       1.0
#define MAIN_EXPERIMENT_ENDY_MIN         0.75
#define MAIN_EXPERIMENT_ENDY_MAX         0.85
#define MAIN_EXPERIMENT_ENDY_DELTA       1.0
#define MAIN_EXPERIMENT_MAXLENGTH_MIN    0.75
#define MAIN_EXPERIMENT_MAXLENGTH_MAX    1.025
#define MAIN_EXPERIMENT_MAXLENGTH_DELTA  0.05
#define MAIN_EXPERIMENT_NUMITERATIONS    100

// OpenGL Defines
#define MAIN_TORADIANS           0.017453278
#define MAIN_HALFANGLE           30.0
#define MAIN_NEARPLANE           0.001
#define MAIN_FARPLANE            20.0
#define MAIN_SPHERE_SLICES       25
#define MAIN_SPHERE_STACKS       25
#define MAIN_CYLINDER_SLICES     5
#define MAIN_CYLINDER_STACKS     1

// Window Parameter Defines
#define MAIN_WINDOWX             50
#define MAIN_WINDOWY             50
#define MAIN_WINDOWWIDTH         500
#define MAIN_WINDOWHEIGHT        500
#define MAIN_CONTROLPANELX       (MAIN_WINDOWX + MAIN_WINDOWWIDTH + 25)
#define MAIN_CONTROLPANELY       (MAIN_WINDOWY)
#define MAIN_STATICTEXTSIZE      20

// Window Control Parameter Defines
#define MAIN_NOTEXT               ""
#define MAIN_FILENAMETEXT         "Output File Name:"
#define MAIN_PAUSETIMETEXT        "Pause Time:"
#define MAIN_NUMSENSORSTEXT       "Number of Sensors:"
#define MAIN_RADIUSTEXT           "Communication Radius:"
#define MAIN_STARTTEXT            "Starting Coordinate:"
#define MAIN_ENDTEXT              "Ending Coordinate:"
#define MAIN_GRIDTEXT             "Grid Size:"
#define MAIN_LENGTHTEXT           "Maximum Length:"
#define MAIN_ALGORITHMTEXT        "Algorithm Options:"
#define MAIN_COLORTEXT            "Object Display Information:"
#define MAIN_SEARCHHEURISTIC      "Heuristic"
#define MAIN_SEARCHGPSR           "GPSR"
#define MAIN_BKG                  "Background"
#define MAIN_UNKNOWNSENSOR        "Unknown Sensor"
#define MAIN_KNOWNSENSOR          "Known Sensor"
#define MAIN_VISITEDSENSOR        "Visited Sensor"
#define MAIN_USELESSSENSOR        "Useless Sensor"
#define MAIN_CURRENTSENSORSEARCH  "Current Sensor - Search"
#define MAIN_CURRENTSENSORFORWARD "Current Sensor - Forward"
#define MAIN_EDGE                 "Edge"
#define MAIN_ELLIPSE              "Ellipse"
#define MAIN_ENDPOINTS            "End Points"
#define MAIN_GRIDPOINTS           "Grid Points"
#define MAIN_GRIDEDGES            "Grid Edges"
#define MAIN_COVGRID              "Covered Grid Areas"
#define MAIN_UNCOVGRID            "Uncovered Grid Areas"
#define MAIN_NEXTCOVGRID          "Next Covered Areas"
#define MAIN_NEXTUNCOVGRID        "Next Uncovered Areas"
#define MAIN_PATH_CEN_MIN         "Centralized Minimal Path"
#define MAIN_PATH_CEN_MAX         "Centralized Maximal Path"
#define MAIN_PATH_LOC_MIN         "Localized Minimal Path"
#define MAIN_PATH_LOC_MAX         "Localized Maximal Path"
#define MAIN_CENMINTEXT           "Centralized Minimal"
#define MAIN_CENMAXTEXT           "Centralized Maximal"
#define MAIN_LOCMINTEXT           "Localized Minimal"
#define MAIN_LOCMAXTEXT           "Localized Maximal"
#define MAIN_CENTEXT              "Centralized"
#define MAIN_LOCTEXT              "Localized"
#define MAIN_MINTEXT              "Minimal Exposure"
#define MAIN_MAXTEXT              "Maximal Exposure"
#define MAIN_EXPOSURETEXT         "Overlay Exposure"
#define MAIN_REDTEXT              "Red:"
#define MAIN_GREENTEXT            "Green:"
#define MAIN_BLUETEXT             "Blue:"
#define MAIN_DRAWTEXT             "Draw Object"
#define MAIN_SIZETEXT             "Size:"
#define MAIN_NEWTEXT              "New"
#define MAIN_QUITTEXT             "Quit"
#define MAIN_RUNTEXT              "Run"
#define MAIN_EXPERIMENTTEXT       "Experiment"
#define MAIN_CENMIN_1             "Default"
#define MAIN_CENMAX_1             "Random"
#define MAIN_CENMAX_2             "Shortest Path"
#define MAIN_CENMAX_3             "Best Point"
#define MAIN_CENMAX_4             "Adjusted Best Point"
#define MAIN_LOCMIN_1             "Default"
#define MAIN_LOCMAX_1             "Border"
#define MAIN_LOCMAX_2             "Counterclockwise"
#define MAIN_MODETEXT             "Mode"
#define MAIN_TYPETEXT             "Type"

// Algorithm Modes
#define MAIN_MODE_CENTRALIZED    (0                        )
#define MAIN_MODE_LOCALIZED      (MAIN_MODE_CENTRALIZED + 1)
#define MAIN_MODE_NUM            (MAIN_MODE_LOCALIZED   + 1)

// Algorithm Types
#define MAIN_TYPE_MINIMAL        (0                    )
#define MAIN_TYPE_MAXIMAL        (MAIN_TYPE_MINIMAL + 1)
#define MAIN_TYPE_NUM            (MAIN_TYPE_MAXIMAL + 1)

// Window Callback Defines
#define MAIN_CALLBACK_QUIT       (0                        + 1)
#define MAIN_CALLBACK_NEW        (MAIN_CALLBACK_QUIT       + 1)
#define MAIN_CALLBACK_COLOR      (MAIN_CALLBACK_NEW        + 1)
#define MAIN_CALLBACK_RED        (MAIN_CALLBACK_COLOR      + 1)
#define MAIN_CALLBACK_GREEN      (MAIN_CALLBACK_RED        + 1)
#define MAIN_CALLBACK_BLUE       (MAIN_CALLBACK_GREEN      + 1)
#define MAIN_CALLBACK_SIZE       (MAIN_CALLBACK_BLUE       + 1)
#define MAIN_CALLBACK_DRAW       (MAIN_CALLBACK_SIZE       + 1)
#define MAIN_CALLBACK_NUMSENSORS (MAIN_CALLBACK_DRAW       + 1)
#define MAIN_CALLBACK_RADIUS     (MAIN_CALLBACK_NUMSENSORS + 1)
#define MAIN_CALLBACK_STARTX     (MAIN_CALLBACK_RADIUS     + 1)
#define MAIN_CALLBACK_STARTY     (MAIN_CALLBACK_STARTX     + 1)
#define MAIN_CALLBACK_ENDX       (MAIN_CALLBACK_STARTY     + 1)
#define MAIN_CALLBACK_ENDY       (MAIN_CALLBACK_ENDX       + 1)
#define MAIN_CALLBACK_GRIDH      (MAIN_CALLBACK_ENDY       + 1)
#define MAIN_CALLBACK_GRIDV      (MAIN_CALLBACK_GRIDH      + 1)
#define MAIN_CALLBACK_MAXLENGTH  (MAIN_CALLBACK_GRIDV      + 1)
#define MAIN_CALLBACK_RUN        (MAIN_CALLBACK_MAXLENGTH  + 1)
#define MAIN_CALLBACK_EXPERIMENT (MAIN_CALLBACK_RUN        + 1) 
#define MAIN_CALLBACK_EXPOSURE   (MAIN_CALLBACK_EXPERIMENT + 1)
#define MAIN_CALLBACK_CENMIN     (MAIN_CALLBACK_EXPOSURE   + 1)
#define MAIN_CALLBACK_CENMAX     (MAIN_CALLBACK_CENMIN     + 1)
#define MAIN_CALLBACK_LOCMIN     (MAIN_CALLBACK_CENMAX     + 1)
#define MAIN_CALLBACK_LOCMAX     (MAIN_CALLBACK_LOCMIN     + 1)
#define MAIN_CALLBACK_ALGMODE    (MAIN_CALLBACK_LOCMAX     + 1)
#define MAIN_CALLBACK_ALGTYPE    (MAIN_CALLBACK_ALGMODE    + 1)
#define MAIN_CALLBACK_NUM        (MAIN_CALLBACK_ALGTYPE    + 1)

// Window Live Variable Initial Value Defines
#define MAIN_OVERLAYEXPOSURE     1
#define MAIN_ALGORITHMMODE       1
#define MAIN_ALGORITHMTYPE       1
#define MAIN_CENMINVALUE         0
#define MAIN_CENMAXVALUE         3
#define MAIN_LOCMINVALUE         0
#define MAIN_LOCMAXVALUE         0
#define MAIN_MINSENSORS          1
#define MAIN_OUTPUTFILENAME      "OutFile.txt"
#define MAIN_PAUSETIME           750
#define MAIN_MAXSENSORS          32767
#define MAIN_NUMSENSORS          10
#define MAIN_COLOROBJECT         0
#define MAIN_DRAW                1
#define MAIN_SIZE                0.025f
#define MAIN_MINRADIUS           0.0f
#define MAIN_MAXRADIUS           1.414213f
#define MAIN_COMMUNICATIONRADIUS 0.1f
#define MAIN_STARTX              0.25f
#define MAIN_STARTY              0.25f
#define MAIN_ENDX                0.75f
#define MAIN_ENDY                0.75f
#define MAIN_MINLENGTH           0.0f
#define MAIN_MAXLENGTH           1000.0f
#define MAIN_LENGTH              0.85f
#define MAIN_GRIDH               15
#define MAIN_GRIDV               15
#define MAIN_RED                 ((float) MAIN_COLOR_BKG_R)
#define MAIN_GREEN               ((float) MAIN_COLOR_BKG_G)
#define MAIN_BLUE                ((float) MAIN_COLOR_BKG_B)

// Color Parameter Defines
#define MAIN_COLOR_BKG_R             0.0
#define MAIN_COLOR_BKG_G             0.0
#define MAIN_COLOR_BKG_B             0.0
#define MAIN_COLOR_SENSOR_UNKNOWN_R  0.5
#define MAIN_COLOR_SENSOR_UNKNOWN_G  0.5
#define MAIN_COLOR_SENSOR_UNKNOWN_B  0.5
#define MAIN_COLOR_SENSOR_KNOWN_R    0.5
#define MAIN_COLOR_SENSOR_KNOWN_G    0.5
#define MAIN_COLOR_SENSOR_KNOWN_B    1.0
#define MAIN_COLOR_SENSOR_VISITED_R  1.0
#define MAIN_COLOR_SENSOR_VISITED_G  1.0
#define MAIN_COLOR_SENSOR_VISITED_B  1.0
#define MAIN_COLOR_SENSOR_USELESS_R  0.5
#define MAIN_COLOR_SENSOR_USELESS_G  0.5
#define MAIN_COLOR_SENSOR_USELESS_B  0.5
#define MAIN_COLOR_SENSOR_SCURRENT_R 1.0
#define MAIN_COLOR_SENSOR_SCURRENT_G 1.0
#define MAIN_COLOR_SENSOR_SCURRENT_B 1.0
#define MAIN_COLOR_SENSOR_FCURRENT_R 1.0
#define MAIN_COLOR_SENSOR_FCURRENT_G 1.0
#define MAIN_COLOR_SENSOR_FCURRENT_B 0.0
#define MAIN_COLOR_EDGE_R            0.0
#define MAIN_COLOR_EDGE_G            0.0
#define MAIN_COLOR_EDGE_B            0.5
#define MAIN_COLOR_ELLIPSE_R         1.0
#define MAIN_COLOR_ELLIPSE_G         1.0
#define MAIN_COLOR_ELLIPSE_B         0.0
#define MAIN_COLOR_ENDPOINTS_R       1.0
#define MAIN_COLOR_ENDPOINTS_G       1.0
#define MAIN_COLOR_ENDPOINTS_B       0.0
#define MAIN_COLOR_GRIDPOINTS_R      1.0
#define MAIN_COLOR_GRIDPOINTS_G      0.0
#define MAIN_COLOR_GRIDPOINTS_B      0.0
#define MAIN_COLOR_GRIDEDGES_R       0.5
#define MAIN_COLOR_GRIDEDGES_G       0.0
#define MAIN_COLOR_GRIDEDGES_B       0.0
#define MAIN_COLOR_COVGRID_R         1.0
#define MAIN_COLOR_COVGRID_G         0.0
#define MAIN_COLOR_COVGRID_B         0.0
#define MAIN_COLOR_UNCOVGRID_R       0.0
#define MAIN_COLOR_UNCOVGRID_G       0.0
#define MAIN_COLOR_UNCOVGRID_B       1.0
#define MAIN_COLOR_NEXTCOVGRID_R     1.0
#define MAIN_COLOR_NEXTCOVGRID_G     1.0
#define MAIN_COLOR_NEXTCOVGRID_B     1.0
#define MAIN_COLOR_NEXTUNCOVGRID_R   0.0
#define MAIN_COLOR_NEXTUNCOVGRID_G   1.0
#define MAIN_COLOR_NEXTUNCOVGRID_B   0.0
#define MAIN_COLOR_CENMINPATH_R      0.75
#define MAIN_COLOR_CENMINPATH_G      0.75
#define MAIN_COLOR_CENMINPATH_B      0.75
#define MAIN_COLOR_CENMAXPATH_R      0.0
#define MAIN_COLOR_CENMAXPATH_G      0.75
#define MAIN_COLOR_CENMAXPATH_B      0.75
#define MAIN_COLOR_LOCMINPATH_R      1.0
#define MAIN_COLOR_LOCMINPATH_G      1.0
#define MAIN_COLOR_LOCMINPATH_B      1.0
#define MAIN_COLOR_LOCMAXPATH_R      0.0
#define MAIN_COLOR_LOCMAXPATH_G      1.0
#define MAIN_COLOR_LOCMAXPATH_B      1.0

#define MAIN_SIZE_BKG                0.0
#define MAIN_SIZE_SENSOR_UNKNOWN     0.005
#define MAIN_SIZE_SENSOR_KNOWN       0.005
#define MAIN_SIZE_SENSOR_VISITED     0.005
#define MAIN_SIZE_SENSOR_USELESS     0.005
#define MAIN_SIZE_SENSOR_SCURRENT    0.0075
#define MAIN_SIZE_SENSOR_FCURRENT    0.0075
#define MAIN_SIZE_EDGE               0.0015
#define MAIN_SIZE_ELLIPSE            0.0015
#define MAIN_SIZE_ENDPOINTS          0.0025
#define MAIN_SIZE_GRIDPOINTS         0.0025
#define MAIN_SIZE_GRIDEDGES          0.0015
#define MAIN_SIZE_COVGRID            0.0025
#define MAIN_SIZE_UNCOVGRID          0.0025
#define MAIN_SIZE_NEXTCOVGRID        0.0025
#define MAIN_SIZE_NEXTUNCOVGRID      0.0025
#define MAIN_SIZE_CENMINPATH         0.003
#define MAIN_SIZE_CENMAXPATH         0.003
#define MAIN_SIZE_LOCMINPATH         0.003
#define MAIN_SIZE_LOCMAXPATH         0.003

#define MAIN_DRAW_BKG               1
#define MAIN_DRAW_SENSOR_UNKNOWN    0
#define MAIN_DRAW_SENSOR_KNOWN      1
#define MAIN_DRAW_SENSOR_VISITED    1
#define MAIN_DRAW_SENSOR_USELESS    0
#define MAIN_DRAW_SENSOR_SCURRENT   1
#define MAIN_DRAW_SENSOR_FCURRENT   1
#define MAIN_DRAW_EDGE              0
#define MAIN_DRAW_ELLIPSE           1
#define MAIN_DRAW_ENDPOINTS         0
#define MAIN_DRAW_GRIDPOINTS        0
#define MAIN_DRAW_GRIDEDGES         0
#define MAIN_DRAW_COVGRID           1
#define MAIN_DRAW_UNCOVGRID         0
#define MAIN_DRAW_NEXTCOVGRID       1
#define MAIN_DRAW_NEXTUNCOVGRID     1
#define MAIN_DRAW_CENMINPATH        1
#define MAIN_DRAW_CENMAXPATH        1
#define MAIN_DRAW_LOCMINPATH        1
#define MAIN_DRAW_LOCMAXPATH        1

#define MAIN_COLOR_BKG                 (0                                )
#define MAIN_COLOR_SENSOR_UNKNOWN      (MAIN_COLOR_BKG                + 1)
#define MAIN_COLOR_SENSOR_KNOWN        (MAIN_COLOR_SENSOR_UNKNOWN     + 1)
#define MAIN_COLOR_SENSOR_VISITED      (MAIN_COLOR_SENSOR_KNOWN       + 1)
#define MAIN_COLOR_SENSOR_USELESS      (MAIN_COLOR_SENSOR_VISITED     + 1)
#define MAIN_COLOR_SENSOR_SCURRENT     (MAIN_COLOR_SENSOR_USELESS     + 1)
#define MAIN_COLOR_SENSOR_FCURRENT     (MAIN_COLOR_SENSOR_SCURRENT    + 1)
#define MAIN_COLOR_EDGE                (MAIN_COLOR_SENSOR_FCURRENT    + 1)
#define MAIN_COLOR_ELLIPSE             (MAIN_COLOR_EDGE               + 1)
#define MAIN_COLOR_ENDPOINTS           (MAIN_COLOR_ELLIPSE            + 1)
#define MAIN_COLOR_GRIDPOINTS          (MAIN_COLOR_ENDPOINTS          + 1)
#define MAIN_COLOR_GRIDEDGES           (MAIN_COLOR_GRIDPOINTS         + 1)
#define MAIN_COLOR_COVGRID             (MAIN_COLOR_GRIDEDGES          + 1)
#define MAIN_COLOR_UNCOVGRID           (MAIN_COLOR_COVGRID            + 1)
#define MAIN_COLOR_NEXTCOVGRID         (MAIN_COLOR_UNCOVGRID          + 1)
#define MAIN_COLOR_NEXTUNCOVGRID       (MAIN_COLOR_NEXTCOVGRID        + 1)
#define MAIN_COLOR_CENMINPATH          (MAIN_COLOR_NEXTUNCOVGRID      + 1)
#define MAIN_COLOR_CENMAXPATH          (MAIN_COLOR_CENMINPATH         + 1)
#define MAIN_COLOR_LOCMINPATH          (MAIN_COLOR_CENMAXPATH         + 1)
#define MAIN_COLOR_LOCMAXPATH          (MAIN_COLOR_LOCMINPATH         + 1)
#define MAIN_NUMCOLOROBJECTS           (MAIN_COLOR_LOCMAXPATH         + 1)

// Miscellaneous
#define MAIN_WINDOWNAME                "Ellipse Cover"
#define MAIN_CONTROLPANELNAME          "Control Panel"
#define MAIN_ANGLEDELTA                3.0



// Global Variables ////////////////////////////////////////////////////////////
// Algorithm Objects
GVExposureSensorNetwork2D SensorNetwork;           // Exposure Sensor Network

// Window Objects
GLUI* Glui = NULL;                                 // Glui Object for Window
GLUI_Panel* GluiInfoPanel = NULL;                  // Panel for Sensor Network Info
GLUI_Panel* GluiStartPanel = NULL;                 // Panel for Starting Coordinate
GLUI_Panel* GluiEndPanel = NULL;                   // Panel for Ending Coordinate
GLUI_Panel* GluiGridPanel = NULL;                  // Panel for Grid Sizes
GLUI_Panel* GluiColorPanel = NULL;                 // Panel for Color Information
GLUI_Panel* GluiButtonPanel = NULL;                // Panel for Buttons
GLUI_Panel* GluiAlgorithmPanel = NULL;             // Panel for Algorithm Options
GLUI_Panel* GluiAlgorithmSubPanel1 = NULL;         // Sub Panel for Algorithm Options
GLUI_Panel* GluiAlgorithmSubPanel2 = NULL;         // Sub Panel for Algorithm Options
GLUI_RadioGroup* GluiColorGroup = NULL;            // Group of Objects that have Color Properties
GLUI_RadioGroup* GluiModeGroup = NULL;             // Mode Group (Localized, Centralized)
GLUI_RadioGroup* GluiAlgorithmTypeGroup = NULL;    // Algorithm Group (Minimal, Maximal)
GLUI_RadioGroup* GluiCenMaxOptionsGroup = NULL;    // Centralized Maximal Options (Random, Shortest Path, Best Point, Adjusted Best Point)
GLUI_RadioGroup* GluiCenMinOptionsGroup = NULL;    // Centralized Minimal Options (Default)
GLUI_RadioGroup* GluiLocMaxOptionsGroup = NULL;    // Localized Maximal Options (Border Heuristic, Counterclockwise Heuristic)
GLUI_RadioGroup* GluiLocMinOptionsGroup = NULL;    // Localized Minimal Options (Default)

// Window Live Variables
int GLUIWindow_NumberOfSensors = MAIN_NUMSENSORS;                    // Number of Sensors
int GLUIWindow_GridH = MAIN_GRIDH;                                   // Horizontal Grid Size
int GLUIWindow_GridV = MAIN_GRIDV;                                   // Vertical Grid Size
int GLUIWindow_PreviousColorObject = MAIN_COLOROBJECT;               // Previous Selected Object
int GLUIWindow_ColorObject = MAIN_COLOROBJECT;                       // Current Selected Object
int GLUIWindow_Draw = MAIN_DRAW;                                     // Draw Selected Object?
int GLUIWindow_OverlayExposure = MAIN_OVERLAYEXPOSURE;               // Draw Exposure On Top of Covered/Uncovered Grid Squares
int GLUIWindow_PauseTime = MAIN_PAUSETIME;                           // Time to Pause Between Steps
int GLUIWindow_AlgorithmMode = MAIN_ALGORITHMMODE;                   // Selected Algorithm Mode
int GLUIWindow_AlgorithmType = MAIN_ALGORITHMTYPE;                   // Selected Algorithm Type
int GLUIWindow_CenMinOptions = MAIN_CENMINVALUE;                     // Selected Centralized Minimal Option
int GLUIWindow_CenMaxOptions = MAIN_CENMAXVALUE;                     // Selected Centralized Maximal Option
int GLUIWindow_LocMinOptions = MAIN_LOCMINVALUE;                     // Selected Localized Minimal Option
int GLUIWindow_LocMaxOptions = MAIN_LOCMAXVALUE;                     // Selected Localized Maximal Option
char GLUIWindow_FileName[sizeof(GLUI_String)] = MAIN_OUTPUTFILENAME; // Output File Name
float GLUIWindow_Size = MAIN_SIZE;                                   // Size of Object
float GLUIWindow_CommunicationRadius = MAIN_COMMUNICATIONRADIUS;     // Communication Radius
float GLUIWindow_StartingPointX = MAIN_STARTX;                       // Starting Point X Coordinate
float GLUIWindow_StartingPointY = MAIN_STARTY;                       // Starting Point Y Coordinate
float GLUIWindow_EndingPointX = MAIN_ENDY;                           // Ending Point X Coordinate
float GLUIWindow_EndingPointY = MAIN_ENDY;                           // Ending Point Y Coordinate
float GLUIWindow_MaximumLength = MAIN_LENGTH;                        // Maximum Length of Maximal Exposure Path
float GLUIWindow_Red = MAIN_RED;                                     // Current Red Color
float GLUIWindow_Green = MAIN_GREEN;                                 // Current Green Color
float GLUIWindow_Blue = MAIN_BLUE;                                   // Current Blue Color

// Drawing Attribute Variables
int DrawObject[MAIN_NUMCOLOROBJECTS] = { 0 };                        // Stores What Objects to Draw
double SizeObject[MAIN_NUMCOLOROBJECTS] = { 0.0 };                   // Stores Size of Objects
double ColorRed[MAIN_NUMCOLOROBJECTS] = { 0.0 };                     // Stores Red Color of Objects
double ColorGreen[MAIN_NUMCOLOROBJECTS] = { 0.0 };                   // Stores Green Color of Objects
double ColorBlue[MAIN_NUMCOLOROBJECTS] = { 0.0 };                    // Stores Blue Color of Objects



// Prototypes //////////////////////////////////////////////////////////////////
void Pause(int Milliseconds);

void myGlutInit(void);
void myGlutKeyboard(unsigned char Key, int X, int Y);
void myGlutMenu(int Value);
void myGlutIdle(void);
void myGlutMouse(int Button, int ButtonState, int X, int Y);
void myGlutMotion(int X, int Y);
void myGlutReshape(int X, int Y);
void myGlutDisplay(void);

void InitializeEnvironment(void);
void CreateGluiWindows(int MainWindow);

void DrawSphere(double X, double Y, double Z, double Radius, double Red, double Green, double Blue);
void DrawQuad(double X1, double Y1, double Z1, double X2, double Y2, double Z2, double X3, double Y3, double Z3, double X4, double Y4, double Z4, double Red, double Green, double Blue);
void DrawCylinder(double X1, double Y1, double X2, double Y2, double Z, double Radius, double Red, double Green, double Blue);
void DrawEllipse(double FX1, double FY1, double FX2, double FY2, double Z, double MaxLength, double Size, double Red, double Green, double Blue);
void DrawCircle(double X, double Y, double Z, double Radius, double Size, double Red, double Green, double Blue);

void ControlCallback(int ControlID);
void OnColor(void);
void OnNew(void);
void OnRadius(void);
void OnNumSensors(void);
void OnChangeEllipse(void);
void OnChangeGrid(void);
void OnChangeMode(void);
void OnChangeType(void);
void OnChangeCenMinOptions(void);
void OnChangeCenMaxOptions(void);
void OnChangeLocMinOptions(void);
void OnChangeLocMaxOptions(void);
void OnRun(void);
void OnExposure(void);
void OnExperiment(void);



// Main ////////////////////////////////////////////////////////////////////////
void main(int argc, char** argv)
/*
  main() initializes the environment and calls the glutMainLoop() for OpenGL
  drawing.
*/
{ // main
  InitializeEnvironment();
  glutMainLoop();
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



void DrawSphere(double X, double Y, double Z, double Radius, double Red, double Green, double Blue)
/*
  DrawSphere() draws a sphere of the specified radius and color and position
  (X, Y, Z).
*/
{ // DrawSphere
  GLfloat MaterialAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };  // Ambient Color
  GLfloat MaterialDiffuse[] = { 0.0f, 0.0f, 0.0f, 1.0f };  // Diffuse Color
  GLfloat MaterialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // Specular Color
  GLfloat MaterialShininess[] = { 128.0f };                // Shininess

  MaterialAmbient[0] = MaterialDiffuse[0] = (GLfloat) Red;
  MaterialAmbient[1] = MaterialDiffuse[1] = (GLfloat) Green;
  MaterialAmbient[2] = MaterialDiffuse[2] = (GLfloat) Blue;
  glMaterialfv(GL_FRONT, GL_AMBIENT, MaterialAmbient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, MaterialDiffuse);
  glMaterialfv(GL_FRONT, GL_SHININESS, MaterialShininess);
  glMaterialfv(GL_FRONT, GL_SPECULAR, MaterialSpecular);

  glPushMatrix();
  glTranslated(X, Y, Z);
  glutSolidSphere((GLfloat) Radius, MAIN_SPHERE_SLICES, MAIN_SPHERE_STACKS);
  glPopMatrix();
} // DrawSphere



void DrawQuad(double X1, double Y1, double Z1, double X2, double Y2, double Z2, double X3, double Y3, double Z3, double X4, double Y4, double Z4, double Red, double Green, double Blue)
/*
  DrawQuad() draws a two-dimensional quadrilateral on the screen at the
  specified depth with the specified color.
*/
{ // DrawQuad
  GLfloat MaterialAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };  // Ambient Color
  GLfloat MaterialDiffuse[] = { 0.0f, 0.0f, 0.0f, 1.0f };  // Diffuse Color
  GLfloat MaterialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // Specular Color
  GLfloat MaterialShininess[] = { 128.0f };                // Shininess

  MaterialAmbient[0] = MaterialDiffuse[0] = (GLfloat) Red;
  MaterialAmbient[1] = MaterialDiffuse[1] = (GLfloat) Green;
  MaterialAmbient[2] = MaterialDiffuse[2] = (GLfloat) Blue;
  glMaterialfv(GL_FRONT, GL_AMBIENT, MaterialAmbient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, MaterialDiffuse);
  glMaterialfv(GL_FRONT, GL_SHININESS, MaterialShininess);
  glMaterialfv(GL_FRONT, GL_SPECULAR, MaterialSpecular);

  glBegin(GL_QUADS);
  glVertex3d(X1, Y1, Z1);
  glVertex3d(X2, Y2, Z2);
  glVertex3d(X3, Y3, Z3);
  glVertex3d(X4, Y4, Z4);
  glEnd();
} // DrawQuad



void DrawCylinder(double X1, double Y1, double X2, double Y2, double Z, double Radius, double Red, double Green, double Blue)
/*
  DrawCylinder() draws a cylinder from (X1, Y1) to (X2, Y2) with a specified
  width (Radius) at a specified depth and with a specified color.  The
  awkward transformations performed in this function are due to the weird way
  that glu does cylinders and transforms them.  This function is similar
  to drawing a line between two points in a plane.
*/
{ // DrawCylinder
  double DX = X2 - X1;                                     // Change in X
  double DY = Y2 - Y1;                                     // Change in Y
  double Length = Distance2D(X1, Y1, X2, Y2);              // Distance Between Points
  double Angle = acos(DY / Length) / MAIN_TORADIANS;       // Current Angle
  GLfloat MaterialAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };  // Ambient Color
  GLfloat MaterialDiffuse[] = { 0.0f, 0.0f, 0.0f, 1.0f };  // Diffuse Color
  GLfloat MaterialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // Specular Color
  GLfloat MaterialShininess[] = { 128.0f };                // Shininess
  GLUquadricObj* qobj = NULL;                              // Quadric Object for Function Call

  if (DX < 0) Angle = -Angle;

  MaterialAmbient[0] = MaterialDiffuse[0] = (GLfloat) Red;
  MaterialAmbient[1] = MaterialDiffuse[1] = (GLfloat) Green;
  MaterialAmbient[2] = MaterialDiffuse[2] = (GLfloat) Blue;
  glMaterialfv(GL_FRONT, GL_AMBIENT, MaterialAmbient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, MaterialDiffuse);
  glMaterialfv(GL_FRONT, GL_SHININESS, MaterialShininess);
  glMaterialfv(GL_FRONT, GL_SPECULAR, MaterialSpecular);

  qobj = gluNewQuadric();
  glPushMatrix();
  glRotated(-90.0, 1.0, 0.0, 0.0);
  glRotated(Angle, 0.0, 1.0, 0.0);
  DY = X1 * cos((Angle - 90) * MAIN_TORADIANS) - Y1 * sin((Angle - 90) * MAIN_TORADIANS);
  DX = X1 * cos((Angle) * MAIN_TORADIANS) - Y1 * sin((Angle) * MAIN_TORADIANS);
  glTranslated(DX, Z, DY);
  gluQuadricDrawStyle(qobj, GLU_FILL);
  gluQuadricOrientation(qobj, GLU_OUTSIDE);
  gluCylinder(qobj, Radius, Radius, Length, MAIN_CYLINDER_SLICES, MAIN_CYLINDER_STACKS);
  gluDeleteQuadric(qobj);  
  glPopMatrix();
} // DrawCylinder



void DrawEllipse(double FX1, double FY1, double FX2, double FY2, double Z, double MaxLength, double Size, double Red, double Green, double Blue)
/*
  DrawEllipse() draws an ellipse using the cylinder function.  It estimates an
  ellipse by drawing a straight line for a specified number of degrees
  (MAIN_ANGLEDELTA).
*/
{ // DrawEllipse
  double Length = Distance2D(FX1, FY1, FX2, FY2);   // Distance Between Foci
  double A = MaxLength / 2.0;                       // The Ellispe "A" Parameter
  double B = sqrt(A * A - Length * Length / 4.0);   // The Ellipse "B" Parameter
  double VX = (FX2 - FX1) / Length;                 // X Coordinate of Vector from F1 to F2
  double VY = (FY2 - FY1) / Length;                 // Y Coordinate of Vector from F1 to F2
  double AX = VX * A;                               // X Coordinate Vector in "A" Direction
  double AY = VY * A;                               // Y Coordinate Vector in "A" Direction
  double BX = -VY * B;                              // X Coordinate Vector in "B" Direction
  double BY = VX * B;                               // Y Coordinate Vector in "B" Direction
  double CX = (FX1 + FX2) / 2.0;                    // X Coordinate Vector in "C" Direction (A, But Smaller)
  double CY = (FY1 + FY2) / 2.0;                    // X Coordinate Vector in "C" Direction (A, But Smaller)
  double X1 = 0.0;                                  // Temporary X Variable
  double Y1 = 0.0;                                  // Temporary Y Variable
  double X2 = 0.0;                                  // Temporary X Variable
  double Y2 = 0.0;                                  // Temporary Y Variable
  double AngleDegrees = 0.0;                        // Current Angle in Degrees
  double MaxAngleDegrees = 360.0 - MAIN_ANGLEDELTA; // Maximum Angle in Degrees
  double AngleRadians = 0.0;                        // Current Angle in Radians
  double NextAngleRadians = 0.0;                    // Next Angle in Radians

  for (AngleDegrees = 0.0; AngleDegrees <= MaxAngleDegrees; AngleDegrees += MAIN_ANGLEDELTA)
  {
    AngleRadians = AngleDegrees * MAIN_TORADIANS;
    NextAngleRadians = (AngleDegrees + MAIN_ANGLEDELTA) * MAIN_TORADIANS;
    X1 = CX + AX * cos(AngleRadians) + BX * sin(AngleRadians);
    Y1 = CY + AY * cos(AngleRadians) + BY * sin(AngleRadians);
    X2 = CX + AX * cos(NextAngleRadians) + BX * sin(NextAngleRadians);
    Y2 = CY + AY * cos(NextAngleRadians) + BY * sin(NextAngleRadians);
    DrawCylinder(X1, Y1, X2, Y2, Z, Size, Red, Green, Blue);
  }
} // DrawEllipse



void DrawCircle(double X, double Y, double Z, double Radius, double Size, double Red, double Green, double Blue)
/*
  DrawCircle() is a sub-function of DrawEllipse but is faster because it does
  not have to calculate the A and B values.
*/
{ // DrawCircle
  double X1 = 0.0;                                  // Temporary X Variable
  double Y1 = 0.0;                                  // Temporary Y Variable
  double X2 = 0.0;                                  // Temporary X Variable
  double Y2 = 0.0;                                  // Temporary Y Variable
  double AngleDegrees = 0.0;                        // Current Angle in Degrees
  double MaxAngleDegrees = 360.0 - MAIN_ANGLEDELTA; // Maximum Angle in Degrees
  double AngleRadians = 0.0;                        // Current Angle in Radians
  double NextAngleRadians = 0.0;                    // Next Angle in Radians

  for (AngleDegrees = 0.0; AngleDegrees <= MaxAngleDegrees; AngleDegrees += MAIN_ANGLEDELTA)
  {
    AngleRadians = AngleDegrees * MAIN_TORADIANS;
    NextAngleRadians = (AngleDegrees + MAIN_ANGLEDELTA) * MAIN_TORADIANS;
    X1 = X + Radius * cos(AngleRadians);
    Y1 = Y + Radius * sin(AngleRadians);
    X2 = X + Radius * cos(NextAngleRadians);
    Y2 = Y + Radius * sin(NextAngleRadians);
    DrawCylinder(X1, Y1, X2, Y2, Z, Size, Red, Green, Blue);
  }
} // DrawCircle



void InitializeEnvironment(void)
/*
  InitializeEnvironment() initializes the environment by creating glut windows
  and such and creates the Glui interface windows.
*/
{ // InitializeEnvironment
  int MainWindow = 0; // ID Number for Main Window (Used for Positioning)

  // Glut Initialization
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowPosition(MAIN_CONTROLPANELX, MAIN_CONTROLPANELY);
  glutInitWindowSize(MAIN_WINDOWWIDTH, MAIN_WINDOWHEIGHT);

  MainWindow = glutCreateWindow(MAIN_WINDOWNAME);
  glutKeyboardFunc(myGlutKeyboard);
  glutDisplayFunc(myGlutDisplay);
  glutReshapeFunc(myGlutReshape);
  glutMotionFunc(myGlutMotion);
  glutMouseFunc(myGlutMouse);
  myGlutInit();

  // Glui Initialization (Creating Windows)
  CreateGluiWindows(MainWindow);

  // Set Window Positions
  glutPositionWindow(MAIN_WINDOWX, MAIN_WINDOWY);
  GLUI_Master.set_glutIdleFunc(myGlutIdle);

  // Standard Initialization
  srand(time(NULL));
} // InitializeEnvironment



void CreateGluiWindows(int MainWindow)
/*
  CreateGluiWindows() creates all the windows using Glui functions.
*/
{ // CreateGluiWindows
  GLUI_StaticText* CurrentText = NULL; // Current Static Text (Used for Sizing)

  Glui = GLUI_Master.create_glui(MAIN_CONTROLPANELNAME, 0);

  GluiInfoPanel = Glui->add_panel(MAIN_NOTEXT, GLUI_PANEL_NONE);

  CurrentText = Glui->add_statictext_to_panel(GluiInfoPanel, MAIN_NUMSENSORSTEXT);
  CurrentText->set_h(MAIN_STATICTEXTSIZE);
  CurrentText = Glui->add_statictext_to_panel(GluiInfoPanel, MAIN_RADIUSTEXT);
  CurrentText->set_h(MAIN_STATICTEXTSIZE);
  CurrentText = Glui->add_statictext_to_panel(GluiInfoPanel, MAIN_LENGTHTEXT);
  CurrentText->set_h(MAIN_STATICTEXTSIZE);
  CurrentText = Glui->add_statictext_to_panel(GluiInfoPanel, MAIN_PAUSETIMETEXT);
  CurrentText->set_h(MAIN_STATICTEXTSIZE);
  CurrentText = Glui->add_statictext_to_panel(GluiInfoPanel, MAIN_FILENAMETEXT);
  CurrentText->set_h(MAIN_STATICTEXTSIZE);

  Glui->add_column_to_panel(GluiInfoPanel, false);

  Glui->add_edittext_to_panel(GluiInfoPanel, MAIN_NOTEXT, GLUI_EDITTEXT_INT, &GLUIWindow_NumberOfSensors, MAIN_CALLBACK_NUMSENSORS, ControlCallback);
  Glui->add_edittext_to_panel(GluiInfoPanel, MAIN_NOTEXT, GLUI_EDITTEXT_FLOAT, &GLUIWindow_CommunicationRadius, MAIN_CALLBACK_RADIUS, ControlCallback);
  Glui->add_edittext_to_panel(GluiInfoPanel, MAIN_NOTEXT, GLUI_EDITTEXT_FLOAT, &GLUIWindow_MaximumLength, MAIN_CALLBACK_MAXLENGTH, ControlCallback);
  Glui->add_edittext_to_panel(GluiInfoPanel, MAIN_NOTEXT, GLUI_EDITTEXT_INT, &GLUIWindow_PauseTime);
  Glui->add_edittext_to_panel(GluiInfoPanel, MAIN_NOTEXT, GLUI_EDITTEXT_TEXT, &GLUIWindow_FileName);

  GluiStartPanel = Glui->add_panel(MAIN_STARTTEXT);
  Glui->add_edittext_to_panel(GluiStartPanel, MAIN_NOTEXT, GLUI_EDITTEXT_FLOAT, &GLUIWindow_StartingPointX, MAIN_CALLBACK_STARTX, ControlCallback);
  Glui->add_column_to_panel(GluiStartPanel, false);
  Glui->add_edittext_to_panel(GluiStartPanel, MAIN_NOTEXT, GLUI_EDITTEXT_FLOAT, &GLUIWindow_StartingPointY, MAIN_CALLBACK_STARTY, ControlCallback);

  GluiEndPanel = Glui->add_panel(MAIN_ENDTEXT);
  Glui->add_edittext_to_panel(GluiEndPanel, MAIN_NOTEXT, GLUI_EDITTEXT_FLOAT, &GLUIWindow_EndingPointX, MAIN_CALLBACK_ENDY, ControlCallback);
  Glui->add_column_to_panel(GluiEndPanel, false);
  Glui->add_edittext_to_panel(GluiEndPanel, MAIN_NOTEXT, GLUI_EDITTEXT_FLOAT, &GLUIWindow_EndingPointY, MAIN_CALLBACK_ENDY, ControlCallback);

  GluiGridPanel = Glui->add_panel(MAIN_GRIDTEXT);
  Glui->add_edittext_to_panel(GluiGridPanel, MAIN_NOTEXT, GLUI_EDITTEXT_INT, &GLUIWindow_GridH, MAIN_CALLBACK_GRIDH, ControlCallback);
  Glui->add_column_to_panel(GluiGridPanel, false);
  Glui->add_edittext_to_panel(GluiGridPanel, MAIN_NOTEXT, GLUI_EDITTEXT_INT, &GLUIWindow_GridV, MAIN_CALLBACK_GRIDV, ControlCallback);

  GluiAlgorithmPanel = Glui->add_panel(MAIN_ALGORITHMTEXT);
  GluiAlgorithmSubPanel1 = Glui->add_panel_to_panel(GluiAlgorithmPanel, MAIN_NOTEXT);
  GluiModeGroup = Glui->add_radiogroup_to_panel(GluiAlgorithmSubPanel1, &GLUIWindow_AlgorithmMode, MAIN_CALLBACK_ALGMODE, ControlCallback);
  Glui->add_radiobutton_to_group(GluiModeGroup, MAIN_CENTEXT);
  Glui->add_radiobutton_to_group(GluiModeGroup, MAIN_LOCTEXT);

  Glui->add_column_to_panel(GluiAlgorithmSubPanel1, false);
  GluiAlgorithmTypeGroup = Glui->add_radiogroup_to_panel(GluiAlgorithmSubPanel1, &GLUIWindow_AlgorithmType, MAIN_CALLBACK_ALGTYPE, ControlCallback);
  Glui->add_radiobutton_to_group(GluiAlgorithmTypeGroup, MAIN_MINTEXT);
  Glui->add_radiobutton_to_group(GluiAlgorithmTypeGroup, MAIN_MAXTEXT);

  GluiAlgorithmSubPanel2 = Glui->add_panel_to_panel(GluiAlgorithmPanel, MAIN_NOTEXT);
  Glui->add_statictext_to_panel(GluiAlgorithmSubPanel2, MAIN_CENMINTEXT);
  GluiCenMinOptionsGroup = Glui->add_radiogroup_to_panel(GluiAlgorithmSubPanel2, &GLUIWindow_CenMinOptions, MAIN_CALLBACK_CENMIN, ControlCallback);
  Glui->add_radiobutton_to_group(GluiCenMinOptionsGroup, MAIN_CENMIN_1);
  Glui->add_statictext_to_panel(GluiAlgorithmSubPanel2, MAIN_NOTEXT);

  Glui->add_statictext_to_panel(GluiAlgorithmSubPanel2, MAIN_CENMAXTEXT);
  GluiCenMaxOptionsGroup = Glui->add_radiogroup_to_panel(GluiAlgorithmSubPanel2, &GLUIWindow_CenMaxOptions, MAIN_CALLBACK_CENMAX, ControlCallback);
  Glui->add_radiobutton_to_group(GluiCenMaxOptionsGroup, MAIN_CENMAX_1);
  Glui->add_radiobutton_to_group(GluiCenMaxOptionsGroup, MAIN_CENMAX_2);
  Glui->add_radiobutton_to_group(GluiCenMaxOptionsGroup, MAIN_CENMAX_3);
  Glui->add_radiobutton_to_group(GluiCenMaxOptionsGroup, MAIN_CENMAX_4);
  Glui->add_column_to_panel(GluiAlgorithmSubPanel2, false);

  Glui->add_statictext_to_panel(GluiAlgorithmSubPanel2, MAIN_LOCMINTEXT);
  GluiLocMinOptionsGroup = Glui->add_radiogroup_to_panel(GluiAlgorithmSubPanel2, &GLUIWindow_LocMinOptions, MAIN_CALLBACK_LOCMIN, ControlCallback);
  Glui->add_radiobutton_to_group(GluiLocMinOptionsGroup, MAIN_LOCMIN_1);
  Glui->add_statictext_to_panel(GluiAlgorithmSubPanel2, MAIN_NOTEXT);

  Glui->add_statictext_to_panel(GluiAlgorithmSubPanel2, MAIN_LOCMAXTEXT);
  GluiLocMaxOptionsGroup = Glui->add_radiogroup_to_panel(GluiAlgorithmSubPanel2, &GLUIWindow_LocMaxOptions, MAIN_CALLBACK_LOCMAX, ControlCallback);
  Glui->add_radiobutton_to_group(GluiLocMaxOptionsGroup, MAIN_LOCMAX_1);
  Glui->add_radiobutton_to_group(GluiLocMaxOptionsGroup, MAIN_LOCMAX_2);

  Glui->add_column(true);

  GluiColorPanel = Glui->add_panel(MAIN_COLORTEXT);
  GluiColorGroup = Glui->add_radiogroup_to_panel(GluiColorPanel, &GLUIWindow_ColorObject, MAIN_CALLBACK_COLOR, ControlCallback);
  Glui->add_radiobutton_to_group(GluiColorGroup, MAIN_BKG);
  Glui->add_radiobutton_to_group(GluiColorGroup, MAIN_UNKNOWNSENSOR);
  Glui->add_radiobutton_to_group(GluiColorGroup, MAIN_KNOWNSENSOR);
  Glui->add_radiobutton_to_group(GluiColorGroup, MAIN_VISITEDSENSOR);
  Glui->add_radiobutton_to_group(GluiColorGroup, MAIN_USELESSSENSOR);
  Glui->add_radiobutton_to_group(GluiColorGroup, MAIN_CURRENTSENSORSEARCH);
  Glui->add_radiobutton_to_group(GluiColorGroup, MAIN_CURRENTSENSORFORWARD);
  Glui->add_radiobutton_to_group(GluiColorGroup, MAIN_EDGE);
  Glui->add_radiobutton_to_group(GluiColorGroup, MAIN_ELLIPSE);
  Glui->add_radiobutton_to_group(GluiColorGroup, MAIN_ENDPOINTS);
  Glui->add_radiobutton_to_group(GluiColorGroup, MAIN_GRIDPOINTS);
  Glui->add_radiobutton_to_group(GluiColorGroup, MAIN_GRIDEDGES);
  Glui->add_radiobutton_to_group(GluiColorGroup, MAIN_COVGRID);
  Glui->add_radiobutton_to_group(GluiColorGroup, MAIN_UNCOVGRID);
  Glui->add_radiobutton_to_group(GluiColorGroup, MAIN_NEXTCOVGRID);
  Glui->add_radiobutton_to_group(GluiColorGroup, MAIN_NEXTUNCOVGRID);
  Glui->add_radiobutton_to_group(GluiColorGroup, MAIN_PATH_CEN_MIN);
  Glui->add_radiobutton_to_group(GluiColorGroup, MAIN_PATH_CEN_MAX);
  Glui->add_radiobutton_to_group(GluiColorGroup, MAIN_PATH_LOC_MIN);
  Glui->add_radiobutton_to_group(GluiColorGroup, MAIN_PATH_LOC_MAX);
  Glui->add_statictext_to_panel(GluiColorPanel, MAIN_NOTEXT);
  Glui->add_checkbox_to_panel(GluiColorPanel, MAIN_EXPOSURETEXT, &GLUIWindow_OverlayExposure, MAIN_CALLBACK_EXPOSURE, ControlCallback);
  Glui->add_column_to_panel(GluiColorPanel, true);
  Glui->add_edittext_to_panel(GluiColorPanel, MAIN_REDTEXT, GLUI_EDITTEXT_FLOAT, &GLUIWindow_Red, MAIN_CALLBACK_RED, ControlCallback);
  Glui->add_edittext_to_panel(GluiColorPanel, MAIN_GREENTEXT, GLUI_EDITTEXT_FLOAT, &GLUIWindow_Green, MAIN_CALLBACK_GREEN, ControlCallback);
  Glui->add_edittext_to_panel(GluiColorPanel, MAIN_BLUETEXT, GLUI_EDITTEXT_FLOAT, &GLUIWindow_Blue, MAIN_CALLBACK_BLUE, ControlCallback);
  Glui->add_edittext_to_panel(GluiColorPanel, MAIN_SIZETEXT, GLUI_EDITTEXT_FLOAT, &GLUIWindow_Size, MAIN_CALLBACK_SIZE, ControlCallback);
  Glui->add_checkbox_to_panel(GluiColorPanel, MAIN_DRAWTEXT, &GLUIWindow_Draw, MAIN_CALLBACK_DRAW, ControlCallback);

  GluiButtonPanel = Glui->add_panel(MAIN_NOTEXT, GLUI_PANEL_NONE);
  Glui->add_button_to_panel(GluiButtonPanel, MAIN_NEWTEXT, MAIN_CALLBACK_NEW, ControlCallback);
  Glui->add_button_to_panel(GluiButtonPanel, MAIN_EXPERIMENTTEXT, MAIN_CALLBACK_EXPERIMENT, ControlCallback);
  Glui->add_column_to_panel(GluiButtonPanel, false);
  Glui->add_button_to_panel(GluiButtonPanel, MAIN_RUNTEXT, MAIN_CALLBACK_RUN, ControlCallback);
  Glui->add_button_to_panel(GluiButtonPanel, MAIN_QUITTEXT, MAIN_CALLBACK_QUIT, ControlCallback);

  Glui->set_main_gfx_window(MainWindow);
} // CreateGluiWindows



void myGlutInit(void)
/*
  This function is used to initialize the Glut.  It initializes all color,
  size, and drawing information as well as enables the lighting.
*/
{ // myGlutInit
  GLfloat LocalAmbient[] = {  0.5,  0.5f,  0.5f,  0.5f }; // Local Ambienet Lighting
  GLfloat Ambient0[]  = {  0.0f,  0.0f,  0.0f,  1.0f };   // Ambient Color of Light
  GLfloat Diffuse0[]  = {  1.0f,  1.0f,  1.0f,  1.0f };   // Diffuse Color of Light
  GLfloat Specular0[] = {  1.0f,  1.0f,  1.0f,  1.0f };   // Specular Color of Light
  GLfloat Position0[] = {  1.0f,  1.0f, -1.0f,  0.0f };   // Position of Light

  // Initialize the Lighting
  glEnable(GL_LIGHTING);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LocalAmbient);
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_POSITION, Position0);
  glLightfv(GL_LIGHT0, GL_AMBIENT, Ambient0);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, Diffuse0);
  glLightfv(GL_LIGHT0, GL_SPECULAR, Specular0);

  // Initialize Object Attributes (Draw and Size)
  DrawObject[MAIN_COLOR_BKG]                 = MAIN_DRAW_BKG;
  DrawObject[MAIN_COLOR_SENSOR_UNKNOWN]      = MAIN_DRAW_SENSOR_UNKNOWN;
  DrawObject[MAIN_COLOR_SENSOR_KNOWN]        = MAIN_DRAW_SENSOR_KNOWN;
  DrawObject[MAIN_COLOR_SENSOR_VISITED]      = MAIN_DRAW_SENSOR_VISITED;
  DrawObject[MAIN_COLOR_SENSOR_USELESS]      = MAIN_DRAW_SENSOR_USELESS;
  DrawObject[MAIN_COLOR_SENSOR_SCURRENT]     = MAIN_DRAW_SENSOR_SCURRENT;
  DrawObject[MAIN_COLOR_SENSOR_FCURRENT]     = MAIN_DRAW_SENSOR_FCURRENT;
  DrawObject[MAIN_COLOR_EDGE]                = MAIN_DRAW_EDGE;
  DrawObject[MAIN_COLOR_ELLIPSE]             = MAIN_DRAW_ELLIPSE;
  DrawObject[MAIN_COLOR_ENDPOINTS]           = MAIN_DRAW_ENDPOINTS;
  DrawObject[MAIN_COLOR_GRIDPOINTS]          = MAIN_DRAW_GRIDPOINTS;
  DrawObject[MAIN_COLOR_GRIDEDGES]           = MAIN_DRAW_GRIDEDGES;
  DrawObject[MAIN_COLOR_COVGRID]             = MAIN_DRAW_COVGRID;
  DrawObject[MAIN_COLOR_UNCOVGRID]           = MAIN_DRAW_UNCOVGRID;
  DrawObject[MAIN_COLOR_NEXTCOVGRID]         = MAIN_DRAW_NEXTCOVGRID;
  DrawObject[MAIN_COLOR_NEXTUNCOVGRID]       = MAIN_DRAW_NEXTUNCOVGRID;
  DrawObject[MAIN_COLOR_CENMINPATH]          = MAIN_DRAW_CENMINPATH;
  DrawObject[MAIN_COLOR_CENMAXPATH]          = MAIN_DRAW_CENMAXPATH;
  DrawObject[MAIN_COLOR_LOCMINPATH]          = MAIN_DRAW_LOCMINPATH;
  DrawObject[MAIN_COLOR_LOCMAXPATH]          = MAIN_DRAW_LOCMAXPATH;

  SizeObject[MAIN_COLOR_BKG]                 = MAIN_SIZE_BKG;
  SizeObject[MAIN_COLOR_SENSOR_UNKNOWN]      = MAIN_SIZE_SENSOR_UNKNOWN;
  SizeObject[MAIN_COLOR_SENSOR_KNOWN]        = MAIN_SIZE_SENSOR_KNOWN;
  SizeObject[MAIN_COLOR_SENSOR_VISITED]      = MAIN_SIZE_SENSOR_VISITED;
  SizeObject[MAIN_COLOR_SENSOR_USELESS]      = MAIN_SIZE_SENSOR_USELESS;
  SizeObject[MAIN_COLOR_SENSOR_SCURRENT]     = MAIN_SIZE_SENSOR_SCURRENT;
  SizeObject[MAIN_COLOR_SENSOR_FCURRENT]     = MAIN_SIZE_SENSOR_FCURRENT;
  SizeObject[MAIN_COLOR_EDGE]                = MAIN_SIZE_EDGE;
  SizeObject[MAIN_COLOR_ELLIPSE]             = MAIN_SIZE_ELLIPSE;
  SizeObject[MAIN_COLOR_ENDPOINTS]           = MAIN_SIZE_ENDPOINTS;
  SizeObject[MAIN_COLOR_GRIDPOINTS]          = MAIN_SIZE_GRIDPOINTS;
  SizeObject[MAIN_COLOR_GRIDEDGES]           = MAIN_SIZE_GRIDEDGES;
  SizeObject[MAIN_COLOR_COVGRID]             = MAIN_SIZE_COVGRID;
  SizeObject[MAIN_COLOR_UNCOVGRID]           = MAIN_SIZE_UNCOVGRID;
  SizeObject[MAIN_COLOR_NEXTCOVGRID]         = MAIN_SIZE_NEXTCOVGRID;
  SizeObject[MAIN_COLOR_NEXTUNCOVGRID]       = MAIN_SIZE_NEXTUNCOVGRID;
  SizeObject[MAIN_COLOR_CENMINPATH]          = MAIN_SIZE_CENMINPATH;
  SizeObject[MAIN_COLOR_CENMAXPATH]          = MAIN_SIZE_CENMAXPATH;
  SizeObject[MAIN_COLOR_LOCMINPATH]          = MAIN_SIZE_LOCMINPATH;
  SizeObject[MAIN_COLOR_LOCMAXPATH]          = MAIN_SIZE_LOCMAXPATH;

  // Initialize Colors
  ColorRed[MAIN_COLOR_BKG]                   = MAIN_COLOR_BKG_R;
  ColorGreen[MAIN_COLOR_BKG]                 = MAIN_COLOR_BKG_G;
  ColorBlue[MAIN_COLOR_BKG]                  = MAIN_COLOR_BKG_B;
  ColorRed[MAIN_COLOR_SENSOR_UNKNOWN]        = MAIN_COLOR_SENSOR_UNKNOWN_R;
  ColorGreen[MAIN_COLOR_SENSOR_UNKNOWN]      = MAIN_COLOR_SENSOR_UNKNOWN_G;
  ColorBlue[MAIN_COLOR_SENSOR_UNKNOWN]       = MAIN_COLOR_SENSOR_UNKNOWN_B;
  ColorRed[MAIN_COLOR_SENSOR_KNOWN]          = MAIN_COLOR_SENSOR_KNOWN_R;
  ColorGreen[MAIN_COLOR_SENSOR_KNOWN]        = MAIN_COLOR_SENSOR_KNOWN_G;
  ColorBlue[MAIN_COLOR_SENSOR_KNOWN]         = MAIN_COLOR_SENSOR_KNOWN_B;
  ColorRed[MAIN_COLOR_SENSOR_VISITED]        = MAIN_COLOR_SENSOR_VISITED_R;
  ColorGreen[MAIN_COLOR_SENSOR_VISITED]      = MAIN_COLOR_SENSOR_VISITED_G;
  ColorBlue[MAIN_COLOR_SENSOR_VISITED]       = MAIN_COLOR_SENSOR_VISITED_B;
  ColorRed[MAIN_COLOR_SENSOR_USELESS]        = MAIN_COLOR_SENSOR_USELESS_R;
  ColorGreen[MAIN_COLOR_SENSOR_USELESS]      = MAIN_COLOR_SENSOR_USELESS_G;
  ColorBlue[MAIN_COLOR_SENSOR_USELESS]       = MAIN_COLOR_SENSOR_USELESS_B;
  ColorRed[MAIN_COLOR_SENSOR_SCURRENT]       = MAIN_COLOR_SENSOR_SCURRENT_R;
  ColorGreen[MAIN_COLOR_SENSOR_SCURRENT]     = MAIN_COLOR_SENSOR_SCURRENT_G;
  ColorBlue[MAIN_COLOR_SENSOR_SCURRENT]      = MAIN_COLOR_SENSOR_SCURRENT_B;
  ColorRed[MAIN_COLOR_SENSOR_FCURRENT]       = MAIN_COLOR_SENSOR_FCURRENT_R;
  ColorGreen[MAIN_COLOR_SENSOR_FCURRENT]     = MAIN_COLOR_SENSOR_FCURRENT_G;
  ColorBlue[MAIN_COLOR_SENSOR_FCURRENT]      = MAIN_COLOR_SENSOR_FCURRENT_B;
  ColorRed[MAIN_COLOR_EDGE]                  = MAIN_COLOR_EDGE_R;
  ColorGreen[MAIN_COLOR_EDGE]                = MAIN_COLOR_EDGE_G;
  ColorBlue[MAIN_COLOR_EDGE]                 = MAIN_COLOR_EDGE_B;
  ColorRed[MAIN_COLOR_ELLIPSE]               = MAIN_COLOR_ELLIPSE_R;
  ColorGreen[MAIN_COLOR_ELLIPSE]             = MAIN_COLOR_ELLIPSE_G;
  ColorBlue[MAIN_COLOR_ELLIPSE]              = MAIN_COLOR_ELLIPSE_B;
  ColorRed[MAIN_COLOR_ENDPOINTS]             = MAIN_COLOR_ENDPOINTS_R;
  ColorGreen[MAIN_COLOR_ENDPOINTS]           = MAIN_COLOR_ENDPOINTS_G;
  ColorBlue[MAIN_COLOR_ENDPOINTS]            = MAIN_COLOR_ENDPOINTS_B;
  ColorRed[MAIN_COLOR_GRIDPOINTS]            = MAIN_COLOR_GRIDPOINTS_R;
  ColorGreen[MAIN_COLOR_GRIDPOINTS]          = MAIN_COLOR_GRIDPOINTS_G;
  ColorBlue[MAIN_COLOR_GRIDPOINTS]           = MAIN_COLOR_GRIDPOINTS_B;
  ColorRed[MAIN_COLOR_GRIDEDGES]             = MAIN_COLOR_GRIDEDGES_R;
  ColorGreen[MAIN_COLOR_GRIDEDGES]           = MAIN_COLOR_GRIDEDGES_G;
  ColorBlue[MAIN_COLOR_GRIDEDGES]            = MAIN_COLOR_GRIDEDGES_B;
  ColorRed[MAIN_COLOR_COVGRID]               = MAIN_COLOR_COVGRID_R;
  ColorGreen[MAIN_COLOR_COVGRID]             = MAIN_COLOR_COVGRID_G;
  ColorBlue[MAIN_COLOR_COVGRID]              = MAIN_COLOR_COVGRID_B;
  ColorRed[MAIN_COLOR_UNCOVGRID]             = MAIN_COLOR_UNCOVGRID_R;
  ColorGreen[MAIN_COLOR_UNCOVGRID]           = MAIN_COLOR_UNCOVGRID_G;
  ColorBlue[MAIN_COLOR_UNCOVGRID]            = MAIN_COLOR_UNCOVGRID_B;
  ColorRed[MAIN_COLOR_NEXTCOVGRID]           = MAIN_COLOR_NEXTCOVGRID_R;
  ColorGreen[MAIN_COLOR_NEXTCOVGRID]         = MAIN_COLOR_NEXTCOVGRID_G;
  ColorBlue[MAIN_COLOR_NEXTCOVGRID]          = MAIN_COLOR_NEXTCOVGRID_B;
  ColorRed[MAIN_COLOR_NEXTUNCOVGRID]         = MAIN_COLOR_NEXTUNCOVGRID_R;
  ColorGreen[MAIN_COLOR_NEXTUNCOVGRID]       = MAIN_COLOR_NEXTUNCOVGRID_G;
  ColorBlue[MAIN_COLOR_NEXTUNCOVGRID]        = MAIN_COLOR_NEXTUNCOVGRID_B;
  ColorRed[MAIN_COLOR_CENMINPATH]            = MAIN_COLOR_CENMINPATH_R;
  ColorGreen[MAIN_COLOR_CENMINPATH]          = MAIN_COLOR_CENMINPATH_G;
  ColorBlue[MAIN_COLOR_CENMINPATH]           = MAIN_COLOR_CENMINPATH_B;
  ColorRed[MAIN_COLOR_CENMAXPATH]            = MAIN_COLOR_CENMAXPATH_R;
  ColorGreen[MAIN_COLOR_CENMAXPATH]          = MAIN_COLOR_CENMAXPATH_G;
  ColorBlue[MAIN_COLOR_CENMAXPATH]           = MAIN_COLOR_CENMAXPATH_B;
  ColorRed[MAIN_COLOR_LOCMINPATH]            = MAIN_COLOR_LOCMINPATH_R;
  ColorGreen[MAIN_COLOR_LOCMINPATH]          = MAIN_COLOR_LOCMINPATH_G;
  ColorBlue[MAIN_COLOR_LOCMINPATH]           = MAIN_COLOR_LOCMINPATH_B;
  ColorRed[MAIN_COLOR_LOCMAXPATH]            = MAIN_COLOR_LOCMAXPATH_R;
  ColorGreen[MAIN_COLOR_LOCMAXPATH]          = MAIN_COLOR_LOCMAXPATH_G;
  ColorBlue[MAIN_COLOR_LOCMAXPATH]           = MAIN_COLOR_LOCMAXPATH_B;
} // myGlutInit



void myGlutKeyboard(unsigned char Key, int X, int Y)
/*
  myGlutKeyboard() is currently unused.
*/
{ // myGlutKeyboard
} // myGlutKeyboard



void myGlutMenu(int Value)
/*
  myGlutMenu() is currently unused.
*/
{ // myGlutMenu
} // myGlutMenu



void myGlutIdle(void)
/*
  myGlutIdle() is currently unused.
*/
{ // myGlutIdle
} // myGlutIdle



void myGlutMouse(int Button, int ButtonState, int X, int Y)
/*
  myGlutMouse() is currently unused.
*/
{ // myGlutMouse
} // myGlutMouse



void myGlutMotion(int X, int Y)
/*
  myGlutMotion() is currently unused.
*/
{ // myGlutMotion
} // myGlutMotion



void myGlutReshape(int X, int Y)
/*
  myGlutReshape() is called upon reshaping the window.  The camera
  is positioned such that a 1x1 square is centered in the display area
  and is as large as possible.
*/
{ // myGlutReshape
	double AspectRatio = 1.0; // Aspect Ratio
  double Z1 = 0.0;          // Z Value if Wider
  double Z2 = 0.0;          // Z Value if Taller
  double Multiplier = 1.1;  // Used so the 1x1 Area has a slight border around it

  if ((X > 0) && (Y > 0))
  {
    // Create the viewport transformation
    glViewport(0, 0, X, Y);
    
    // Create a default viewing frustum
    AspectRatio = ((double) X) / ((double) Y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(MAIN_HALFANGLE * 2.0, AspectRatio, MAIN_NEARPLANE, MAIN_FARPLANE);

    // This should translate the viewing area to be from 0 to 1
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if (AspectRatio > 1.0)
    {
      glTranslatef((GLfloat) -0.5, (GLfloat) -0.5, (GLfloat) -0.5 / tan(MAIN_HALFANGLE * MAIN_TORADIANS));
    }
    else
    {
      glTranslatef((GLfloat) -0.5, (GLfloat) -0.5, (GLfloat) -0.5 / AspectRatio / tan(MAIN_HALFANGLE * MAIN_TORADIANS));
    }
  }
} // myGlutReshape



void myGlutDisplay(void)
/*
  myGlutDisplay() draws the sensor network.
*/
{ // myGlutDisplay
  int i = 0;                                // Index Variable
  int j = 0;                                // Index Variable
  int NumNeighbors = 0;                     // Number of Neighbors of a Sensor
  int NeighborIndex = 0;                    // Neighbor Index
  int MaxIndex = 0;                         // Maximum Index (i.e. Number of Sensors)
  int GridH = SensorNetwork.GetGridSizeH(); // Horizontal Grid Size
  int GridV = SensorNetwork.GetGridSizeV(); // Vertical Grid Size
  int PathColor = 0;                        // Color of Path to Draw
  double ExposureVal = 0.0;                 // Exposure of Grid Square
  double X1 = 0.0;                          // Temporary X Coordinate
  double Y1 = 0.0;                          // Temporary Y Coordinate
  double X2 = 0.0;                          // Temporary X Coordinate
  double Y2 = 0.0;                          // Temporary Y Coordinate
  double X3 = 0.0;                          // Temporary X Coordinate
  double Y3 = 0.0;                          // Temporary Y Coordinate
  double X4 = 0.0;                          // Temporary X Coordinate
  double Y4 = 0.0;                          // Temporary Y Coordinate

  // The Following Are Used Only For Drawing the Next Sensor Radius Outline
  int k = 0;                                                           // Index Variable
  int ijAdded = 0;                                                     // Did We Add This (i, j) Grid Location
  int iNext = 0;                                                       // Next i Coordinate
  int jNext = 0;                                                       // Next j Coordinate
  int iOffset[4] = { 1, -1, 0, 0 };                                    // i Offsets for Next Coordinates
  int jOffset[4] = { 0, 0, 1, -1 };                                    // j Offsets for Next Coordinates
  double D = 0.0;                                                      // Temporary Distance
  double DNext = 0.0;                                                  // Distance of Neighbor
  double L = 0.0;                                                      // Temporary Length
  double LNext = 0.0;                                                  // Length of Neighbor
  double X = 0.0;                                                      // Temporary X Value
  double Y = 0.0;                                                      // Temporary Y Value
  double Z = 0.0;                                                      // Temporary Z Value
  double GX = 0.0;                                                     // Grid Temporary X Value
  double GY = 0.0;                                                     // Grid Temporary Y Value
  double GZ = 0.0;                                                     // Grid Temporary Z Value
  double SensorX = SensorNetwork.GetX(SensorNetwork.GetSensorNext());  // Sensor X Value
  double SensorY = SensorNetwork.GetY(SensorNetwork.GetSensorNext());  // Sensor Y Value
  double LSensor = Distance2D(SensorX, SensorY, GLUIWindow_StartingPointX, GLUIWindow_StartingPointY) + Distance2D(SensorX, SensorY, GLUIWindow_EndingPointX, GLUIWindow_EndingPointY);

  // Erase the Background
  glClearColor((GLfloat) ColorRed[MAIN_COLOR_BKG], (GLfloat) ColorGreen[MAIN_COLOR_BKG], (GLfloat) ColorBlue[MAIN_COLOR_BKG], 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Draw Edges
  if (DrawObject[MAIN_COLOR_EDGE])
  {
    MaxIndex = SensorNetwork.GetNumSensors();
    for (i = 0; i < MaxIndex; ++i)
    {
      NumNeighbors = SensorNetwork.GetNumNeighbors(i);
      for (j = 0; j < NumNeighbors; ++j)
      {
        NeighborIndex = SensorNetwork.GetNeighbor(i, j);
        DrawCylinder(SensorNetwork.GetX(i), SensorNetwork.GetY(i), SensorNetwork.GetX(NeighborIndex), SensorNetwork.GetY(NeighborIndex), 0.0, SizeObject[MAIN_COLOR_EDGE], ColorRed[MAIN_COLOR_EDGE], ColorGreen[MAIN_COLOR_EDGE], ColorBlue[MAIN_COLOR_EDGE]);
      }
    }
  }

  // Draw Grid Stuff
  if ((DrawObject[MAIN_COLOR_GRIDPOINTS]) || (DrawObject[MAIN_COLOR_GRIDEDGES]) || (DrawObject[MAIN_COLOR_COVGRID]) || (DrawObject[MAIN_COLOR_UNCOVGRID]) || (DrawObject[MAIN_COLOR_NEXTCOVGRID]) || (DrawObject[MAIN_COLOR_NEXTUNCOVGRID]))
  {
    for (i = 0; i < GridH; ++i)
    {
      for (j = 0; j < GridV; ++j)
      {
        // Grid Edges
        if (DrawObject[MAIN_COLOR_GRIDEDGES])
        {
          X1 = SensorNetwork.GetGridPointX(i, j, GVGRIDPOINT2D_TOPLEFT);
          Y1 = SensorNetwork.GetGridPointY(i, j, GVGRIDPOINT2D_TOPLEFT);
          X2 = SensorNetwork.GetGridPointX(i, j, GVGRIDPOINT2D_TOPRIGHT);
          Y2 = SensorNetwork.GetGridPointY(i, j, GVGRIDPOINT2D_TOPRIGHT);
          X3 = SensorNetwork.GetGridPointX(i, j, GVGRIDPOINT2D_BOTTOMLEFT);
          Y3 = SensorNetwork.GetGridPointY(i, j, GVGRIDPOINT2D_BOTTOMLEFT);
          DrawCylinder(X1, Y1, X2, Y2, 0.0, SizeObject[MAIN_COLOR_GRIDEDGES], ColorRed[MAIN_COLOR_GRIDEDGES], ColorGreen[MAIN_COLOR_GRIDEDGES], ColorBlue[MAIN_COLOR_GRIDEDGES]);
          DrawCylinder(X1, Y1, X3, Y3, 0.0, SizeObject[MAIN_COLOR_GRIDEDGES], ColorRed[MAIN_COLOR_GRIDEDGES], ColorGreen[MAIN_COLOR_GRIDEDGES], ColorBlue[MAIN_COLOR_GRIDEDGES]);
        }

        // Grid Points
        if (DrawObject[MAIN_COLOR_GRIDPOINTS]) DrawSphere(SensorNetwork.GetGridPointX(i, j, GVGRIDPOINT2D_TOPLEFT), SensorNetwork.GetGridPointY(i, j, GVGRIDPOINT2D_TOPLEFT), 0.0, SizeObject[MAIN_COLOR_GRIDPOINTS], ColorRed[MAIN_COLOR_GRIDPOINTS], ColorGreen[MAIN_COLOR_GRIDPOINTS], ColorBlue[MAIN_COLOR_GRIDPOINTS]);

        ijAdded = 0;
        if ((DrawObject[MAIN_COLOR_NEXTCOVGRID]) || (DrawObject[MAIN_COLOR_NEXTUNCOVGRID]))
        {
          X = SensorNetwork.GetGridPointX(i, j, GVGRIDPOINT2D_CENTERPOINT);
          Y = SensorNetwork.GetGridPointY(i, j, GVGRIDPOINT2D_CENTERPOINT);
          L = Distance2D(X, Y, GLUIWindow_StartingPointX, GLUIWindow_StartingPointY) + Distance2D(X, Y, GLUIWindow_EndingPointX, GLUIWindow_EndingPointY);
          if (L <= GLUIWindow_MaximumLength)
          {
            D = Distance2D(X, Y, SensorX, SensorY);
            for (k = 0; (!ijAdded) && (k < 4); ++k)
            {
              iNext = i + iOffset[k];
              jNext = j + jOffset[k];
              if ((iNext >= 0) && (iNext < GridH) && (jNext >= 0) && (jNext < GridV))
              {
                GX = SensorNetwork.GetGridPointX(iNext, jNext, GVGRIDPOINT2D_CENTERPOINT);
                GY = SensorNetwork.GetGridPointY(iNext, jNext, GVGRIDPOINT2D_CENTERPOINT);
                DNext = Distance2D(GX, GY, SensorX, SensorY);
                LNext = Distance2D(GX, GY, GLUIWindow_StartingPointX, GLUIWindow_StartingPointY) + Distance2D(GX, GY, GLUIWindow_EndingPointX, GLUIWindow_EndingPointY);
                if ((D > GLUIWindow_CommunicationRadius) && (DNext <= GLUIWindow_CommunicationRadius) && (L < GLUIWindow_MaximumLength))
                {
                  if (SensorNetwork.GetGridCovered(i, j))
                  {
                    if ((GLUIWindow_AlgorithmType) && (DrawObject[MAIN_COLOR_NEXTCOVGRID]))
                    {
                      // DrawSphere(X, Y, Z, SizeObject[MAIN_COLOR_NEXTCOVGRID], ColorRed[MAIN_COLOR_NEXTCOVGRID], ColorGreen[MAIN_COLOR_NEXTCOVGRID], ColorBlue[MAIN_COLOR_NEXTCOVGRID]);
                      X1 = SensorNetwork.GetGridPointX(i, j, GVGRIDPOINT2D_TOPLEFT);
                      Y1 = SensorNetwork.GetGridPointY(i, j, GVGRIDPOINT2D_TOPLEFT);
                      X2 = SensorNetwork.GetGridPointX(i, j, GVGRIDPOINT2D_TOPRIGHT);
                      Y2 = SensorNetwork.GetGridPointY(i, j, GVGRIDPOINT2D_TOPRIGHT);
                      X3 = SensorNetwork.GetGridPointX(i, j, GVGRIDPOINT2D_BOTTOMRIGHT);
                      Y3 = SensorNetwork.GetGridPointY(i, j, GVGRIDPOINT2D_BOTTOMRIGHT);
                      X4 = SensorNetwork.GetGridPointX(i, j, GVGRIDPOINT2D_BOTTOMLEFT);
                      Y4 = SensorNetwork.GetGridPointY(i, j, GVGRIDPOINT2D_BOTTOMLEFT);
                      DrawQuad(X1, Y1, 0.0, X2, Y2, 0.0, X3, Y3, 0.0, X4, Y4, 0.0, ColorRed[MAIN_COLOR_NEXTCOVGRID], ColorGreen[MAIN_COLOR_NEXTCOVGRID], ColorBlue[MAIN_COLOR_NEXTCOVGRID]);
                      ijAdded = 1;
                    }
                  }
                  else
                  {
                    if ((GLUIWindow_AlgorithmType) && (DrawObject[MAIN_COLOR_NEXTUNCOVGRID]))
                    {
                      // DrawSphere(X, Y, Z, SizeObject[MAIN_COLOR_NEXTUNCOVGRID], ColorRed[MAIN_COLOR_NEXTUNCOVGRID], ColorGreen[MAIN_COLOR_NEXTUNCOVGRID], ColorBlue[MAIN_COLOR_NEXTUNCOVGRID]);
                      X1 = SensorNetwork.GetGridPointX(i, j, GVGRIDPOINT2D_TOPLEFT);
                      Y1 = SensorNetwork.GetGridPointY(i, j, GVGRIDPOINT2D_TOPLEFT);
                      X2 = SensorNetwork.GetGridPointX(i, j, GVGRIDPOINT2D_TOPRIGHT);
                      Y2 = SensorNetwork.GetGridPointY(i, j, GVGRIDPOINT2D_TOPRIGHT);
                      X3 = SensorNetwork.GetGridPointX(i, j, GVGRIDPOINT2D_BOTTOMRIGHT);
                      Y3 = SensorNetwork.GetGridPointY(i, j, GVGRIDPOINT2D_BOTTOMRIGHT);
                      X4 = SensorNetwork.GetGridPointX(i, j, GVGRIDPOINT2D_BOTTOMLEFT);
                      Y4 = SensorNetwork.GetGridPointY(i, j, GVGRIDPOINT2D_BOTTOMLEFT);
                      DrawQuad(X1, Y1, 0.0, X2, Y2, 0.0, X3, Y3, 0.0, X4, Y4, 0.0, ColorRed[MAIN_COLOR_NEXTUNCOVGRID], ColorGreen[MAIN_COLOR_NEXTUNCOVGRID], ColorBlue[MAIN_COLOR_NEXTUNCOVGRID]);
                      ijAdded = 1;
                    }
                  }
                }
                else if ((LSensor <= GLUIWindow_MaximumLength) && (D <= GLUIWindow_CommunicationRadius) && (LNext > GLUIWindow_MaximumLength))
                {
                  if ((GLUIWindow_AlgorithmType) && (DrawObject[MAIN_COLOR_NEXTCOVGRID]))
                  {
                    // DrawSphere(X, Y, Z, SizeObject[MAIN_COLOR_NEXTCOVGRID], ColorRed[MAIN_COLOR_NEXTCOVGRID], ColorGreen[MAIN_COLOR_NEXTCOVGRID], ColorBlue[MAIN_COLOR_NEXTCOVGRID]);
                    X1 = SensorNetwork.GetGridPointX(i, j, GVGRIDPOINT2D_TOPLEFT);
                    Y1 = SensorNetwork.GetGridPointY(i, j, GVGRIDPOINT2D_TOPLEFT);
                    X2 = SensorNetwork.GetGridPointX(i, j, GVGRIDPOINT2D_TOPRIGHT);
                    Y2 = SensorNetwork.GetGridPointY(i, j, GVGRIDPOINT2D_TOPRIGHT);
                    X3 = SensorNetwork.GetGridPointX(i, j, GVGRIDPOINT2D_BOTTOMRIGHT);
                    Y3 = SensorNetwork.GetGridPointY(i, j, GVGRIDPOINT2D_BOTTOMRIGHT);
                    X4 = SensorNetwork.GetGridPointX(i, j, GVGRIDPOINT2D_BOTTOMLEFT);
                    Y4 = SensorNetwork.GetGridPointY(i, j, GVGRIDPOINT2D_BOTTOMLEFT);
                    DrawQuad(X1, Y1, 0.0, X2, Y2, 0.0, X3, Y3, 0.0, X4, Y4, 0.0, ColorRed[MAIN_COLOR_NEXTCOVGRID], ColorGreen[MAIN_COLOR_NEXTCOVGRID], ColorBlue[MAIN_COLOR_NEXTCOVGRID]);
                    ijAdded = 1;
                  }
                }
              }
            }
          }
        }

        // Draw Covered/UnCovered Areas
        if (!ijAdded)
        {
          switch(SensorNetwork.GetGridCovered(i, j))
          {
            case 1:
              if (DrawObject[MAIN_COLOR_COVGRID])
              {
                X1 = SensorNetwork.GetGridPointX(i, j, GVGRIDPOINT2D_TOPLEFT);
                Y1 = SensorNetwork.GetGridPointY(i, j, GVGRIDPOINT2D_TOPLEFT);
                X2 = SensorNetwork.GetGridPointX(i, j, GVGRIDPOINT2D_TOPRIGHT);
                Y2 = SensorNetwork.GetGridPointY(i, j, GVGRIDPOINT2D_TOPRIGHT);
                X3 = SensorNetwork.GetGridPointX(i, j, GVGRIDPOINT2D_BOTTOMRIGHT);
                Y3 = SensorNetwork.GetGridPointY(i, j, GVGRIDPOINT2D_BOTTOMRIGHT);
                X4 = SensorNetwork.GetGridPointX(i, j, GVGRIDPOINT2D_BOTTOMLEFT);
                Y4 = SensorNetwork.GetGridPointY(i, j, GVGRIDPOINT2D_BOTTOMLEFT);
                if (GLUIWindow_OverlayExposure)
                {
                  ExposureVal = SensorNetwork.GetGridNormalizedExposureValue(i, j);
                  DrawQuad(X1, Y1, 0.0, X2, Y2, 0.0, X3, Y3, 0.0, X4, Y4, 0.0, ExposureVal * ColorRed[MAIN_COLOR_COVGRID], ExposureVal * ColorGreen[MAIN_COLOR_COVGRID], ExposureVal * ColorBlue[MAIN_COLOR_COVGRID]);
                }
                else
                {
                  DrawQuad(X1, Y1, 0.0, X2, Y2, 0.0, X3, Y3, 0.0, X4, Y4, 0.0, ColorRed[MAIN_COLOR_COVGRID], ColorGreen[MAIN_COLOR_COVGRID], ColorBlue[MAIN_COLOR_COVGRID]);
                }
              }
            break;

            case 0:
            default:
              if (DrawObject[MAIN_COLOR_UNCOVGRID])
              {
                X1 = SensorNetwork.GetGridPointX(i, j, GVGRIDPOINT2D_TOPLEFT);
                Y1 = SensorNetwork.GetGridPointY(i, j, GVGRIDPOINT2D_TOPLEFT);
                X2 = SensorNetwork.GetGridPointX(i, j, GVGRIDPOINT2D_TOPRIGHT);
                Y2 = SensorNetwork.GetGridPointY(i, j, GVGRIDPOINT2D_TOPRIGHT);
                X3 = SensorNetwork.GetGridPointX(i, j, GVGRIDPOINT2D_BOTTOMRIGHT);
                Y3 = SensorNetwork.GetGridPointY(i, j, GVGRIDPOINT2D_BOTTOMRIGHT);
                X4 = SensorNetwork.GetGridPointX(i, j, GVGRIDPOINT2D_BOTTOMLEFT);
                Y4 = SensorNetwork.GetGridPointY(i, j, GVGRIDPOINT2D_BOTTOMLEFT);
                if (GLUIWindow_OverlayExposure)
                {
                  ExposureVal = SensorNetwork.GetGridNormalizedExposureValue(i, j);
                  DrawQuad(X1, Y1, 0.0, X2, Y2, 0.0, X3, Y3, 0.0, X4, Y4, 0.0, ExposureVal * ColorRed[MAIN_COLOR_UNCOVGRID], ExposureVal * ColorGreen[MAIN_COLOR_UNCOVGRID], ExposureVal * ColorBlue[MAIN_COLOR_UNCOVGRID]);
                }
                else
                {
                  DrawQuad(X1, Y1, 0.0, X2, Y2, 0.0, X3, Y3, 0.0, X4, Y4, 0.0, ColorRed[MAIN_COLOR_UNCOVGRID], ColorGreen[MAIN_COLOR_UNCOVGRID], ColorBlue[MAIN_COLOR_UNCOVGRID]);
                }
              }
            break;
          }
        }
      }
    }
    // Draw "Leftover" Edges and Grid Points
    if ((DrawObject[MAIN_COLOR_GRIDPOINTS]) || (DrawObject[MAIN_COLOR_GRIDEDGES]))
    {
      for (i = 0, j = GridV - 1; i < GridH; ++i)
      {
        X1 = SensorNetwork.GetGridPointX(i, j, GVGRIDPOINT2D_BOTTOMLEFT);
        Y1 = SensorNetwork.GetGridPointY(i, j, GVGRIDPOINT2D_BOTTOMLEFT);
        if (DrawObject[MAIN_COLOR_GRIDEDGES])
        {
          X2 = SensorNetwork.GetGridPointX(i, j, GVGRIDPOINT2D_BOTTOMRIGHT);
          Y2 = SensorNetwork.GetGridPointY(i, j, GVGRIDPOINT2D_BOTTOMRIGHT);
          DrawCylinder(X1, Y1, X2, Y2, 0.0, SizeObject[MAIN_COLOR_GRIDEDGES], ColorRed[MAIN_COLOR_GRIDEDGES], ColorGreen[MAIN_COLOR_GRIDEDGES], ColorBlue[MAIN_COLOR_GRIDEDGES]);
        }
        if (DrawObject[MAIN_COLOR_GRIDPOINTS])
        {
          DrawSphere(X1, Y1, 0.0, SizeObject[MAIN_COLOR_GRIDPOINTS], ColorRed[MAIN_COLOR_GRIDPOINTS], ColorGreen[MAIN_COLOR_GRIDPOINTS], ColorBlue[MAIN_COLOR_GRIDPOINTS]);
        }
      }
      for (i = GridH - 1, j = 0; j < GridV; ++j)
      {
        X1 = SensorNetwork.GetGridPointX(i, j, GVGRIDPOINT2D_TOPRIGHT);
        Y1 = SensorNetwork.GetGridPointY(i, j, GVGRIDPOINT2D_TOPRIGHT);
        if (DrawObject[MAIN_COLOR_GRIDEDGES])
        {
          X2 = SensorNetwork.GetGridPointX(i, j, GVGRIDPOINT2D_BOTTOMRIGHT);
          Y2 = SensorNetwork.GetGridPointY(i, j, GVGRIDPOINT2D_BOTTOMRIGHT);
          DrawCylinder(X1, Y1, X2, Y2, 0.0, SizeObject[MAIN_COLOR_GRIDEDGES], ColorRed[MAIN_COLOR_GRIDEDGES], ColorGreen[MAIN_COLOR_GRIDEDGES], ColorBlue[MAIN_COLOR_GRIDEDGES]);
        }
        if (DrawObject[MAIN_COLOR_GRIDPOINTS])
        {
          DrawSphere(X1, Y1, 0.0, SizeObject[MAIN_COLOR_GRIDPOINTS], ColorRed[MAIN_COLOR_GRIDPOINTS], ColorGreen[MAIN_COLOR_GRIDPOINTS], ColorBlue[MAIN_COLOR_GRIDPOINTS]);
        }
      }
      if (DrawObject[MAIN_COLOR_GRIDPOINTS])
      {
        i = GridH - 1;
        j = GridV - 1;
        DrawSphere(SensorNetwork.GetGridPointX(i, j, GVGRIDPOINT2D_BOTTOMRIGHT), SensorNetwork.GetGridPointY(i, j, GVGRIDPOINT2D_BOTTOMRIGHT), 0.0, SizeObject[MAIN_COLOR_GRIDPOINTS], ColorRed[MAIN_COLOR_GRIDPOINTS], ColorGreen[MAIN_COLOR_GRIDPOINTS], ColorBlue[MAIN_COLOR_GRIDPOINTS]);
      }
    }
  }

  // Draw Ellipse
  if ((GLUIWindow_AlgorithmType) && (DrawObject[MAIN_COLOR_ELLIPSE]))
  {
    DrawEllipse(GLUIWindow_StartingPointX, GLUIWindow_StartingPointY, GLUIWindow_EndingPointX, GLUIWindow_EndingPointY, 0.0, GLUIWindow_MaximumLength, SizeObject[MAIN_COLOR_ELLIPSE], ColorRed[MAIN_COLOR_ELLIPSE], ColorGreen[MAIN_COLOR_ELLIPSE], ColorBlue[MAIN_COLOR_ELLIPSE]);
  }

  // Draw End Points
  if (DrawObject[MAIN_COLOR_ENDPOINTS])
  {
    DrawSphere(GLUIWindow_StartingPointX, GLUIWindow_StartingPointY, 0.0, SizeObject[MAIN_COLOR_ENDPOINTS], ColorRed[MAIN_COLOR_ENDPOINTS], ColorGreen[MAIN_COLOR_ENDPOINTS], ColorBlue[MAIN_COLOR_ENDPOINTS]);
    DrawSphere(GLUIWindow_EndingPointX, GLUIWindow_EndingPointY, 0.0, SizeObject[MAIN_COLOR_ENDPOINTS], ColorRed[MAIN_COLOR_ENDPOINTS], ColorGreen[MAIN_COLOR_ENDPOINTS], ColorBlue[MAIN_COLOR_ENDPOINTS]);
  }

  // Draw Path Points
  for (i = GVEXPOSURESENSORNETWORK2D_PATH_CENMIN; i < GVEXPOSURESENSORNETWORK2D_PATH_NUM; ++i)
  {
    PathColor = MAIN_COLOR_CENMINPATH + i;
    if (DrawObject[PathColor])
    {
      MaxIndex = SensorNetwork.GetNumPathPoints(i) - 1;
      for (j = 0; j < MaxIndex; ++j)
      {
        NeighborIndex = j + 1;
        X1 = SensorNetwork.GetPathPointX(i, j);
        Y1 = SensorNetwork.GetPathPointY(i, j);
        X2 = SensorNetwork.GetPathPointX(i, NeighborIndex);
        Y2 = SensorNetwork.GetPathPointY(i, NeighborIndex);
        DrawCylinder(X1, Y1, X2, Y2, 0.0, SizeObject[PathColor], ColorRed[PathColor], ColorGreen[PathColor], ColorBlue[PathColor]);
      }
    }
  }

  // Draw Sensors
  MaxIndex = SensorNetwork.GetNumSensors();
  if ((DrawObject[MAIN_COLOR_SENSOR_UNKNOWN]) || (DrawObject[MAIN_COLOR_SENSOR_KNOWN]) || (DrawObject[MAIN_COLOR_SENSOR_VISITED]) || (DrawObject[MAIN_COLOR_SENSOR_USELESS]) || (DrawObject[MAIN_COLOR_SENSOR_SCURRENT]) || (DrawObject[MAIN_COLOR_SENSOR_FCURRENT]))
  {
    for (i = 0; i < MaxIndex; ++i)
    {
      if (i == SensorNetwork.GetSensorCurrent())
      {
        switch(SensorNetwork.GetMessageMode())
        {
          case GVEXPOSURESENSORNETWORK2D_MESSAGEMODE_SEARCH:
            if (DrawObject[MAIN_COLOR_SENSOR_SCURRENT]) DrawSphere(SensorNetwork.GetX(i), SensorNetwork.GetY(i), 0.0, SizeObject[MAIN_COLOR_SENSOR_SCURRENT], ColorRed[MAIN_COLOR_SENSOR_SCURRENT], ColorGreen[MAIN_COLOR_SENSOR_SCURRENT], ColorBlue[MAIN_COLOR_SENSOR_SCURRENT]);
          break;

          case GVEXPOSURESENSORNETWORK2D_MESSAGEMODE_FORWARD:
            if (DrawObject[MAIN_COLOR_SENSOR_FCURRENT]) DrawSphere(SensorNetwork.GetX(i), SensorNetwork.GetY(i), 0.0, SizeObject[MAIN_COLOR_SENSOR_FCURRENT], ColorRed[MAIN_COLOR_SENSOR_FCURRENT], ColorGreen[MAIN_COLOR_SENSOR_FCURRENT], ColorBlue[MAIN_COLOR_SENSOR_FCURRENT]);
          break;
        }
      }
      else
      {
        switch(SensorNetwork.GetSensorKnown(i))
        {
          case GVEXPOSURESENSORNETWORK2D_SENSOR_UNKNOWN:
            if (DrawObject[MAIN_COLOR_SENSOR_UNKNOWN]) DrawSphere(SensorNetwork.GetX(i), SensorNetwork.GetY(i), 0.0, SizeObject[MAIN_COLOR_SENSOR_UNKNOWN], ColorRed[MAIN_COLOR_SENSOR_UNKNOWN], ColorGreen[MAIN_COLOR_SENSOR_UNKNOWN], ColorBlue[MAIN_COLOR_SENSOR_UNKNOWN]);
          break;

          case GVEXPOSURESENSORNETWORK2D_SENSOR_USELESS:
            if (DrawObject[MAIN_COLOR_SENSOR_USELESS]) DrawSphere(SensorNetwork.GetX(i), SensorNetwork.GetY(i), 0.0, SizeObject[MAIN_COLOR_SENSOR_USELESS], ColorRed[MAIN_COLOR_SENSOR_USELESS], ColorGreen[MAIN_COLOR_SENSOR_USELESS], ColorBlue[MAIN_COLOR_SENSOR_USELESS]);
          break;

          case GVEXPOSURESENSORNETWORK2D_SENSOR_KNOWN:
            if (DrawObject[MAIN_COLOR_SENSOR_KNOWN]) DrawSphere(SensorNetwork.GetX(i), SensorNetwork.GetY(i), 0.0, SizeObject[MAIN_COLOR_SENSOR_KNOWN], ColorRed[MAIN_COLOR_SENSOR_KNOWN], ColorGreen[MAIN_COLOR_SENSOR_KNOWN], ColorBlue[MAIN_COLOR_SENSOR_KNOWN]);
          break;

          case GVEXPOSURESENSORNETWORK2D_SENSOR_VISITED:
            if (DrawObject[MAIN_COLOR_SENSOR_VISITED]) DrawSphere(SensorNetwork.GetX(i), SensorNetwork.GetY(i), 0.0, SizeObject[MAIN_COLOR_SENSOR_VISITED], ColorRed[MAIN_COLOR_SENSOR_VISITED], ColorGreen[MAIN_COLOR_SENSOR_VISITED], ColorBlue[MAIN_COLOR_SENSOR_VISITED]);
          break;
        }
      }
    }
  }

  glFlush();
  glutSwapBuffers();
} // myGlutDisplay



void ControlCallback(int ControlID)
/*
  ControlCallBack() is used to call a function when an event occurs.
*/
{ // ControlCallback
  switch(ControlID)
  {
    case MAIN_CALLBACK_QUIT:
      exit(0);
    break;

    case MAIN_CALLBACK_NEW:
      OnNew();
    break;

    case MAIN_CALLBACK_NUMSENSORS:
      OnNumSensors();
    break;

    case MAIN_CALLBACK_RADIUS:
      OnRadius();
    break;

    case MAIN_CALLBACK_COLOR:
    case MAIN_CALLBACK_RED:
    case MAIN_CALLBACK_GREEN:
    case MAIN_CALLBACK_BLUE:
    case MAIN_CALLBACK_SIZE:
    case MAIN_CALLBACK_DRAW:
      OnColor();
    break;

    case MAIN_CALLBACK_STARTX:
    case MAIN_CALLBACK_STARTY:
    case MAIN_CALLBACK_ENDX:
    case MAIN_CALLBACK_ENDY:
    case MAIN_CALLBACK_MAXLENGTH:
      OnChangeEllipse();
    break;

    case MAIN_CALLBACK_GRIDH:
    case MAIN_CALLBACK_GRIDV:
      OnChangeGrid();
    break;

    case MAIN_CALLBACK_RUN:
      OnRun();
    break;

    case MAIN_CALLBACK_EXPERIMENT:
      OnExperiment();
    break;

    case MAIN_CALLBACK_EXPOSURE:
      OnExposure();
    break;

    case MAIN_CALLBACK_CENMIN:
      OnChangeCenMinOptions();
    break;

    case MAIN_CALLBACK_CENMAX:
      OnChangeCenMaxOptions();
    break;

    case MAIN_CALLBACK_LOCMIN:
      OnChangeLocMinOptions();
    break;

    case MAIN_CALLBACK_LOCMAX:
      OnChangeLocMaxOptions();
    break;

    case MAIN_CALLBACK_ALGMODE:
      OnChangeMode();
    break;

    case MAIN_CALLBACK_ALGTYPE:
      OnChangeType();
    break;

    default:
    break;
  }
} // ControlCallback



void OnColor(void)
/*
  OnColor() is called when one of the color text boxes is modified. It
  stores the color and loads the next color.
*/
{ // OnColor
  ColorRed[GLUIWindow_PreviousColorObject] = GLUIWindow_Red;
  ColorGreen[GLUIWindow_PreviousColorObject] = GLUIWindow_Green;
  ColorBlue[GLUIWindow_PreviousColorObject] = GLUIWindow_Blue;
  DrawObject[GLUIWindow_PreviousColorObject] = GLUIWindow_Draw;
  SizeObject[GLUIWindow_PreviousColorObject] = GLUIWindow_Size;

  GLUIWindow_Red = ColorRed[GLUIWindow_ColorObject];
  GLUIWindow_Green = ColorGreen[GLUIWindow_ColorObject];
  GLUIWindow_Blue = ColorBlue[GLUIWindow_ColorObject];
  GLUIWindow_Draw = DrawObject[GLUIWindow_ColorObject];
  GLUIWindow_Size = SizeObject[GLUIWindow_ColorObject];

  Glui->sync_live();
  GLUIWindow_PreviousColorObject = GLUIWindow_ColorObject;

  myGlutDisplay();
} // OnColor



void OnNew(void)
/*
  OnNew() is called when the New button is pressed.  It creates a new
  sensor network with specified number of sensors and communication radius.
*/
{ // OnNew
  SensorNetwork.New(GLUIWindow_NumberOfSensors, GLUIWindow_CommunicationRadius);
  OnExposure();
  OnChangeMode();
  OnChangeType();
  OnChangeCenMinOptions();
  OnChangeCenMaxOptions();
  OnChangeLocMinOptions();
  OnChangeLocMaxOptions();
  SensorNetwork.SetGridParameters(GLUIWindow_GridH, GLUIWindow_GridV);
  if (GLUIWindow_AlgorithmType) SensorNetwork.SetPathParameters(GLUIWindow_StartingPointX, GLUIWindow_StartingPointY, GLUIWindow_EndingPointX, GLUIWindow_EndingPointY, GLUIWindow_MaximumLength);
  else SensorNetwork.SetPathParameters(GLUIWindow_StartingPointX, GLUIWindow_StartingPointY, GLUIWindow_EndingPointX, GLUIWindow_EndingPointY);
} // OnNew



void OnRadius(void)
/*
  OnRadius() is called when the Communication Radius changes.
*/
{ // OnRadius
  SensorNetwork.SetRadius(GLUIWindow_CommunicationRadius);
} // OnRadius



void OnNumSensors(void)
/*
  OnNumSensors() is called when the number of sensors changes.
*/
{ // OnNumSensors
  SensorNetwork.New(GLUIWindow_NumberOfSensors, GLUIWindow_CommunicationRadius);
  OnExposure();
  OnChangeMode();
  OnChangeType();
  OnChangeCenMinOptions();
  OnChangeCenMaxOptions();
  OnChangeLocMinOptions();
  OnChangeLocMaxOptions();
  SensorNetwork.SetGridParameters(GLUIWindow_GridH, GLUIWindow_GridV);
  if (GLUIWindow_AlgorithmType) SensorNetwork.SetPathParameters(GLUIWindow_StartingPointX, GLUIWindow_StartingPointY, GLUIWindow_EndingPointX, GLUIWindow_EndingPointY, GLUIWindow_MaximumLength);
  else SensorNetwork.SetPathParameters(GLUIWindow_StartingPointX, GLUIWindow_StartingPointY, GLUIWindow_EndingPointX, GLUIWindow_EndingPointY);
} // OnNumSensors



void OnChangeEllipse(void)
/*
  OnChangeEllipse() is called when one of the ellipse parameters is changed.
*/
{ // OnChangeEllipse
  switch(GLUIWindow_AlgorithmType)
  {
    case MAIN_TYPE_MINIMAL:
      SensorNetwork.SetPathParameters(GLUIWindow_StartingPointX, GLUIWindow_StartingPointY, GLUIWindow_EndingPointX, GLUIWindow_EndingPointY);
    break;

    case MAIN_TYPE_MAXIMAL:
    default:
      SensorNetwork.SetPathParameters(GLUIWindow_StartingPointX, GLUIWindow_StartingPointY, GLUIWindow_EndingPointX, GLUIWindow_EndingPointY, GLUIWindow_MaximumLength);
      OnChangeGrid();
    break;
  }
} // OnChangeEllipse



void OnChangeGrid(void)
/*
  OnChangeGrid() is called when one of the grid parameters is changed.
*/
{ // OnChangeGrid
  SensorNetwork.SetGridParameters(GLUIWindow_GridH, GLUIWindow_GridV);
  OnExposure();
} // OnChangeGrid



void OnRun(void)
/*
  OnRun() occurs when the user presses the Run button.
*/
{ // OnRun
  int KeepGoing = 1; // Keep Running The Algorithm

  if (GLUIWindow_PauseTime < 0)
  {
    switch(GLUIWindow_AlgorithmType)
    {
      case MAIN_TYPE_MINIMAL:
        if (SensorNetwork.MinExposure_Run() == 0) SensorNetwork.OutputResults(GLUIWindow_FileName);
      break;

      case MAIN_TYPE_MAXIMAL:
      default:
        if (SensorNetwork.MaxExposure_Run() == 0) SensorNetwork.OutputResults(GLUIWindow_FileName);
      break;
    }
  }
  else
  {
    while(KeepGoing == 1)
    {
      switch(GLUIWindow_AlgorithmType)
      {
        case MAIN_TYPE_MINIMAL:
          KeepGoing = SensorNetwork.MinExposure_Run();
        break;

        case MAIN_TYPE_MAXIMAL:
        default:
          KeepGoing = SensorNetwork.MaxExposure_Run();
        break;
      }
      myGlutDisplay();
      Pause(GLUIWindow_PauseTime);
    }
    if (KeepGoing == 0)
    {
      SensorNetwork.OutputResults(GLUIWindow_FileName);
    }
  }
} // OnRun



void OnExposure(void)
/*
  OnExposure() is called when the Exposure Check box is changed.
*/
{ // OnExposure
  // if (GLUIWindow_OverlayExposure) SensorNetwork.CalculateGridExposure();
} // OnExposure



void OnChangeMode(void)
/*
  OnChangeMode() is called when the mode is switched from centralized to
  localized and vice versa.
*/
{ // OnChangeMode
  SensorNetwork.SetAlgorithmMode(GLUIWindow_AlgorithmMode);
} // OnChangeMode



void OnChangeType(void)
/*
  OnChangeType() is called when the type is switched from minimal to maximal
  and vice versa.
*/
{ // OnChangeType
  if (GLUIWindow_AlgorithmType) SensorNetwork.SetPathParameters(GLUIWindow_StartingPointX, GLUIWindow_StartingPointY, GLUIWindow_EndingPointX, GLUIWindow_EndingPointY, GLUIWindow_MaximumLength);
  else SensorNetwork.SetPathParameters(GLUIWindow_StartingPointX, GLUIWindow_StartingPointY, GLUIWindow_EndingPointX, GLUIWindow_EndingPointY);
} // OnChangeType



void OnChangeCenMinOptions(void)
/*
  OnChangeCenMinOptions() is called when the centralized minimal exposure path
  options are changed.
*/
{ // OnChangeCenMinOptions
  SensorNetwork.SetCenMinExposureMode(GLUIWindow_CenMinOptions);
} // OnChangeCenMinOptions



void OnChangeCenMaxOptions(void)
/*
  OnChangeCenMaxOptions() is called when the centralized maximal exposure path
  options are changed.
*/
{ // OnChangeCenMaxOptions
  SensorNetwork.SetCenMaxExposureMode(GLUIWindow_CenMaxOptions);
} // OnChangeCenMaxOptions



void OnChangeLocMinOptions(void)
/*
  OnChangeLocMinOptions() is called when the localized minimal exposure path
  options are changed.
*/
{ // OnChangeLocMinOptions
  SensorNetwork.SetLocMinExposureMode(GLUIWindow_LocMinOptions);
} // OnChangeLocMinOptions



void OnChangeLocMaxOptions(void)
/*
  OnChangeLocMaxOptions() is called when the localized maximal exposure path
  options are changed.  
*/
{ // OnChangeLocMaxOptions
  SensorNetwork.SetLocMaxExposureMode(GLUIWindow_LocMaxOptions);
} // OnChangeLocMaxOptions



void OnExperiment(void)
/*
  OnExperiment() is called when the Experiment button is pressed.
*/
{ // OnExperiment
  int KeepGoing = 1;                                        // Keep Trying Current Experiment
  int Iteration = 0;                                        // Current Iteration
  int NumSensors = 0;                                       // Num Sensors
  int NumSensors_Min = MAIN_EXPERIMENT_NUMSENSORS_MIN;      // Num Sensors Minimum
  int NumSensors_Max = MAIN_EXPERIMENT_NUMSENSORS_MAX;      // Num Sensors Maximum 
  int NumSensors_Delta = MAIN_EXPERIMENT_NUMSENSORS_DELTA;  // Num Sensors Delta
  int GridH = 0;                                            // Grid X
  int GridH_Min = MAIN_EXPERIMENT_GRIDX_MIN;                // Grid X Minimum
  int GridH_Max = MAIN_EXPERIMENT_GRIDX_MAX;                // Grid X Maximum
  int GridH_Delta = MAIN_EXPERIMENT_GRIDX_DELTA;            // Grid X
  int GridV = 0;                                            // Grid Y
  int GridV_Min = MAIN_EXPERIMENT_GRIDY_MIN;                // Grid Y Minimum
  int GridV_Max = MAIN_EXPERIMENT_GRIDY_MAX;                // Grid Y Maximum
  int GridV_Delta = MAIN_EXPERIMENT_GRIDY_DELTA;            // Grid Y Delta
  double Radius = 0.0;                                      // Radius
  double Radius_Min = MAIN_EXPERIMENT_RADIUS_MIN;           // Radius Minimum
  double Radius_Max = MAIN_EXPERIMENT_RADIUS_MAX;           // Radius Maximum
  double Radius_Delta = MAIN_EXPERIMENT_RADIUS_DELTA;       // Radius Delta
  double StartX = 0.0;                                      // StartX
  double StartX_Min = MAIN_EXPERIMENT_STARTX_MIN;           // StartX Minimum
  double StartX_Max = MAIN_EXPERIMENT_STARTX_MAX;           // StartX Maximum
  double StartX_Delta = MAIN_EXPERIMENT_STARTX_DELTA;       // StartX Delta
  double StartY = 0.0;                                      // StartY
  double StartY_Min = MAIN_EXPERIMENT_STARTY_MIN;           // StartY Minimum
  double StartY_Max = MAIN_EXPERIMENT_STARTY_MAX;           // StartY Maximum
  double StartY_Delta = MAIN_EXPERIMENT_STARTY_DELTA;       // StartY Delta
  double EndX = 0.0;                                        // EndX
  double EndX_Min = MAIN_EXPERIMENT_ENDX_MIN;               // EndX Minimum
  double EndX_Max = MAIN_EXPERIMENT_ENDX_MAX;               // EndX Maximum
  double EndX_Delta = MAIN_EXPERIMENT_ENDX_DELTA;           // EndX Delta
  double EndY = 0.0;                                        // EndY
  double EndY_Min = MAIN_EXPERIMENT_ENDY_MIN;               // EndY Minimum
  double EndY_Max = MAIN_EXPERIMENT_ENDY_MAX;               // EndY Maximum
  double EndY_Delta = MAIN_EXPERIMENT_ENDY_DELTA;           // EndY Delta
  double MaxLength = 0.0;                                   // MaxLength
  double MaxLength_Min = MAIN_EXPERIMENT_MAXLENGTH_MIN;     // MaxLength Minimum
  double MaxLength_Max = MAIN_EXPERIMENT_MAXLENGTH_MAX;     // MaxLength Maximum
  double MaxLength_Delta = MAIN_EXPERIMENT_MAXLENGTH_DELTA; // MaxLength Delta

  OnChangeMode();
  OnChangeType();
  OnChangeCenMinOptions();
  OnChangeCenMaxOptions();
  OnChangeLocMinOptions();
  OnChangeLocMaxOptions();
  for (NumSensors = NumSensors_Min; NumSensors <= NumSensors_Max; NumSensors += NumSensors_Delta)
  {
    for (Radius = Radius_Min; Radius <= Radius_Max; Radius += Radius_Delta)
    {
      for (GridH = GridH_Min; GridH <= GridH_Max; GridH += GridH_Delta)
      {
        for (GridV = GridV_Min; GridV <= GridV_Max; GridV += GridV_Delta)
        {
          for (StartX = StartX_Min; StartX <= StartX_Max; StartX += StartX_Delta)
          {
            for (StartY = StartY_Min; StartY <= StartY_Max; StartY += StartY_Delta)
            {
              for (EndX = EndX_Min; EndX <= EndX_Max; EndX += EndX_Delta)
              {
                for (EndY = EndY_Min; EndY <= EndY_Max; EndY += EndY_Delta)
                {
                  for (MaxLength = MaxLength_Min; MaxLength <= MaxLength_Max; MaxLength += MaxLength_Delta)
                  {
                    Iteration = 0;
                    while (Iteration < MAIN_EXPERIMENT_NUMITERATIONS)
                    {
                      // SensorNetwork.SetMaxExposureSearchMode(GLUIWindow_SearchMode);
                      SensorNetwork.New(NumSensors, Radius);
                      SensorNetwork.SetGridParameters(GridH, GridV);
                      if (GLUIWindow_AlgorithmType) SensorNetwork.SetPathParameters(GLUIWindow_StartingPointX, GLUIWindow_StartingPointY, GLUIWindow_EndingPointX, GLUIWindow_EndingPointY, GLUIWindow_MaximumLength);
                      else SensorNetwork.SetPathParameters(GLUIWindow_StartingPointX, GLUIWindow_StartingPointY, GLUIWindow_EndingPointX, GLUIWindow_EndingPointY);
                      KeepGoing = 1;
                      while(KeepGoing == 1)
                      {
                        switch(GLUIWindow_AlgorithmType)
                        {
                          case MAIN_TYPE_MINIMAL:
                            KeepGoing = SensorNetwork.MinExposure_Run();
                          break;
  
                          case MAIN_TYPE_MAXIMAL:
                          default:
                            KeepGoing = SensorNetwork.MaxExposure_Run();
                          break;
                        }
                      } 
                      if (!KeepGoing)
                      {
                        cout << "Current Iteration: " << Iteration << endl;
                        ++Iteration;
                        SensorNetwork.OutputResults(GLUIWindow_FileName);
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
} // OnExperiment