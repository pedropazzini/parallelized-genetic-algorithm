/**
 * @file population.h
 * @Synopsis Arquivo que contém a definição de uma população do algoritmo genético.
 * @author Pedro Pazzini
 * @version 0.0.1
 * @date 2010-10-29
 */
#ifndef POPULATION_H
#define POPULATION_H

#include <vector>
#include <iostream>
#include <algorithm>

#include "coordinate.h"
#include "individual.h"
#include "definitions.h"

/* ----------------------------------------*/
/**
 * @Synopsis Valor default, definido no arquivo definitions.h
 *
 * @tparam _ty
 * @tparam _realTy
 */
/* ----------------------------------------*/
template <typename _ty = GAtype, typename _realTy = GAreal_type> class population;

/* ----------------------------------------*/
/**
 * @Synopsis Valor default, definido no arquivo definitions.h
 *
 * @tparam _ty
 * @tparam _realTy
 */
/* ----------------------------------------*/
template <typename _ty = GAtype, typename _realTy = GAreal_type> class population_compare;


/* --------------------------------------------------------------------------*/
/** 
 * @Synopsis  Function object used in the sort method to sort the elements of 
 container that stores the individuals of the population.
 */
/* ----------------------------------------------------------------------------*/
template <typename _ty,typename _realTy>
struct population_compare
{
  bool operator() (const individual<_ty,_realTy>* a,const individual<_ty,_realTy>* b ) const
  {
    return *a < *b;
  }
};


/* --------------------------------------------------------------------------*/
/** 
 * @Synopsis  Class that encapsulates the concept of a population in a Genetic
 algorithm optimization method.

 Importantan Variables:

 _myPop: Container that stores the individuals of the population
 _bestId,_worseId: Respectivily best and worse individuals of the consteiner _myPop
 _avarege: Avarage value of the objective function of the population

 */
/* ----------------------------------------------------------------------------*/
template <typename _ty,typename _realTy>
class population
{
  public:

    /* --------------------------------------------------------------- *
     * --- Algumas definições básicas                                  *
     * --------------------------------------------------------------- */

    /* ----------------------------------------*/
    /**
     * @Synopsis Definição de uma população.
     */
    /* ----------------------------------------*/
    typedef typename std::vector<individual<_ty,_realTy>*> _pop;

    /* ----------------------------------------*/
    /**
     * @Synopsis Definição de um iterator para percorrer o vetor de indivíduos.
     */
    /* ----------------------------------------*/
    typedef typename std::vector<individual<_ty,_realTy>*>::iterator it_; 

    /* ----------------------------------------*/
    /**
     * @Synopsis Definição de um iterator constante para percorrer o vetor de indivíduos.
     */
    /* ----------------------------------------*/
    typedef typename std::vector<individual<_ty,_realTy>*>::const_iterator const_it_;

    /* --------------------------------------------------------------- *
     * --- Construtores/Destrutores                                    *
     * --------------------------------------------------------------- */

    /* ----------------------------------------*/
    /**
     * @Synopsis Método contrutor que recebe apenas o tamanho da população, ou seja, o número de indivíduos da mesma.
     *
     * @Param number_ids O número de indivíduos da população.
     */
    /* ----------------------------------------*/
    population (const int& number_ids = def::population::population_size);

    /* ----------------------------------------*/
    /**
     * @Synopsis Construtor de cópia.
     *
     * @Param pop População a ser copiada.
     */
    /* ----------------------------------------*/
    population (const population<_ty,_realTy>& pop);

