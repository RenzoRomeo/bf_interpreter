#!/bin/sh

set -xe

gcc main.c -o interpreter -std=c11 -Wall -Wextra
