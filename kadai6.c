#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define INTMAX 2147483648

void reset(int *heap, int);
/*int decide_arraysize(int);*/

int main(int argc, char* argv[]){
  if (argc != 4) {
    printf("usage: %s <filename> <filesize(Byte)> <result filename>\n", argv[0]);
    exit(1);
  }
  char* filename = argv[1];
  long long int filesize = atoll(argv[2]);
  char* result_filename = argv[3];
    
  long long int num_int = filesize / sizeof(int);
    
  int array_size = INTMAX / (int)pow(2.0, 4.0);
    
  int num_loop = INTMAX / array_size;
    
  FILE *result = fopen(result_filename, "wb");
  if(result == NULL){
    printf("Cannot open result file\n");
    exit(1);
  }
    
  int* heap = (int *)malloc(array_size * sizeof(int));
  if(heap == NULL){
    printf("Cannot allocate memory.\n");
    exit(1);
  }
  reset(heap, array_size);
    
  for(int i = 0; i < num_loop ; i++){
        
    FILE* fp = fopen(filename, "rb");
    if(fp == NULL){
      printf("Cannot open file: %s\n", filename);
      exit(1);
    }
        
    int hold;
    for(long long int j = 0; j < num_int; j++){
      fread(&hold, sizeof(int), 1, fp);
      if(hold >= array_size * i && hold <= array_size * (i + 1) - 1){
	hold = hold - array_size * i;
	heap[hold]++;
      }
    }
        
    for(int j = 0; j < array_size ; j++){
      if(heap[j] != 0){
	for(int k = 0; k < heap[j]; k++){
	  hold = array_size * i + j;
	  fwrite(&hold, sizeof(int), 1, result);
	}
      }
    }
        
    reset(heap, array_size);
    fclose(fp);
  }
    
  free(heap);
  fclose(result);
  return 0;
}

void reset(int *heap, int array_size){
  for(int i = 0; i < array_size; i++){
    heap[i] = 0;
  }
  return;
}
