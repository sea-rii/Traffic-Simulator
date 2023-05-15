#include "trafficSimulator.h"
int max( int a, int b );

/* printNames
 * input: none
 * output: none
 *
 * Prints names of the students who worked on this solution
 * REMINDER: You may only work with one other cs2124 student
 */
void printNames( )
{
 /* TODO : Fill in you and your partner's names (or N/A if you worked individually) */
 printf("\nThis solution was completed by:\n");
 printf("Sai Siri Chittineni\n");
 printf("Melissa Aradillaz\n\n");
}

/* createTrafficData
 * input: char* filename of file to read
 * output: TrafficData* which stores the data of this road network
 *
 * Create a new TrafficData variable and read in the traffic data from the file whose name is stored in char* filename
 */
TrafficData* createTrafficData( char* filename )
{
  TrafficData* newTD = (TrafficData*)malloc(sizeof(TrafficData));
  RoadData* Road;
  
 /* open the file */
 /* TODO: read in all the data in pFile */
 /* HINT: use fscanf( pFile, "%d", &<your int variable here> ) to read an int from pFile */
 /* HINTs:
    * Each road can be stored in a `RoadData` struct (see `road.h`).
    *
    * You'll want to store all of these `RoadData` structs in a single array. This is important to track the order that 
    * roads were added to your graph. These are also the order they are processed in each time step. You can store this
    * array in your TrafficData struct.
    *
    * You'll add edges to a graph to represent each road using `setEdge` (see `graph.c`). The vertices of the graph 
    * represent intersections. The graph will do the work of finding shortest paths (see `getNextOnShortestPath` in graph.c 
    * and the longest shortest path problem on Blackboard).
    *
    * It will also help to add your `RoadData` structs to your graph using `setEdgeData` (see `graph.c`). You can then 
    * retrieve that `RoadData` struct by calling `getEdgeData`. This will be helpful for finding roads when cars pass 
    * through intersections.
    * 
    * Each event will be stored in an Event struct (see `event.h`).
    * 
    * It's recommended to store all of the `Event` structs in a single priority queue (using time step as the priority). This 
    * allows you to quickly find the next event to execute by time step (see `enqueueByPriority`, `getFrontPriority`, and 
    * `dequeuePQ`).
    *
    * Each car is stored in a Car struct (see `car.h`).
 */
 
  int i;        // represents "to" or current intersection && current car event
  int j;        // represents number of incoming roads && number of cars to add
  int k;        // represents currRoads && current car
  int n=0;      // tracks order of roads entered in an array
  int numInter; // temporarily stores number of interactions
 
  FILE *pFile = fopen( filename, "r" );
  if (pFile == NULL) {         // checking if the file is empty, if it is empty generating an error message
    printf("Error: could not open file %s\n", filename);
    return NULL;
  }
 
  fscanf(pFile, "%d%d", &numInter, &newTD->numRoads);         // reads in number of intersections and roads 
  newTD->roadNetworks = createGraph((2 *numInter) * newTD->numRoads);      // creating graph with capacity parameter
  newTD->roadNetworks->numVertices = numInter;
 
  if (newTD->roadNetworks == NULL) {
    printf("ERROR: could not create graph\n");
    free(newTD);
    fclose(pFile);
    return NULL;
  }
 
  newTD->roads = (RoadData**)malloc(newTD->numRoads * sizeof(RoadData*));
  int s,l;
  for (i=0; i < numInter; i++) {             // stores roads in array
    fscanf(pFile, "%d", &j);                 // scaning the number of incoming roads on current intersection
    if (j > 0) {                             // checking if there are incoming roads 
       for (k=0; k < j; k++) {
         fscanf(pFile, "%d%d", &s, &l);
         Road = createRoad(l, s, i);
         fscanf(pFile, "%d%d%d", &Road->on, &Road->off, &Road->reset);
         newTD->roads[n] = Road;                    // add current road to array of roads and increment order
         n++;
         setEdge(newTD->roadNetworks, Road->startInter, i, Road->length);           // create edges to graph (add roads to road network)
         setEdgeData(newTD->roadNetworks, Road->startInter, i, Road);             // add edge data to graph (add road data to road network)
       }
    }
  }
 
  int m;                // store number of car events
  int eventFrom;        // event FROM of starting edge
  int eventTo;          // event TO of starting edge
  int eventTStep; 
  int carDest;          // car destination
  Car* Car;    
  //Car = (Car*)malloc(sizeof(Car));
  Event* event;
  event = (Event*)malloc(sizeof(Event)); 
  newTD->eventQueue = createPQ(); 
 
  if (newTD->eventQueue == NULL) { 
     printf("Error: could not create event queue"); 
     free(newTD->roads);
     freeGraph(newTD->roadNetworks); 
     free(newTD);
     fclose(pFile);
     return NULL;
  }
 
  fscanf(pFile, "%d", &m); 
 
  for (i=0; i < m; i++) {
    fscanf(pFile, "%d%d%d%d", &eventFrom, &eventTo, &eventTStep, &j);
    event = createAddCarEvent(eventTStep, Road);            // create event and add to event queue
    enqueueByPriority(newTD->eventQueue, event, eventTStep);
   //event = newTD->eventQueue; 
   //event->pCarQueue = createQueue();
 
   if (event->pCarQueue == NULL) { 
     printf("Error: could not create car queue"); 
     free(newTD->eventQueue);
     free(newTD->roads);
     freeGraph(newTD->roadNetworks);
     free(newTD);
     fclose(pFile);
   }
   newTD->numCars = 0;
 
   Road = getEdgeData(newTD->roadNetworks, eventFrom, eventTo);
   for (k=0; k < j; k++) {
     fscanf(pFile, "%d", &carDest);
     Car = createCar(eventTStep, eventFrom, eventTo, carDest);
     enqueue(event->pCarQueue, Car);
     newTD->numCars++;
   }
 }
 
 int currPRE;
 fscanf(pFile, "%d", &newTD->numPRE);
 newTD->printRC = (Event**)malloc(newTD->numPRE * sizeof(Event*));
 
 for (i=0; i < newTD->numPRE; i++) {
   fscanf(pFile, "%d", &currPRE);
   newTD->printRC[i] = createPrintRoadsEvent(currPRE);
 }
 /* close file */
 fclose( pFile ); 
 return newTD;       /* TODO: Replace this with your TrafficData pointer */
}

