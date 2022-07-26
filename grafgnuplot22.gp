# GRAFICA CUATRO GRAFICAS EN UNA VENTANA
set terminal png size 1920,1080
set output  "espsiszoom.png"

set autoscale
set multiplot layout 2,1 rowsfirst
set grid
set style data lines

#------------- Graf 1 ----------------
set title "Magnitud"
#unset label
set xlabel   "w[Radianes]"
set ylabel   "|y(w)|"
set xrange [0:0.5]
plot  "espsis1.dat"
unset xlabel
unset ylabel
unset title

#------------- Graf 2 ----------------
set title "Fase"
# unset label
set angles radians
set xlabel   "w[Radianes]"
set ylabel   "theta(w)"
plot  "espsis2.dat"
unset xlabel
unset ylabel
unset title

pause(3)
#unset multiplot

#   