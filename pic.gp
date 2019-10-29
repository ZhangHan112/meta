reset
set term png enhanced font 'Helvetica,10' 
set output "runtime.png"
set xlabel "Iteration" font "Helvetica,16"
set ylabel "Best" font "Helvetica,16"  offset 1,0
set title "TSPACO" font "Helvetica,16"
set grid
set xrange[0:1000]
set xtics 100
set ytics 10
set key box w 3 h 0.5 
set label 1 "Evaporation = 0.05, Runs = 30" font "Helvetica,12"
set label 1 at graph 0.6, 0.75 tc lt 8 

plot 'output1.dat' every 100 using 1:2 with linespoints pt 7 ps 1 lw 2.5 lc 6 title "TSPACO", \
     ''          every 100 using 1:2:2 with labels font "Helvetica,9" offset 0,0.5 notitle,\
     'output2.dat' every 100 using 1:2 with linespoints pt 7 ps 1 lw 2.5 lc 4 title "TSPMMAS", \
     ''          every 100 using 1:2:2 with labels font "Helvetica,9" offset 0,-0.5 notitle,\
     'base.dat' using 1:2  with lines lw 2.5 lc 7  title "BASE" ,\
     ''          every 10000 using 1:2:2 with labels tc lt 7 font "Helvetica,13" offset -2.5,0 notitle ,\