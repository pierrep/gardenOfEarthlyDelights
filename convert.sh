#!/bin/bash

ffmpeg -f image2  -r 10 -i %3d.png -c:v copy -s 256x256 output.mov
