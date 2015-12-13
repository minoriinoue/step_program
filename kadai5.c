/*
1. Get x, y from csv file and put them into structure, also set min and max of x and y.
2. Set a path to go through - the starting point and goal should be close to minimize the distance i.e. devide the area of given dots to minimize the possible farthest distance.
3. Greedy method in an area.
4. Change area (method: changeDirection) and repeat 3.
5. Repeat 4 till the last area.
6. Connect starting and last dots since the first dot connects with a random point.
7. Show result.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define CHALLENGE_0 5
#define CHALLENGE_1 8
#define CHALLENGE_2 16
#define CHALLENGE_3 64
#define CHALLENGE_4 128
#define CHALLENGE_5 512
#define CHALLENGE_6 2048

typedef struct {
  double x;
  double y;
  double distance;
  int flag;
} dot;

int getNumOfCity(char filename[]);
void initialization(dot *heap, char filename[], int);
void findMinMax(dot *heap, double result[], int);
int findStartingPoint(dot *heap, int);
void findRoute(dot *heap, int cutnumber, double minmax[], int start, int route[]);
double calDistance(dot current, double lastpoint[]);
void setPivot(dot *heap, double distance, double lastpoint[], int);
void setRoute(int route[], int, int);
void setDirection(double widthX, double widthY, double threshold[], int cutnumber, int num);
void writeResult(int route[], int);

int main(int argc, char *argv[]) {
    
  /*get which challenge you'll work on*/
  int N = 0; /*number of cities*/
  if (argc != 2) {
    printf("usage: %s N\n", argv[0]);
    exit(1);
  }else{
    if((N = getNumOfCity(argv[1])) == -1){
      printf("Wrong file name. Try again.\n");
      exit(1);
    }
  }
    
  /*get an array of N for structure, dot*/
  dot *heap;
  heap = (dot *)malloc(sizeof(dot) * N);
  if(heap == NULL){
    printf("Failed to allocate memory.\n");
    exit(1);
  }
    
  /*if cutnumber is ....
             ___      _______      ____________
          2 |   |  4 |   |   |  6 |   |   |   |
            | | |    | | | | |    | | | | | | |
            | | |    | |___| |    | |___|___| |
            |___|    |_______|    |___________|
     starting point will always be at lower left. 
     The width of path except the first path is (horizontal length) / cutnumber
     the width of the first path is (vertical length) /cutnumber
  */
    
  int cutnumber;
  while(1){
    printf("Enter an even number you want to cut the screen into.\n");
    scanf("%d", &cutnumber);
    if(cutnumber == 0){
      cutnumber = 1;
      break;
    }else if(cutnumber % 2 == 0){
      break;
    }else printf("Even number please!\n");
  }
    
  /*open file and initialize dots*/
  initialization(heap, argv[1], N);
    
  /*find xy min and max*/
  double minmax[4]; /*minmax[4] = {min of x, max of x, min of y, max of y}*/
  findMinMax(heap, minmax, N);
    
  int *route;
  route = (int *)malloc(sizeof(int) * N);
  if(route == NULL){
    printf("Failed to allocate memoty for route\n");
    exit(1);
  }
  findRoute(heap, cutnumber, minmax, N, route);
    
  /*deleteCrossingRoute(heap, cutnumber, minmax, N, route);
    Will add this method later if had time*/
    
  /*write result into csv.file*/
  writeResult(route, N);
    
  free(route);
  free(heap);
  return 0;
}

int getNumOfCity(char filename[]){
    
  if(!strcmp(filename, "input_0.csv")){
    return CHALLENGE_0;
  }else if(!strcmp(filename, "input_1.csv")){
    return CHALLENGE_1;
  }else if(!strcmp(filename, "input_2.csv")){
    return CHALLENGE_2;
  }else if(!strcmp(filename, "input_3.csv")){
    return CHALLENGE_3;
  }else if(!strcmp(filename, "input_4.csv")){
    return CHALLENGE_4;
  }else if(!strcmp(filename, "input_5.csv")){
    return CHALLENGE_5;
  }else if(!strcmp(filename, "input_6.csv")){
    return CHALLENGE_6;
  }else{
    return -1;
  }
}

void initialization(dot *heap, char filename[], int numofcity){
    
  FILE *fp = fopen(filename, "r");
  if(fp == NULL){
    printf("Failed to open file.\n");
  }
    
  char hold;
  fscanf(fp, "%c,%c", &hold, &hold);
    
  for(int i = 0; i < numofcity; i++){
    fscanf(fp, "%lf,%lf", &heap[i].x, &heap[i].y);
    heap[i].flag = 0;
  }
  fclose(fp);
}

void findMinMax(dot *heap, double result[], int numofcity){
  /*initialize result value*/
  /*minmax[4] = {min of x, max of x, min of y, max of y}*/
  result[0] = heap[0].x;
  result[1] = heap[0].x;
  result[2] = heap[0].y;
  result[3] = heap[0].y;
    
  for(int i = 1; i < numofcity; i++){
    if(heap[i].x < result[0]) result[0] = heap[i].x;
    if(heap[i].x > result[1]) result[1] = heap[i].x;
    if(heap[i].y < result[2]) result[2] = heap[i].y;
    if(heap[i].y > result[3]) result[3] = heap[i].y;
  }
    
  /*for later use (for func. findRoute) so as not to miss dots on threshold*/
  result[1] += 1;
  result[3] += 1;
    
  return;
}

