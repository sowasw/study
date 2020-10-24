#!/bin/bash

# sed -n '4p' programs.gawk | sh 单条执行
echo "this is a test" | gawk '{ print $1, $NF; }'

gawk '{ print $1, $NF; } END { print "end:", "\nFNR =", FNR, "\nNR =",NR; }' data3.txt
gawk '{ print $1, $NF; } END { OFS="\n"; print "end:", "FNR = " FNR, "NR = " NR; }' data3.txt
gawk 'END { OFS = "\n"; print "end:", "FNR = " FNR, "NR = " NR; }' data3.txt
