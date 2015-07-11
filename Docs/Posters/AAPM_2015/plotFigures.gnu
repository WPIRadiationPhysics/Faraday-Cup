# Poster figures macro

# PNG file output
set term png

# Model gain comparison
set output "figures/G4_HIT_gain.png"
set title "HIT Measured and Geant4 Simulation Gain" font "Helvetica,20"
unset key
set xrange [70:222]
set yrange [0.93:1.015]
set xlabel "Beam Energy [MeV]" font "Helvetica,20"
set ylabel "Gain [incident proton^-1]" font "Helvetica,20"
set ylabel offset +1,0
set ytics font "Helvetica,20"
set xtics font "Helvetica,20"
set label "G4 Cu" at 80,1.01 font "Helvetica,16"
set label "HIT S59" at 80,0.99 font "Helvetica,16"
set label "HIT S100" at 74,0.9525 font "Helvetica,16"
set label "HIT S200" at 80,0.94 font "Helvetica,16"
set label "G4 S59-200" at 120,0.9775 font "Helvetica,16"
set label "G4 S59-200+Ag" at 108.5,0.9665 font "Helvetica,16"
set datafile separator ","
plot "../../Data/HIT/gain.csv" u 1:2 t "HIT S59" w p lt 1 lc rgb "red", \
     "../../Data/HIT/gain.csv" u 1:3 t "HIT S100" w p lt 2 lc rgb "red", \
     "../../Data/HIT/gain.csv" u 1:4 t "HIT S100" w p lt 3 lc rgb "red", \
     "../../Data/Geant4/2015-07-04_vacuum/model0/modelGain.csv" u 1:2 t "Pure Cu" w lp lc rgb "black", \
     "../../Data/Geant4/2015-07-04_vacuum/model1/S59/modelGain.csv" u 1:2 t "S59" w lp lt 1 lc rgb "brown", \
     "../../Data/Geant4/2015-07-04_vacuum/model1/S100/modelGain.csv" u 1:2 t "S100" w lp lt 2 lc rgb "brown", \
     "../../Data/Geant4/2015-07-04_vacuum/model1/S200/modelGain.csv" u 1:2 t "S200" w lp lt 3 lc rgb "brown", \
     "../../Data/Geant4/2015-07-04_vacuum/model2/S59/modelGain.csv" u 1:2 t "S59+AgKA" w lp lt 1 lc rgb "gray", \
     "../../Data/Geant4/2015-07-04_vacuum/model2/S100/modelGain.csv" u 1:2 t "S100+AgKA" w lp lt 2 lc rgb "gray", \
     "../../Data/Geant4/2015-07-04_vacuum/model2/S200/modelGain.csv" u 1:2 t "S200+AgKA" w lp lt 3 lc rgb "gray"

# Model gain comparison
set output "figures/MCNP_gain_radius.png"
set title "MCNP Cu gain measurement" font "Helvetica,20"
unset key
set label "with e-" at 13,0.98 font "Helvetica,20"
set label "no e-" at 20,0.95 font "Helvetica,20"
set xrange [10:50]
set yrange [0.8:1]
set xlabel "Radius [mm]" font "Helvetica,20"
set ylabel "Gain [incident proton^-1]" font "Helvetica,20"
set ylabel offset +1,0
#set xlabel offset 0,+0.75
set datafile separator ","
plot "../../Data/MCNP/gain_radius.csv" u 1:2 notitle w lp lt 4 lc rgb "red", \
     "../../Data/MCNP/gain_radius.csv" u 1:3 notitle w lp lt 5 lc rgb "red"

## Unneeded, using Andrew's contour plot
# defect percent error
#set output "figures/G4_HIT_error.png"
#set title "Gain Percent Error - Geant4 and HIT"
#set key bottom right
#set xrange [70:225]
#set yrange [50:200]
#set zrange [-3:3]
#set xlabel "Energy [MeV]"
#set ylabel "Kapton Thickness [Micrometers]"
#set ylabel offset -1,0
#set contour
#set key at 100,160
#set view map
#unset surface
#set datafile separator ","
#splot "../Data/G4_HIT_error.csv" w l t ""
