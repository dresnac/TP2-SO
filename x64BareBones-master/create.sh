#!/bin/bash
docker run -d -v ${PWD}:/root --security-opt seccomp:unconfined -it --name TPE_ARQUI agodio/itba-so:2.0
