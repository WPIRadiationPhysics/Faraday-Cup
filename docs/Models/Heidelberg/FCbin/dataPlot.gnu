set term png

set output "GainResults.png"

set xlabel "Energy (MeV)"
set ylabel "Gain (I/B)"

plot "data/film0gain.txt" u 1:2:4 w errorbars t "S59", \
     "data/film1gain.txt" u 1:2:4 w errorbars t "S100", \
     "data/film2gain.txt" u 1:2:4 w errorbars t "S200"


set output "GainError.png"

set xlabel "Energy (MeV)"
set ylabel "Gain Error (I/B)"

plot "data/film0gain.txt" u 1:4 w lines t "S59", \
     "data/film1gain.txt" u 1:4 w lines t "S100", \
     "data/film2gain.txt" u 1:4 w lines t "S200"