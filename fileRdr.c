#include "fileRdrLib.h"

/* FOR REFERENCE with Threading
int err;
struct arg_addWord{
  struct lFiles *head;
  char* word;
};
struct arg_addFile{
  struct lFiles **head;
  char *fileName;
};
*/

//GLOBALS - err = threading errors / *head = initialization of LL 
int err;
struct lFiles *head = NULL;
//struct oPut *op = NULL;
//struct mCons *headCon = NULL;
//Adds files to linked list.
void fileHndlr(struct stat fileinfo, struct dirent *dp, DIR* dirp){
  const char *delims = " \t\r\n\v\f,.'!?";
  const char *filename = "";

  //File variables.
  int f = open(dp->d_name,O_RDONLY);
  int size = lseek(f, 0, SEEK_END);
  char buffer [size];
  char *lastTok;
  FILE *fileName = fopen(dp->d_name, "r");
  int fLen = strlen(dp->d_name);

  //THREAD and MUTEX Variables  
  pthread_t thread;
  pthread_mutex_t mut;
  pthread_mutex_init(&mut,NULL);

  //IF txt file then begin to add to LL...
  if(strncmp(dp->d_name + fLen -4 , ".txt",4) == 0){

    //LOCK and initiate thread
    pthread_mutex_lock(&mut);
    struct arg_addFile* aFile = safeMalc(sizeof(*aFile));
    aFile->head = head;
    aFile->fileName = dp->d_name;
    //Create PThread
    err = pthread_create(&thread, NULL, addFile, (void*)aFile);        
    if(err){
      printf("Error creating thread.\n");
      exit(EXIT_FAILURE);
    }
    //Join PThread
    err = pthread_join(thread, (void*)aFile);    
    if (err){
      printf("Error joining thread. \n");
      exit (EXIT_FAILURE);
    }
    //Update head
    head = aFile->head;
    //UNLOCK
    pthread_mutex_unlock(&mut);
    //REFERENCE - addFile(&head,dp->d_name);    
    //Tokenize and add words to LL
    while ( fgets(buffer, size, fileName)){
      lastTok = strtok(buffer, delims);
      while( lastTok != NULL){	   	    
	int strLen = strlen(lastTok);
	//add word to LLoLL
	 //LOCK and initiate thread
	pthread_mutex_lock(&mut);
	struct arg_addWord* aWord = safeMalc(sizeof(*aWord));
	aWord->head = aFile->head;
	aWord->word = lastTok;
	//Create PThread
	err = pthread_create(&thread, NULL, addWord, (void*)aWord);
	if(err){
	  printf("Error creating thread.\n");
	  exit(EXIT_FAILURE);
	}
	//Join PThread
	err = pthread_join(thread, (void*)aWord);
	if (err){
	  printf("Error joining thread. \n");
	  exit (EXIT_FAILURE);
	}
	//addWord(aFile->head,lastTok);
	lastTok = strtok(NULL, delims);
	//UNLOCK
	pthread_mutex_unlock(&mut);
      }
    }	
  }else{
    return;
  }  
}
  

void directHndlr(char* direcName){  
  DIR *dirp = opendir(direcName);
  struct dirent *dp;
  struct stat fileinfo;
  chdir(direcName);  
      while(dp = readdir(dirp)){
	lstat(dp->d_name,&fileinfo);
	//If Directory then begin process to open
       if(S_ISDIR(fileinfo.st_mode)){
	 if( (strcmp(".",dp->d_name)== 0) || (strcmp("..",dp->d_name) == 0)){
	   continue;
	 }
	 directHndlr(dp -> d_name);	 
       }else if(S_ISREG(fileinfo.st_mode)){
	 fileHndlr(fileinfo, dp, dirp);	
       } else {
	 return;
       }
      }
      chdir("..");
      closedir(dirp);     
}

//MAIN
void main(int argc, char *argv[]){
  
  if( argc != 2) {
    printf("Wrong number of arguments. Try again.\n");
    return;
  }
  
  DIR* dir = opendir(argv[1]);
  if(dir){
    closedir(dir);
  }else if (ENOENT == errno){
    printf("Directory does not exist.\n");
    return;   
  }
  
  //opendir("argv[1]");
  
  
  directHndlr(argv[1]);

  if(head == NULL){
    printf("No files to read.\n");
    return;
  }

  if(head->next == NULL){
    printf("Only one file.\n");
    return;
  }
  
  readLLLL(head);
  
  struct lFiles* curr = safeMalc(sizeof(*curr));
  struct lFiles* currN = safeMalc(sizeof(*currN));

  curr = head;
  currN = head->next;
  
  while(curr != NULL){
    
    while(currN != NULL){
      //printf("%s,%s \n",curr,currN);
      meanCons(curr,currN);
      currN=currN->next;
    
    }

    curr = curr->next;
    currN = curr->next;
    if(currN == NULL){break;}
    // printf("Curr: %s CurrN: %s\n",curr,currN);
    }  
  // readLLLL(head);
  
  /*
  while(head->headWord!=NULL){
    while(head->next->headWord!=NULL){ 
      meanCons(head->headWord,head->next->headWord);
      head->next->headWord = head->next->next->headWord;
    }
    head->headWord = head->next->headWord;
  }
  */
  return;

}

  /*
  
  //Directory Variables
  DIR *dirp = opendir(argv[1]);
  struct dirent *dp;
  struct stat fileinfo;
  chdir(dp->d_name);
  //FileReading Variables
  //const char *delims = " \t\r\n\v\f";
  // const char *filename = "";


    //while dp has a file read
  while ((dp = readdir(dirp))){
    stat(dp->d_name,&fileinfo);  //get file info to determine type
    
    if(S_ISDIR(fileinfo.st_mode)){
      directHndlr(fileinfo, dp, dirp);
    }else if(S_ISREG(fileinfo.st_mode)){
      fileHndlr(fileinfo, dp, dirp);
    }
  }
  closedir(dirp);
  return;

*/
  
