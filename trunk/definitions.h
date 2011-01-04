/**
 * @file definitions.h
 * @Synopsis Arquivo de configuração do GA. Contém parâmetros defaults como valores máximo mínimo das coordenadas, número de indivíduos e número de gerações.
 * @author Pedro Pazzini
 * @version 0.0.1
 * @date 2010-10-29
 */
#ifndef DEFIINITIONS_H
#define DEFIINITIONS_H

#define function0 false
#define function4 false
#define function5 true
#define function6 false


namespace def
{
  #define GAtype unsigned int
  #define GAreal_type float
  
  namespace coord
  {

    //definições básicas de coordenadas da função de otimização 1
    #if function0

    #define pc_cte false //seta um ponto de corte constan
    #define def_pc 12
    const int size = sizeof(GAtype)*8;
    const int precision = 5;
    const GAreal_type max = 5;
    const GAreal_type min = -5;
    const int indice = 0;

    #endif

    //definições básicas da função de otimização 4 do artigo
    #if function4
    #define pc_cte false //seta um ponto de corte constan
    #define def_pc 12
    const int size = sizeof(GAtype)*8;
    const int precision = 6;
    const GAreal_type max = 500;
    const GAreal_type min = -500;
    const int indice = 0;
    #endif
    
    //definições básicas da função de otimização 5 do artigo
    #if function5
    #define pc_cte false //seta um ponto de corte constan
    #define def_pc 12
    const int size = sizeof(GAtype)*8;
    const int precision = 6;
    const GAreal_type max = 5.12;
    const GAreal_type min = -5.12;
    const int indice = 0;
    #endif
    
    //definições básicas da função de otimização 6 do artigo
    #if function6
    #define pc_cte false //seta um ponto de corte constan
    #define def_pc 12
    const int size = sizeof(GAtype)*8;
    const int precision = 6;
    const GAreal_type max = 600;
    const GAreal_type min = -600;
    const int indice = 0;
    #endif
  }//namespace coord
  
  //basic definitions of the individual class
  namespace individual
  {
    const int identifier = 0;
    #if function0
    const int dimension=2;
    #endif
    #if function4
    const int dimension = 30;
    #endif
    #if function5
    const int dimension = 30;
    #endif
    #if function6
    const int dimension = 30;
    #endif
    const int size = coord::size;
  }//namespace individual

  namespace population
  {
      #if function0
      const int population_size = 20;
      #endif

      #if function4
      const int population_size = 300;
      #endif

      #if function5
      const int population_size = 300;
      #endif
      
      #if function6
      const int population_size = 300;
      #endif
  }//namespace population

  namespace genetic_operator 
  {
    const float probability = 0.01;
    const int precision = coord::precision+1;
    const char go_down = 'd';
    const char go_up = 'u';
    const char go_forward = 'f';
    const int numberOfGenerations = 1000;

    namespace mutate_bit_by_bit
    {
      const float probability = 0.01;
    }//namespace mutate_bit_by_bit

    namespace cross_over
    {
     const float probability = 0.8;//probabilidade de cruzamento de cada coordenada dos indivíduos
      const int number_coordinate = individual::dimension;
    }//namespace cross_over


  }//namespace genetic_operator

  /* ----------------------------------------*/
  /**
   * @Synopsis Namespace com as características dos atributos de estatísticas do GA.
   */
  /* ----------------------------------------*/
  namespace statistics
  {
    /* --------------------------------------------------------------- *
     * --- Região de saída dos dados                                   *
     * --------------------------------------------------------------- */

    /* ----------------------------------------*/
    /**
     * @Synopsis Máskara para enviar o melhor indivíduo para o arquivo de estatística
     */
    /* ----------------------------------------*/
    const short int _maskBestIdFitness = 1;

    /* ----------------------------------------*/
    /**
     * @Synopsis Máscara para enviar o pior indivíduo para o arquivo de estatística.
     */
    /* ----------------------------------------*/
    const short int _maskWorseIdFitness = 2;

    /* ----------------------------------------*/
    /**
     * @Synopsis Máscara para enviar o desempenho médio da população para a saída padrão.
     */
    /* ----------------------------------------*/
    const short int _maskAvaregeFitness = 4;

    /* ----------------------------------------*/
    /**
     * @Synopsis Máscara para enviar a variância do desempenho médio da população para o arquivo de saída.
     */
    /* ----------------------------------------*/
    const short int _maskDeviationFitness = 8;

    /* ----------------------------------------*/
    /**
     * @Synopsis Máscara que envia os dados de desempenho médio, variância do desempenho médio, e fitness do melhor e pior indivíduo.
     */
    /* ----------------------------------------*/
    const short int _maskAllData = 15;
  }//namespace statistics

}//namespace def




#endif //DEFIINITIONS_H
