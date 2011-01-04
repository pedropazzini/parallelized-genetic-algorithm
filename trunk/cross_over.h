/**
 * @file cross_over.h
 * @Synopsis Arquivo que contém a classe que simula um operador genético de cruzamento.
 * @author Pedro Pazzini
 * @version 0.0.1
 * @date 2010-10-30
 */
#ifndef CROOS_OVER_H
#define CROOS_OVER_H

#include <vector>
#include "genetic_operator.h"
#include "definitions.h"

/* ----------------------------------------*/
/**
 * @Synopsis Tipos default da classe, definidos no arquivo definitions.h
 *
 * @tparam _ty
 * @tparam _realTy
 */
/* ----------------------------------------*/
template <typename _ty = GAtype,typename _realTy = GAreal_type> class cross_over;

/* ----------------------------------------*/
/**
 * @Synopsis Classe que encapsula um operador genético de cruzamento. Herda os atributos da classe genetic_operator, já que o o operador genético é um tipo de operador. O método principal é o método CrossOver(), que realiza efetivamente o cruzamento entre dois indivíduos.
 *
 * @tparam _ty
 * @tparam _realTy
 */
/* ----------------------------------------*/
template <typename _ty,typename _realTy>
class cross_over : public genetic_operator<_ty,_realTy>
{
  public:

