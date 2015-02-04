// SensorNetwork.cpp //////////////////////////////////////////////////////////
// Giacomino Veltri                                                          //
// January 12, 2001                                                          //
//                                                                           //
// This file contains a sensor network that evaluates sleep algorithms.      //
///////////////////////////////////////////////////////////////////////////////



#include "StdAfx.h"
#include "SensorQueue.h"



// SensorQueueEntry Implmentation /////////////////////////////////////////////
SensorQueueEntry::SensorQueueEntry()
/*
  This function initializes an SensorQueueEntry
*/
{ // SensorQueueEntry
  NodeNumber = -1;
  NodeCost = 0;
  Next = NULL;
} // SensorQueueEntry



SensorQueueEntry::SensorQueueEntry(int N, double C, SensorQueueEntry* Nx)
/*
  This function initializes an SensorQueueEntry with the specified values.
*/
{ // SensorQueueEntry
  NodeNumber = N;
  NodeCost = C;
  Next = Nx;
} // SensorQueueEntry



SensorQueueEntry::~SensorQueueEntry()
/*
  This function deletes an SensorQueueEntry
*/
{ // ~SensorQueueEntry
} // ~SensorQueueEntry



// SensorQueue Class Implementation ////////////////////////////////////////////
SensorQueue::SensorQueue()
/*
  This function initializes the SensorQueue
*/
{ // SensorQueue
  Head = NULL;
  Init();
} // SensorQueue



SensorQueue::~SensorQueue()
{ // ~SensorQueue
  Delete();
} // ~SensorQueue



void SensorQueue::Init(void)
/*
  This function initializes an SensorQueue
*/
{ // Init
  Delete();
  // Make a New Head Node (Head->Next is First Entry in Queue)
  Head = new SensorQueueEntry(-1, 0, NULL);
} // Init



void SensorQueue::Delete(void)
/*
  This function de-initializes an SensorQueue by removing all entries in the
  list.
*/
{ // Delete
  SensorQueueEntry* Current = Head;
  SensorQueueEntry* Next = NULL;

  // Delete Every Entry In The List
  while (Current != NULL)
  {
    Next = Current->Next;
    delete Current;
    Current = Next;
  }
  // Erase the List
  Head = NULL;
} // Delete



void SensorQueue::Enqueue(int N, double C)
/*
  This function inserts an item into the list (in order)
*/
{ // Enqueue
  int Continue = 1;                                         // Keep Processing List
  SensorQueueEntry* Current = Head->Next;                   // Current Node in List
  SensorQueueEntry* Previous = Head;                        // Previous Node in List
  SensorQueueEntry* New = new SensorQueueEntry(N, C, NULL); // New Entry in Lst

  while(Continue)
  {
    // If The Value is Smaller than the Current Node Cost, Stop Searching
    // otherwise go to the next node
    if (Current == NULL)
    {
      Continue = 0;
    }
    else if (C < Current->NodeCost)
    {
      Continue = 0;
    }
    else
    {
      Previous = Current;
      Current = Current->Next;
    }
  }
  // Add the Node to the List
  New->Next = Current;
  Previous->Next = New;
} // Enqueue



void SensorQueue::Dequeue(int N)
/*
  This deletes all entries with a node number of N from the queue.
*/
{ // Dequeue
  SensorQueueEntry* Current = Head->Next; // Current Node in List
  SensorQueueEntry* Previous = Head;      // Previous Node in List

  while (Current != NULL)
  {
    if (Current->NodeNumber == N)
    {
      // Delete the Node and Adjust the Current
      Previous->Next = Current->Next;
      delete Current;
      Current = Previous->Next;
    }
    else
    {
      Previous = Current;
      Current = Current->Next;
    }
  }
} // Dequeue