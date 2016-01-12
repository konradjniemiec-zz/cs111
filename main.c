
#include <stdio.h>
#include <getopt.h>

int verboseFlag = 0;

int main (int argc, char **argv){
  int op;
  struct option long_opts[]
  {
    {"verbose", no_argument, NULL, "v"},
      {"command", required_argument, NULL, 'c'},
	{"rdonly",required_argument,NULL, 'r'},
	  {"wronly",required_argument,NULL,'w' }
	  //more options go here
  };
  int option_index = 0;

  int c = getopt_long(argc, argv,long_opts,&option_index);

  if (c== -1)
    break;

  switch (c) {

  case 'v': {
    verboseFlag=1;
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
  }
    //          case wronly f:
    //open file f for writing
  case 'w': {
    if (optarg)
      {
	//optarg is our string
	//check fd
      }
    else {
      //NO ARGUMENT ERROR
    }

  }
  case 'c': {
    int stdin, stdout, stderr;
    //parse all args
    //check all fds
   
  }
  case '?':
    break;
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
