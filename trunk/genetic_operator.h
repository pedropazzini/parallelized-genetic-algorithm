/**
 * @file genetic_operator.h
 * @Synopsis Arquivo que contém as definições da classe abstrata genetic_operator.
 * @author Pedro Pazzini
 * @version 0.0.1
 * @date 2010-10-30
 */
#ifndef GENETIC_OPERATOR_H
#define GENETIC_OPERATOR_H

#include <string>

#include "population.h"
#include "definitions.h"

/* ----------------------------------------*/
/**
 * @Synopsis Valores default da classe abstrata. Os valores defaults são definidos no arquivo definitions.h.
 *
 * @tparam _ty
 * @tparam _realTy
 */
/* ----------------------------------------*/
template <typename _ty = GAtype,typename _realTy = GAreal_type> class genetic_operator;

template <typename _ty,typename _realTy>
class genetic_operator
{
  public:

    /* ----------------------------------------*/
    /**
     * @Synopsis Método construtor.
     *
     * @Param pt_to_apply_operator Ponteiro para o objeto população aonde será aplicado o operador genético.
     * @Param probability Probabilidade do operador genético realizar uma operação.
     */
    /* ----------------------------------------*/
    genetic_operator (population<_ty,_realTy>* pt_to_apply_operator = NULL,const float& probability = def::genetic_operator::probability)
                     :_to_apply_operator(pt_to_apply_operator),_probability(probability){};
    
    /* ----------------------------------------*/
    /**
     * @Synopsis Método virtual que realiza a mutação
     */
    /* ----------------------------------------*/
    virtual void doApplyGeneticOperator (void);

    /* ----------------------------------------*/
    /**
     * @Synopsis Método que retorna um número aleatório entre min e max do tipo _realTy(ponto flutuante) com precisão de precision casas decimais
     *
     * @Param max Valor máximo do intervalo do número aleatório a ser gerado.
     * @Param min Valor mínimo do intervalo do número aleatório a ser gerado.
     * @Param precision Número de casas decimais do número aleatório gerado.
     *
     * @return Um número aleatório com precision casas decimais e entre os valores min e max.
     */
    /* ----------------------------------------*/
    const _realTy GenerateRandom (const _realTy& max = 1,const _realTy& min = 0, const int& precision = def::genetic_operator::precision);

    /* ----------------------------------------*/
    /**
     * @Synopsis Método que retorna um número entre min e max do tipo _ty(sem ponto flutuante)
     *
     * @Param max Valor máximo do intervalo do número aleatório a ser gerado.
     * @Param min Valor mínimo do intervalo do número aleatório a ser gerado.
     *
     * @return Um número aleatório entre os valores min e max.
     */
    /* ----------------------------------------*/
    const _ty GenerateRandom (const _ty& max,const _ty& min = 0);

    /* ----------------------------------------*/
    /**
     * @Synopsis Método de gancho puramente virtual para as classes filhas, usadas no padrão Template Method. Caminha sobre o conteiner de indivíduos da populacao, e aplica o operador nos indivíduos
     *
     * @Param id Indivíduo em que será aplicado o operador genético.
     *
     * @return A direção da caminhada nos containers.
     */
    /* ----------------------------------------*/
    virtual const char& WalkOnPopulationHook (individual<_ty,_realTy>& id) =0;
    
    /* ----------------------------------------*/
    /**
     * @Synopsis Método de gancho puramente virtual para as classes filhas, usadas no padrão Template Method. Caminha sobre o conteiner de coordenadas do indivíduo, e aplica o operador nas coordenadas.
     *
     * @Param coo A coordenada onde será aplicado o operador.
     *
     * @return A direção da caminhada nos conteiners.
     */
    /* ----------------------------------------*/
    virtual const char& WalkOnIndividualHook (coordinate<_ty,_realTy>& coo) =0;

    /* ----------------------------------------*/
    /**
     * @Synopsis Método que retorna o nome da classe, usado no padrão AbstractFactory
     *
     * @return Uma string com o nome do operador.
     */
    /* ----------------------------------------*/
    virtual std::string GetName (void) {return std::string("genetic_operator");}

  protected:

    population<_ty,_realTy>* _to_apply_operator;//ponteiro para a coordenada que sofrerá a mutação
    float _probability;//probabilidade de mutaçao, a classe base definirá se é bit a bit, de coordenada ou indivíduo

};

template <typename _ty,typename _realTy>
void genetic_operator<_ty,_realTy>::doApplyGeneticOperator (void)
{
  for (typename population<_ty,_realTy>::const_it_ it = _to_apply_operator->begin(); it != _to_apply_operator->end(); ++it)
  {
    char direction = WalkOnPopulationHook(*(*it));//aplica o operador nos indivíduos da população
    if (direction == def::genetic_operator::go_up)
      return;//termina com o operador
    else if (direction == def::genetic_operator::go_down)
    {//Se o operador é aplicado nas coordenadas desce para o vetor de coordenadas do indivíduo
      for (typename individual<_ty,_realTy>::const_it_ it_id = (*it)->begin(); it_id != (*it)->end(); ++it_id)
      {
	char other_direction = WalkOnIndividualHook(*(*it_id));//aplica o operador nas coordenadas do individuo (*it_id) 
	if (other_direction == def::genetic_operator::go_up)
	  break;
      }
    }
  }
}

template <typename _ty,typename _realTy>
const _realTy genetic_operator<_ty,_realTy>::GenerateRandom (const _realTy& max, const _realTy& min,const int& precision)
{
  _realTy to_int_max = max*pow(10,precision);
  _realTy to_int_min = min*pow(10,precision);
  int int_min = floor(to_int_min);
  int int_max = floor(to_int_max);
  int random = int_min + rand()%int_max +1;
  _realTy ret = _realTy(random);
  ret *= pow(10,-precision);
  return ret;
}

template <typename _ty,typename _realTy>
const _ty genetic_operator<_ty,_realTy>::GenerateRandom (const _ty& max, const _ty& min)
{
  _ty random = min + _ty(rand())%max + 1;
  return random;
}

#endif //GENETIC_OPERATOR_H
