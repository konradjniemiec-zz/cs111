#!/bin/bash
VAR=1
rm -f list_graph_3.dat
touch list_graph_3.dat

STR="\t"1.00"\t"`./sltest --threads=1 --iter=500 --lists=1| tail -1 | rev | cut -d ' ' -f 2 | rev`"\t"`./sltest --threads=1 --iter=500 --lists=1 --sync=m | tail -1 | rev | cut -d ' ' -f 2 | rev`"\t"`./sltest --threads=1 --iter=500 --lists=1 --sync=s| tail -1 | rev | cut -d ' ' -f 2 | rev`
echo -e $STR >> list_graph_3.dat

while [ $VAR -lt 41 ]; do
    NUM=`bc <<< "scale=2 ; 20/$VAR"`
    STR="\t"$NUM"\t"`./sltest --threads=20 --iter=500 --lists=$VAR| tail -1 | rev | cut -d ' ' -f 2 | rev`"\t"`./sltest --threads=20 --iter=500 --lists=$VAR --sync=m | tail -1 | rev | cut -d ' ' -f 2 | rev`"\t"`./sltest --threads=20 --iter=500 --lists=$VAR --sync=s| tail -1 | rev | cut -d ' ' -f 2 | rev`
    echo -e $STR >> list_graph_3.dat
    let VAR+=1
done
