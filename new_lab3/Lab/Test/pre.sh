#!/bin/bash

mkdir -p ./workdir

for fout in ./tests/*.output; do
    sed 's/Error Type/Error type/' $fout > workdir/temp.output
    mv workdir/temp.output $fout

done
