#!/bin/bash

PLOT_CSV=true
PLOT_ROOT=true
scriptDIR="plotScripts"

if [ $PLOT_CSV ]; then

  octave $scriptDIR/plotGainProfile.m
  octave $scriptDIR/plotGainErrorProfile.m

  octave $scriptDIR/plotLossProfile.m
  octave $scriptDIR/plotLossErrorProfile.m

  #octave $scriptDIR/plotCascade.m
fi

if [ $PLOT_ROOT ]; then

  #root -q $scriptDIR/plotGain.C
  root -q $scriptDIR/plotGainProfile.C
  #root -q $scriptDIR/plotSpectra.C
fi
