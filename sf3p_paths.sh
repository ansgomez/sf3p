#!/bin/bash

alias sudo='sudo env PATH=$PATH $@'

SF3P=`pwd`
export SF3P

PATH=$PATH:$SF3P/bin
export PATH

OCTAVE_PATH=$SF3P/scripts/octave
octave --no-window-system -qf --eval "addpath(\"$OCTAVE_PATH\"); savepath()"
