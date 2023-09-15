#!/bin/bash

# Define the message to send
message="PASS pass\r\nNICK nick\r\nUSER user 0 * guy\r\n"

# Use netcat to send the message to localhost on port 6667
echo -n "$message" | nc localhost 6667 > output.log 2>&1 &
