set term png

set output "fig_results.png"

set key samplen 2 spacing 0.9 font ",8" below
set title "Experimental Beam Stop and Geant4 Simulation Gains"

set xrange [70:222]
set yrange [0.95:1.005]
set xlabel "Energy (MeV)"
set ylabel "Gain (I/B)"

plot "DATA.dat" u 1:2 t "HIT S59" w p lt 4 lc rgb "red", \
     "DATA.dat" u 1:3 t "HIT S100" w p lt 6 lc rgb "red", \
     "DATA.dat" u 1:4 t "HIT S200" w p lt 8 lc rgb "red", \
     "DATA.dat" u 1:5 t "G4_S59-Ag/KA" w lp lt 4 lc rgb "blue", \
     "DATA.dat" u 1:7 t "G4_S100-Ag/KA" w lp lt 6 lc rgb "blue", \
     "DATA.dat" u 1:9 t "G4_S200-Ag/KA" w lp lt 8 lc rgb "blue", \
     "DATA.dat" u 1:6 t "G4_S59-Ag/KA_v" w lp lt 4 lc rgb "cyan", \
     "DATA.dat" u 1:8 t "G4_S100-Ag/KA_v" w lp lt 6 lc rgb "cyan", \
     "DATA.dat" u 1:10 t "G4_S200-Ag/KA_v" w lp lt 8 lc rgb "cyan", \
     "DATA.dat" u 1:11 t "G4_S59+Ag/KA" w lp lt 4 lc rgb "gray", \
     "DATA.dat" u 1:13 t "G4_S100+Ag/KA" w lp lt 6 lc rgb "gray", \
     "DATA.dat" u 1:15 t "G4_S200+Ag/KA" w lp lt 8 lc rgb "gray", \
     "DATA.dat" u 1:12 t "G4_S59+Ag/KA_v" w lp lt 4 lc rgb "black", \
     "DATA.dat" u 1:14 t "G4_S100+Ag/KA_v" w lp lt 6 lc rgb "black", \
     "DATA.dat" u 1:16 t "G4_S200+Ag/KA_v" w lp lt 8 lc rgb "black"
