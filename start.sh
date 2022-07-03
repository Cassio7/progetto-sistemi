#!/bin/bash

ps ax | grep ./torre | awk '{print $1}' | xargs kill -9 2> /dev/null

rm /tmp/myfifo 2> /dev/null
gcc Hangar.c Aereo.c -o aereoporto
gcc Torre.c -o torre
./torre &
./aereoporto