    /* ----------------------------------------*/
    /**
     * @Synopsis Método destrutor.
     */
    /* ----------------------------------------*/
    ~population(void);

    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (get). 
     *
     * @return O inteiro que identifica unicamente um indivíduo.
     */
    /* ----------------------------------------*/
    int GetId (void) const {return _id;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (set). 
     *
     * @Param new_id O novo valor que identifica o indivíduo unicamente.
     */
    /* ----------------------------------------*/
    void SetId (const int& new_id) {_id = new_id;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (get).	
     *
     * @return O número de indivíduos da população.
     */
    /* ----------------------------------------*/
    int GetNumerOfIndividuals (void) const {return _myPop.size();}

    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (get). 
     *
     * @return O valor médio do fitness dos indivíduos da população.
     */
    /* ----------------------------------------*/
    _realTy GetAveragePerformance (void) const {return _average;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (set).
     *
     * @Param new_average O novo valor médio do fitness dos indivíduos da população.
     */
    /* ----------------------------------------*/
    void SetAveragePerformance (const _realTy& new_average) {_average = new_average;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (get). 
     *
     * @return A soma dos valores de fitness de todos os indivíduos da população.
     */
    /* ----------------------------------------*/
    _realTy GetSum (void) {return _sum;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (set).
     *
     * @Param new_sum O novovalor da soma a ser setado.
     */
    /* ----------------------------------------*/
    void SetSum (const _realTy& new_sum) {_sum = new_sum;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (get). 
     *
     * @return O valor da variância do fitness dos indivíduos da população.
     */
    /* ----------------------------------------*/
    _realTy GetDeviation (void) const {return _deviation;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (set). 
     *
     * @Param new_deviation O novo valor da variância do fitness dos indivíduos da população.
     */
    /* ----------------------------------------*/
    void SetDeviation (const _realTy& new_deviation) { _deviation = new_deviation; }

    /* ----------------------------------------*/
    /**
     * @Synopsis Envia os dados da população (fitness, variancia, etc) para a stream.
     *
     * @Param os A stream para onde são enviado os dados.
     */
    /* ----------------------------------------*/
    void SendGenerationDataToStream (std::ostream& os) const {os<<_bestId->GetID() << " " <<_bestId->GetValue() << " " <<_worseId->GetID() << " " << _worseId->GetValue() << " " << _average << " " << _deviation << std::endl;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Retorna um iterator para manipular os indivíduos da população.
     *
     * @return Um iterator para o começo do container que armazena os indivíduos.
     */
    /* ----------------------------------------*/
    const_it_ begin (void) const {return _myPop.begin();}

    /* ----------------------------------------*/
    /**
     * @Synopsis Retorna um iterator para manipular os indivíduos da população.
     *
     * @return Um iterator para o fim  do container que armazena os indivíduos.
     */
    /* ----------------------------------------*/
    const_it_ end (void) const {return _myPop.end();}

    /* ----------------------------------------*/
    /**
     * @Synopsis Retorna um iterator para manipular os indivíduos da população.
     *
     * @return Um iterator para o começo do container que armazena os indivíduos.
     */
    /* ----------------------------------------*/
    it_ begin (void) {return _myPop.begin();}

    /* ----------------------------------------*/
    /**
     * @Synopsis Retorna um iterator para manipular os indivíduos da população.
     *
     * @return Um iterator para o fim  do container que armazena os indivíduos.
     */
    /* ----------------------------------------*/
    it_ end (void) {return _myPop.end();}

    /* ----------------------------------------*/
    /**
     * @Synopsis Seta os novos indivíduos da população.
     *
     * @Param new_pop População que contém os indivíduos que formarão a nova população.
     */
    /* ----------------------------------------*/
    void SetNewIndividuals (const _pop& new_pop); 

    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (get). 
     *
     * @return Um ponteiro para o indivíduo com o pior fitness da população.
     */
    /* ----------------------------------------*/
    individual<_ty,_realTy>* GetWorseId (void) const {return _worseId;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (get). 
     *
     * @return Um ponteiro para o indivíduo com o melhor fitness da população.
     */
    /* ----------------------------------------*/
    individual<_ty,_realTy>* GetBestId (void) const {return _bestId;}

    /* --------------------------------------------------------------- *
     * --- Sobrecarga de operadores                                    *
     * --------------------------------------------------------------- */

    /* ----------------------------------------*/
    /**
     * @Synopsis Operador de acesso aleatório. Acesso um indivíduo em determinado índice do container de indivíduos.
     *
     * @Param indice O índice do container do indivíduo que se deseja acessar.
     *
     * @return Um iterator para o indivíduo que se deseja acessar.
     */
    /* ----------------------------------------*/
    const_it_ operator [] (const int& indice) const {const_it_ it = _myPop.begin();if((indice>=0)&&(indice<_myPop.size())) it += indice;return it;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Operador de igualdade. Retorna se dois indivíduos têm o mesmo fitness.
     *
     * @Param pop A população que se deseja comparar o fitness.
     *
     * @return True caso os fitnesses sejam iguais, false caso contrário.
     */
    /* ----------------------------------------*/
    bool operator == (const population<_ty,_realTy>& pop) const;

    /* ----------------------------------------*/
    /**
     * @Synopsis Operador de acesso aleatório. Acesso um indivíduo em determinado índice do container de indivíduos.
     *
     * @Param indice O índice do container do indivíduo que se deseja acessar.
     *
     * @return Um ponteiro para o indivíduo que se deseja acessar.
     */
    /* ----------------------------------------*/
    individual<_ty,_realTy>* operator[] (const int& indice) {return (((indice>=0)&&(indice<_myPop.size()))?_myPop[indice]:NULL);}

    template <typename T,typename U> friend
    std::istream& operator >> (std::istream& is, population<T,U>& pop);

    template <typename T,typename U> friend
    std::ostream& operator << (std::ostream& os, const population<T,U>& pop);

    /* ----------------------------------------*/
    /**
     * @Synopsis Imprime os pares da população na saída padrão.
     */
    /* ----------------------------------------*/
    void PrintPairs (void) const;

    /* ----------------------------------------*/
    /**
     * @Synopsis Gera uma população de indivíduos aleatórios.
     */
    /* ----------------------------------------*/
    void GeneratePopulation (void);

    /* ----------------------------------------*/
    /**
     * @Synopsis Seta o valor da função objetivo para cada indivíduo da população
     */
    /* ----------------------------------------*/
    void SetIndividualsValue (void);

    /* ----------------------------------------*/
    /**
     * @Synopsis Ordena o vetor de indivíduos da população.
     */
    /* ----------------------------------------*/
    void SortPopulation (void) {population_compare<_ty,_realTy> object_compare;std::sort(_myPop.begin(), _myPop.end(),object_compare);}

    /* ----------------------------------------*/
    /**
     * @Synopsis Limpa o container de indivíduos da população.
     */
    /* ----------------------------------------*/
    void CleanPopulation (void);

    /* ----------------------------------------*/
    /**
     * @Synopsis Adiciona um indivíduo no vetor caso o vetor não esteja cheio, e retorna se foi possível adicionar o novo indivíduo
     *
     * @Param newId Novo indivíduo a ser adicionado.
     *
     * @return True se a inserção foi realzada com sucesso, false caso contrário.
     */
    /* ----------------------------------------*/
    bool AddIndividualToPopulation (individual<_ty,_realTy>* newId);

    /* ----------------------------------------*/
    /**
     * @Synopsis Remove o indivíduo da posição pos da população
     *
     * @Param pos A posição no container de indivíduos do indivíduo a ser removido.
     *
     * @return True se a remoção foi realizada com sucesso, false caso contrário.
     */
    /* ----------------------------------------*/
    bool RemoveIndividualAt (const int& pos);

    /* ----------------------------------------*/
    /**
     * @Synopsis Seta um individuo como nulo (usado antes de remover para manter o individuo antigo)
     *
     * @Param pos A posição no container de indivíduos do indivíduo a ser removido.
     */
    /* ----------------------------------------*/
    void SetNullIndividualAt (const int& pos);

    /* ----------------------------------------*/
    /**
     * @Synopsis Atualiza os dados da nova geração da população
     */
    /* ----------------------------------------*/
    void UpdateData (void);

  private:

    /* ----------------------------------------*/
    /**
     * @Synopsis Atualiza a variância da nova geração
     */
    /* ----------------------------------------*/
    void UpdateDeviation (void);

    int _id;//Inteiro que identifica a população
    _pop _myPop;//Populacao de indivivíduos
    individual<_ty,_realTy>* _bestId;
    individual<_ty,_realTy>* _worseId;//Respectivily best and worse individuals of the consteiner _myPop
    _realTy _average;//Average value of the objective function of the population
    _realTy _deviation;//Variãncia do valor da função objetivo na população
    _realTy _sum;//Soma do valor da função objetivo para todos os indivíduos

};

template<typename _ty,typename _realTy>
void population<_ty,_realTy>::PrintPairs (void) const
{
  for (const_it_ it = _myPop.begin(); it != _myPop.end(); ++it)
  {
    std::cout << (*it)->GetID() << " casou com ";
  if ((*it)->GetPair()==NULL)
    std::cout << "ninguem" << std::endl;
  else
    std::cout << (*it)->GetPair()->GetID() << std::endl;
  }
}

/* --------------------------------------------------------------- *
 * --- Construtores/Destrutores                                    *
 * --------------------------------------------------------------- */

template <typename _ty,typename _realTy>
population<_ty,_realTy>::population (const int& number_ids)
{
  int number_ = number_ids;
  int count = 0;
  while(number_--)
  {
    _myPop.push_back(new individual<_ty,_realTy>(count++));
  }
  _bestId = _worseId = NULL;
  _average = _deviation = 0;
  _id = 0;
  _sum = 0;
}

template <typename _ty,typename _realTy>
population<_ty,_realTy>::population (const population<_ty,_realTy>& pop)
{
  //begin debug
  std::cout << "Entrou no contrutor de copia da população" << std::endl;
 //end debug 
  for(const_it_ it = pop.begin(); it != pop.end(); ++it)
  {
    this->_myPop.push_back(new individual<_ty,_realTy>());
    (*this->_myPop.back()) = (*(*it));
    if (*it == pop.GetWorseId())
      this->_worseId = this->operator[](_myPop.size()-1);
    if (*it == pop.GetBestId())
      this->_bestId = this->operator[](_myPop.size()-1);
  }
  this->_average = pop.GetAveragePerformance();
  this->_deviation = pop.GetDeviation();
  this->_id = pop.GetId();
  this->_sum = pop.GetSum();
}

template <typename _ty,typename _realTy>
population<_ty,_realTy>::~population (void)
{
  
  //begin debug
  std::cout << "Entrou no destrutor da população." << std::endl;
 //end debug 
  for (it_ it = _myPop.begin(); it != _myPop.end(); ++it)
    delete *it;
}

template <typename _ty,typename _realTy>
void population<_ty,_realTy>::UpdateData (void)
{
  _average=0;
  _bestId = _worseId = NULL;
  for (it_ it = _myPop.begin(); it != _myPop.end(); ++it)
  {
    _average += (*it)->GetValue();

    if (_bestId != NULL )
    {
      if ((*it)->GetValue() < _bestId->GetValue())
	_bestId = *it;
    }
    else
      _bestId = *it;

    if (_worseId != NULL)
    {
      if ( (*it)->GetValue() > _worseId->GetValue())
	_worseId = *it;
    }
    else
      _worseId = *it;


  }
  _sum = _average;
  _average /= _myPop.size();
  UpdateDeviation();
}

template <typename _ty,typename _realTy>
void population<_ty,_realTy>::UpdateDeviation(void)
{
  _deviation = 0;
  for (it_ it = _myPop.begin(); it != _myPop.end(); ++it)
    _deviation += pow ((*it)->GetValue()-_average,2);

  _deviation /= (_myPop.size()-1);
  
  _deviation = pow (_deviation,0.5);
}

template <typename _ty,typename _realTy>
void population<_ty,_realTy>::GeneratePopulation (void)
{
  for (it_ it = _myPop.begin(); it != _myPop.end(); ++it)
    (*it)->GeneratePosition();
}

template<typename _ty,typename _realTy>
void population<_ty,_realTy>::SetIndividualsValue (void)
{
  for (it_ it = _myPop.begin(); it != _myPop.end(); ++it)
  {
    std::vector<_realTy> position = (*it)->GetRealPosition();
    (*it)->SetValue(GetObjectiveFunction(position));
  }
  UpdateData();
}

template <typename _ty,typename _realTy>
void population<_ty,_realTy>::SetNewIndividuals (const typename population<_ty,_realTy>::_pop& new_pop)
{
  CleanPopulation();
  int count = 0;
  for (const_it_ it = new_pop.begin(); it != new_pop.end(); ++it)
  {
    _myPop.push_back(*it);
  }
  
}

template <typename _ty,typename _realTy>
void population<_ty,_realTy>::CleanPopulation (void)
{
  for (it_ it = _myPop.begin(); it != _myPop.end(); ++it)
    delete *it;
  _myPop.clear();
}

template <typename _ty,typename _realTy>
bool population<_ty,_realTy>::AddIndividualToPopulation (individual<_ty,_realTy>* newId)
{
  if(_myPop.size() >= def::population::population_size)
    return false;
  else
  {
    _myPop.push_back(newId);
    return true;
  }
}

template <typename _ty,typename _realTy>
bool population<_ty,_realTy>::RemoveIndividualAt (const int& pos)
{
  if (pos<0 || pos >= _myPop.size())
    return false;
  else
  {
    it_ toRemove = _myPop.begin()+pos;
    _myPop.erase(toRemove);
    return true;
  }
}

///Sobrecarga de operadores

template <typename T,typename U> 
std::istream& operator >> (std::istream& is, population<T,U>& pop)
{
  is >> pop._id;
  is >> pop._average;
  is >> pop._deviation;
  is >> pop._sum;
  int population_size;
  is >> population_size;
  pop._myPop.clear();//limpa o conteiner para a inserção dos novos indivíduos
  for (;population_size > 0; --population_size)
  {
    individual<T,U>* new_id = new individual<T,U>();
    is >> *new_id;
    pop._myPop.push_back(new_id); 
  }
  return is;
}

template <typename T,typename U> 
std::ostream& operator << (std::ostream& os, const population<T,U>& pop)
{
  os << pop._id << " " << pop._average << " " << pop._deviation << " " << pop._sum << " " << pop._myPop.size() << " ";
  for (typename population<T,U>::const_it_ it = pop.begin(); it != pop.end(); ++it)
    os << *(*it);
   return os;

}

template <typename _ty,typename _realTy>
bool population<_ty,_realTy>::operator == (const population<_ty,_realTy>& pop) const
{
  if (pop.GetNumerOfIndividuals() != this->GetNumerOfIndividuals())
    return false;

  bool ret = true;
  for (int indice = 0; indice != _myPop.size(); ++indice) 
  {
    ret = ( *(*this)[indice] == *pop[indice] );
    if (!ret)
      return false;
  }
  return true;
}

template <typename _ty,typename _realTy>
void population<_ty,_realTy>::SetNullIndividualAt (const int& pos)
{
  if (pos < 0 || pos > _myPop.size()-1)
    return;

  it_ it = _myPop.begin();
  it += pos;
  *it = NULL;
}

#endif
