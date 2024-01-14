#!/bin/bash

echo "digraph G {"

# Get a list of all functions in the codebase
functions=$(cscope -L1 "" | awk '{print $1}' | sort -u)

# For each function, find the functions it calls
for func in $functions; do
    cscope -L2 "$func" | awk -v source="$func" '{print "    \"" source "\" -> \"" $1 "\";"}'
done

echo "}"

