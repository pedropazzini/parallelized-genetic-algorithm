/**
 * @file selection_by_roulette.h
 * @Synopsis Arquivo que contém a definição da classe que encapsula os conceitos de um operador genético de seleção por roleta.
 * @author Pedro Pazzini
 * @version 0.0.1
 * @date 2010-10-30
 */
#ifndef SELECTION_BY_ROULETTE_H
#define SELECTION_BY_ROULETTE_H 

#include <string>
#include <utility>//std::pair
#include <vector>

#include "genetic_operator.h"
#include "definitions.h"

/* ----------------------------------------*/
/**
 * @Synopsis Valore tamplates default para a classe, definidos no arquivo definitions.h
 *
 * @tparam _ty
 * @tparam _realTy
 */
/* ----------------------------------------*/
template <typename _ty = GAtype,typename _realTy= GAreal_type> class selection_by_roulette; 

/* ----------------------------------------*/
/**
 * @Synopsis Classe template que encapsula os métodos e atributos de um operador genético de seleção por torneio. Herda da classe genetic_operator. Os métods principais são GenerateRoulette() e RotateRoulette().
 *
 * @tparam _ty
 * @tparam _realTy
 */
/* ----------------------------------------*/
template<typename _ty,typename _realTy>
class selection_by_roulette : public genetic_operator<_ty,_realTy>
{
  public:

    /* --------------------------------------------------------------- *
     * --- Algumas definições básicas da classe                        *
     * --------------------------------------------------------------- */

    /* ----------------------------------------*/
    /**
     * @Synopsis Definição do container que armazena uma roleta.
     */
    /* ----------------------------------------*/
   typedef typename std::vector<std::pair<_realTy,individual<_ty,_realTy>*> > _RouletteTy;

   /* ----------------------------------------*/
   /**
    * @Synopsis Definição para um iterator que percorre o container da roleta.
    */
   /* ----------------------------------------*/
   typedef typename std::vector<std::pair<_realTy,individual<_ty,_realTy>*> >::iterator it_;

   /* ----------------------------------------*/
   /**
    * @Synopsis Definição para um iterator constante que percorre o container da roleta.
    */
   /* ----------------------------------------*/
   typedef typename std::vector<std::pair<_realTy,individual<_ty,_realTy>*> >::const_iterator const_it_;

   /* ----------------------------------------*/
   /**
    * @Synopsis Método construtor.
    *
    * @Param pt_to_apply_operator Ponteiro para a população na qual será aplicado o operador.
    */
   /* ----------------------------------------*/
   selection_by_roulette(population<_ty,_realTy>* pt_to_apply_operator = NULL):genetic_operator<_ty,_realTy>::genetic_operator(pt_to_apply_operator){}

   /* ----------------------------------------*/
   /**
    * @Synopsis Retorna o nome da classe para o padrão AbstractFactory
    *
    * @return Uma string com o nome da classe.
    */
   /* ----------------------------------------*/
   virtual std::string GetName (void) {return genetic_operator<_ty,_realTy>::GetName() + "->" + "selection_by_roulette";}

   /* ----------------------------------------*/
   /**
    * @Synopsis O método de seleção por roleta não realiza nenhuma operação direta sobre as coordenadas
    *
    * @Param coo A coordenada onde seria aplicado o operador.
    *
    * @return A direção de caminhada do vetor.
    */
   /* ----------------------------------------*/
   virtual const char& WalkOnIndividualHook (coordinate<_ty,_realTy>& coo) {return def::genetic_operator::go_up;}

   /* ----------------------------------------*/
   /**
    * @Synopsis O método de seleção por roleta realiza operações diretamente nos indivíduos
    *
    * @Param id O indivíduo aonde será aplicado o operador de seleção.
    *
    * @return A direção da caminhada do vetor.
    */
   /* ----------------------------------------*/
   virtual const char& WalkOnPopulationHook (individual<_ty,_realTy>& id);

   /* ----------------------------------------*/
   /**
    * @Synopsis Método que retorna um iterator para percorrer a roleta.
    *
    * @return Iterator para o começo do container que armazena a roleta.
    */
   /* ----------------------------------------*/
   const_it_ begin (void) const {return _myRoulette.begin();}

   /* ----------------------------------------*/
   /**
    * @Synopsis Métod que retorna um iterator para percorre a roleta.
    *
    * @return Iterator para o fim da roleta.
    */
   /* ----------------------------------------*/
   const_it_ end (void) const {return _myRoulette.end();}

  protected:

   /* ----------------------------------------*/
   /**
    * @Synopsis Constrói a roleta(preenche _myRoulette)
    */
   /* ----------------------------------------*/
   virtual void GenerateRoulette (void);

   /* ----------------------------------------*/
   /**
    * @Synopsis Realiza a seleção propriamente dita, ou seja roda a roleta
    */
   /* ----------------------------------------*/
   virtual void RotateRoulette (void);
   
  private:

   /* ----------------------------------------*/
   /**
    * @Synopsis Container que armazena a roleta.
    */
   /* ----------------------------------------*/
   _RouletteTy _myRoulette;

   /* ----------------------------------------*/
   /**
    * @Synopsis Nova população formada a cada seleção.
    */
   /* ----------------------------------------*/
   typename population<_ty,_realTy>::_pop new_pop;//nova população após a seleção

};

template<typename _ty,typename _realTy>
const char& selection_by_roulette<_ty,_realTy>::WalkOnPopulationHook (individual<_ty,_realTy>& id)
{
  GenerateRoulette();
  RotateRoulette();
  genetic_operator<_ty,_realTy>::_to_apply_operator->SetNewIndividuals(new_pop);
  new_pop.clear();
  return def::genetic_operator::go_up;
}

template<typename _ty,typename _realTy>
void selection_by_roulette<_ty,_realTy>::GenerateRoulette (void)
{
  _realTy partial = 0;  
  _realTy sum = genetic_operator<_ty,_realTy>::_to_apply_operator->GetSum();
  for (typename population<_ty,_realTy>::const_it_ it = genetic_operator<_ty,_realTy>::_to_apply_operator->begin();
       it != genetic_operator<_ty,_realTy>:: _to_apply_operator->end(); ++it)
  {
    partial += (*it)->GetValue()/sum;
    _myRoulette.push_back(std::make_pair(partial,*it));
  }
}

template<typename _ty,typename _realTy>
void selection_by_roulette<_ty,_realTy>::RotateRoulette (void)
{
  for (int i = 0; i != genetic_operator<_ty,_realTy>::_to_apply_operator->GetNumerOfIndividuals(); ++i)
  {
     _realTy coin = genetic_operator<_ty,_realTy>::GenerateRandom(); //gera um numero aleatório de 0 a 1
     for ( const_it_ it = _myRoulette.begin(); it != _myRoulette.end(); ++ it)
     {
       if (it->first >= coin)
       {
	 it->second->SetID(i);
	 individual<_ty,_realTy> *new_id = new individual<_ty,_realTy>(*it->second);
         new_pop.push_back(new_id);
         break;//sai do ultimo laço(laço do iterator)
       }
     }
  }
  //esvazia a roleta para a próxima seleção
  _myRoulette.clear();
}

#endif //SELECTION_BY_ROULETTE_H
