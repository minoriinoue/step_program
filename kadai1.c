#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

void lowcase(char array[], int);
void sortdictionary(void);
void count(char array[], int number[]);
void lookup(int array[], int);
int lookupinEachDic(int, int array[]);
void copyword(char array[], int);
int wordlength(char array[]);
int compare(int array[], int array1[]);

int main(void) {
    
  FILE *check = fopen("check.txt", "r");
  /*sort dictionary if not sorted before*/
  if(check == NULL){
    printf("Sorting dictionary...\n");
    sortdictionary();
  }
  fclose(check);
    
  char input[17] = {'0'};
  printf("Input 16 letters.\n");
  scanf("%s", input);
  lowcase(input, (int)strlen(input));
    
  int inputlength = wordlength(input);
    
  int numinput[27] = {0};
  /*count alphabets of input*/
  count(input, numinput);
    
  /*find word*/
  lookup(numinput, inputlength);
    
  return 0;
}

/*all the letters into lowercase*/
void lowcase(char array[], int length){
  for(int i = 0; i < length; i++){
    if(isupper(array[i])){
      array[i] = tolower(array[i]);
    }
  }
  return;
}


/*sort word according to the number of letters and put it into text file*/
void sortdictionary(){
  FILE *dict;
  dict = fopen("/usr/share/dict/words", "r");
    
  /*make this file for later use -- so as not to sort dictionary eachtime*/
  FILE *check;
  check = fopen("check.txt", "w");
  fclose(check);
    
    
  do {
    char hold[40] = {'0'};
    if(fscanf(dict, "%s", hold)== -1) break; /*if reaches to EOF, break*/
    int length = wordlength(hold);
    if(length <= 16){ /*if a word from dictionary is
			less than or equal to 16 letters, add to dictionary*/
      copyword(hold, length);
    }
        
  }while(1);
    
  fclose(dict);
    
  return;
}

/*count number of letters*/
int wordlength(char word[]){
  int counter;
  for(counter = 0; word[counter] != 0; counter++);
  return counter;
}

/*put word into each text file*/
void copyword(char word[], int wlength){
    
  FILE *eachdict = NULL;
    
  switch(wlength){
  case 16:
    eachdict = fopen("16letters.txt", "a");
    break;
  case 15:
    eachdict = fopen("15letters.txt", "a");
    break;
  case 14:
    eachdict = fopen("14letters.txt", "a");
    break;
  case 13:
    eachdict = fopen("13letters.txt", "a");
    break;
  case 12:
    eachdict = fopen("12letters.txt", "a");
    break;
  case 11:
    eachdict = fopen("11letters.txt", "a");
    break;
  case 10:
    eachdict = fopen("10letters.txt", "a");
    break;
  case 9:
    eachdict = fopen("9letters.txt", "a");
    break;
  case 8:
    eachdict = fopen("8letters.txt", "a");
    break;
  case 7:
    eachdict = fopen("7letters.txt", "a");
    break;
  case 6:
    eachdict = fopen("6letters.txt", "a");
    break;
  case 5:
    eachdict = fopen("5letters.txt", "a");
    break;
  case 4:
    eachdict = fopen("4letters.txt", "a");
    break;
  case 3:
    eachdict = fopen("3letters.txt", "a");
    break;
  case 2:
    eachdict = fopen("2letters.txt", "a");
    break;
  case 1:
    eachdict = fopen("1letters.txt", "a");
    break;
  default:
    break;
  }
  for(int i = 0; word[i] != 0; i++){
    fputc(word[i], eachdict);
  }
  fputc('\n', eachdict);
  fclose(eachdict);
    
  return;
}

/*count number of alphabets in each word: ex) number[27] = {#of'a', #of'b', ... }*/
void count(char array[], int number[]){
  for(int i = 0; array[i] != 0; i++){
    if(isalpha(array[i])){
      number[array[i]-'a']++;
    }
  }
  return;
}


void lookup(int numinput[], int inputlength){
  int noresult = 1;
    
  while(noresult && inputlength >= 1){
    if(lookupinEachDic(inputlength, numinput)){
      noresult = 0;
    }else{
      inputlength--;
    }
        
  }
  return;
}

int lookupinEachDic(int dicnum, int numinput[]){
    
  FILE *fp = NULL;
    
  switch(dicnum){
  case 16:
    fp = fopen("16letters.txt", "r");
    break;
  case 15:
    fp = fopen("15letters.txt", "r");
    break;
  case 14:
    fp = fopen("14letters.txt", "r");
    break;
  case 13:
    fp = fopen("13letters.txt", "r");
    break;
  case 12:
    fp = fopen("12letters.txt", "r");
    break;
  case 11:
    fp = fopen("11letters.txt", "r");
    break;
  case 10:
    fp = fopen("10letters.txt", "r");
    break;
  case 9:
    fp = fopen("9letters.txt", "r");
    break;
  case 8:
    fp = fopen("8letters.txt", "r");
    break;
  case 7:
    fp = fopen("7letters.txt", "r");
    break;
  case 6:
    fp = fopen("6letters.txt", "r");
    break;
  case 5:
    fp = fopen("5letters.txt", "r");
    break;
  case 4:
    fp = fopen("4letters.txt", "r");
    break;
  case 3:
    fp = fopen("3letters.txt", "r");
    break;
  case 2:
    fp = fopen("2letters.txt", "r");
    break;
  case 1:
    fp = fopen("1letters.txt", "r");
    break;
  default:
    break;
  }
    
  do{
    char wordhold[17] = {'0'};
    char wordhold2[17] = {'0'};
    int numarray[27] = {0};
    if(fscanf(fp, "%s", wordhold)==-1) break;
    strcpy(wordhold2, wordhold); /*in case this word matches the criteria*/
    lowcase(wordhold, (int)strlen(wordhold));
    count(wordhold, numarray);
    if(compare(numarray, numinput)){
      printf("%s\n", wordhold2);
      fclose(fp);
      return 1; /*return to tell the word found*/
    }
        
  }while(1);
    
  fclose(fp);
  return 0; /*if no word matches the criteria*/
    
}


int compare(int dicword[], int inputword[]){
  int negcounter = 0;
  for(int i = 0; i < 27; i++){
    int diff = inputword[i] - dicword[i];
    if(diff < 0){
      negcounter++;
    }
  }
  if(negcounter > 0) return 0; else return 1;
}
