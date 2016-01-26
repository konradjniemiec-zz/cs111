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
#include <errno.h>
struct thread_info {
  char* str;
  pid_t pid;
};
//////
/// dont wanna use static, ruins thread safety
int verboseFlag;
int errFlag;
int waitFlag;
int numPipes;
int numFds;
int numThreads;
int* pipes;
int* fds;
struct thread_info* threads;
int maxFds;
int maxThreads;
int maxPipes;
int flags = 0;
//////

char * buildStr(char** argv) {
  int size = strlen(argv[0])+1;
  char * buf = malloc(size * sizeof(char));
  strcpy(buf,argv[0]);
  argv++;
  while (argv[0] != 0)
    {
      if (strlen(buf) + strlen(argv[0])+1 >= size) {
	size = 2 * (strlen(buf) + strlen(argv[0]) + 1);
	buf = realloc(buf,size);
      }
      strcat(strcat(buf," "),argv[0]);
      argv++;
    }
  return buf;
}

int parseArgs(int* expected_arg_num,int i, int argc,char*** _arg_array, char** argv) {
  int counter = 1; //starts at 1 because optarg contains the first required argument
  int argnum = *expected_arg_num;
  char** dest_arg_arr = *_arg_array;
  while (i < argc) {
    if (strstr(argv[i],"--")) {
      break; //if we hit a -- option
    }
    if (counter == argnum) {
      argnum = argnum * 2;
      dest_arg_arr = realloc(dest_arg_arr, (argnum + 1) * sizeof(char*));
    }
    
    dest_arg_arr[counter] = argv[i]; //maybe strcopy?
    i++; counter++;
  }
  optind = i;
  dest_arg_arr[counter]='\0';
  *expected_arg_num = argnum;
  *_arg_array = dest_arg_arr;
  return counter;
}
void sig_handler(int signum) {
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
  if(numPipes >= maxPipes) {
    maxPipes *=2;
    pipes = realloc(pipes,maxPipes);
    if (pipes == NULL) {
      fprintf(stderr,"Error Reallocating Memory of Pipes\n");
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
	  break;
    	}
    	case 'w': {
	  printf("--wronly %s\n",optarg);
	  break;
    	}
    	case '-': {
	  printf("--rdwr %s\n",optarg);
	  break;
    	}
    }
  }
  if ((flags & O_CREAT) == O_CREAT) {
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    fds[numFds] = open(optarg, flags,mode);
    if (fds[numFds] < 0) {
      fprintf(stderr,"Error opening : %s\n", optarg);
      return 0;
    }
    numFds++; 
    fprintf(stderr,"Opening file with create: %d %d\n",numFds-1,fds[numFds-1]);
    checkMem();
    

    return numFds-1;
  }
  switch(c){
  case 'r':{
    if (access(optarg, R_OK) == -1) {
      fprintf(stderr, "Error: Read Permission Denied\n", optarg);
      return -1;
    }
    break;
  }
  case 'w': {
    if (access(optarg, W_OK) == -1) {
      fprintf(stderr, "Error: Write Permission Denied\n", optarg);
      return -1;
    }
    break;
  }
  case '-': {
    if (access(optarg, R_OK) == -1 && access(optarg, W_OK) == -1) {
      fprintf(stderr, "Error: Read-Write Permission Denied\n", optarg);
      return -1;
    }
    break;
  }
  }
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
    fprintf(stderr,"Opening file with without create: %d %d\n",numFds-1,fds[numFds-1]);
    return numFds-1;
}
int isPipe(int fd) {
  int realFd = fds[fd];
  for (int i = 0; i < numPipes; i++) {
    if (pipes[i] == realFd) {
      return 1;
    }
  }
  return 0;
}
int checkFD(int fd) {
  if (fd < 0 || fd > numFds) {
    fprintf(stderr,"fd out of range %d\n",fd);
    return 0;
  }
  return (fcntl(fds[fd], F_GETFD) != -1);
}
int closePipeCompanion(int fd) {
  if (checkFD(fd)) {
    fprintf(stderr,"Closing companion %d %d\n",fd,fds[fd]);
    return (close(fds[fd])==0);
  }
  return 0;
}
int checkReadPipe(int fd) {
  if (isPipe(fd) && isPipe(fd+1)) {
    return closePipeCompanion(fd+1);
  }
  return 0;
}
int checkWritePipe(int fd) {
  if (isPipe(fd) && isPipe(fd-1)) {
    return closePipeCompanion(fd-1);
  }
  return 0;
}


