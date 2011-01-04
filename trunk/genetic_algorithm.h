/**
 * @file genetic_algorithm.h
 * @Synopsis Arquivo que encapsula os conceitos de um algoritmo genético.
 * @author Pedro Pazzini
 * @version 0.0.1
 * @date 2010-10-30
 */
#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

#include "population.h"
#include "genetic_operator.h"
#include "selection_by_roulette.h"
#include "selection_by_tournament_operator.h"
#include "cross_over.h"
#include "mutate_bit_by_bit.h"
#include "definitions.h"
#include <fstream>
#include <ctime>
#include <sys/time.h>

/* ----------------------------------------*/
/**
 * @Synopsis Valores template default. Os valores defaults estão definidos no arquivo definitions.h
 *
 * @tparam _ty
 * @tparam _realTy
 */
/* ----------------------------------------*/
template <typename _ty = GAtype,typename _realTy = GAreal_type> class genetic_algorithm;

/* ----------------------------------------*/
/**
 * @Synopsis Classe que encapsula os conceitos do algoritmo genético e inicia o GA.
 *
 * @tparam _ty
 * @tparam _realTy
 */
/* ----------------------------------------*/
template <typename _ty,typename _realTy>
class genetic_algorithm
{
  public:

    /* ----------------------------------------*/
    /**
     * @Synopsis Método construtor
     *
     * @Param max_generation Número máximo de gerações.
     */
    /* ----------------------------------------*/
    genetic_algorithm (const int& max_generation = 1000,const bool& genStatistic = false);

    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (get). 
     *
     * @return O número máximo de gerações.
     */
    /* ----------------------------------------*/
    const int& GetMaxGeneration (void) const {return _max_generation;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (set). 
     *
     * @Param new_max O novo valor do número máximo de gerações.
     */
    /* ----------------------------------------*/
    void SeMaxGeneration (const int& new_max) {_max_generation = new_max;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Método que irá iniciar o algoritmo genético.
     */
    /* ----------------------------------------*/
    void StartGA (void);

  protected:

    std::ofstream out;

    /* ----------------------------------------*/
    /**
     * @Synopsis Variável que quando é true gera os arquivos de estatística do ga.
     */
    /* ----------------------------------------*/
    const bool _generate_statistic;

    /* ----------------------------------------*/
    /**
     * @Synopsis Aruivo que registra o tempo de execução do ga
     */
    /* ----------------------------------------*/
    std::ofstream _timeOutput;

    /* ----------------------------------------*/
    /**
     * @Synopsis População do GA
     */
    /* ----------------------------------------*/
    population<_ty,_realTy>* _population;

    /* ----------------------------------------*/
    /**
     * @Synopsis Número máximo de gerações do GA.
     */
    /* ----------------------------------------*/
    int _max_generation;

    /* ----------------------------------------*/
    /**
     * @Synopsis Operador genético de mutação do GA.
     */
    /* ----------------------------------------*/
    genetic_operator<_ty,_realTy>* _mutation;

    /* ----------------------------------------*/
    /**
     * @Synopsis Operador genético de cruzamento do GA.
     */
    /* ----------------------------------------*/
    cross_over<_ty,_realTy>* _cross_over;

    /* ----------------------------------------*/
    /**
     * @Synopsis Operador genético de seleção do GA.
     */
    /* ----------------------------------------*/
    genetic_operator<_ty,_realTy>* _selection;

    /* ----------------------------------------*/
    /**
     * @Synopsis Método que envia os dados estatísticos da população para um arquivo de saída
     */
    /* ----------------------------------------*/
    void SendDataToOutput (const int& generation);
};

template <typename _ty,typename _realTy>
genetic_algorithm<_ty,_realTy>::genetic_algorithm (const int& max_generation,const bool& genStatistic)
  :out("OutputGA.dat")
   ,_timeOutput("timeGA.dat",std::ios::app)
   ,_generate_statistic(genStatistic)
{
  _max_generation = max_generation;
  _population = new population<_ty,_realTy>();
  _mutation = new mutate_bit_by_bit<_ty,_realTy>(_population);
  _cross_over = new cross_over<_ty,_realTy>(_population);
  _selection = new selection_by_tournament_operator<_ty,_realTy>(_population);
}

template <typename _ty,typename _realTy>
void genetic_algorithm<_ty,_realTy>::StartGA (void)
{
  int iter = _max_generation;
  _population->GeneratePopulation();//Gera a população aleatória

  /* --------------------------------------------------------------- *
   * --- Inicia o GA e mede o tempo de execução                      *
   * --------------------------------------------------------------- */
  struct timeval start, end;
  long mtime, seconds, useconds;    

  gettimeofday(&start, NULL);//inicia o medidor de tempo
  
  do
  {

    //Calcula o valor da função objetivo com as novas coordenadas(preparação para a seleção)
    _population->SetIndividualsValue();
    
    //Aplica o operador de seleção
    _selection->doApplyGeneticOperator();

    //Cria os pares de indivíduos, e aplica o operador de cruzamento
    _cross_over->MakePairs();
    _cross_over->doApplyGeneticOperator();

    //Finalmente aplica o operador de mutação
    _mutation->doApplyGeneticOperator();

    //envia os dados para os arquivos de saída
    if(_generate_statistic)
      SendDataToOutput(_max_generation-iter);

  } while (--iter);

  _population->SetIndividualsValue();
  
  gettimeofday(&end, NULL);//termina o cantador de tempo

  seconds  = end.tv_sec  - start.tv_sec;
  useconds = end.tv_usec - start.tv_usec;

  //e calcula o tempo
  mtime = ((seconds) * 1000000.0 + useconds) + 0.5;

  std::cout << "Duração em microsegundos: " << mtime << std::endl;

  std::cout << _population->GetBestId()->GetValue() << std::endl;

  _timeOutput << mtime << std::endl;
}

template <typename _ty,typename _realTy>
void genetic_algorithm<_ty,_realTy>::SendDataToOutput (const int& generation) 
{
    //geração na coluna 0
    out << generation << " ";

    out << _population->GetBestId()->GetValue() << " ";
    out << _population->GetWorseId()->GetValue() << " ";
    out << _population->GetAveragePerformance() << " ";
    out << _population->GetDeviation() << " ";

    //quebra a linha
    out << std::endl;

}

#endif //GENETIC_ALGORITHM_H
