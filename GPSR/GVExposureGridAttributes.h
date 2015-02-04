////////////////////////////////////////////////////////////////////////////////
// GVExposureGridAttributes.h                                                 //
//                                                                            //
// Header file for the attributes for an exposure grid.                       //
////////////////////////////////////////////////////////////////////////////////



#ifndef GVEXPOSUREGRIDATTRIBUTES_H
#define GVEXPOSUREGRIDATTRIBUTES_H



// Defines /////////////////////////////////////////////////////////////////////
#define GVEXPOSUREGRIDATTRIBUTES_DEFAULT_NEARESTSENSOR           0
#define GVEXPOSUREGRIDATTRIBUTES_DEFAULT_BORDERLEVEL             0
#define GVEXPOSUREGRIDATTRIBUTES_DEFAULT_COVERED                 0
#define GVEXPOSUREGRIDATTRIBUTES_DEFAULT_ENQUEUED                0
#define GVEXPOSUREGRIDATTRIBUTES_DEFAULT_NOPARENT                -1
#define GVEXPOSUREGRIDATTRIBUTES_DEFAULT_COST                    -1.0
#define GVEXPOSUREGRIDATTRIBUTES_DEFAULT_EXPOSUREVALUE           0.0
#define GVEXPOSUREGRIDATTRIBUTES_DEFAULT_NORMALIZEDEXPOSUREVALUE 0.0
#define GVEXPOSUREGRIDATTRIBUTES_DEFAULT_IPARENT                 0
#define GVEXPOSUREGRIDATTRIBUTES_DEFAULT_JPARENT                 1



// GVExposureGridAttributes ////////////////////////////////////////////////////
class GVExposureGridAttributes
{ // GVExposureGridAttributes
  protected:
    int Enqueued;                   // Is the Grid Point In A Queue?
    int NearestSensor;              // Closest Sensor to Grid Square
    int BorderLevel;                // How Close to Border Grid Square Is
    int Covered;                    // Has the Grid Square Been Examined?
    int Parent[2];                  // (i, j) of Parent Node
    double Cost;                    // Cost to Get To This Point In A Search
    double ExposureValue;           // Exposure Value For Grid Square
    double NormalizedExposureValue; // Exposure Value For Grid Square Normalized to 1

  public:
    GVExposureGridAttributes();   // Creates a Blank Exposure Grid Attribute
    ~GVExposureGridAttributes();  // Deletes anm Exposure Grid Attribute

    void SetEnqueued(int E);                      // Sets Enqueued Attribute
    void SetNearestSensor(int NS);                // Sets Nearest Sensor
    void SetBorderLevel(int BL);                  // Sets Border Level
    void SetCovered(int C);                       // Sets Covered
    void SetCost(double C);                       // Sets the Cost Value
    void SetExposureValue(double EV);             // Sets Exposure Value
    void SetNormalizedExposureValue(double NEV);  // Sets Normalized Exposure Value
    void SetParent(int i, int j);                 // Sets the Parent Value

    int GetEnqueued(void);                        // Gets Enqueued Attribute
    int GetNearestSensor(void);                   // Gets Nearest Sensor
    int GetBorderLevel(void);                     // Gets Border Level
    int GetCovered(void);                         // Gets Covered
    int GetParentH(void);                         // Gets Horizontal Parent Index
    int GetParentV(void);                         // Gets Vertical Parent Index
    double GetCost(void);                         // Gets Cost
    double GetExposureValue(void);                // Gets Exposure Value
    double GetNormalizedExposureValue(void);      // Gets Normalized Exposure Value
}; // GVExposureGridAttributes



#endif