#define _XOPEN_SOURCE
#define _GNU_SOURCE
//fixes a bunch of compilation errors
#include <signal.h>
#include <unistd.h> 
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/signal.h>
#include <fcntl.h>


//////
/// dont wanna use static, ruins thread safety
int verboseFlag;
int errFlag;
int numFds;
int numThreads;
int* fds;
pid_t* threads;
int maxFds;
int maxThreads;
int flags = 0;
//////

int parseArgs(int* expected_arg_num,int i, int argc,char** dest_arg_arr, char** argv) {
  int counter = 1; //starts at 1 because optarg contains the first required argument
  while (i < argc) {
    if (strstr(argv[i],"--")) {
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
void sig_handler(int signum) {
  fprintf(stderr,"%d caught",signum);
  exit(signum);
}
void checkMem(){
  if(numFds >= maxFds){
    maxFds*=2;
    ///     if ((fds = realloc(fds, maxFds)) == NULL){
    fds = realloc(fds, maxFds*sizeof(int));
    if (fds== NULL){
      fprintf(stderr,"Error Reallocating Memory of Fds\n");
      exit(EXIT_FAILURE);
    }
  }
  if(numThreads >= maxThreads){
    maxThreads*=2;
    threads = realloc(threads, maxThreads);
    if (threads== NULL){
      fprintf(stderr,"Error Reallocating Memory of Threads\n");
      exit(EXIT_FAILURE);
    }

  }
}

int OpenFile(int c){
  if (verboseFlag) {
    //this is a complicated print
    switch(c){
    	case 'r':{
	  printf("--rdonly %s\n",optarg);
    	}
    	case 'w': {
	  printf("--wronly %s\n",optarg);
    	}
    	case '-': {
	  printf("--rdwr %s\n",optarg);
    	}
    }
  }
    //check existence for errors
    //use a switch on c to create  a string R_OK, W_OK, etc to use in access, calling it s???
    //seemslike a lot of ppl juts use F_OK though
  //if (access(optarg, s) != -1
  //switch(c){ case'r':
    if (access(optarg, F_OK) == -1) {
      fprintf(stderr, "Error: %s does not exist\n", optarg);
      return 0;
    }
    fds[numFds] = open(optarg, flags);
    if (fds[numFds] < 0) {
      fprintf(stderr,"Error opening : %s\n", optarg);
      return 0;
    }
    numFds++; 
    checkMem();
    return numFds-1;
}
int checkFD(int fd) {
  if (fd < 0 || fd > numFds) return 0;
  return (fcntl(fds[fd], F_GETFD) != -1);
}

int main (int argc, char **argv){
  ///might want to make these static globals so that we can easily write checkmem; EDIT: moved them
  numFds = 0;
  numThreads = 0; ///proccesses
  fds = malloc(100*sizeof(int)); /// 50 fd's good? 100? 500?
  threads = malloc(100*sizeof(int));
  if ((fds == NULL) || (threads == NULL)){
    fprintf(stderr,"Error Allocating Initial Memory\n");
  }
  maxFds = 100;
  maxThreads = 100;
  while(1){
    static struct option long_opts[] = 
      {
	{"verbose", no_argument, NULL, 'v'},
	{"command", required_argument, NULL, 'c'},
	{"rdonly",required_argument,NULL, 'r'},
	{"wronly",required_argument,NULL,'w' },
	{"rdwr",required_argument,NULL,'-'},
	{"pipe",no_argument,NULL,'p'},
	{"wait",no_argument,NULL,'a'},
	{"close",required_argument,NULL,'l'},
	{"abort",no_argument,NULL,'b'},
	{"catch",required_argument,NULL,'t'},
	{"ignore",required_argument,NULL,'i'},
	{"default",required_argument,NULL,'d'},
	{"pause",no_argument,NULL,'u'},
	/**/
        {"append",no_argument, 0, 'A'},
        {"cloexec",no_argument, 0, 'B'},
	{"creat", no_argument, 0, 'C'},
        {"directory",no_argument, 0, 'D'},
        {"dsync",no_argument, 0, 'E'},
        {"excl",no_argument, 0, 'F'},
        {"nofollow",no_argument, 0, 'G'},
        {"nonblock",no_argument, 0, 'H'},
        {"rsync",no_argument, 0, 'I'},
        {"sync",no_argument, 0, 'J'},
        {"trunc", no_argument, 0, 'K'},
	//more options go here
	{0,0,0,0}
      };
    int x = getopt_long(argc, argv,"",long_opts,NULL); //Not null, &some_option_index
    if (x== -1)
      break;
    switch (x) {
    case 'A': {
    	if (verboseFlag){
    		printf("--append ");
    	}
    	flags |= O_APPEND;
    }
    case 'B': {
    	if (verboseFlag){
    		printf("--cloexec ");
    	}
    	flags |= O_CLOEXEC;
    }
    case 'C': {
    	if (verboseFlag){
    		printf("--creat ");
    	}
    	flags |= O_CREAT;
    }
    case 'D': {
    	if (verboseFlag){
    		printf("--directory ");
    	}
    	flags |= O_DIRECTORY;
    }
    case 'E': {
    	if (verboseFlag){
    		printf("--dsync ");
    	}
    	flags |= O_DSYNC;
    }
    case 'F': {
    	if (verboseFlag){
    		printf("--excl ");
    	}
    	flags |= O_EXCL;
    }
    case 'G': {
    	if (verboseFlag){
    		printf("--nofollow ");
    	}
    	flags |= O_NOFOLLOW;
    }
    case 'H': {
    	if (verboseFlag){
    		printf("--nonblock ");
    	}
    	flags |= O_NONBLOCK;
    }
    case 'I': {
    	if (verboseFlag){
    		printf("--rsync ");
    	}
    	flags |= O_RSYNC;
    }
    case 'J': {
    	if (verboseFlag){
    		printf("--sync ");
    	}
    	flags |= O_SYNC;
    }
    case 'K': {
    	if (verboseFlag){
    		printf("--trunc ");
    	}
    	flags |= O_TRUNC;
    }
    
    case 'v': {
      verboseFlag=1;
      break;
    }
      //rdwr option
    case '-': {
      if (optarg)
	{
	  //optarg is our string
	  flags |= O_RDWR;
	  int fd= OpenFile(x);
	  if ((fd < 0) || !checkFD(fd)){
	    fprintf(stderr,"Error in opening file %s\n",optarg);
	    errFlag = 1;
	  }
	  flags = 0;
	}
      else {
	fprintf(stderr,"No arguments provided for --rdwr\n");
	errFlag = 1;
      }
      break;
    }
      //          case rdonly f:
      //open file f for reading
      /////////////
    case 'r': {
      if (optarg)
	{
	  //optarg is our string
	  flags |= O_RDONLY;
	  int fd= OpenFile(x);
	  if ((fd < 0) || !checkFD(fd)){
	    fprintf(stderr,"Error in opening file %s\n",optarg);
	    errFlag = 1;
	  }
	  flags = 0;
	}
      else {
	fprintf(stderr,"No arguments provided for --rdonly\n");
	errFlag = 1;
      }
      break;
    }
      //          case wronly f:
      //open file f for writing
    case 'w': {
      if (optarg)
	{
	  //optarg is our string
	  flags|= O_WRONLY;
	  int fd= OpenFile(x);
	  if ((fd < 0) || !checkFD(fd)){
	    fprintf(stderr,"Error in opening file %s",optarg);
	    errFlag = 1;
	  }
	  flags= 0;
	}
      else {
	fprintf(stderr,"No arguments provided for --wronly\n");      
	errFlag = 1;
      }
      break;
    }
      //pipe
    case 'p': {
      //use pipe system call
    }
      //catch
    case 't': {
      //catch signal optarg
      if (optarg)
	{
	  if (verboseFlag)
	    printf("--catch %s\n",optarg);
	  int signum = sscanf("%d",optarg);
	  struct sigaction action;
	  action.sa_handler = sig_handler;
	  sigemptyset(&action.sa_mask);
	  action.sa_flags = 0;
	  if (sigaction(signum,&action,NULL) < 0)
	    fprintf(stderr,"Error when creating handler for signal %s",optarg);
	}
      else 
	{
	  fprintf(stderr,"No arguments provided for --catch\n");
	  errFlag = 1;
	}
    }
    case 'd': {
      if (optarg)
	{
	  if (verboseFlag)
	    printf("--default %s\n",optarg);
	  int signum = sscanf("%d",optarg);
	  struct sigaction def;
	  def.sa_handler = SIG_DFL;
	  sigemptyset(&def.sa_mask);
	  def.sa_flags = 0;
	  if (sigaction(signum,&def,NULL) < 0)
	    fprintf(stderr,"Error when creating default handler");
	}
      else 
	{
	  fprintf(stderr,"No arguments provided for --default\n");
	  errFlag = 1;
	}
    }
    case 'u': {
      //pause
      if (verboseFlag)
	printf("--pause\n");
      pause();
    }
      //command 
    case 'c': {
      int numArgs = 4; 

      int _stdin, _stdout, _stderr;
      char** arg_array = malloc((numArgs+1) * sizeof(char*));
      if (optarg == NULL) {
	fprintf(stderr,"Not enough arguments for --command, found 0 need 4");
	errFlag = 1;
	break;
      }
      arg_array[0] = optarg;
      char** command_arg = arg_array+3;
      //parse args, if correct apply to optind for next options
      int returnVal = parseArgs(&numArgs,optind,argc,arg_array,argv);
      if (returnVal < 4) {
	fprintf(stderr,"Not enough arguments for --command, found %d need 4",returnVal);
	errFlag = 1;
	break;
      }
      if (verboseFlag) {
	printf("--command");
	for (int i = 0; i < returnVal; i++){
	  printf(" %s",arg_array[i]);
	}
	printf("\n");
        fflush(stdout);
      }
      //error checking //check if fd's are valid
      sscanf(arg_array[0],"%d",&_stdin);
      sscanf(arg_array[1],"%d",&_stdout);
      sscanf(arg_array[2],"%d",&_stderr);
      if (!checkFD(_stdin) || !checkFD(_stdout) || !checkFD(_stderr))
	{
	  fprintf(stderr,"ERROR in File Descriptor Options: %d %d %d\n",_stdin,_stdout,_stderr);
	errFlag = 1;
	}
      char * file = arg_array[3];
      pid_t child_pid = fork();
      if (child_pid==0) {
	//ChildProcess
	dup2(fds[_stdin],0); // actually go into file descriptor array
	dup2(fds[_stdout],1); // same for these
	dup2(fds[_stderr],2);
	execvp(file,command_arg);
	//print error if this comes back
	fprintf(stderr,"ERROR in command: %s\n",file);
	errFlag = 1;
	exit(EXIT_FAILURE);
      }
      else if (child_pid==-1) {
	fprintf(stderr,"ERROR in command: %s\n",file);
	errFlag = 1;
      }
      else {
	threads[numThreads++] = child_pid;
	//TODO store command_arg in a struct
	checkMem();
      }
      free(arg_array);
      break;
    }
      //wait option
    case 'a': {
      int exitStatus = 0;
      if (verboseFlag)
	printf("--wait\n");
      for (int i = 0; i < numThreads; i++) {
	int status;
	waitpid(threads[i],&status,0);
	
	if (status > exitStatus)
	  exitStatus = status;
	
	
	//TODO print exit status and copy of the command

      }
      errFlag = exitStatus;
    }
      //close option
    case 'l': {
      if (optarg)
	{
	  if (verboseFlag)
	    printf("--close %s",optarg);
	  //optarg is our string
	  int fd;
	  sscanf(optarg,"%d",&fd);
	  if (checkFD(fd)){
	    close(fd);
	  }
	  else {
	    fprintf(stderr,"Error in closing file %s\n",optarg);
	    errFlag = 1;
	  }

	}
      else {
	fprintf(stderr,"No arguments provided for --close\n");
	errFlag = 1;
      }
      break;


    }
    case 'b': {
      if (verboseFlag)
	printf("--abort\n");
      abort();
    }
    case '?':
      break;
    }
  }
  free(fds);
  free(threads);
  for (int k = 0; k < numFds; k++){
  	close(fds[numFds]);
  	numFds--;
  }
  return errFlag;
}

  
