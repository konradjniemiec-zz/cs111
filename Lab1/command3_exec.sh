#!/usr/local/cs/execline-2.1.4.5/bin/execlineb

/usr/local/cs/execline-2.1.4.5/bin/pipeline { cat b } /usr/local/cs/execline-2.1.4.5/bin/pipeline { tr " " "\n" } /usr/local/cs/execline-2.1.4.5/bin/pipeline { sort }  grep "abc" 										 										
