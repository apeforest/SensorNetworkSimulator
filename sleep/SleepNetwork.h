// SensorNetwork.h ////////////////////////////////////////////////////////////
// Giacomino Veltri                                                          //
// January 12, 2001                                                          //
//                                                                           //
// This file contains a sensor network that evaluates sleep algorithms.      //
///////////////////////////////////////////////////////////////////////////////

#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>
#include <math.h>
#include "SensorNetwork.h"

#ifndef SLEEP_H
#define SLEEP_H

// Constants //////////////////////////////////////////////////////////////////

// Sleep Class ////////////////////////////////////////////////////////////////
class SleepNetwork: public SensorNetwork
{ // SleepNetwork
  public:
    int ProducerNum;    // Sensor Number of Producer
    int NumConsumers;   // Number of Consumers
    int OnIndex;        // Last Element of Sensors On
    int NumComs;        // Number of Communicatons
    int QPos;           // Position of Next Node to Inspect
    int NextQPos;       // Position to Place Next Node in Queue
    int* BFSQueue;      // BFS Queue
    int* SleepResults;  // BFS Results Sleep Paths
    int* MinResults;    // BFS Results Minimum Paths
    int* ConsumerArray; // Array of Consumer Sensor Numbers
    int* SensorsOn;     // Array of Indices of On Sensors
    double MinSavings;  // Minimum Percent Savings
    double MaxSavings;  // Maximum Percent Savings
    double AvgSavings;  // Average Percent Savings

    SleepNetwork()
    { // Sleep
      Radius = 0;
      NumSensors = 0;
      SensorArray = NULL;
      EdgeTable = NULL;
      ConsumerArray = NULL;
      SensorsOn = NULL;
      BFSQueue = NULL;
      SleepResults = NULL;
      MinResults = NULL;
    } // Sleep



    ~SleepNetwork()
    { // ~Sleep
      Delete();
      if (ConsumerArray != NULL) delete[] ConsumerArray;
      if (SensorsOn != NULL) delete[] SensorsOn;
      if (BFSQueue != NULL) delete[] BFSQueue;
      if (SleepResults != NULL) delete[] SleepResults;
      if (MinResults != NULL) delete[] MinResults;
    } // ~Sleep



    void InitializeSleep(int Mode, int Num, double Rad, int NC)
    { // InitializeSleep
      switch(Mode)
      {
        case 0: // Random2D
          InitializeStandard(Num, Rad);
          InitializeSleep_Random2D();
          InitializeSleep_Consumers(NC);
        break;
        case 1: // Grid
          InitializeStandard(Num, Rad);
          InitializeSleep_Grid();
          InitializeSleep_Consumers(NC);
        break;
        case 2: // Manual
          InitializeManual();
        break;
        case 3: // Contrived Graph
          GenerateInstance(Num, NC, Rad);
        break;
        default: // Random2D
          InitializeStandard(Num, Rad);
          InitializeSleep_Random2D();
          InitializeSleep_Consumers(NC);
        break;
      }
      CalculateEdges();
    } // InitializeSleep



    void InitializeStandard(int Num, double Rad)
    { // InitializeStandard
      int i = 0;

      InitializeSensorNetwork(Num, Rad);
      
      if (SensorsOn != NULL) delete[] SensorsOn;
      SensorsOn = new int[NumSensors + 1];
      for (i = 0; i < NumSensors; ++i)
      {
        SensorsOn[i] = -1;
        SensorArray[i].Type = SENSOR_TYPE_LISTENER;
        SensorArray[i].SubType = SENSOR_MSTTYPE_NONE;
        SensorArray[i].Mode = SENSOR_MODE_OFF;
      }
      SensorsOn[i] = -1;
      OnIndex = 0;
    } // InitializeStandard



