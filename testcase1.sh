#!/bin/bash
#This one announces double occurences of members which are a security issue.
#it does, however, also tell you that group can be nested with itself, which is allright
for i in $(seq 1 $(cat filestructure.draft|wc -l)); do cat filestructure.draft|awk "FNR == $i {print}" |sed -e 's/[; ]/\n/g'|grep -v '^$'|grep -v '^[^ ]$'|sort|uniq -c; done|grep -v '^ *1'
