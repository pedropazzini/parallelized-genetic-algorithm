#ifndef MUTATE_BIT_BY_BIT_H
#define MUTATE_BIT_BY_BIT_H

#include <string>

#include "genetic_operator.h"
#include "definitions.h"

template <typename _ty = GAtype,typename _realTy = GAreal_type> class mutate_bit_by_bit;

template <typename _ty,typename _realTy>
class mutate_bit_by_bit : public genetic_operator<_ty,_realTy>
{
  public:

    /* ----------------------------------------*/
    /**
     * @Synopsis Método contrutor.
     *
     * @Param pt_to_apply_operator Ponteiro para a população aonde o operador genético será aplicado.
     * @Param probability Probabilidade do operador genético ser aplicado.
     */
    /* ----------------------------------------*/
    mutate_bit_by_bit (population<_ty,_realTy>* pt_to_apply_operator = NULL,
		       const float& probability = def::genetic_operator::mutate_bit_by_bit::probability)
		       :genetic_operator<_ty,_realTy>::genetic_operator(pt_to_apply_operator,probability){}

    /* ----------------------------------------*/
    /**
     * @Synopsis Nenhuma operação é realizada diretamente nos indivíduos, apenas nas coordenadas
     *
     * @Param id Indivíduo aonde será aplicado o operador.
     *
     * @return A direção da caminhada sobre os containers.
     */
    /* ----------------------------------------*/
    virtual const char& WalkOnPopulationHook (individual<_ty,_realTy>& id ){return def::genetic_operator::go_down;}

    /* ----------------------------------------*/
    /**
     * @Synopsis A operação bit a bit de mutação é realizada nas coordenadas
     *
     * @Param coo A coordenada em que será aplicado o operador.
     *
     * @return A direção da caminhada sobre os conteiners.
     */
    /* ----------------------------------------*/
    virtual const char& WalkOnIndividualHook (coordinate<_ty,_realTy>& coo);

    /* ----------------------------------------*/
    /**
     * @Synopsis Retorna o nome da classe, usado no padrão AbstractFactory
     *
     * @return Uma string com o nome da classe.
     */
    /* ----------------------------------------*/
    virtual std::string GetName (void) {return (genetic_operator<_ty,_realTy>::GetName()+ "->" + "mutate_bit_by_bit");}

};

template<typename _ty,typename _realTy>
const char& mutate_bit_by_bit<_ty,_realTy>::WalkOnIndividualHook (coordinate<_ty,_realTy>& coo)
{
  int size = coo.GetSize();
  int mask_1 = 1;
  for (int i = 0; i != size; ++i)
  {

    if (genetic_operator<_ty,_realTy>::GenerateRandom()<genetic_operator<_ty,_realTy>::_probability)
    {
      int mask_2 = coo.GetValue() & mask_1;
      if (mask_2)
       coo.SetValue(coo.GetValue() - mask_1);
      else
	coo.SetValue(coo.GetValue() + mask_1);
    }
    mask_1 = mask_1 << 1;
  }
  return def::genetic_operator::go_forward;
}

#endif //MUTATE_BIT_BY_BIT_H