    void InitializeManual()
    { // InitializeManual
      InitializeStandard(100, 0.1);
      InitializeSleep_Grid();
      NumConsumers = 5;
      if (ConsumerArray != NULL) delete[] ConsumerArray;
      ConsumerArray = new int[NumConsumers];
      SensorArray[17].Type = SENSOR_TYPE_CONSUMER;
      SensorArray[27].Type = SENSOR_TYPE_CONSUMER;
      SensorArray[28].Type = SENSOR_TYPE_CONSUMER;
      SensorArray[83].Type = SENSOR_TYPE_CONSUMER;
      SensorArray[84].Type = SENSOR_TYPE_CONSUMER;
      ConsumerArray[0] = 17;
      ConsumerArray[1] = 27;
      ConsumerArray[2] = 28;
      ConsumerArray[3] = 83;
      ConsumerArray[4] = 84;
      SensorArray[45].Type = SENSOR_TYPE_PRODUCER;
      ProducerNum = 45;
    } // InitializeManual



    void InitializeSleep_Random2D(void)
    { // InitializeSleep_Random2D
      int i = 0;            // Index Variable
      int Accuracy = 10000; // Accuracy of Digits
      double Temp = 0;      // Temporary Variable

      for (i = 0; i < NumSensors; ++i)
      {
        SensorArray[i].X = double(rand() % (Accuracy + 1))/(double(Accuracy));
        SensorArray[i].Y = double(rand() % (Accuracy + 1))/(double(Accuracy));
      }
    } // InitializeSleep_Random2D



    void InitializeSleep_Grid(void)
    { // InitializeSleep_Grid
      int i = 0;                            // Index Variable
      int j = 0;                            // Index Variable
      int Size = int(sqrt(NumSensors));     // Size of Grid
      double Space = 1.0 / (double (Size + 1)); // Space Between Grid Vertices

      NumSensors = Size * Size;
      for (i = 0; i < Size; ++i)
      {
        for (j = 0; j < Size; ++j)
        {
          SensorArray[j + Size * i].X = (j + 1) * Space;
          SensorArray[j + Size * i].Y = (i + 1) * Space;
        }
      }
    } // InitializeSleep_Grid



    void InitializeSleep_Consumers(int NC)
    { // InitializeSleep_Consumers
      int Current = 0; // Current Number of Consumers
      int Node = 0;    // Number of Current Sensor

      if (ConsumerArray != NULL) delete[] ConsumerArray;
      NumConsumers = NC;
      ConsumerArray = new int[NumConsumers];
      while (Current < NumConsumers)
      {
        Node = rand() % NumSensors;
        if (SensorArray[Node].Type == SENSOR_TYPE_LISTENER)
        {
          ConsumerArray[Current] = Node;
          ++Current;
          SensorArray[Node].Type = SENSOR_TYPE_CONSUMER;
        }
      }

      Current = 0;
      while(!Current)
      {
        Node = rand() % NumSensors;
        if (SensorArray[Node].Type == SENSOR_TYPE_LISTENER)
        {
          ProducerNum = Node;
          SensorArray[Node].Type = SENSOR_TYPE_PRODUCER;
          Current = 1;
        }
      }
    } // InitializeSleep_Consumers



    void CalculateSleepNodes(void)
    { // CalculateSleepNodes
      int i = 0;            // Index Variable
      int j = 0;            // Index Variable
      int Iteration = -1;   // Number of Iterations
      int N = 0;            // Number of Lines to Consider
      int C = 0;            // Consumer Number
      int Continue = 1;     // Continue Searching
      int Next = 0;         // Next Node
      double D1 = 0;        // Distance from Point to Line
      double D2 = 0;        // Distance from Point to Consumer
      double Sum = 0;       // Sum of Distances
      double Min = DBL_MAX; // Current Minimum

      for (i = 0; i < NumSensors; ++i)
      {
        SensorArray[i].Mode = SENSOR_MODE_OFF;
      }
      OnIndex = 0;
      TurnOnSensor(ProducerNum);
      while(Continue)
      {
        ++Iteration;
        Next = -1;
        Continue = 0;
        Min = DBL_MAX;
        for (i = 0; i < NumSensors; ++i)
        {
          if (SensorArray[i].Mode == SENSOR_MODE_INFLUENCED)
          {
            N = 0;
            Sum = 0;
            for (j = 0; j < NumConsumers; ++j)
            {
              C = ConsumerArray[j];
              if (SensorArray[C].Mode == SENSOR_MODE_OFF)
              {
                Continue = 1;
                D1 = NodeLineDistance(i, C, ProducerNum);
                if (D1 >= 0)
                {
                  D2 = Distance(SensorArray[i].X, SensorArray[i].Y, SensorArray[i].Z, SensorArray[C].X, SensorArray[C].Y, SensorArray[C].Z);
                  Sum += D1 + D2;
                  ++N;
                }
              }
            }
            if (N > 0)
            {
              Sum /= N;
              if ((Sum < Min) && (Sum > 0))
              {
                Next = i;
                Min = Sum;
              }
              else
              {
                N = N;
              }
            }
          }
        }
        if (Next < 0)
        {
          Next = 0;
        }
        else
        {
          TurnOnSensor(Next);
        }
        if (Iteration > 2 * NumSensors)
        {
          Continue = 0;
        }
      }
    } // CalculateSleepNodes


