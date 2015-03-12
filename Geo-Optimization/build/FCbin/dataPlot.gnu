set term png

set output "GainResults.png"

set xlabel "Energy (MeV)"
set ylabel "Gain (I/B)"

plot "data/S1_gain.txt" u 1:8 w lines t "S1", \
     "data/S5_gain.txt" u 1:8 w lines t "S5", \
     "data/S10_gain.txt" u 1:8 w lines t "S10", \
     "data/S50_gain.txt" u 1:8 w lines t "S50", \
     "data/S100_gain.txt" u 1:8 w lines t "S100", \
     "data/S500_gain.txt" u 1:8 w lines t "S500", \
     "data/S1000_gain.txt" u 1:8 w lines t "S1000", \
     "data/S5000_gain.txt" u 1:8 w lines t "S5000"

set output "GainError.png"

set xlabel "Energy (MeV)"
set ylabel "Gain Error (I/B)"

plot "data/S1_gain.txt" u 1:10 w lines t "S1", \
     "data/S5_gain.txt" u 1:10 w lines t "S5", \
     "data/S10_gain.txt" u 1:10 w lines t "S10", \
     "data/S50_gain.txt" u 1:10 w lines t "S50", \
     "data/S100_gain.txt" u 1:10 w lines t "S100", \
     "data/S500_gain.txt" u 1:10 w lines t "S500", \
     "data/S1000_gain.txt" u 1:10 w lines t "S1000", \
     "data/S5000_gain.txt" u 1:10 w lines t "S5000"


set output "pKASignal.png"

set xlabel "Energy (MeV)"
set ylabel "Gain (I/B)"
set yrange [-0.0001:0.0001]

plot "data/S1_gain.txt" u 1:5 w lines t "S1", \
     "data/S5_gain.txt" u 1:5 w lines t "S5", \
     "data/S10_gain.txt" u 1:5 w lines t "S10", \
     "data/S50_gain.txt" u 1:5 w lines t "S50", \
     "data/S100_gain.txt" u 1:5 w lines t "S100", \
     "data/S500_gain.txt" u 1:5 w lines t "S500", \
     "data/S1000_gain.txt" u 1:5 w lines t "S1000", \
     "data/S5000_gain.txt" u 1:5 w lines t "S5000"

set output "eKASignal.png"

set xlabel "Energy (MeV)"
set ylabel "Gain (I/B)"
set yrange [-0.002:0.002]

plot "data/S1_gain.txt" u 1:6 w lines t "S1", \
     "data/S5_gain.txt" u 1:6 w lines t "S5", \
     "data/S10_gain.txt" u 1:6 w lines t "S10", \
     "data/S50_gain.txt" u 1:6 w lines t "S50", \
     "data/S100_gain.txt" u 1:6 w lines t "S100", \
     "data/S500_gain.txt" u 1:6 w lines t "S500", \
     "data/S1000_gain.txt" u 1:6 w lines t "S1000", \
     "data/S5000_gain.txt" u 1:6 w lines t "S5000"
