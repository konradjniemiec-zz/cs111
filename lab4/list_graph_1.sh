#!/bin/bash
VAR=10
rm -f list_graph_1.dat
touch list_graph_1.dat

while [ $VAR -lt 5000 ]; do
    STR="\t"$VAR"\t"`./sltest --threads=1 --iter=$VAR | tail -1 | rev | cut -d ' ' -f 2 | rev`
    echo -e $STR >> list_graph_1.dat
    let VAR+=10
done