    void TurnOnSensor(int N)
    { // TurnOnSensor
      int i = 0;
      int S = 0;

      SensorArray[N].Mode = SENSOR_MODE_ON;
      SensorsOn[OnIndex] = N;
      ++OnIndex;
      S = *(EdgeTable + i + N * NumSensors);
      while (S >= 0)
      {
        if (SensorArray[S].Mode == SENSOR_MODE_OFF)
        {
          if (SensorArray[S].Type == SENSOR_TYPE_CONSUMER)
          {
            TurnOnSensor(S);
          }
          else
          {
            SensorArray[S].Mode = SENSOR_MODE_INFLUENCED;
          }
        }
        ++i;
        S = *(EdgeTable + i + N * NumSensors);
      }
    } // TurnOnSensor



    double NodeLineDistance(int Node, int P1, int P2)
    { // NodeLineDistance
      double u = 0;                    // How Far Across Line to Node
      double D = -1;                   // Distance to Line
      double X1 = SensorArray[P1].X;   // P1 X Coordinate
      double Y1 = SensorArray[P1].Y;   // P1 Y Coordinate
      double X2 = SensorArray[P2].X;   // P2 X Coordinate
      double Y2 = SensorArray[P2].Y;   // P2 Y Coordinate
      double X3 = SensorArray[Node].X; // Node X Coordinate
      double Y3 = SensorArray[Node].Y; // Node Y Coordinate
      double X = 0;                    // Intersection X Coordinate
      double Y = 0;                    // Intersection Y Coordinate

      u = ((X3-X1)*(X2-X1)+(Y3-Y1)*(Y2-Y1))/((X2-X1)*(X2-X1)+(Y2-Y1)*(Y2-Y1));

      if ((u >= 0) && (u <= 1))
      {
        X = X1 + u * (X2 - X1);
        Y = Y1 + u * (Y2 - Y1);
        D = sqrt((X3-X)*(X3-X)+(Y3-Y)*(Y3-Y));
      }
      return(D);
    } // NodeLineDistance



    void MakeMSTInterior(int N)
    { // MakeMSTInterior
      int i = 0;
      int S = 0;

      SensorArray[N].SubType = SENSOR_MSTTYPE_INTERIOR;
      for (S = *(EdgeTable + i + N * NumSensors); S >= 0; ++i, S = *(EdgeTable + i + N * NumSensors))
      {
        if ((SensorArray[S].Mode == SENSOR_MODE_ON) && (SensorArray[S].SubType == SENSOR_MSTTYPE_NONE))
        {
          SensorArray[S].SubType = SENSOR_MSTTYPE_LEAF;
          SensorArray[S].Parent = N;
        }
      }
    } // MakeMSTInterior



    void CalculateMST(void)
    { // CalculateMST
      int i = 0;
      int S = 0;
      int Max = 0;
      int Neighbors = 0;
      int Next = -1;
      int Continue = 1;
      int Iterations = -1;

      MakeMSTInterior(ProducerNum);

      while(NodesNotInMST() && Continue)
      {
        ++Iterations;
        Next = -1;
        Max = 0;
        for (i = 0; i < OnIndex; ++i)
        {
          S = SensorsOn[i];
          if (SensorArray[S].SubType == SENSOR_MSTTYPE_LEAF)
          {
            Neighbors = NumNeighborsNotInMST(S);
            if (Neighbors > Max)
            {
              Next = S;
              Max = Neighbors;
            }
          }
        }
        if (Next >= 0) MakeMSTInterior(Next);
        if (Iterations > NumSensors * 2) Continue = 0;
      }
    } // CalculateMST



