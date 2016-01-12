
#include <stdio.h>
#include <getopt.h>

//////
#include <unistd.h> //for access in openFile

int parseArgs(int expected_arg_num,int i, int argc,char** dest_arg_arr, char** argv) {
  int counter = 0; //maybe 1? if we can include 
  while (i < argc) {
    if (argv[i][0]== '-' & argv[i][1]=='-') break; //if we hit a -- option
    if (counter >= expected_arg_num) return -1; //error too many arguments, might need to change this
    dest_arg_arr[counter] = argv[i]; //maybe strcopy?
      i++; counter++;
  }
  if (counter != expected_arg_num)
    return -1; //error, not enough arguments
  return i;
}

int main (int argc, char **argv){
  int op;
  int verboseFlag;
  struct option long_opts[]
  {
    {"verbose", no_argument, "", "v"},
      {"command", required_argument, "", 'c'},
	{"rdonly",required_argument,"", 'r'},
	  {"wronly",required_argument,"",'w' }
	  //more options go here
  };
  int option_index = 0;
  
  //malloc the space for everyhting here??

  while(true){


    int x = getopt_long(argc, argv,long_opts,&option_index);

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
	  openFile(x) //add option for write
	    }
      else {
	//NO ARGUMENT ERROR
      
      }
      break;
    }
    case 'c': {
      size_t numArgs = 3; //maybe 2? depends on if we get the first arg
      int stdin, stdout, stderr;
      char** arg_array = malloc(numArgs * sizeof(char*));
      //parse args, if correct apply to optin for next options
      int returnVal = parseArgs(numArgs,optin,argc,arg_array,argv);
      if (returnVal < 0) {
	//return error, set 
      }
      optin = returnVal;
      sscanf(arg_array[0],"%d",&stdin);
      sscanf(arg_array[1],"%d",&stdout);
      sscanf(arg_array[2],"%d",&stderr);

      pid_t child_pid;

      
      
      //check all fds

      //fork and execvp
   
    }
    case '?':
      break;
    }
  }

  
  void OpenFile(int flagg){
    if (isVerbose == 1)
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
      */
	  
	  
      if (access(opt_arg, s) != -1){
	//good
      }
      else{
	fprintf("ERROR OPENING FILE")
	  exit();
      }
	
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
