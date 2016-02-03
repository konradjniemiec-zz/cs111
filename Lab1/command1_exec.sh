#!/usr/local/cs/execline-2.1.4.5/bin/execlineb

/usr/local/cs/execline-2.1.4.5/bin/pipeline  { cat aasdf }  /usr/local/cs/execline-2.1.4.5/bin/pipeline  { tr " " "\n" }  /usr/local/cs/execline-2.1.4.5/bin/pipeline { tr "[a-z]" "A-Z]" } /usr/local/cs/execline-2.1.4.5/bin/redirfd -a 1 out sort