    /* ----------------------------------------*/
    /**
     * @Synopsis Método construtor
     *
     * @Param _to_apply_operator Ponteiro para a população na qual o operador irá realizar o cruzamento.
     * @Param number_coordinate Búmero de coordenadas das coordeanadas da população.
     * @Param probability Probabilidade da realazição do cruzamento.
     * @Param CP Ponto de corte do cruzamento.
     * @Param aleatory Se o ponto de corte é aleatório ou não.
     */
    /* ----------------------------------------*/
    cross_over (population<_ty,_realTy>* _to_apply_operator = NULL,
		const int& number_coordinate = def::genetic_operator::cross_over::number_coordinate,
		const float& probability = def::genetic_operator::cross_over::probability,
      		const int& CP = 9,
		const bool& aleatory = true);

    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (get). 
     *
     * @return Retorna se o ponto de corte do operador é aleatório.
     */
    /* ----------------------------------------*/
    const bool& IsAleatory (void) const {return _aleatory;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (set). 
     *
     * @Param new_aleatory Valor que fará com que o ponto de corte do operador seja aleatório ou não.
     */
    /* ----------------------------------------*/
    void SetAleatory (const bool& new_aleatory) {_aleatory = new_aleatory;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Realiza o casamento dos individuos.
     */
    /* ----------------------------------------*/
    virtual void MakePairs (void);

    /* ----------------------------------------*/
    /**
     * @Synopsis Realiza o cruzamento efetivamente.
     *
     * @Param c1 Um dos indivíduos do par que poderá ser cruzado.
     * @Param c2 O outro indivíduo do par que poderá ser cruzado.
     * @Param CP O ponto de corte do cruzamento.
     *
     * @return 
     */
    /* ----------------------------------------*/
    bool CrossOver ( coordinate<_ty,_realTy>& c1, coordinate<_ty,_realTy>& c2,const int& CP);

  protected:

    /* ----------------------------------------*/
    /**
     * @Synopsis Realiza o cruzamento entre duas coordenadas.
     *
     * @Param coord_1 Uma das coordenadas que participará do cruzamento.
     * @Param coord_2 A outra coordenada que participará do cruzamento.
     * @Param CP O ponto de corte do cruzamento.
     */
    /* ----------------------------------------*/
    virtual void CrossOver (_ty& coord_1, _ty& coord_2,const int& CP);

    /* ----------------------------------------*/
    /**
     * @Synopsis Método de gancho que caminha no vetor de indivíduos aplicando o operador de cruzamento.
     *
     * @Param id Indivíduo onde será aplicado o operador de cruzamento.
     *
     * @return Um identificador da nova direção de caminhada
     */
    /* ----------------------------------------*/
    virtual const char& WalkOnPopulationHook (individual<_ty,_realTy>& id); 

    /* ----------------------------------------*/
    /**
     * @Synopsis Método de gancho que caminha no vetor de coordenadas aplicando o operador de cruzamento.
     *
     * @Param coo Coordenada onde será aplicado o operador genético.
     *
     * @return A direção de caminhada do algoritmo.
     */
    /* ----------------------------------------*/
    virtual const char& WalkOnIndividualHook (coordinate<_ty,_realTy>& coo);

    
  private:

    int _numberCoordinate;//Número de coordenadas para cada indivíduo
    std::vector<int> _cuttof_point;//Ponto de corte para cada coordenada de um individuo
    std::vector<bool> _aleatory;//Informação se é aleatorio ou nao o ponto de corte para cada coordenada
};

template<typename _ty,typename _realTy>
cross_over<_ty,_realTy>::cross_over (population<_ty,_realTy>* _to_apply_operator,const int& number_coordinate,const float& probability,const int& CP,const bool& aleatory)
:genetic_operator<_ty,_realTy>(_to_apply_operator,probability)
{
  _numberCoordinate = number_coordinate;
  for (int i = _numberCoordinate; i !=0; --i)
  {
    _aleatory.push_back(aleatory);
    _cuttof_point.push_back(CP);
  }
}

template <typename _ty,typename _realTy>
void cross_over<_ty,_realTy>::CrossOver (_ty& coord_1, _ty& coord_2, const int& CP)
{
  _ty temp_1,temp_2,mask_high,mask_low;
  
  mask_low = _ty(pow(2,CP));
  mask_low -= 1;//faz uma máscara com os bits baixos
  mask_high = ~mask_low;//faz uma máscara com os bits altos
  
  temp_1 = coord_1 & mask_low;//grava os bits baixos de coord_1 em temp_1
  coord_1 = coord_1 & mask_high;//limpa os bits baixos de coord_1
  
  temp_2 = coord_2 & mask_low;//grava os bits baixos de coord_2 em temp_2
  coord_2 = coord_2 & mask_high;//limpa os bits baixos de coord_2
  
  //realiza p cruzamento
  coord_1 += temp_2;
  coord_2 += temp_1;
}

template <typename _ty,typename _realTy>
bool cross_over<_ty,_realTy>::CrossOver ( coordinate<_ty,_realTy>& c1, coordinate<_ty,_realTy>& c2, const int& CP)
{  
  if(c1.GetIndice()==c2.GetIndice())
  {//se o cruzamento ocorrerá para duas dimensões idênticas
    CrossOver(c1.GetValue(),c2.GetValue(),CP);
    return true;
  }
  else
    return false;
}

template<typename _ty,typename _realTy>
const char& cross_over<_ty,_realTy>::WalkOnPopulationHook (individual<_ty,_realTy>& id)
{
  //se o indivíduo possui um par
  if(id.GetPair() != NULL)
  {//Se os individuos estao realmente casados
    if (id.GetPair()->GetPair() == &id)
    {
      int i = 0;
      for (typename individual<_ty,_realTy>::it_ it = id.begin(); it != id.end(); ++it)
      {
	if (genetic_operator<_ty,_realTy>::_probability > genetic_operator<_ty,_realTy>::GenerateRandom())
	{
	  //Realiza o Cruzamento
	  if (_aleatory[i])
	  {//se for cruzamento do tipo aleatório, gera o ponto de corte aleatório
	    int new_pc;
	    //Gera o número aleatório do tamanho da coordenada
	    do
	    {
	      new_pc =  genetic_operator<_ty,_realTy>::GenerateRandom(_ty((*it)->GetSize()-1));
	    }while ((new_pc<0)&&(new_pc >= (*it)->GetSize()));

	    _cuttof_point[i] = new_pc;//seta o novo ponto de corte aleatório
	  }
	  //Realiza propriamente o cruzamento
	  CrossOver(*(*it),*(*(id.GetPair()->begin()+i)),_cuttof_point[i]);
	}
	i++;
      }
      //Separa os indivíduos para a próxima geração
      individual<_ty,_realTy>::SeparetePair(id,*id.GetPair());
      return def::genetic_operator::go_forward;
    }
  }
  return def::genetic_operator::go_forward;
}


template<typename _ty,typename _realTy>
const char& cross_over<_ty,_realTy>::WalkOnIndividualHook (coordinate<_ty,_realTy>& coo)
{
  return def::genetic_operator::go_up;
}

template<typename _ty,typename _realTy>
void cross_over<_ty,_realTy>::MakePairs (void)
{

  for (typename population<_ty,_realTy>::it_ it = genetic_operator<_ty,_realTy>::_to_apply_operator->begin();
     					     it != genetic_operator<_ty,_realTy>::_to_apply_operator->end();
					     ++it)
  {
    //Se o indivíduo ainda não possui parceiro
    if ( (*it)->GetPair() == NULL )
    {
      int indice;
      bool keep_whiling;
      individual<_ty,_realTy>* got_pair;
      //Acha um indivíduo sem par
      do
      {
	_ty max = genetic_operator<_ty,_realTy>::_to_apply_operator->GetNumerOfIndividuals();
	max -= 1;
	indice = genetic_operator<_ty,_realTy>::GenerateRandom(max);
	got_pair = *(genetic_operator<_ty,_realTy>::_to_apply_operator->begin()+indice);
	if ((got_pair != NULL) && (got_pair->GetPair() == NULL) && (got_pair != *it) )
	  keep_whiling = false;
	else
	  keep_whiling = true;
      } while(keep_whiling);

      //Marca os pares
      individual<_ty,_realTy>::MakePair (*(*it),*got_pair);
    }
  }
}

#endif //CROOS_OVER_H
