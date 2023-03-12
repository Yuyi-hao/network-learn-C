#!/bin/sh -e

set -xe 
CC=cc
CFLAGS="-Wall -Wextra"

$CC $CFLAGS -o tcp_client tcp_client.c 
./tcp_client google.com 80
