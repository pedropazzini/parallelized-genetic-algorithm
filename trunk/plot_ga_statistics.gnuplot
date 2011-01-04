#Script do gnuplot para plotar um gráfico com as estatisticas do GA
reset
set autoscale
unset label
set title "Estatísticas do GA function6"
set ylabel "Valor"
set xlabel "Geração"
set term png
set output "pior_id.png"
plot  "plotGA.dat" using 1:3 title 'pior indivíduo' with lines
set output "melhor_id.png"
plot "plotGA.dat" using 1:2 title 'melhor indivíduo' with lines 
set output "fitness_medio.png"
plot "plotGA.dat" using 1:4 title 'fitness médio' with lines
set output "variancia_fitness_medio.png"
plot "plotGA.dat" using 1:5 title 'variância do fitness médio' with lines
