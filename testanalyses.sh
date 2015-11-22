#/bin/bash
# Macro to print all analytical figures
# Needs to be updated

if [ ! `ls | grep build` ]; then
  echo "You must build and run the project before you can perform the analyses!"
  exit
fi

cd build

# Apply analysis scripts
for model in `seq 0 2`; do

  # Unsheathed copper
  if [ "$model" == "0" ]; then

    cd "data/model$model"

    # Plot particle spectra, deposition and cascade histograms
    root -blnq plotSpectra.C
    root -blnq plotDepHistoCu.C
    octave plotCascadeHisto.m

    cd ../..

  # Loop Kapton thickness iteration
  elif [ "$model" > "0" ]; then
    for KA in 59 100 200; do

    cd "data/model$model/S$KA"

    # Plot particle spectra, deposition and cascade histograms
    root -blnq plotSpectra.C
    root -blnq plotDepHistoCuKA.C
    octave plotCascadeHisto.m

    cd ../../..

    done
  fi
done
