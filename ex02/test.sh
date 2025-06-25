#!/bin/bash

for n in {1..10}
do
    VALUES=$(shuf -i "1-$n")
    SORTED=$(echo "$VALUES" | sort -n | xargs)
    RESULT=$(./PmergeMe $VALUES | xargs)
    if [[ $SORTED == $RESULT ]]; then
        echo "TEST $n OK"
    else
        echo "TEST $n ERROR"
        echo "RESULT = $RESULT"
        echo "SORTED = $SORTED"
    fi
done