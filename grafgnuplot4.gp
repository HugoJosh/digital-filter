# GRAFICA CUATRO GRAFICAS EN UNA VENTANA
set terminal png size 1920,1080
set output  "salida.png"

set autoscale
set multiplot layout 1,1 rowsfirst
set grid
set style data lines

#------------- Graf 1 ----------------
set title "Señal de salida"
#unset label
set xlabel   "t"
set ylabel   "y(t)"
set yrange[-1.5:1.5]
plot  "salfil.dat"
unset xlabel
unset ylabel
unset title
#pause(3)
#unset multiplot

#   