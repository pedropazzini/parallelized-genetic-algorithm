#ifndef SELECTION_BY_TOURNAMENT_OPERATOR_H
#define SELECTION_BY_TOURNAMENT_OPERATOR_H

#include "genetic_operator.h"
#include "definitions.h"
#include "individual.h"
#include "population.h"

template <typename _ty = GAtype,typename _realTy = GAreal_type> class selection_by_tournament_operator;

template <typename _ty,typename _realTy>
class selection_by_tournament_operator : public genetic_operator<_ty,_realTy>
{
  public:

     
    selection_by_tournament_operator (population<_ty,_realTy>* pt_to_apply_operator = NULL):genetic_operator<_ty,_realTy>(pt_to_apply_operator){new_pop.clear();}

    /* ----------------------------------------*/
    /**
     * @Synopsis Método que aplica o operador de seleção por torneio (sem threads)
     */
    /* ----------------------------------------*/
    void doApplyGeneticOperator (void);

    const char& WalkOnPopulationHook (individual<_ty,_realTy>& id){};
    const char& WalkOnIndividualHook (coordinate<_ty,_realTy>& coo){};

  private:

    /* ----------------------------------------*/
    /**
     * @Synopsis Nétodo que realiza um torneio e retorna o indivíduo selecionado.
     *
     * @return O indivíduo selecionado após uma seleção por torneio.
     */
    /* ----------------------------------------*/
    individual<_ty,_realTy>* ReturnSelected (void);

   /* ----------------------------------------*/
   /**
    * @Synopsis Nova população formada a cada seleção.
    */
   /* ----------------------------------------*/
   typename population<_ty,_realTy>::_pop new_pop;//nova população após a seleção

};

template <typename _ty,typename _realTy>
void selection_by_tournament_operator<_ty,_realTy>::doApplyGeneticOperator (void)
{

  //contador de indivíduos selecrionados
  int countId = this->_to_apply_operator->GetNumerOfIndividuals();

  //realiza a seleção e adiciona à nova população
  while (countId--)
  {
    new_pop.push_back(ReturnSelected());
  }

  //limpa a populção do antigo indivíduo
  this->_to_apply_operator->SetNewIndividuals(new_pop);

  //limpa a nova população para a próxima seleção
  new_pop.clear();


}

template <typename _ty,typename _realTy>
individual<_ty,_realTy>* selection_by_tournament_operator<_ty,_realTy>::ReturnSelected (void)
{
  //gera os índices aleatórios para comparar dois indivíduos(com a mesma probabilidade)
  int indice_1 = rand() % this->_to_apply_operator->GetNumerOfIndividuals();
  int indice_2 = rand() % this->_to_apply_operator->GetNumerOfIndividuals();

  //o ondivíduo a ser retornado
  individual<_ty,_realTy>* idPt; 
  
  //faz o torneio propriamente dito
  individual<_ty,_realTy>* id_1 = (*this->_to_apply_operator)[indice_1];
  individual<_ty,_realTy>* id_2 = (*this->_to_apply_operator)[indice_2];
  if (id_1->GetValue() <= id_2->GetValue())
    idPt = new individual<_ty,_realTy>(*id_1);
  else
    idPt = new individual<_ty,_realTy>(*id_2);

  return idPt;

}

#endif //SELECTION_BY_TOURNAMENT_OPERATOR_H
