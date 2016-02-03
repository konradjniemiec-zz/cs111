#!/usr/local/cs/execline-2.1.4.5/bin/execlineb -P

pipeline { cat a2 } pipeline { tr " " "\n" } pipeline { sort } pipeline { shuf } pipeline { sort } pipeline { shuf } pipeline { sort } pipeline { shuf } redirfd -w 1 o cat -
