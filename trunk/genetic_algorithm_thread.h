/**
 * @file genetic_algorithm_thread.h
 * @Synopsis Arquivo que contém a classe que implementa a versao paralelizada do algoritmo genético.
 * @author Pedro Pazzini
 * @version 0.0.1
 * @date 2010-10-30
 */
#ifndef GENETIC_ALGORITHM_THREAD_H
#define GENETIC_ALGORITHM_THREAD_H

#include "genetic_algorithm.h"
#include "population_thread.h"
#include "genetic_operator_thread.h"
#include "selection_by_tournament.h"
#include "cross_over_thread.h"
#include "mutate_bit_by_bit_thread.h"
#include "definitions.h"
#include <utility>
#include <string>
#include <ctime>
#include <sys/time.h>
#include <fstream>

/* ----------------------------------------*/
/**
 * @Synopsis Valores default do algoritmo genético.
 *
 * @tparam _ty
 * @tparam _realTy
 */
/* ----------------------------------------*/
template <typename _ty = GAtype,typename _realTy = GAreal_type> class genetic_algorithm_thread;

/* ----------------------------------------*/
/**
 * @Synopsis Classe que implementa a versão paralelizada do algoritmo genético. Todas as threds são criadas no método construtor, assim como o resultado final do algoritmo.
 *
 * @tparam _ty
 * @tparam _realTy
 */
/* ----------------------------------------*/
template <typename _ty,typename _realTy>
class genetic_algorithm_thread : public genetic_algorithm<_ty,_realTy>
{
  public:

    /* ----------------------------------------*/
    /**
     * @Synopsis Método construtor.
     *
     * @Param max_generation Número máximo de gerações.
     */
    /* ----------------------------------------*/
    genetic_algorithm_thread (const int& max_generation = def::genetic_operator::numberOfGenerations,short int mask=0);


  private:


    /* ----------------------------------------*/
    /**
     * @Synopsis Aruivo que registra o tempo de execução do ga
     */
    /* ----------------------------------------*/
    std::ofstream _timeOutput;



    /* ----------------------------------------*/
    /**
     * @Synopsis População na qual será aplicado o operador genético.
     */
    /* ----------------------------------------*/
    population_thread<_ty,_realTy>* _population;

    /* ----------------------------------------*/
    /**
     * @Synopsis Faz o join com as threads do operator op
     *
     * @Param op Faz o join com as threads de um operador genético.
     */
    /* ----------------------------------------*/
    void JoinThreadsOperator (genetic_operator_thread<_ty,_realTy>* op);

    /* ----------------------------------------*/
    /**
     * @Synopsis Operador de seleção.
     */
    /* ----------------------------------------*/
    genetic_operator_thread<_ty,_realTy>* selection;

    /* ----------------------------------------*/
    /**
     * @Synopsis Operador de cruzamento.
     */
    /* ----------------------------------------*/
    genetic_operator_thread<_ty,_realTy>* cross_over;

    /* ----------------------------------------*/
    /**
     * @Synopsis Operador de mutação.
     */
    /* ----------------------------------------*/
    genetic_operator_thread<_ty,_realTy>* mutation;

};

template <typename _ty,typename _realTy>
genetic_algorithm_thread<_ty,_realTy>::genetic_algorithm_thread (const int& max_generation,short int mask)
:genetic_algorithm<_ty,_realTy>(max_generation)//chama o construtor da classe mãe
  ,_timeOutput("timeGA_thread.dat",std::ios::app)//cria um arquivo para jogar o tempo final no operador
{

  

  //cria o operador de seleçõa por tornei
  selection = new selection_by_tournament<_ty,_realTy>(genetic_algorithm<_ty,_realTy>::_population,mask);

  //cria o operador de cruzamento
  cross_over = new cross_over_thread<_ty,_realTy>(NULL);

  //cria o operador de mutação
  mutation = new mutate_bit_by_bit_thread<_ty,_realTy>(NULL);

  //seta os onsumiores e podutores de cada operador
  selection->SetProducer(mutation->GetReference());//o operador de seleção consome os dados produzidos pelo operador de mutação
  selection->SetConsumer(cross_over->GetReference());//o operador de seleção prouz os dados do operador de cruzamento
  cross_over->SetProducer(selection->GetReference());//o operador de cruzamento consome os dados poduzios pelo operador de seleção
  cross_over->SetConsumer(mutation->GetReference());//o operador de cruzamento produz os dados do operador de mutação
  mutation->SetProducer(cross_over->GetReference());//o operador de mutação consomeos dados produzidos pelo operador de cruzamento
  mutation->SetConsumer(selection->GetReference());//o operador de mutação produz os dados do operador de seleção

  /* --------------------------------------------------------------- *
   * --- Inicia o GA e mede o tempo de execução                      *
   * --------------------------------------------------------------- */
  struct timeval start, end;
  long mtime, seconds, useconds;    

  gettimeofday(&start, NULL);//inicia o medidor de tempo
  
  //dispara asthreads do operador de seleção
  selection->ApplyGeneticOperator();


  //faz os joins com as threads
  JoinThreadsOperator(selection);
  JoinThreadsOperator(cross_over);
  JoinThreadsOperator(mutation);
  
  
  gettimeofday(&end, NULL);//termina o cantador de tempo

  seconds  = end.tv_sec  - start.tv_sec;
  useconds = end.tv_usec - start.tv_usec;

  //e calcula o tempo
  mtime = ((seconds) * 1000000.0 + useconds) + 0.5;

  std::cout << "Duração em microsegundos: " << mtime << std::endl;

  _timeOutput << mtime << std::endl;


  
  //atualiza os dados da população de seleção para obter o melhor indivíduo
  selection->UpdatePopulation();

  individual<_ty,_realTy>* id = selection->GetBestIndividual(); 
  std::cout << id->GetValue() << std::endl;
  //end debug 


}

//faz o join com as threads do operator op
template <typename _ty,typename _realTy>
void genetic_algorithm_thread<_ty,_realTy>::JoinThreadsOperator (genetic_operator_thread<_ty,_realTy>* op)
{
  for (typename genetic_operator_thread<_ty,_realTy>::it_ it = op->Begin(); it != op->End(); ++it)
  {
    //confere se é nulo antes de realizar o join
    if (*it)
      (*it)->join();
  }

}

#endif //GENETIC_ALGORITHM_THREAD_H
