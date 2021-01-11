#include "fileRdrLib.h"

/* FOR REFERENCE
//List of words within file
struct lWords{
  char word[100];
  struct lWords *next;
};

//List of files wihtin directory
struct lFiles{
  char fileName[200];
  struct lWords *headWord;
  struct lFiles *next;
  }; */


struct oPut* op = NULL;
//Malloc checker
void *safeMalc(size_t size){
  void* mem = malloc(size);
  if(mem == NULL){
    printf("Not enough memory to allocate. Try again. \n");
    return 0;
  }
  return mem;
}

//Add file to 1st linked list
void* addFile(void* args){
  struct lFiles *new = safeMalc(sizeof(*new));
  strcpy(new->fileName, ((struct arg_addFile*)args)->fileName);
  new->next = ((struct arg_addFile*)args)->head;
  ((struct arg_addFile*)args)->head = new;
  new->headWord = NULL;
  //printf("Pthread ended.\n");
}

char* toLowerCase(char* str){
  unsigned char *newstr = (unsigned char*)str;
  while(*newstr){
    *newstr = tolower(*newstr);
    newstr++;
  }
  return str;
}


//scans to see if words are already in files
int scan(struct lFiles* head, char* word){
  struct lWords* curr = head->headWord;    
  while (curr != NULL){
    if( (strcmp(toLowerCase(curr->word), toLowerCase(word))) == 0){
      curr->freq++;
      return 1;    
    } else {
      curr = curr->next;
    }
  }
    return 0;  
}

//void addWord(struct lFiles* head, char* word){
void* addWord(void* args){

  struct lWords* new = safeMalc(sizeof(*new));
  struct lWords* curr = safeMalc(sizeof(*curr));  
  char* newWord = toLowerCase(((struct arg_addWord*)args)->word);
  
  //Start with scan to see if word is already in list...
  if(scan(((struct arg_addWord*)args)->head, ((struct arg_addWord*)args)->word) == 1){
    //((struct arg_addWord*)args)->head->headWord->freq++;
    // printf("%s,%f\n",((struct arg_addWord*)args)->head->headWord,((struct arg_addWord*)args)->head->headWord->freq);
    ((struct arg_addWord*)args)->head->totalWrds++;
    //Else begin process of adding to list...  
  } else{
    new->freq = 1;
    if(((struct arg_addWord*)args)->head->headWord == NULL || (strcmp(((struct arg_addWord*)args)->head->headWord->word, newWord) >= 0)){    
      new->next = ((struct arg_addWord*)args)->head -> headWord;
      ((struct arg_addWord*)args)->head ->headWord = new;
      strcpy(new->word, ((struct arg_addWord*)args)->word);
    } else {
      curr = ((struct arg_addWord*)args)->head->headWord;
      char* currNextWord = curr->next->word;

      while(curr->next != NULL && (strcmp(currNextWord, newWord) < 0)){
	curr = curr->next;
	currNextWord = curr->next->word;
      }      
      strcpy(new->word,toLowerCase(toLowerCase(((struct arg_addWord*)args)->word)));
      new->next = curr->next;
      curr->next = new; 
    }
  
  ((struct arg_addWord*)args)->head->totalWrds++;
  }
}

int scanMath(struct lFiles* head, char* word){
  struct lWords* curr = head->headWord;
  while (curr != NULL){
    if( (strcmp(toLowerCase(curr->word), toLowerCase(word))) == 0){
      return 1;
    } else {
      curr = curr->next;
    }
  }
    return 0;
}

int retFreq(struct lFiles* head, char* word){
  struct lWords* curr = head->headWord;
  while(curr!=NULL){
    if(strcmp(curr->word,word)==0){
      return curr->freq;
    }
   curr= curr->next;
  }
}


void createOput(struct oPut* op, char* f1,char* f2, double val){
  struct oPut* new = safeMalc(sizeof(*new)); 
  struct oPut* curr= safeMalc(sizeof(*curr));



  if( val > 0.0 && val < 0.1){
    printf("\033[1;31m");
    printf("%f \"%s\" and \"%s\"\n",val,f1,f2);
    printf("\033[0m");
  } else if (val >0.1 && val < 0.15){
    printf("\033[1;33m");
    printf("%f \"%s\" and \"%s\"\n",val,f1,f2);
    printf("\033[0m");
  }else if (val >0.15 && val < 0.2){
    printf("\033[1;32m");
    printf("%f \"%s\" and \"%s\"\n",val,f1,f2);
    printf("\033[0m");
  }else if (val >0.2 && val < 0.25){
    printf("\033[1;36m");
    printf("%f \"%s\" and \"%s\"\n",val,f1,f2);
    printf("\033[0m");
  }else if (val >0.25 && val < 0.3){
    printf("\033[1;34m");
    printf("%f \"%s\" and \"%s\"\n",val,f1,f2);
    printf("\033[0m");
  }else if (val > 0.3){
    printf("\033[1;0m");
    printf("%f \"%s\" and \"%s\"\n",val,f1,f2);
    printf("\033[0m");
  }
  


  /*
  if(op == NULL){
    //printf("%f == %f\n",op->val, val);
    strcpy(new->f1, f1);
    strcpy(new->f2, f2);
    new->val = val;
    new->next=op;
    op = new;
  } else {
    curr = op;
    if(curr->val < val){
      strcpy(new->f1, f1);
      strcpy(new->f2, f2);
      new->val = val;
      new->next = curr->next;
      op = new;
    }
    
  while(curr->next!=NULL && curr->val > val){
    //printf("%f == %f\n",curr->val, val);
    curr=curr->next;
  } 
   strcpy(new->f1, f1);
   strcpy(new->f2, f2);
   new->val = val;
   new->next = curr->next;
   curr->next = new;
  }

  */
}
/*
void prnOpt(struct oPut* head){
  struct oPut* curr = safeMalc(sizeof(*curr));
  curr = head;
  while(curr !=NULL){
  printf("%s and %s = %f",curr->f1,curr->f2,curr->val);
  curr = curr->next;
  }
}
*/

