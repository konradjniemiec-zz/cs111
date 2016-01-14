## cs111
###Lab 1A
####Avik Mohan && Konrad Niemiec

In our implementation there are a couple of areas posibly limited in their efficacy. Our checkMemory function, for example, is quick and simple way to implement this feature in terms of writing code, but it is less efficient due to its checkingof both the thread and file arrays.

Additionally, we weren't anal about checking the incoming files Read and Write priviledges in our implemntation of checkFD for the command option of the shell.

Aside from these issues, however, we feel we have a robust program. We implemented the specified behaviours and have numerous error check methods, including for our memory allocation and file accesses. We also pay close attention to allocatex memory to make sure we do not fail to free used resources.