int main (int argc, char **argv){
  ///might want to make these static globals so that we can easily write checkmem; EDIT: moved them
  waitFlag = 0;
  numFds = 0;
  numThreads = 0; ///proccesses
  numPipes = 0;
  maxFds = 100;
  maxThreads = 100;
  maxPipes = 25;
  fds = malloc(maxFds*sizeof(int)); /// 50 fd's good? 100? 500?
  threads = malloc(maxThreads*sizeof(struct thread_info));
  pipes = malloc(maxPipes*sizeof(int));
  if ((fds == NULL) || (threads == NULL) || (pipes == NULL)){
    fprintf(stderr,"Error Allocating Initial Memory\n");
  }
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
	break;
    }
    case 'B': {
    	if (verboseFlag){
    		printf("--cloexec ");
    	}
    	flags |= O_CLOEXEC;
	break;
    }
    case 'C': {
    	if (verboseFlag){
    		printf("--creat ");
    	}
    	flags |= O_CREAT;
	break;
    }
    case 'D': {
    	if (verboseFlag){
    		printf("--directory ");
    	}
    	flags |= O_DIRECTORY;
	break;
    }
    case 'E': {
    	if (verboseFlag){
    		printf("--dsync ");
    	}
    	flags |= O_DSYNC;
	break;
    }
    case 'F': {
    	if (verboseFlag){
    		printf("--excl ");
    	}
    	flags |= O_EXCL;
	break;
    }
    case 'G': {
    	if (verboseFlag){
    		printf("--nofollow ");
    	}
    	flags |= O_NOFOLLOW;
	break;
    }
    case 'H': {
    	if (verboseFlag){
    		printf("--nonblock ");
    	}
    	flags |= O_NONBLOCK;
	break;
    }
    case 'I': {
    	if (verboseFlag){
    		printf("--rsync ");
    	}
    	flags |= O_RSYNC;
	break;
    }
    case 'J': {
    	if (verboseFlag){
    		printf("--sync ");
    	}
    	flags |= O_SYNC;
	break;
    }
    case 'K': {
    	if (verboseFlag){
    		printf("--trunc ");
    	}
    	flags |= O_TRUNC;
	break;
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
	    fprintf(stderr,"Error in opening file %s\n",optarg);
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
      if (verboseFlag)
	printf("--pipe\n");
      int pipeaddr[2];
      if (pipe (pipeaddr) == -1)
	{
	  fprintf(stderr,"Error when opening pipe\n");
	  break;
	}
      fds[numFds]=pipeaddr[0];
      pipes[numPipes]=pipeaddr[0];
      numFds++;
      numPipes++;
      checkMem();
      fds[numFds]=pipeaddr[1];
      pipes[numPipes]=pipeaddr[1];
      numPipes++;
      numFds++;
      fprintf(stderr,"Opening pipes Read: %d %d %d and and Write: %d %d %d\n",numFds-2,fds[numFds-2],pipes[numPipes-2],numFds-1,fds[numFds-1],pipes[numPipes-1]);
      checkMem();
      
      break;
    }
      //catch
    case 't': {
      //catch signal optarg
      if (optarg)
	{
	  if (verboseFlag)
	    printf("--catch %s\n",optarg);
	  int signum;
	  sscanf(optarg,"%d",&signum);
	  struct sigaction action;
	  action.sa_handler = sig_handler;
	  sigfillset(&action.sa_mask);
	  action.sa_flags = 0;
	  if (sigaction(signum,&action,NULL) == -1)
	    fprintf(stderr,"Error when creating handler for signal %s\n",optarg);
	}
      else 
	{
	  fprintf(stderr,"No arguments provided for --catch\n");
	  errFlag = 1;
	}
      break;
    }
    case 'd': {
      if (optarg)
	{
	  if (verboseFlag)
	    printf("--default %s\n",optarg);
	  int signum;
	  sscanf(optarg,"%d",&signum);
	  struct sigaction def;
	  def.sa_handler = SIG_DFL;
	  sigemptyset(&def.sa_mask);
	  def.sa_flags = 0;
	  if (sigaction(signum,&def,NULL) < 0)
	    fprintf(stderr,"Error when creating default handler\n");
	}
      else 
	{
	  fprintf(stderr,"No arguments provided for --default\n");
	  errFlag = 1;
	}
      break;
    }
    case 'i': {
      if (optarg)
	{
	  if (verboseFlag)
	    printf("--ignore %s\n",optarg);
	  int signum;
	  sscanf(optarg,"%d",&signum);
	  struct sigaction def;
	  def.sa_handler =  SIG_IGN;
	  sigemptyset(&def.sa_mask);
	  def.sa_flags = 0;
	  if (sigaction(signum,&def,NULL) < 0)
	    fprintf(stderr,"Error when creating ignore handler\n");
	}
      else
	{
	  fprintf(stderr,"No arguments provided for --default\n");
	  errFlag = 1;
	}
      break;
    }
    case 'u': {
      //pause
      if (verboseFlag)
	printf("--pause\n");
      pause();
      break;
    }
      //command 
    case 'c': {
      int numArgs = 4; 
      int _stdin, _stdout, _stderr;
      char** arg_array = malloc((numArgs+1) * sizeof(char*));
      if (optarg == NULL) {
	fprintf(stderr,"Not enough arguments for --command, found 0 need 4\n");
	errFlag = 1;
	break;
      }
      char** command_arg = arg_array+3;
            arg_array[0] = optarg;

      //parse args, if correct apply to optind for next options
      int returnVal = parseArgs(&numArgs,optind,argc,&arg_array,argv);
      if (returnVal < 4) {
	fprintf(stderr,"Not enough arguments for --command, found %d need 4\n",returnVal);
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
	fprintf(stderr,"Again %d %d %d\n",checkReadPipe(_stdin),checkWritePipe(_stdout),checkWritePipe(_stderr));
	dup2(fds[_stdin],0); // actually go into file descriptor array
	dup2(fds[_stdout],1); // same for these
	dup2(fds[_stderr],2);
	execvp(file,command_arg);
	//print error if this comes back
	fprintf(stderr,"ERROR in command: %s\n",file);
	errFlag = 1;
	_exit(EXIT_FAILURE);
      }
      else if (child_pid==-1) {
	fprintf(stderr,"ERROR in command: %s\n",file);
	errFlag = 1;
      }
      else {
	threads[numThreads].pid = child_pid;
	threads[numThreads++].str = buildStr(command_arg);
	checkMem();
      }
      free(arg_array);
      break;
    }
      //wait option
    case 'a': {
            if (verboseFlag)
	printf("--wait\n");

      waitFlag = 1;
      break;
    }
      //close option
    case 'l': {
      if (optarg)
	{
	  if (verboseFlag)
	    printf("--close %s\n",optarg);
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
      break;
    }
    case '?':
      break;
    }
  }  
  for (int k = 0; k < numFds; k++){
    fprintf(stderr,"Closing fd: %d %d\n",k,fds[k]);
    close(fds[k]);
  }
  if (waitFlag) {
    int exitStatus = 0;
    int status;
    pid_t pid;
    while ((pid = waitpid(-1,&status,WUNTRACED)) > 0) {
      int j;
      for (int i = 0; i < numThreads; i++) {
	if (threads[i].pid==pid) {
	  j = i;
	  break;
	}
      }
      int realStatus = WEXITSTATUS(status);
      if (realStatus > exitStatus)
	exitStatus = realStatus;

      printf("%d %s\n",realStatus,threads[j].str);
      free(threads[j].str);
    }
    errFlag = exitStatus;
  }
  free(fds);
  free(threads);
  free(pipes);
  return errFlag;
}

  
