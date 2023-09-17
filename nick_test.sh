#!/bin/bash

target_host="127.1"
target_port=6667

num_requests=1000


for ((i=1; i<=$num_requests; i++)); do
        ncat -c "echo -ne 'PASS pass\r\nNICK nick$i\r\nUSER user host * name\r\n'; tr -d n" $target_host $target_port &
        # sleep 0.1  # Sleep for 100 milliseconds between requests
done

echo "Requests sent."