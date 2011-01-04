#!/bin/bash

init=0
end=25

#realiza iterações pegando estatísticas do ga
for ((i=$init; i!=$end; i++))
do
echo 'Iteração estatistica: '
echo $i
./ga 7
mv OutputGA_thread.dat OutputGA_thread.dat_$i
done

#gera o arquivo com os dados médios da população
./statistic/a.out OutputGA_thread.dat_ $end 1000

cp saida_stat plotGA.dat

gnuplot plot_ga_statistics.gnuplot
mv *.png image/

rm OutputGA_thread*


#limpa os arquivos de tempo
:>timeGA.dat
:>timeGA_thread.dat

#realiza iterações pegando dados de tempo do ga
for ((i=$init; i!=end; i++))
do
echo 'Iteração de tempo: '
echo $i
./ga
done

./timeGA/a.out timeGA.dat timeGA_thread.dat $end

rm saida_stat

exit

