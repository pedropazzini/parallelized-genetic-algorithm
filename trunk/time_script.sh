#!/bin/bash

i_init=0;
  i_end=100;
#limpa os arquios de tempo
:>timeGA.dat
:>timeGA_thread.dat
for ((i=$i_init;i<$i_end; i++))
do
echo $i
./ga
done

exit 0;
