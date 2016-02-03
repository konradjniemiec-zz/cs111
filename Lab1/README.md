## cs111
###Lab 1A
####Avik Mohan 404339887  && Konrad Niemiec 804423978

Overall we have a solid solution to the 1A problem, with some solutions looking forward to future options added. We store our command threads in anticipation of wait, and have a flag bit integer to hold all file options we will add in the future portions.

In our implementation there are a couple of areas posibly limited in their efficacy. Our checkMemory function, for example, is quick and simple way to implement this feature in terms of writing code, but it is less efficient due to its checkingof both the thread and file arrays.

Additionally, we weren't anal about checking the incoming files Read and Write privileges in our implemntation of checkFD for the command option of the shell.

Aside from these issues, however, we feel we have a robust program. We implemented the specified behaviours and have numerous error check methods, including for our memory allocation and file accesses. We also pay close attention to allocatex memory to make sure we do not fail to free used resources.

###Lab 1B

The 1B portion of the lab has been completed. There are no known issues or errors. We were stuck for multiple days on a single error where the program would hang after a command was given. Eventually we found on piazza a TA's suggestion to close file descriptors after calling dup2, and this fixed the error. Test cases were added to test the new functions, all of which produce the expected result.


###Lab 1C

The 1C portion of the lab has been completed. The --profile option was implemented sucessfully with no apparent issues. additionally, we created 3 test cases to run in bash, simpsh, and exceline to compare the relative effectivenesses, as detialed in the report.pdf file.
