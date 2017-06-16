#!/bin/bash

rm -fr *_* *.html

simulate edf fifo fixedpriority fixedprioritytop tdma tdmatop

simfig edf fifo fixedpriority fixedprioritytop tdma tdmatop
