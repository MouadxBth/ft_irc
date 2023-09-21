#!/bin/bash

target_host="127.1"
target_port=6667

num_requests=1000

netcat=`which nc`
#[`echo $?` -nq 0] && netcat=`which ncat`

for ((i=1; i<=$num_requests; i++)); do
	$netcat -c "echo 'PASS pass\r\nNICK nick$i\r\nUSER user 0 0 0\r\n'" $target_host $target_port &
    	# sleep 0.1  # Sleep for 100 milliseconds between requests
		sleep 0.00001
done

echo "Requests sent."
