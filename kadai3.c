#include <stdio.h>
#include <stdlib.h>

typedef struct{
  int listnumber;
  int interlinked;
}List;

int ifinterlinked(int, int);
void sortheap(List *p, int);
void makeresult(List *p, int);

int main(void){

  int datasize = 1000000; /*assumed number of pages*/
    
  List *heap = (List *)malloc(sizeof(List)* datasize);
  if(heap == NULL){
    printf("failed to allocate memory\n");
    exit(1);
  }
    
  /*make a copy of links.txt*/
  FILE *check = fopen("check.txt", "r");
  if(check == NULL){
    /*if not copied before make a copy file*/
    printf("making a copy of links.txt\n");
    FILE *fp = fopen("links.txt", "r");
    FILE *copyfp = fopen("linkscopied.txt", "w");
    if(fp == NULL){
      printf("cannot find links.txt. Please get the file.\n");
      exit(1);
    }
    char hold;
    /*copy the entire file*/
    while((hold = fgetc(fp)) != EOF){
      fputc(hold, copyfp);
    }
    /*make check file for later use*/
    FILE *checkfp = fopen("check.txt", "w");
    fclose(fp);
    fclose(checkfp);
    fclose(copyfp);
  }
  fclose(check);
  printf("finished making linkscopied.txt\n");
    
  FILE *fp = fopen("links.txt", "r");
  if(fp == NULL){
    printf("cannot find links.txt\n");
    exit(1);
  }
  int largestpagenum = -1;
  int page, linked; /*page: # of the page; linked: # of the page linked to page*/
  while(fscanf(fp, "%d\t%d", &page, &linked) != EOF){
    if(page > largestpagenum){
      largestpagenum = page;
      printf("updated largestpagenumber to: %d\n", largestpagenum);
      /*initialization for new # page*/
      heap[largestpagenum].interlinked = 0;
      heap[largestpagenum].listnumber = page;
            
    }
    if(largestpagenum > datasize){
      datasize += 1000000;
      heap = (List *)realloc(heap, sizeof(List)* datasize);
    }
    if(ifinterlinked(page, linked)){
      heap[page].interlinked++;
      printf("updated interlinked number of page %d to: %d\n", page, heap[page].interlinked);
    }
  }
    
  sortheap(heap, largestpagenum);
  makeresult(heap, largestpagenum);
    
  free(heap);
  fclose(fp);
  return 0;
}

int ifinterlinked(int page, int linked){
  FILE *copyfp = fopen("linkscopied.txt", "r");
  if(copyfp == NULL){
    printf("cannot find linkscopied.txt\n");
    exit(1);
  }
    
  int pagehold, linkedhold;
  while(fscanf(copyfp, "%d\t%d", &pagehold, &linkedhold) != EOF){
    if(pagehold == linked && linkedhold == page){
      return 1;
    }else if(pagehold > linked){
      return 0; /*if checked all the choice*/
    }
  }
  fclose(copyfp);
  return 0;
}

void sortheap(List *heap, int pagenum){
  printf("sorting heap...\n");
  List hold;
  for(int i = 0; i <= pagenum; i++){
    for(int l = 0; l <= pagenum - 1; l++){
      if(heap[l].interlinked < heap[l+1].interlinked){
	hold = heap[l];
	heap[l] = heap[l+1];
	heap[l+1] = heap[l];
      }
    }
  }
  return;
}

void makeresult(List *heap, int pagenum){
  FILE *fphold = fopen("interlinkedrank.csv", "r");
  printf("outsourcing the result\n");
  /*delete privous file to avoid making multiple same files*/
  if(fphold != NULL){
    if(!remove("interlinkedrank.csv")){
      printf("deleted privious interlinkedrank.csv file\n");
    }else{
      printf("failed to delete file\n");
    }
  }
  fclose(fphold);
    
  FILE *fp = fopen("interlinkedrank.csv", "w");
  if(fp == NULL){
    printf("failed to open interlinkedrank.csv");
    exit(1);
  }
  for(int i = 0; i <= pagenum; i++){
    fprintf(fp, "%d,%d\n", heap[i].listnumber, heap[i].interlinked);
  }
  fclose(fp);
  printf("printed result in interlinkedrank.csv. Please make a graph in excel.\n");
    
  return;
}
