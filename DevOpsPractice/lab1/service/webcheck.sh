#!/bin/bash

LOG=/home/service/web_check.log

SERVER_URL=http://localhost:80

while true
do
    echo "$(date) - [*] ping"

    status_code=$(curl -s -o /dev/null -w "%{http_code}" $SERVER_URL)

    if [[ ! $status_code =~ ^(2|3)[0-9]{2}$ ]]; then
       echo "$(date) - [*] Web Status: $status_code" >> $LOG
    else
       echo "$(date) - [-] Web Status: $status_code" >> $LOG
    fi

    sleep 10

done
