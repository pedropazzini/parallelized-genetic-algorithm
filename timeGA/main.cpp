#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>

int main (int argc, char* argv[])
{
  //confere a passagem como parâmetros
  if (argc != 4)
  {
    std::cout << "Erro na passagem por parâmetros!" << std::endl;
    std::cout << "Formato: ./a.out <nome_do_arquivo_de_tempo_SEM_thread> <nome_do_arquivo_de_tempo_COM_thread> <numero_de_linhas_do_arquivo>" << std::endl;
    exit(0);
  }

  //abre os arquivos com o tamanho do tempo passado como parâmetro
  std::ifstream timeGA (argv[1]);
  std::ifstream timeGA_thread (argv[2]);
  std::ofstream time("time.dat");

  //número de linhas do arquivo
  int countLine = atoi(argv[3]);
  long divisor = long(countLine);

  long sum = 0;
  long sumThread = 0;
  long data;

  //calcula a média
  while(countLine--)
  {
    timeGA >> data;
    sum += data/100000;
    timeGA_thread >> data;
    sumThread += data/100000;
  }

  long average = sum/divisor;
  long averageThread = sumThread/divisor;

  //envia para o arquivo
  time << average << std::endl;
  time << averageThread << std::endl;

  //a partir daqui calcula a variância
  timeGA.seekg(0,std::ios::beg);	
  timeGA_thread.seekg(0,std::ios::beg);

  //prepara os números para calcular a variancia
  sum = 0;
  sumThread = 0;
  countLine = divisor;


  while (countLine--)
  {
    timeGA >> data;
    std::cout << data << std::endl;
    sum += std::pow((data/100000)-average,2);
    std::cout << sum << std::endl;
    timeGA_thread >> data;
    std::cout << data << std::endl;
    sumThread += pow ((data/100000)-averageThread,2);
    std::cout << sumThread << std::endl;
  }
  std::cout << sum << " " << sumThread << std::endl;

  long variance = std::sqrt(sum/(divisor-1));
  long varianceThread = std::sqrt(sumThread/(divisor-1));

  time << variance << std::endl;
  time << varianceThread << std::endl;

  return 0;
}
