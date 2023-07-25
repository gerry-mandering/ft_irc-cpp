#!/bin/bash

for i in {1..500}
do
	(echo "hi $i"; sleep 500) | nc 127.0.0.1 6667 &
done

# wait for all background jobs to finish
wait
