#!/bin/bash

gcc Hangar.c Aereo.c -o aereoporto
gcc Torre.c -o torre
./aereoporto&
./torre