    int NodesNotInMST(void)
    { // NodesNotInMST
      int Success = 0;
      int i = 0;

      for (i = 0; (!Success) && (i < OnIndex); ++i)
      {
        if (SensorArray[SensorsOn[i]].SubType == SENSOR_MSTTYPE_NONE)
        {
          Success = 1;
        }
      }
      return(Success);
    } // NodesNotInMST



    int NumNeighborsNotInMST(int N)
    { // NumNeighborsNotInMST
      int i = 0;
      int S = 0;
      int ReturnVal = 0;

      for (i = 0, S = *(EdgeTable + i + N * NumSensors); S >= 0; ++i, S = *(EdgeTable + i + N * NumSensors))
      {
        if ((SensorArray[S].Mode == SENSOR_MODE_ON) && (SensorArray[S].SubType == SENSOR_MSTTYPE_NONE))
        {
          ++ReturnVal;
        }
      }
      return(ReturnVal);
    } // NumNeighborsNotInMST



    void PruneMST(void)
    { // PruneMST
      int NodePruned = 1;
      int i = 0;
      int S = 0;

      while(NodePruned)
      {
        NodePruned = 0;
        for (i = 0; i < OnIndex; ++i)
        {
          S = SensorsOn[i];
          if ((SensorArray[S].SubType == SENSOR_MSTTYPE_LEAF) && (SensorArray[S].Type == SENSOR_TYPE_LISTENER))
          {
            PruneNode(S);
            NodePruned = 1;
          }
        }
      }
    } // PruneMST



    void PruneNode(int Node)
    { // PruneNode
      int i = 0;
      int S = 0;
      int Parent = SensorArray[Node].Parent;
      int Children = 0;

      SensorArray[Node].Mode = SENSOR_MODE_OFF;
      SensorArray[Node].SubType = SENSOR_MSTTYPE_NONE;
      
      for (i = 0, S = *(EdgeTable + i + Parent * NumSensors); (!Children) && (S >= 0); ++i, S = *(EdgeTable + i + Parent * NumSensors))
      {
        if ((SensorArray[S].Parent == Parent) && (SensorArray[S].Mode == SENSOR_MODE_ON))
        {
          ++Children;
        }
      }

      if (Children == 0)
      {
        SensorArray[Parent].SubType = SENSOR_MSTTYPE_LEAF;
      }
    } // PruneNode



    void CalcNumNodesOn(void)
    { // CalcNumNodesOn
      int i = 0;
      OnIndex = 0;

      for (i = 0; i < NumSensors; ++i)
      {
        if (SensorArray[i].Mode == SENSOR_MODE_ON)
        {
          SensorsOn[OnIndex] = i;
          ++OnIndex;
        }
      }
    } // CalcNumNodesOn



    int ConsumersStillListening(void)
    { // ConsumersStillListening
      int i = 0;
      int C = 0;
      int Listening = 0;

      for (i = 0; (!Listening) && (i < NumConsumers); ++i)
      {
        C = ConsumerArray[i];
        if (SensorArray[C].Status == SENSOR_STATUS_LISTENING)
        {
          Listening = 1;
        }
      }
      return(Listening);
    } // ConsumersStillListening



    void Communicate(int Sensor)
    { // Communicate
      int i = 0;
      int S = 0;

      SensorArray[Sensor].Status = SENSOR_STATUS_DONE;
      ++NumComs;
      for (i = 0, S = *(EdgeTable + i + Sensor * NumSensors); S >= 0; ++i, S = *(EdgeTable + i + Sensor * NumSensors))
      {
        if ((SensorArray[S].Mode == SENSOR_MODE_ON) && (SensorArray[S].Status == SENSOR_STATUS_LISTENING))
        {
          SensorArray[S].Status = SENSOR_STATUS_HEARD;
        }
      }
    } // Communicate



