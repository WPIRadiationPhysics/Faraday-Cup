set term png

set output "fig_G4_results.png"

set key samplen 2 spacing 0.9 font ",8" below
set title "Experimental Beam Stop and Geant4 Simulation Gains"

set xrange [70:222]
set yrange [0.95:1.005]
set xlabel "Energy (MeV)"
set ylabel "Gain (I/B)"

plot "DATA.dat" u 1:5 t "G4_Cu" w lp lt 1 lc rgb "brown", \
     "DATA.dat" u 1:7 t "G4_S59-Ag/KA" w lp lt 4 lc rgb "cyan", \
     "DATA.dat" u 1:8 t "G4_S100-Ag/KA" w lp lt 6 lc rgb "cyan", \
     "DATA.dat" u 1:9 t "G4_S200-Ag/KA" w lp lt 8 lc rgb "cyan", \
     "DATA.dat" u 1:10 t "G4_S59-Ag/KA_v" w lp lt 4 lc rgb "blue", \
     "DATA.dat" u 1:11 t "G4_S100-Ag/KA_v" w lp lt 6 lc rgb "blue", \
     "DATA.dat" u 1:12 t "G4_S200-Ag/KA_v" w lp lt 8 lc rgb "blue", \
     "DATA.dat" u 1:13 t "G4_S59+Ag/KA" w lp lt 4 lc rgb "gray", \
     "DATA.dat" u 1:14 t "G4_S100+Ag/KA" w lp lt 6 lc rgb "gray", \
     "DATA.dat" u 1:15 t "G4_S200+Ag/KA" w lp lt 8 lc rgb "gray", \
     "DATA.dat" u 1:16 t "G4_S59+Ag/KA_v" w lp lt 4 lc rgb "black", \
     "DATA.dat" u 1:17 t "G4_S100+Ag/KA_v" w lp lt 6 lc rgb "black", \
     "DATA.dat" u 1:18 t "G4_S200+Ag/KA_v" w lp lt 8 lc rgb "black"


set output "fig_G4_results_air.png"

set key samplen 2 spacing 0.9 font ",8" below
set title "Geant4 Simulation Gains in air"

set xrange [70:222]
set yrange [0.95:1]
set xlabel "Energy (MeV)"
set ylabel "Gain (I/B)"

plot "DATA.dat" u 1:7 t "G4_S59-Ag/KA" w lp lt 4 lc rgb "cyan", \
     "DATA.dat" u 1:8 t "G4_S100-Ag/KA" w lp lt 6 lc rgb "cyan", \
     "DATA.dat" u 1:9 t "G4_S200-Ag/KA" w lp lt 8 lc rgb "cyan", \
     "DATA.dat" u 1:13 t "G4_S59+Ag/KA" w lp lt 4 lc rgb "gray", \
     "DATA.dat" u 1:14 t "G4_S100+Ag/KA" w lp lt 6 lc rgb "gray", \
     "DATA.dat" u 1:15 t "G4_S200+Ag/KA" w lp lt 8 lc rgb "gray"


set output "fig_G4_results_vac.png"

set key samplen 2 spacing 0.9 font ",8" below
set title "Geant4 Simulation Gains in vacuo"

set xrange [70:222]
set yrange [0.95:1]
set xlabel "Energy (MeV)"
set ylabel "Gain (I/B)"

plot "DATA.dat" u 1:10 t "G4_S59-Ag/KA" w lp lt 4 lc rgb "blue", \
     "DATA.dat" u 1:11 t "G4_S100-Ag/KA" w lp lt 6 lc rgb "blue", \
     "DATA.dat" u 1:12 t "G4_S200-Ag/KA" w lp lt 8 lc rgb "blue", \
     "DATA.dat" u 1:16 t "G4_S59+Ag/KA" w lp lt 4 lc rgb "black", \
     "DATA.dat" u 1:17 t "G4_S100+Ag/KA" w lp lt 6 lc rgb "black", \
     "DATA.dat" u 1:18 t "G4_S200+Ag/KA" w lp lt 8 lc rgb "black"


set output "fig_results.png"

set key samplen 2 spacing 0.9 font ",8" below
set title "Experimental Beam Stop Geant4 Simulation Gains"

set xrange [70:222]
set yrange [0.93:1.015]
set xlabel "Energy (MeV)"
set ylabel "Gain (I/B)"

