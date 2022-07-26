# GRAFICA CUATRO GRAFICAS EN UNA VENTANA
set terminal png size 1920,1080
set output  "entradasistema.png"

set autoscale
set multiplot layout 1,1 rowsfirst
set grid
set style data lines

#------------- Graf 2 ----------------
set title "Señal de entrada al sistema"
#unset label
set xlabel   "t"
set ylabel   "y(t)"
set yrange[-1.5:1.5]
plot  "salsfil.dat"
unset xlabel
unset ylabel
unset title

#pause(3)
#unset multiplot

#   