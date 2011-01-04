#ifndef POPULATION_THREAD_H
#define POPULATION_THREAD_H

#include "definitions.h"
#include "population.h"

template <typename _ty = GAtype,typename _realTy = GAreal_type> class population_thread; 

template <typename _ty,typename _realTy> 
class population_thread : public population<_ty,_realTy>
{
  public:

    //seta os valores dos indivíduso no intervalo entre os iterators first e second do vetor de indivíduos
    void SetIndividualsValue (typename population<_ty,_realTy>::it_ first, typename population<_ty,_realTy>::it_ second);
};

template<typename _ty,typename _realTy>
void population_thread<_ty,_realTy>::SetIndividualsValue(typename population<_ty,_realTy>::it_ first, typename population<_ty,_realTy>::it_ second)
{
  for (typename population<_ty,_realTy>::it_ pop_it = first; pop_it < second; ++pop_it)
  {
    std::vector<_realTy> position = (*pop_it)->GetRealPosition();
    (*pop_it)->SetValue(GetObjectiveFunction(position));
  }
}

#endif //POPULATION_THREAD_H  
