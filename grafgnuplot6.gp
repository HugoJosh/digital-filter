# GRAFICA CUATRO GRAFICAS EN UNA VENTANA
set terminal png size 1920,1080
set output  "espinoutzoom.png" 

set autoscale
set multiplot layout 1,2 rowsfirst
set grid
set style data lines

#------------- Graf 1 ----------------
set title "Espectro de entrada"
#unset label
set xlabel   "k"
set ylabel   "|y(k)|"
set xrange [0:600]
plot  "espin.dat"
unset xlabel
unset ylabel
unset title

#------------- Graf 2 ----------------
set title "Espectro de salida"
#unset label
set xlabel   "k"
set ylabel   "|y(k)|"
plot  "espout.dat"
unset xlabel
unset ylabel
unset title

unset multiplot
unset output
#pause(3)
#unset multiplot

#   