plot "DATA.dat" u 1:5 t "G4_Cu" w lp lt 1 lc rgb "brown", \
     "DATA.dat" u 1:2 t "HIT S59" w p lt 4 lc rgb "red", \
     "DATA.dat" u 1:3 t "HIT S100" w p lt 6 lc rgb "red", \
     "DATA.dat" u 1:4 t "HIT S200" w p lt 8 lc rgb "red", \
     "DATA.dat" u 1:7 t "G4_S59-Ag/KA" w lp lt 4 lc rgb "cyan", \
     "DATA.dat" u 1:8 t "G4_S100-Ag/KA" w lp lt 6 lc rgb "cyan", \
     "DATA.dat" u 1:9 t "G4_S200-Ag/KA" w lp lt 8 lc rgb "cyan", \
     "DATA.dat" u 1:10 t "G4_S59-Ag/KA_v" w lp lt 4 lc rgb "blue", \
     "DATA.dat" u 1:11 t "G4_S100-Ag/KA_v" w lp lt 6 lc rgb "blue", \
     "DATA.dat" u 1:12 t "G4_S200-Ag/KA_v" w lp lt 8 lc rgb "blue", \
     "DATA.dat" u 1:13 t "G4_S59+Ag/KA" w lp lt 4 lc rgb "gray", \
     "DATA.dat" u 1:14 t "G4_S100+Ag/KA" w lp lt 6 lc rgb "gray", \
     "DATA.dat" u 1:15 t "G4_S200+Ag/KA" w lp lt 8 lc rgb "gray", \
     "DATA.dat" u 1:16 t "G4_S59+Ag/KA_v" w lp lt 4 lc rgb "black", \
     "DATA.dat" u 1:17 t "G4_S100+Ag/KA_v" w lp lt 6 lc rgb "black", \
     "DATA.dat" u 1:18 t "G4_S200+Ag/KA_v" w lp lt 8 lc rgb "black"


set output "fig_results_normal.png"

set key samplen 2 spacing 0.9 font ",8" below
set title "Normalized Experimental Beam Stop Geant4 Simulation Gains"

set xrange [70:222]
set yrange [0.93:1]
set xlabel "Energy (MeV)"
set ylabel "Gain (I/B)"

plot "DATA_normal.dat" u 1:5 t "G4_Cu" w lp lt 1 lc rgb "brown", \
     "DATA_normal.dat" u 1:2 t "HIT S59" w p lt 4 lc rgb "red", \
     "DATA_normal.dat" u 1:3 t "HIT S100" w p lt 6 lc rgb "red", \
     "DATA_normal.dat" u 1:4 t "HIT S200" w p lt 8 lc rgb "red", \
     "DATA_normal.dat" u 1:7 t "G4_S59-Ag/KA" w lp lt 4 lc rgb "cyan", \
     "DATA_normal.dat" u 1:8 t "G4_S100-Ag/KA" w lp lt 6 lc rgb "cyan", \
     "DATA_normal.dat" u 1:9 t "G4_S200-Ag/KA" w lp lt 8 lc rgb "cyan", \
     "DATA_normal.dat" u 1:10 t "G4_S59-Ag/KA_v" w lp lt 4 lc rgb "blue", \
     "DATA_normal.dat" u 1:11 t "G4_S100-Ag/KA_v" w lp lt 6 lc rgb "blue", \
     "DATA_normal.dat" u 1:12 t "G4_S200-Ag/KA_v" w lp lt 8 lc rgb "blue", \
     "DATA_normal.dat" u 1:13 t "G4_S59+Ag/KA" w lp lt 4 lc rgb "gray", \
     "DATA_normal.dat" u 1:14 t "G4_S100+Ag/KA" w lp lt 6 lc rgb "gray", \
     "DATA_normal.dat" u 1:15 t "G4_S200+Ag/KA" w lp lt 8 lc rgb "gray", \
     "DATA_normal.dat" u 1:16 t "G4_S59+Ag/KA_v" w lp lt 4 lc rgb "black", \
     "DATA_normal.dat" u 1:17 t "G4_S100+Ag/KA_v" w lp lt 6 lc rgb "black", \
     "DATA_normal.dat" u 0:18 t "G4_S200+Ag/KA_v" w lp lt 8 lc rgb "black"


set output "fig_results2.png"

set key samplen 2 spacing 0.9 font ",8" below
set title "Experimental Beam Stop and Geant4 Simulation Gains"

set xrange [70:222]
set yrange [0.93:1.015]
set xlabel "Energy (MeV)"
set ylabel "Gain (I/B)"

plot "DATA.dat" u 1:2 t "HIT S59" w p lt 4 lc rgb "red", \
     "DATA.dat" u 1:3 t "HIT S100" w p lt 6 lc rgb "red", \
     "DATA.dat" u 1:4 t "HIT S200" w p lt 8 lc rgb "red", \
     "DATA.dat" u 1:5 t "G4_Cu" w lp lt 1 lc rgb "brown", \
     "DATA.dat" u 1:7 t "G4_S59" w lp lt 4 lc rgb "cyan", \
     "DATA.dat" u 1:8 t "G4_S100" w lp lt 6 lc rgb "cyan", \
     "DATA.dat" u 1:9 t "G4_S200" w lp lt 8 lc rgb "cyan", \
     "DATA.dat" u 1:13 t "G4_S59 + Ag" w lp lt 4 lc rgb "gray", \
     "DATA.dat" u 1:14 t "G4_S100 + Ag" w lp lt 6 lc rgb "gray", \
     "DATA.dat" u 1:15 t "G4_S200 + Ag" w lp lt 8 lc rgb "gray"


