
#include <stdio.h>
#include <getopt.h>

//////
#include <unistd.h> //for access in openFile

int main (int argc, char **argv){
  int op;
  struct option long_opts[]
  {
    {"verbose", no_argument, NULL, "v"},
    {"command", required_argument, NULL, 'c'},//'c' -> O_WRONLY?
    {"rdonly",required_argument,NULL, 'r'}, //'r' -> O_RDONLY?
    {"wronly",required_argument,NULL,'w' }
  };
  int option_index = 0;
  int isVerbose = 0;

//malloc the space for everyhting here??

//
  //while(true){ ???
//

int x = getopt_long(argc, argv,long_opts,&option_index);

  if (x== -1)
    break;
    //capture error?

  switch (x) {
    case 'v': {
    	isVerbose = 1
    	break;
    }
    case 'r': 
    case 'w': { //'r' -> O_RDONLY?
      openFile(x)
    }
    case 'c': {

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