void findRoute(dot *heap, int cutnumber, double minmax[], int numofcity, int route[]){
  double widthX = (minmax[1] - minmax[0])/cutnumber; /*path width*/
  double widthY = (minmax[3] - minmax[2])/cutnumber; /*path hight*/
  double threshold[] = {minmax[0], minmax[0] + widthX, minmax[2], minmax[2] + widthY};
  double lastpoint[2] = {minmax[0], minmax[2] + widthY}; /*last dot's coordination in the previous area*/
  int dotcounter = 0;
  double total = 0;
    
  for(int i = 0; i < cutnumber * cutnumber; i++){
    int *dotholder; /*for dots in the area*/
    int size = 100;
    dotholder = (int *)malloc(sizeof(int) * size);
    if(dotholder == NULL){
      printf("Couldn't allocate memory for dotholder\n");
      exit(1);
    }
        
    int numcounter = 0;
    double min = INFINITY;
    int startingdot = 0;
        
    /*find dots in the area*/
    for(int j = 0; j < numofcity; j++){
      if(heap[j].flag != 1){
	if( heap[j].x >= threshold[0] && heap[j].x < threshold[1]
	    && heap[j].y >= threshold[2] && heap[j].y < threshold[3]){
	  double distance;
	  dotholder[numcounter] = j;
                
	  if( (distance = calDistance(heap[j], lastpoint)) < min ){
	    min = distance;
	    startingdot = j;
	  }
                
	  numcounter++;
	  if(numcounter > size){
	    size += 100;
	    dotholder = (int *)realloc(dotholder, sizeof(int) * size );
	  }
	}
      }
    }
    if(numcounter != 0){
      /* set startingpoint */
      setPivot(heap, min, lastpoint, startingdot);
      setRoute(route, dotcounter, startingdot);
      dotcounter++;
      total += min;
        
      for(int l = 0; l < numcounter - 1; l++){
	double minDistance = INFINITY;
	int nearest = 0;
	for(int j = 0; j < numcounter ; j++){
	  if(heap[dotholder[j]].flag == 0){
	    heap[dotholder[j]].distance = calDistance(heap[dotholder[j]], lastpoint);
	    if(heap[dotholder[j]].distance < minDistance){
	      minDistance = heap[dotholder[j]].distance;
	      nearest = dotholder[j];
	    }
	  }
	}
            
	total += minDistance;
	/*update pivot*/
	setPivot(heap, minDistance, lastpoint, nearest);
	setRoute(route, dotcounter, nearest);
	dotcounter++;
      }
    }
    free(dotholder);
    setDirection(widthX, widthY, threshold, cutnumber, i);
        
  }
    
  /*connect starting point and last point*/
  total = total - heap[route[0]].distance;
  total = total + calDistance(heap[route[0]], lastpoint);
    
  /*show result*/
  printf("The total length is %f\n", total);
  printf("For Confirmation. Number of dot: %d\n", dotcounter);
  return;
}

double calDistance(dot current, double lastpoint[]){
  double distance;
  distance = sqrt( pow(current.x - lastpoint[0], 2.0) + pow(current.y - lastpoint[1], 2.0) );
  return distance;
}

void setPivot(dot *heap, double distance, double lastpoint[], int nearest){
  heap[nearest].distance = distance;
  heap[nearest].flag = 1;
  lastpoint[0] = heap[nearest].x;
  lastpoint[1] = heap[nearest].y;
  return;
}

void setRoute(int route[], int dotcounter, int nearest){
  route[dotcounter] = nearest;
  return;
}

/*move to the next area*/
void setDirection(double widthX, double widthY, double threshold[], int cutnumber, int num){
  if(num >= 0 && num < cutnumber - 1){
    threshold[0] += widthX;
    threshold[1] += widthX;
  }else if(num == (cutnumber - 1)){
    threshold[2] += widthY;
    threshold[3] += widthY;
  }else if((num % (cutnumber - 1)) >= 1 && (num % (cutnumber - 1)) <= (cutnumber - 2)){
    int hold = num + ( (cutnumber - 1) - (num % (cutnumber - 1)));
    hold = hold / (cutnumber - 1);
    if(hold % 2 == 1){
      threshold[2] -= widthY;
      threshold[3] -= widthY;
    }else if(hold % 2 == 0){
      threshold[2] += widthY;
      threshold[3] += widthY;
    }
  }else if(num % (cutnumber -1) == 0){
    threshold[0] -= widthX;
    threshold[1] -= widthX;
  }
  return;
}

void writeResult(int route[], int N){
  FILE *fp;
  switch(N){
  case 5:
    fp = fopen("solution_yours_0.csv", "w");
    break;
  case 8:
    fp = fopen("solution_yours_1.csv", "w");
    break;
  case 16:
    fp = fopen("solution_yours_2.csv", "w");
    break;
  case 64:
    fp = fopen("solution_yours_3.csv", "w");
    break;
  case 128:
    fp = fopen("solution_yours_4.csv", "w");
    break;
  case 512:
    fp = fopen("solution_yours_5.csv", "w");
    break;
  case 2048:
    fp = fopen("solution_yours_6.csv", "w");
    break;
  default:
    printf("No such city case.\n");
    exit(1);
  }
    
  if(fp == NULL){
    printf("Failed to open result file.\n");
    exit(1);
  }
    
  fprintf(fp, "%s\n", "index");
  for(int i = 0; i < N; i++){
    fprintf(fp, "%d\n", route[i]);
  }
    
  fclose(fp);
  return;
}