    void CalculateCommunications(void)
    { // CalculateCommunications
      int i = 0;
      int Continue = 1;
      int Iterations = 0;
      NumComs = 0;
      for (i = 0; i < NumSensors; ++i)
      {
        SensorArray[i].Status = SENSOR_STATUS_LISTENING;
      }

      Communicate(ProducerNum);
      while(Continue && ConsumersStillListening())
      {
        for (i = 0; i < NumSensors; ++i)
        {
          if ((SensorArray[i].Status == SENSOR_STATUS_HEARD) && (SensorArray[i].SubType != SENSOR_MSTTYPE_LEAF))
          {
            Communicate(i);
          }
        }
        if (++Iterations > NumSensors)
        {
          Continue = 0;
          NumComs = NumSensors + 1;
        }
      }
    } // CalculateCommunications



    void CalcPathStats(void)
    { // CalcPathStats
      int i = 0;
      int j = 0;
      int C = 0;
      double Temp;

      if (BFSQueue != NULL) delete[] BFSQueue;
      if (SleepResults != NULL) delete[] SleepResults;
      if (MinResults != NULL) delete[] MinResults;
      BFSQueue = new int[NumSensors + 1];
      SleepResults = new int[NumConsumers];
      MinResults = new int[NumConsumers];

      for (i = 0; i < NumConsumers; ++i)
      {
        for (j = 0; j < NumSensors; ++j)
        {
          SensorArray[j].Enqueued = 0;
          SensorArray[j].Cost = 0;
        }
        C = ConsumerArray[i];
        QPos = 0;
        NextQPos = 0;
        SensorArray[C].Enqueued = 1;
        BFSQueue[NextQPos++] = C;
        SleepResults[i] = BFS(ProducerNum);
      }

      for (i = 0; i < NumSensors; ++i)
      {
        SensorArray[i].Mode = SENSOR_MODE_ON;
      }

      for (i = 0; i < NumConsumers; ++i)
      {
        for (j = 0; j < NumSensors; ++j)
        {
          SensorArray[j].Enqueued = 0;
          SensorArray[j].Cost = 0;
        }
        C = ConsumerArray[i];
        QPos = 0;
        NextQPos = 0;
        SensorArray[C].Enqueued = 1;
        BFSQueue[NextQPos++] = C;
        MinResults[i] = BFS(ProducerNum);
      }

      MinSavings = DBL_MAX;
      MaxSavings = 0;
      AvgSavings = 0;
      for (i = 0; i < NumConsumers; ++i)
      {
        Temp = 100.0 * double(SleepResults[i] - MinResults[i]) / double(MinResults[i]);
        AvgSavings += Temp;
        if (Temp < MinSavings) MinSavings = Temp;
        if (Temp > MaxSavings) MaxSavings = Temp;
      }
      AvgSavings /= NumConsumers;
      AvgSavings = int(AvgSavings);
      MinSavings = int(MinSavings);
      MaxSavings = int(MaxSavings);
    } // CalcPathStats




    int BFS(int End)
    { // BFS
      while ((BFSQueue[QPos] != End) && (QPos < NextQPos))
      {
        AddNeighborsToQueue(BFSQueue[QPos++]);
      }
      return ((int) SensorArray[End].Cost);
    } // BFS



    void AddNeighborsToQueue(int Sensor)
    { // AddNeighborsToQueue
      int i = 0;
      int S = 0;

      for (i = 0, S = *(EdgeTable + i + Sensor * NumSensors); S >= 0; S = *(EdgeTable + (++i) + Sensor * NumSensors))
      {
        if ((SensorArray[S].Mode == SENSOR_MODE_ON) && (SensorArray[S].Enqueued == 0))
        {
          BFSQueue[NextQPos] = S;
          ++NextQPos;
          SensorArray[S].Enqueued = 1;
          SensorArray[S].Cost = SensorArray[Sensor].Cost + 1;
        }
      }
    } // AddNeighborsToQueue



