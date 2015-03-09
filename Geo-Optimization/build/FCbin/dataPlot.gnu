set term png

set output "GainResults.png"

set xlabel "Energy (MeV)"
set ylabel "Gain (I/B)"

plot "data/S2_gain.txt" u 1:2 w lines t "S10", \
     "data/S4_gain.txt" u 1:2 w lines t "S20", \
     "data/S6_gain.txt" u 1:2 w lines t "S30", \
     "data/S8_gain.txt" u 1:2 w lines t "S40", \
     "data/S10_gain.txt" u 1:2 w lines t "S50"


set output "GainError.png"

set xlabel "Energy (MeV)"
set ylabel "Gain Error (I/B)"

plot "data/S2_gain.txt" u 1:4 w lines t "S10", \
     "data/S4_gain.txt" u 1:4 w lines t "S20", \
     "data/S6_gain.txt" u 1:4 w lines t "S30", \
     "data/S8_gain.txt" u 1:4 w lines t "S40", \
     "data/S10_gain.txt" u 1:4 w lines t "S50"
