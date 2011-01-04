#include <iostream>
#include <fstream>
#include "population.h"
#include "individual.h"
#include "coordinate.h"
#include "definitions.h"
#include "mutate_bit_by_bit.h"
#include "cross_over.h"
#include "selection_by_roulette.h"
#include "genetic_algorithm.h"
#include "genetic_algorithm_thread.h"
#include "genetic_operator_thread.h"


//bibliotecas para gerar os números aleatórios
#include<stdio.h>
#include<stdlib.h>
#include<time.h>


float GetFunction0 (const std::vector<float>& vec)
{
  float val_1 =0;
  float val_2 =0;
  int i = 0;
  for (std::vector<float>::const_iterator it = vec.begin(); it != vec.end(); ++it)
  {
    val_1 += pow((*it)-(++i),2);
  }
  return val_1; 
}

float GetFunction4 (const std::vector<float>& vec)
{
  float valRet = 0;
  for (std::vector<float>::const_iterator it = vec.begin(); it != vec.end(); ++it)
  {
    valRet += -(*it)*std::sin(std::sqrt(std::abs(*it)));
  }
  return valRet;
}

float GetFunction5 (const std::vector <float>& vec)
{
  float valRet = 0;
  for (std::vector<float>::const_iterator it = vec.begin(); it != vec.end(); ++it)
  {
    float x = *it;
    valRet += x*x-10*std::cos(2*3.1416*x) +10;
  }
  return valRet;
}

float GetFunction6 (const std::vector <float>& vec)
{
  float valRet = 0;
  for (std::vector<float>::const_iterator it = vec.begin(); it != vec.end(); ++it)
  {
    float x = 1;
    float count = 1;
    for (std::vector<float>::const_iterator it_2 = vec.begin(); it_2 != vec.end(); ++it_2)
    {
      x *= std::cos((*it_2)/(std::sqrt(count)));
      count++;
    }
    valRet += (*it)*(*it) - x +1;    
  }
  valRet = valRet/4000.0;
  return valRet;
}


float GetObjectiveFunction (const std::vector<float>& vec)
{
  #if function0
  return GetFunction0(vec);
  #endif
  #if function4
  return GetFunction4(vec);
  #endif
  #if function5
  return GetFunction5(vec);
  #endif
  #if function6
  return GetFunction6(vec);
  #endif
}

int main (int argc, char* argv[])
{
  //Inicia o gerador de números aleatórios
  srand(time(NULL));

  int mask = 0;
  if (argc==2)
    mask = atoi(argv[1]);

  //descomente a linha abaixo para realizar o ga PARALELIZADO
  genetic_algorithm_thread <> ga_thread(1000,mask);

  //descomente as duas linhas abaixo para iniciar o ga NÂO PARALELIZADO
  genetic_algorithm<> ga(1000,mask);
  ga.StartGA();
  return 0;
}