    void AddToGroup(int Start, int Dest)
    { // AddToGroup
      int i = 0;
      int S = 0;
      int NumCurrent = 0;
      int NumNext = 1;
      int* Q;              // BFS Queue

      Q = new int[NumSensors];
      for (i = 0; i < NumSensors; ++i)
      {
        SensorArray[i].Enqueued = 0;
      }
      Q[NumCurrent] = Start;
      SensorArray[Start].Enqueued = 1;
      while ((Q[NumCurrent] != Dest) && (NumCurrent != NumNext))
      {
        for (i = 0, S = *(EdgeTable + i + Q[NumCurrent] * NumSensors); S >= 0; S = *(EdgeTable + (++i) + Q[NumCurrent] * NumSensors))
        {
          if (SensorArray[S].Enqueued == 0)
          {
            SensorArray[S].Enqueued = 1;
            SensorArray[S].Parent = Q[NumCurrent];
            SensorArray[S].Cost = SensorArray[Q[NumCurrent]].Cost + 1;
            Q[NumNext++] = S;
          }
        }
        ++NumCurrent;
      }
      if (Q[NumCurrent] == Dest)
      {
        for (NumCurrent = Dest; NumCurrent != Start; NumCurrent = SensorArray[NumCurrent].Parent)
        {
          SensorArray[NumCurrent].Mode = SENSOR_MODE_ON;
        }
      }
      delete[] Q;
    } // AddToGroup



    int AddToGraph(int Node, int Index)
    { // AddToGraph
      int Success = 1;
      int Angle = 0;
      int i = 0;
      int Difference = 0;
      double Percent = 0;

      Angle = rand() % 360;
      Percent =  double ((rand() % 10000) + 1) / 10000.0;
      SensorArray[Index].X = Percent * Radius * cos(3.14159 * Angle / 180.0);
      SensorArray[Index].Y = Percent * Radius * sin(3.14159 * Angle / 180.0);
      SensorArray[Index].Z = 0;
      SensorArray[Index].Cost = SensorArray[Node].Cost + 1;
      SensorArray[Index].SubType = SENSOR_MSTTYPE_NONE;
      SensorArray[Index].Parent = Node;
      SensorArray[Index].Status = SENSOR_STATUS_LISTENING;
      SensorArray[Index].Mode = SENSOR_MODE_OFF;
      SensorArray[Index].Enqueued = 0;

      for (i = 0; Success && (i < NumSensors); ++i)
      {
        if (Distance(SensorArray[Index].X, SensorArray[Index].Y, SensorArray[Index].Z, SensorArray[i].X, SensorArray[i].Y, SensorArray[i].Z) < Radius)
        {
          Difference = (int) (SensorArray[Index].Cost - SensorArray[i].Cost);
          if ((Difference < -1) || (Difference > 1))
          {
            Success = 0;
          }
        }
      }
      return(Success);
    } // AddToGraph



