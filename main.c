#include <unistd.h> 
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
//////
/// dont wanna use static, ruins thread safety
int verboseFlag;
int numFds;
int* numThreads;
int* fds;
int* threads;
int maxFds;
int maxThreads;
//////

int parseArgs(int* expected_arg_num,int i, int argc,char** dest_arg_arr, char** argv) {
  int counter = 1; //starts at 1 because optarg contains the first required argument
  while (i < argc) {
    if (strstr(argv[i],"--")) {
      printf("Found option so I stopped!\n");
      break; //if we hit a -- option
    }
    if (counter == *expected_arg_num) {
      *expected_arg_num = *expected_arg_num * 2;
      dest_arg_arr = realloc(dest_arg_arr, (*expected_arg_num + 1) * sizeof(char*));
    }
    
    dest_arg_arr[counter] = argv[i]; //maybe strcopy?
    i++; counter++;
  }
  optind = i;
  dest_arg_arr[counter]='\0';
  return counter;
}

void OpenFile(int c){
  if (verboseFlag) {
    //this is a complicated print
    printf("%s--Only\n", ((c=='r') ? "READ" : "WRITE"));
    
    //check existence for errors
    //use a switch on c to create  a string R_OK, W_OK, etc to use in access, calling it s???
    //seemslike a lot of ppl juts use F_OK though
  //if (access(optarg, s) != -1
  //switch(c){ case'r':
    if (access(optarg, F_OK) == -1) {
      printf(stderr, "Error: %s does not exist\n", optarg);
      exit(); ///need to add exit val's
    }
    else{
    	//file exists 
    ///	printf(stderr, "Found file: %s", optarg);
    	printf("Found file: %s", optarg);
    }
    if (fds[numFds] = open(optarg, c) ==-1){
      printf("Error opening : %s", optarg);
    }
    else{
    	numFds++;
    	///checkMem()
    }
    
  }
  /*
    if (verboseFlag) printLineToSTDOUT

    switch on the option
    

    case wronly f:
    open file f for writing
    case command i o e cmd args:
    execute a command with standard input i standard output o standard error e. Executable is cmd and has args args
    case verbose:
    Just before executing an option, output a line to standard output containing that option.
    verboseFlag=1;
    }
  */	  
}

/*
bool checkMem(){
	if(numFds >= maxFds){
		maxFds*=2;
	///	if ((fds = realloc(fds, maxFds)) == NULL){
		fds = realloc(fds, maxFds);
		if (fds== NULL){
			printf("Error Reallocating Memory");
			exit();
		}
	}
	if(numThreads >= maxThreads){
		maxThreads*=2;
		threads = realloc(threads, maxThreads);
		if (threads== NULL){
			printf("Error Reallocating Memory");
			exit();
		}
	
}
*/

int main (int argc, char **argv){
  ///might want to make these static globals so that we can easily write checkmem; EDIT: moved them
  numFds = 0;
  numThreads = 0; ///proccesses
  fds = malloc(100*sizeof(int)); /// 50 fd's good? 100? 500?
  threads = malloc(100*sizeof(int));
  if ((fds == NULL) || (threads == NULL)){
  	printf("Error Allocating Initial Memory")
  }
  int maxFds = (100*sizeof(int));///MUST be same as the array malloc'd in main
  int maxThreads = (100*sizeof(int));
  while(1){
    static struct option long_opts[] = 
      {
	{"verbose", no_argument, NULL, 'v'},
	{"command", required_argument, NULL, 'c'},
	{"rdonly",required_argument,NULL, 'r'},
	{"wronly",required_argument,NULL,'w' },
	//more options go here
	{0,0,0,0}
      };
    int x = getopt_long(argc, argv,"",long_opts,NULL); //Not null, &some_option_index
    if (x== -1)
      break;
    //capture error?


    switch (x) {

    case 'v': {
      verboseFlag=1;
      break; //is this break correct
    }
      //          case rdonly f:
      //open file f for reading
      /////////////
    case 'r': {
      if (optarg)
	{
	  //optarg is our string
	  //check fd
	}
      else {
	//NO ARGUMENT ERROR
      }
      break;
    }
      //          case wronly f:
      //open file f for writing
    case 'w': {
      if (optarg)
	{
	  //optarg is our string
	  //check fd
	  OpenFile(x); //add option for write
	}
      else {
	//NO ARGUMENT ERROR
      
      }
      break;
    }
    case 'c': {
      int numArgs = 4; ///hoist?

      int _stdin, _stdout, _stderr;
      char** arg_array = malloc((numArgs+1) * sizeof(char*));
      arg_array[0] = optarg;
      char** command_arg = arg_array+3;
      //parse args, if correct apply to optind for next options
      int returnVal = parseArgs(&numArgs,optind,argc,arg_array,argv);
      if (returnVal < 0) {
	//return error, set 
      }
      if (verboseFlag) {
	write(fileno(stdout),"--command",8);
	for (int i = 0; i < returnVal; i++){
	  write(fileno(stdout)," ",1);
	  write(fileno(stdout),argv[i],strlen(argv[i]));
	}
	write(fileno(stdout),"\n",1);
        fflush(stdout);
      }
      //error checking //check if fd's are valid
      sscanf(arg_array[0],"%d",&_stdin);
      sscanf(arg_array[1],"%d",&_stdout);
      sscanf(arg_array[2],"%d",&_stderr);

      char * file = arg_array[3];
      pid_t child_pid = fork();
      if (child_pid==0) {
	//ChildProcess
	dup2(_stdin,fileno(stdin)); // actually go into file descriptor array
	dup2(_stdout,fileno(stdout)); // same for these
	dup2(_stderr,fileno(stderr));
	execvp(file,command_arg);
	//print error if this comes back
	fprintf(stderr,"ERROR in command: %s",file);
	exit(255);
      }
      else if (child_pid==-1) {
	fprintf(stderr,"ERROR in command: %s",file);
      }
      free(arg_array);
      break;
      
      
      //check all fds

      //fork and execvp
   
    }
    case '?':
      break;
    }
  }
  free(fds);
  free(threads);
  return 0;
}

  
