#!/usr/local/cs/execline-2.1.4.5/bin/execlineb -P

/usr/local/cs/execline-2.1.4.5/bin/pipeline { cat a } /usr/local/cs/execline-2.1.4.5/bin/pipeline { tr " " "\n" } /usr/local/cs/execline-2.1.4.5/bin/pipeline { sort } /usr/local/cs/execline-2.1.4.5/bin/pipeline { shuf } /usr/local/cs/execline-2.1.4.5/bin/pipeline { sort } /usr/local/cs/execline-2.1.4.5/bin/pipeline { shuf } /usr/local/cs/execline-2.1.4.5/bin/pipeline { sort } /usr/local/cs/execline-2.1.4.5/bin/pipeline { shuf } /usr/local/cs/execline-2.1.4.5/bin/redirfd -w 1 o cat -