void meanCons(struct lFiles *headA, struct lFiles *headB){
  double avg;
  double totA = headA->totalWrds;
  double totB = headB->totalWrds;
  int total = totA+totB;
  double *mCon=malloc(total*sizeof(double));
  struct lWords* currA = safeMalc(sizeof(*currA));
  struct lWords* currB = safeMalc(sizeof(*currB));

  currA = headA->headWord;
  currB = headB->headWord;
  
  double avgA = 0.0;
  double avgB = 0.0;
  double sumA = 0.0;
  double sumB = 0.0;
  double sums = 0.0;
  int simis = 0;
  int i = 0;

  //Get similar tokens
  while(currA!=NULL){
      if(scanMath(headA,currA->word)==scanMath(headB,currA->word)){
	avgA = (currA->freq/totA);
        avgB = (retFreq(headB,currA->word)/totB);
        mCon[i] = ((avgA)+(avgB))/2;
        //printf("Word: %s Mean: %f\n",currA->word,mCon[i]);
	
	sumA = sumA + (avgA)*log((avgA)/mCon[i]);
        sumB = sumB + (avgB)*log((avgB)/mCon[i]);
       	
	i++;
        currA=currA->next;
      } else if(scanMath(headA,currA->word)>scanMath(headB,currA->word)){
       
	avgA = (currA->freq/totA);
        avgB = 0.0;
        mCon[i] = ((avgA)+(avgB))/2;
        //printf("Word: %s Mean: %f\n",currA->word,mCon[i]);
	
	sumA = sumA + (avgA)*log((avgA)/mCon[i]);
        sumB = sumB + 0.0;//(avgB)*log((avgB)/mCon[i]);	
	
	i++;
        currA=currA->next;
      }
  }

    while(currB!=NULL){
      if(scanMath(headB,currB->word)==scanMath(headA,currB->word)){
       
	currB=currB->next;
	continue;
      } else if(scanMath(headB,currB->word)>scanMath(headA,currB->word)){
	
	avgB = (currB->freq/totB);
        avgA = 0.0;
        mCon[i] = ((avgA)+(avgB))/2;
        //printf("Word: %s Mean: %f\n",currB->word,mCon[i]);

	sumA = sumA + 0.0;//(avgA)*log((avgA)/mCon[i]);
        sumB = sumB + (avgB)*log((avgB)/mCon[i]);
	i++;
        currB=currB->next;
      }
    }


  /*
i=0;
for(i;i<simis;i++ ){
  printf("Mean Cons: %f\n",mCon[i]);
}
  */
    
    sums = (sumA + sumB)/2;
    // printf("%f \"%s\" and \"%s\"\n",sums,headA->fileName,headB->fileName );

    createOput(op,headA->fileName,headB->fileName,sums);
    // prnOpt(op);
    
    
    free(mCon);
}


//Prints output
void readLLLL(struct lFiles *currF){
  double avg;
  while(currF != NULL){
    printf("%s\n", currF->fileName);
    printf("File: %s Word Count: %f\n", currF->fileName,currF->totalWrds);
    struct lWords *currW = currF->headWord;
    if(currW == NULL){
      printf("Empty.\n");
    }else{
      while(currW != NULL){
        avg = (double)currW->freq / (double)currF->totalWrds;
        printf("\t Word: %s Freq: %f Avg: %f\n",currW->word,currW->freq, avg);
        currW = currW->next;
      }
    }
    currF = currF->next;
  }
  
}

/*
int main(){
  struct lFiles* headFile = NULL;
  char* one = "aa";
  
  addFile(&headFile, "1st.txt");
  addWord(headFile,one);
  addWord(headFile,"A");
  addWord(headFile,"b");
  addWord(headFile,"c");
  addWord(headFile, "d");
  addWord(headFile, "e");
  addWord(headFile, "f");
  addWord(headFile, "G");



  
  /*
  addFile(&headFile, "2st.txt");
  addWord(headFile, "2stWord");

  addFile(&headFile, "3st.txt");
  addWord(headFile, "3stWord");

  addWord(headFile, "3stWord");
  
  
 
  readLLLL(headFile);

  return 0;
}

*/


