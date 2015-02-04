// SensorNetwork.h ////////////////////////////////////////////////////////////
// Giacomino Veltri                                                          //
// January 12, 2001                                                          //
//                                                                           //
// This file contains a sensor network that evaluates sleep algorithms.      //
///////////////////////////////////////////////////////////////////////////////



// SensorQueueEntry Class /////////////////////////////////////////////////////
class SensorQueueEntry
{ // SensorQueueEntry
  public:
    int NodeNumber;         // Node ID Number
    double NodeCost;        // Cost For Current Node
    SensorQueueEntry* Next; // Next Node in List

    // Constructors/Destructors
    SensorQueueEntry();
    SensorQueueEntry(int N, double C, SensorQueueEntry* Nx);
    ~SensorQueueEntry();
}; // SensorQueueEntry



// SensorQueue Class ////////////////////////////////////////////////////////////
class SensorQueue
{ // SensorQueue
  public:
    SensorQueueEntry* Head;

    // Constructors/Destructors
    SensorQueue();
    ~SensorQueue();

    // List Functions
    void Init(void);
    void Delete(void);
    void Enqueue(int N, double C);
    void Dequeue(int N);
}; // SensorQueue