#!/bin/bash
VAR=1
rm -f add_graph_2.dat
touch add_graph_2.dat

while [ $VAR -lt 26 ]; do
    STR="\t"$VAR"\t"`./addtest --threads=$VAR --iter=500 | tail -1 | rev | cut -d ' ' -f 2 | rev`"\t"`./addtest --threads=$VAR --iter=500 --sync=m| tail -1 | rev | cut -d ' ' -f 2 | rev`"\t"`./addtest --threads=$VAR --iter=500 --sync=s| tail -1 | rev | cut -d ' ' -f 2 | rev`"\t"`./addtest --threads=$VAR --iter=500 --sync=c| tail -1 | rev | cut -d ' ' -f 2 | rev`
    echo -e $STR >> add_graph_2.dat
    let VAR+=1
done
