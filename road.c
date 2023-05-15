#include "road.h"

/* Suggested TODO - Write a function to create road.
 */
RoadData* createRoad (int start, int end, int length) {
 RoadData* newRoad = (RoadData*)malloc(sizeof(RoadData));
 newRoad->cars = (Car**) malloc(length * sizeof(Car*));
 newRoad->length = length;
 newRoad->waitingCars = createQueue();
 newRoad->startInter = start;
 newRoad->endInter = end;
 newRoad->isRed = true;
 //newRoad->numCars = 0;
 newRoad->on = 0;
 newRoad->off = 0;
 newRoad->reset = 0;
 
 return newRoad;
}

/* Suggested TODO - Write a function to free a road.
 */
void freeRoad (RoadData* road) {
 free(road->cars);
 //freeQueue(road->waitingCars);
 free(road);
}

/* Suggested TODO - Write a function that attempts to move the cars on a given road forward (if space ahead of them was empty).
 */
void carForward (RoadData* road) {
 int i;
 Car* currCar;
 Car* nextCar;
 
 if (road->isRed == true) {
    return;
 }
 
 for (i=0; i< road->length; i++) {         // move right to left
  currCar = road->cars[i];                 // current position
  if (currCar != NULL) {                   // checking if the current position is not empty
     if (currCar == road->cars[0]) {       // check if the car is already in first position
         currCar->moved = false;           // return to attempt to get on new road 
     }
     nextCar = road->cars[i-1];            // check if next space is open
     if (nextCar == NULL) {
        road->cars[i-1] = currCar;         // move car into next space
        road->cars[i] = NULL; 
        currCar->moved = true; 
     }
     else {                                // no open space, car did not move 
        currCar->moved = false;
     }
   }
 }
}

/* Suggested TODO - Write a function to attempt to move the car at the first position on a given road
 * to the last position of a another given road.
 */
void moveCarNextRoad (RoadData* currRoad, RoadData* nextRoad, int tStep) {
 Car* carMove;
 if (currRoad->isRed == true) {
    return;
 } 
 if (currRoad->cars[0] == NULL) {        // checking if there is a car in the first position
    return;
 }
 if (nextRoad->cars[nextRoad->length - 1] != NULL) { // checking if the last position of nextRoad is open 
    return;
 }
 
 carMove = currRoad->cars[0]; 
 currRoad->cars[0] = NULL;
 //currRoad->numCars--;
 nextRoad->cars[nextRoad->length - 1] = carMove;
 //nextRoad->numCars++;
 carMove->moved = true;
 
 if (carMove->destination == carMove->next) {        // check if the car reached desired destinatiom
    printf("STEP %d - Car successfully traveled from %d to %d in %d time steps.\n\n", tStep, carMove->origin, carMove->destination, tStep + 1);
    free(carMove);
 }
 return;
}

/* Suggested TODO - Write a function to attempt to move a car from the add car queue onto the end of this road.
 */
void carFromQueueToRoad (RoadData* road, int tStep) { 
 if (isEmpty(road->waitingCars) == true) {          // checking if there is a car in the queue
    return;
 }
 if (road->cars[road->length - 1] != NULL) {        // checking if the last position on the road is open
    return;
 }
 
 Car* carAdd = dequeue(road->waitingCars);      // removing the queue and adding it to carAdd
 road->cars[road->length -1] = carAdd;          // adding end of the road to carAdd
 carAdd->origin = road->startInter; 
 carAdd->stepAdded = tStep;                     // updating the carAdd origin
 //road->numCars++;                             // incrementing numCars on the road to keep track
 
 //printf("\nSTEP %d - ADD_CAR_EVENT - Cars enqueued on road from %d to %d\n", tStep, road->startInter, road->endInter);
 return;
}

/* Suggested TODO - Write a function to update the current state of the traffic light based on the current cycle.
 */
void updateTrafficLight (RoadData* road, int tStep) {
  int tempTStep = tStep;          // temp variable to allow manipulation to time step
 
  while (!(tempTStep >= 0 && tempTStep < road->reset)) {          // bring time step within cycle range
     tempTStep = tempTStep - road->reset;
  }
  if (tempTStep < road->on || tempTStep == road->off) {           // light is red until time step reaches greenOn or is greenOff
     road->isRed = true;
  }
  else if (tempTStep >= road->on && tempTStep < road->off) {        // light is green from greenOn to greenoff
     road->isRed = false;
  }
  
  return;
}

/* Suggested TODO - Write a function to print the destinations of the cars on the road
 */
//printf("Cars on the road from %d to %d: \n", ????, ????);
void printDestinations (RoadData* road, int tStep) {
 int i;
 printf("\nSTEP %d - PRINT_ROADS_EVENT - Current contents of the roads:\nCars on the road from %d to %d: \n", tStep, road->startInter, road->endInter);
 for (i=0; i < road->length; i++) {
   if (road->cars[i] != NULL) {
      printf("%d ", road->cars[i]->destination);
   }
   else {
      printf("- ");
   }
 }
 if (road->isRed == true) {
    printf("(RED light)\n\n");
 }
 else {
    printf("(GREEN light)\n\n");
 }
  
 return;
  
}
