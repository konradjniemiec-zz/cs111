#!/bin/bash
VAR=1
rm -f list_graph_2.dat
touch list_graph_2.dat

while [ $VAR -lt 26 ]; do
    STR="\t"$VAR"\t"`./sltest --threads=$VAR --iter=250 | tail -1 | rev | cut -d ' ' -f 2 | rev`"\t"`./sltest --threads=$VAR --iter=250 --sync=m| tail -1 | rev | cut -d ' ' -f 2 | rev`"\t"`./sltest --threads=$VAR --iter=250 --sync=s| tail -1 | rev | cut -d ' ' -f 2 | rev`"\t"
    echo -e $STR >> list_graph_2.dat
    let VAR+=1
done
