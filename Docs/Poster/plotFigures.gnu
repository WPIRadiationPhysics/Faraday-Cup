# Poster figures macro

# PNG file output
set term png

# Model gain comparison
set output "figures/G4_gain.png"
set title "Geant4 Model Simulation Gain in Vacuum"
set key bottom right
set xrange [70:222]
set yrange [0.95:1.01]
set xlabel "Beam Energy [MeV]"
set ylabel "Gain [incident proton^-1]"
set ylabel offset +2,0
set datafile separator ","
plot "../Data/Geant4/2015-07-04_vacuum/model0/modelGain.csv" u 1:2 t "Pure Cu" w lp lc rgb "black", \
     "../Data/Geant4/2015-07-04_vacuum/model1/S59/modelGain.csv" u 1:2 t "S59" w lp lt 1 lc rgb "brown", \
     "../Data/Geant4/2015-07-04_vacuum/model1/S100/modelGain.csv" u 1:2 t "S100" w lp lt 2 lc rgb "brown", \
     "../Data/Geant4/2015-07-04_vacuum/model1/S200/modelGain.csv" u 1:2 t "S200" w lp lt 3 lc rgb "brown", \
     "../Data/Geant4/2015-07-04_vacuum/model2/S59/modelGain.csv" u 1:2 t "S59+AgKA" w lp lt 1 lc rgb "gray", \
     "../Data/Geant4/2015-07-04_vacuum/model2/S100/modelGain.csv" u 1:2 t "S100+AgKA" w lp lt 2 lc rgb "gray", \
     "../Data/Geant4/2015-07-04_vacuum/model2/S200/modelGain.csv" u 1:2 t "S200+AgKA" w lp lt 3 lc rgb "gray"

# defect percent error
set output "figures/G4_HIT_error.png"
set title "Gain Percent Error - Geant4 and HIT"
set key bottom right
set xrange [70:225]
set yrange [50:200]
set zrange [-3:3]
set xlabel "Energy [MeV]"
set ylabel "Kapton Thickness [Micrometers]"
set ylabel offset -1,0
set contour
set key at 100,160
set view map
unset surface
set datafile separator ","
splot "../Data/G4_HIT_error.csv" w l t ""
