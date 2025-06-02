#!/bin/bash
docker run -d -v ${PWD}:/root --security-opt seccomp:unconfined -it --name TP2_SO agodio/itba-so:2.0
