#!/bin/sh

set -xe

cc -Wall -Wextra -o texture main.c -lglfw -lGL -lm

./texture

