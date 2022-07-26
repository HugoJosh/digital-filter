# GRAFICA CUATRO GRAFICAS EN UNA VENTANA
set terminal png size 1920,1080
set output  "voz.png"

set autoscale
set grid
set style data lines

#------------- Graf 1 ----------------
set title "Voz original"
#unset label
set xlabel   "t"
set ylabel   "y(t)"
set yrange[-1.5:1.5]
plot  "voz2.dat"

unset xlabel
unset ylabel
unset title
unset output

#pause(3)
#unset multiplot

#   