    void GenerateInstance(int NS, int NC, double R)
    { // GenerateInstance
      int i = 0;
      int Iterations = 0;
      int j = 0;
      int C = 0;
      int Branch = 0;
      int BranchC = 0;
      int NextC = 0;
      int Current = 0;
      int Continue = 1;
      Sensor* Temp = NULL;
      double D = 0;
      double Min = DBL_MAX;
      double MinC = DBL_MAX;
      
      InitializeStandard(NS * 2, R);
      InitializeSleep_Random2D();
      CalculateEdges();
      ProducerNum = rand() % NumSensors;
      SensorArray[ProducerNum].Type = SENSOR_TYPE_PRODUCER;
      SensorArray[ProducerNum].Mode = SENSOR_MODE_ON;

      if (ConsumerArray != NULL) delete[] ConsumerArray;
      NumConsumers = NC;
      ConsumerArray = new int[NumConsumers];
      while (Current < NumConsumers)
      {
        C = rand() % NumSensors;
        if (SensorArray[C].Type == SENSOR_TYPE_LISTENER)
        {
          ConsumerArray[Current] = C;
          ++Current;
          SensorArray[C].Type = SENSOR_TYPE_CONSUMER;
        }
      }

      for (i = 0; i < NumSensors; ++i)
      {
        SensorArray[i].Mode = SENSOR_MODE_OFF;
      }
      SensorArray[ProducerNum].Mode = SENSOR_MODE_ON;

      while(Continue && (Iterations <= NumConsumers))
      {
        ++Iterations;
        Continue =  0;
        NextC = -1;
        MinC = DBL_MAX;
        Min = DBL_MAX;
        for (i = 0; i < NumConsumers; ++i)
        {
        
          C = ConsumerArray[i];
          if (SensorArray[C].Mode == SENSOR_MODE_OFF)
          {
            Continue = 1;
            for (j = 0; j < NumSensors; ++j)
            {
              D = Distance(SensorArray[C].X, SensorArray[C].Y, SensorArray[C].Z, SensorArray[j].X, SensorArray[j].Y, SensorArray[j].Z);
              if ((D < MinC) && (SensorArray[j].Mode == SENSOR_MODE_ON) && (C != j))
              {
                MinC = D;
                BranchC = j;
              }
            }
            if (MinC < Min)
            {
              NextC = C;
              Min = MinC;
              Branch = BranchC;
            }
          }
        }
        if (NextC >= 0) AddToGroup(Branch, NextC);
      }



      Temp = new Sensor[NS];
      for (i = 0, Current = 0; i < NumSensors; ++i)
      {
        SensorArray[i].Enqueued = 0;
        if ((SensorArray[i].Mode == SENSOR_MODE_ON) || (SensorArray[i].Type == SENSOR_TYPE_CONSUMER) || (SensorArray[i].Type == SENSOR_TYPE_PRODUCER))
        {
          SensorArray[i].Enqueued = 1;
          Temp[Current].X = SensorArray[i].X;
          Temp[Current].Y = SensorArray[i].Y;
          Temp[Current].Z = SensorArray[i].Z;
          Temp[Current].Type = SensorArray[i].Type;
          Temp[Current].SubType = SensorArray[i].SubType;
          Temp[Current].Parent = SensorArray[i].Parent;
          Temp[Current].Status = SensorArray[i].Status;
          Temp[Current].Mode = SensorArray[i].Mode;
          Temp[Current].Enqueued = SensorArray[i].Enqueued;
          Temp[Current].Cost = SensorArray[i].Cost;
          ++Current;
        }
      }
      while ((Current < NS) && (Iterations < NumSensors))
      {
        C = rand() % NumSensors;
        if (SensorArray[C].Enqueued == 0)
        {
          Iterations = 0;
          SensorArray[i].Enqueued = 1;
          Temp[Current].X = SensorArray[i].X;
          Temp[Current].Y = SensorArray[i].Y;
          Temp[Current].Z = SensorArray[i].Z;
          Temp[Current].Type = SensorArray[i].Type;
          Temp[Current].SubType = SensorArray[i].SubType;
          Temp[Current].Parent = SensorArray[i].Parent;
          Temp[Current].Status = SensorArray[i].Status;
          Temp[Current].Mode = SensorArray[i].Mode;
          Temp[Current].Enqueued = SensorArray[i].Enqueued;
          Temp[Current].Cost = SensorArray[i].Cost;
          ++Current;
        }
        else
        {
          ++Iterations;
        }
      }
      delete[] SensorArray;
      SensorArray = Temp;
      NumSensors = NS;
      for (i = 0, Current = 0; i < NS; ++i)
      {
        if (SensorArray[i].Type == SENSOR_TYPE_CONSUMER)
        {
          ConsumerArray[Current++] = i;
        }
        if (SensorArray[i].Type == SENSOR_TYPE_PRODUCER)
        {
          ProducerNum = i;
        }
      }
      /*
      while ((Current < NumSensors) && (Iterations <= NumSensors))
      {
        while (C = NextC)
        {
          C = rand() % Current;
        }
        NextC = C;
        if (AddToGraph(C, Current))
        {
          ++Current;
          Iterations = 0;
        }
        else
        {
          ++Iterations;
        }
      }
      */
      CalculateEdges();
    } // GenerateInstance
}; // SleepNetwork

#endif