/* trafficSimulator
 * input: char* name of file to read
 * output: N/A
 *
 * Simulate the road network in the given TrafficData variable
 */
void trafficSimulator( TrafficData* pTrafficData )
{
 
/* TODO: complete this function */
 
/* Loop until all events processed and either all cars reached destination or gridlock has occurred */
 
/* Update the state of every traffic light */
 
/* Loop on events associated with this time step */
 
/* First try to move cars through the next intersection */
 
/* Second move waiting cars onto the end of each road if possible */
 
/* Third move cars forward on every road (only those that haven't moved yet this time step) */
 
 
 int timeStep = 0;
 int i;
 int j;
 Event* e = (Event*)malloc(sizeof(Event)); 
 RoadData* pRoad = (RoadData*)malloc(sizeof(RoadData)); 
 pTrafficData->isGridlock = false; 
 
 testGridlock (pTrafficData, timeStep);
 
 while ((isEmptyPQ(pTrafficData->eventQueue) == false) && ((pTrafficData->numCars != 0) || (pTrafficData->isGridlock == false))) {
   j = getFrontPriority(pTrafficData->eventQueue);
   if (j == timeStep) {
     e = dequeuePQ(pTrafficData->eventQueue);
     pRoad = e->pRoadData;
     if (e->eventCode == ADD_CAR_EVENT) {
       printf("\nSTEP %d - ADD_CAR_EVENT - Cars enqueued on road from %d to %d\n", timeStep, pRoad->startInter, pRoad->endInter);
       mergeCarQueues(pTrafficData, e, pRoad, timeStep);
     }
     else {
       printDestinations(pRoad, timeStep);
     }
     freeEvent(e);
     for (i=0; i < pTrafficData->numRoads - 1; i++) {
       pRoad = pTrafficData->roads[i];
       RoadData* pNextRoad = pTrafficData->roads[i+1];
       updateTrafficLight(pRoad, timeStep);
       moveCarNextRoad(pRoad, pNextRoad, timeStep);
       carFromQueueToRoad(pRoad, timeStep);
       carForward(pRoad);
     }
   }
   testGridlock(pTrafficData, timeStep);
   timeStep++;
 }
}

/* freeTrafficData
 * input: TrafficData* pTrafficData 
 * output: N/A
 *
 * Free the data in the TrafficData variable
 */
void freeTrafficData( TrafficData* pTrafficData )
{
  /* TODO: complete this function */
  int i;
  for (i=0; i < pTrafficData->numRoads; i++) {
    freeRoad(pTrafficData->roads[i]);
  }
  free(pTrafficData->roads);
  freePQ(pTrafficData->eventQueue);
  freeGraph(pTrafficData->roadNetworks);
  free(pTrafficData);
}

int max( int a, int b )
{
  if(a>b)
    return a;
 return b;
}

bool testGridlock(TrafficData* pTD, int timeStep)
{
 pTD->isGridlock = false;
 return false;
}

void mergeCarQueues (TrafficData* pTD, Event* e, RoadData* road, int tStep)
{
 if (tStep == e->eventTimeStep)
 {
   road = getEdgeData(pTD->roadNetworks, e->pRoadData->startInter, e->pRoadData->endInter);
   mergeQueues(e->pCarQueue, road->waitingCars);
 }
 return;
  
}
