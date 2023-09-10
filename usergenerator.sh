#!/bin/bash

target_host="127.1"
target_port=6667

num_requests=10

netcat=`which nc`
[`echo $?` -nq 0] && netcat=`which ncat`

for ((i=1; i<=$num_requests; i++)); do
	$netcat -c "echo -ne 'NICK nick\r\nUSER user host * name\r\nPASS pass\r\n'; cat" $target_host $target_port &
    	# sleep 0.1  # Sleep for 100 milliseconds between requests
done

echo "Requests sent."
