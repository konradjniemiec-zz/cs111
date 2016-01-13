#include <unistd.h> 
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
//////
static int verboseFlag;
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
void OpenFile(int flagg){
  if (verboseFlag) {
    //this is a complicated print
    //printf("%s-Only\n", ((c=='r') ? "READ" : "WRITE")
	
    //attempt to find it
    //use a switch on c to create  a string R_OK, W_OK, etc to use in access, calling it s
    //seemslike a lot of ppl juts use F_OK though
    //Also Iknow you use optarg here but what the hell cant find anything on it
    // like some secret keyword no one talks about
	  
    /*
    // Check to make sure that all of the requested descriptors exist
    for (int i = 0; i < 3; i++) {
    if (command_args[i][0] - '0' > n_files) {
    fprintf(stderr, "error: bad file descriptor\n");
    return NULL;
    }
    }
      
	  
	  
    if (access(optarg, s) != -1){
    //good
    }
    else{
    fprintf(stderr,"ERROR OPENING FILE");
    exit(0);
    }
    */	
    //update number files, check memory, good to go
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


int main (int argc, char **argv){
  //malloc for the fd's

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
    int x = getopt_long(argc, argv,"",long_opts,NULL);
    if (x== -1)
      break;
    //capture error?


    switch (x) {

    case 'v': {
      verboseFlag=1;
      break;
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
      int numArgs = 4; 
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
  return 0;
}

  
