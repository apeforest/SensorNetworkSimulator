#ifndef GVOBSTACLE_H
#define GVOBSTACLE_H



// Defines /////////////////////////////////////////////////////////////////////
#define GVOBSTACLE_DEFAULTX1 0.0
#define GVOBSTACLE_DEFAULTX2 0.0
#define GVOBSTACLE_DEFAULTY1 0.0
#define GVOBSTACLE_DEFAULTY2 0.0


// GVObstacle ///////////////////////////////////////////////////////////////////
class GVObstacle
{ // GVObstacle
  protected:
    double MinX; // Minimum X Coordinate of Obstacle
    double MinY; // Minimum Y Coordinate of Obstacle
	double MaxX; // Maximum X Coordinate of Obstacle
	double MaxY; // Maximum Y Coordinate of Obstacle
  public:
    GVObstacle();                               // Constructor
    ~GVObstacle();                              // Destructor
    void New(double X1, double Y1, double X2, double Y2);            // Creates a New Obstacle
	void Delete(void);                         // Deletes an Obstacle
	int CoveredPosition(double X, double Y);
}; // GVObstacle



#endif