#!/bin/bash
VAR=10
rm -f add_graph_1.dat
touch add_graph_1.dat

while [ $VAR -lt 5000 ]; do
    STR="\t"$VAR"\t"`./addtest --threads=1 --iter=$VAR | tail -1 | rev | cut -d ' ' -f 2 | rev`
    echo -e $STR >> add_graph_1.dat
    let VAR+=10
done