set output "fig_results3.png"

set key samplen 2 spacing 0.9 font ",8" below
set title "Experimental Beam Stop and Geant4 Simulation Gains"

set xrange [70:222]
set yrange [0.93:1.015]
set xlabel "Energy (MeV)"
set ylabel "Gain (I/B)"

plot "DATA.dat" u 1:2 t "HIT S59" w p lt 4 lc rgb "red", \
     "DATA.dat" u 1:3 t "HIT S100" w p lt 6 lc rgb "red", \
     "DATA.dat" u 1:4 t "HIT S200" w p lt 8 lc rgb "red", \
     "DATA.dat" u 1:5 t "G4_Cu" w lp lt 1 lc rgb "brown", \
     "DATA.dat" u 1:6 t "G4_Cu_v" w lp lt 2 lc rgb "brown", \
     "DATA.dat" u 1:7 t "G4_S59" w lp lt 4 lc rgb "cyan", \
     "DATA.dat" u 1:8 t "G4_S100" w lp lt 6 lc rgb "cyan", \
     "DATA.dat" u 1:9 t "G4_S200" w lp lt 8 lc rgb "cyan", \
     "DATA.dat" u 1:10 t "G4_S59_v" w lp lt 2 lc rgb "blue", \
     "DATA.dat" u 1:11 t "G4_S100_v" w lp lt 2 lc rgb "blue", \
     "DATA.dat" u 1:12 t "G4_S200_v" w lp lt 2 lc rgb "blue", \
     "DATA.dat" u 1:13 t "G4_S59 + Ag" w lp lt 4 lc rgb "gray", \
     "DATA.dat" u 1:14 t "G4_S100 + Ag" w lp lt 6 lc rgb "gray", \
     "DATA.dat" u 1:15 t "G4_S200 + Ag" w lp lt 8 lc rgb "gray", \
     "DATA.dat" u 1:16 t "G4_S59_v + Ag" w lp lt 2 lc rgb "black", \
     "DATA.dat" u 1:17 t "G4_S100_v + Ag" w lp lt 2 lc rgb "black", \
     "DATA.dat" u 1:18 t "G4_S200_v + Ag" w lp lt 2 lc rgb "black"


set output "fig_HIT_results.png"

set key samplen 2 spacing 0.9 font ",8" below
set title "Experimental Beam Stop Gains"

set xrange [70:222]
set yrange [0.93:1.015]
set xlabel "Energy (MeV)"
set ylabel "Gain (I/B)"

plot "DATA.dat" u 1:2 t "HIT S59" w p lt 4 lc rgb "red", \
     "DATA.dat" u 1:3 t "HIT S100" w p lt 6 lc rgb "red", \
     "DATA.dat" u 1:4 t "HIT S200" w p lt 8 lc rgb "red"


set output "fig_pcd_results.png"

set key samplen 2 spacing 0.9 font ",8" below
set title "Experimental vs. Simulated Beam Stop % charge defect"

set xrange [70:222]
set yrange [-7:2]
set xlabel "Energy (MeV)"
set ylabel "% charge defect"

plot "DATA_pcd.dat" u 1:2 t "HIT S59" w p lt 4 lc rgb "red", \
     "DATA_pcd.dat" u 1:3 t "HIT S100" w p lt 6 lc rgb "red", \
     "DATA_pcd.dat" u 1:4 t "HIT S200" w p lt 8 lc rgb "red", \
     "DATA_pcd.dat" u 1:13 t "G4_S59 + Ag" w lp lt 4 lc rgb "gray", \
     "DATA_pcd.dat" u 1:14 t "G4_S100 + Ag" w lp lt 6 lc rgb "gray", \
     "DATA_pcd.dat" u 1:15 t "G4_S200 + Ag" w lp lt 8 lc rgb "gray", \
     "DATA_pcd.dat" u 1:5 t "G4_Cu" w lp lt 1 lc rgb "brown"


set output "fig_mcnp_pcd_results.png"

set key samplen 2 spacing 0.9 font ",8" below
set title "MCNP6 charge defect measurements"

set xrange [2:10]
set yrange [-7:1]
set xlabel "Diameter [cm]"
set ylabel "% charge defect"

plot "DATA_mcnp_pcd.dat" u 1:2 t "MCNP" w p lt 4 lc rgb "black", \
     "DATA_mcnp_pcd.dat" u 1:3 t "MCNP no e-" w p lt 6 lc rgb "black"

