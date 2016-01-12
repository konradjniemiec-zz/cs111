
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
    }
    case 'r': {
    }
    case 'c': {

    }
    case '?':
      break;
  }
  
  
  /*
    create structure for getopt_long
    while (we have an option left) {

    if (verboseFlag) printLineToSTDOUT

    switch on the option
    
    case rdonly f:
    open file f for reading
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
