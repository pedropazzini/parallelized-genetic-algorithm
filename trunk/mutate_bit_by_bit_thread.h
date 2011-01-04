/**
 * @file mutate_bit_by_bit_thread.h
 * @Synopsis Arquivo com as definições da classe mutate_biy_by_bit_thread.
 * @author Pedro Pazzini
 * @version 1
 * @date 2010-09-11
 */
#ifndef MUTATE_BIT_BY_BIT_THREAD_H
#define MUTATE_BIT_BY_BIT_THREAD_H 

#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/condition.hpp>
#include <boost/bind.hpp>


#include "mutate_bit_by_bit.h"
#include "genetic_operator_thread.h"
#include "definitions.h"
#include "semaphore.h"

/* ----------------------------------------*/
/**
 * @Synopsis Valores template default. Os valores default são definidos no arquivo definitions.h
 *
 * @tparam _ty
 * @tparam _realTy
 */
/* ----------------------------------------*/
template <typename _ty = GAtype,typename _realTy = GAreal_type> class mutate_bit_by_bit_thread;

/* ----------------------------------------*/
/**
 * @Synopsis Classe que define um objeto do tipo operador genético. Este objeto herda os atributos das classes genetic_operator e mutate_bit_by_bit.
 *
 * @tparam _ty
 * @tparam _realTy
 */
/* ----------------------------------------*/
template <typename _ty,typename _realTy>
class mutate_bit_by_bit_thread : public mutate_bit_by_bit<_ty,_realTy>, public genetic_operator_thread<_ty,_realTy>
{
  public:

    /* ----------------------------------------*/
    /**
     * @Synopsis Definição do padrão scoped lock.
     */
    /* ----------------------------------------*/
    typedef boost::mutex::scoped_lock scoped_lock;

    /* ----------------------------------------*/
    /**
     * @Synopsis Método construtor.
     *
     * @Param popPt Ponteiro para a populção na qual será aplicado o operador.
     */
    /* ----------------------------------------*/
    mutate_bit_by_bit_thread (population<_ty,_realTy>* popPt);

    /* ----------------------------------------*/
    /**
     * @Synopsis Método a ser executado pelas threads
     */
    /* ----------------------------------------*/
    void ConsumeAndProduce (void);

  protected:
    
    /* ----------------------------------------*/
    /**
     * @Synopsis Usado para criar as threads
     *
     * @Param v Ponteiro para o operador que chamará as threads.
     *
     * @return Ponteiro NULL.
     */
    /* ----------------------------------------*/
    static void* CallConsumeAndProduce (void* v){mutate_bit_by_bit_thread<_ty,_realTy>* mbb = (mutate_bit_by_bit_thread<_ty,_realTy>*)v; mbb->ConsumeAndProduce();return 0;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Aplica o operador de mutação em um indivíduo
     *
     * @Param id O indivíduo aonde será aplicado o operador de mutação.
     */
    /* ----------------------------------------*/
    void ApplyMutateOperatorInIndividual (individual<_ty,_realTy>* id);

  private:
};

template <typename _ty,typename _realTy>
mutate_bit_by_bit_thread<_ty,_realTy>::mutate_bit_by_bit_thread(population<_ty,_realTy>* popPt)
:genetic_operator_thread<_ty,_realTy>(popPt) //chama oo construtor da classe mãe
  ,mutate_bit_by_bit<_ty,_realTy>()
{
  //inicia os semáforos
  this->_semConsumer = new semaphore (0,def::population::population_size);
  this->_semProducer = new semaphore (0,def::population::population_size);

  //cria e inicia as threads principais (consumidoras)
  for (int i = 0; i != genetic_operator_thread<_ty,_realTy>::_coreNumbers; ++i)
  {
    boost::thread* t = new boost::thread(boost::bind(&CallConsumeAndProduce,this));
    this->_threadVec.push_back(t);
  }

}


/* ----------------------------------------*/
/**
 * @Synopsis Método a ser executado pelas threads do operador de mutação bit a bit. O operador remove um indivíduo da sua população, aplica o operador de mutação, e já deposita o mesmo no operador que consome os indivíduos do operador de mutação, que no caso, é o operador de seleção.
 *
 * @tparam _ty
 * @tparam _realTy
 */
/* ----------------------------------------*/
template <typename _ty,typename _realTy>
void mutate_bit_by_bit_thread<_ty,_realTy>::ConsumeAndProduce (void)
{
  do
  {
    if (!this->EndOfGA())
    {
      //decrementa o semáforo para consumir
      if (!this->EndOfGA())
	this->_semConsumer->wait();


      /*********************************** ínicio da seção crítica do semáforo **************************************************/
      
      if (!this->EndOfGA())
      {
	individual<_ty,_realTy>* id;

	//tenta obter o mutex
	if (true)
	{
	  scoped_lock lock(this->_outMutex);

	  /*********************************** início da seção crítica do mutex *****************************************************/
	  id = *genetic_operator_thread<_ty,_realTy>::_outIterator;
	  *genetic_operator_thread<_ty,_realTy>::_outIterator = NULL;

	  //testa se houve erro de sincronização
	  if (id==NULL && !this->EndOfGA())
	  {
	    std::cout << "Erro de sincronização no operador consumidor/produtor do operador de mutação" << std::endl;
	    exit(0);
	  }


	  //e incrementa o semáforo
	  IncrementIterator(this->_outIterator);
	}
	/*********************************** fim da seção crítica do mutex ********************************************************/

	//se o indivíduo não é nulo, aplica o operador de mutação
	ApplyMutateOperatorInIndividual(id);

	//atualiza o fitness do indivíduo
	id->SetValue(GetObjectiveFunction(id->GetRealPosition()));

	//Espera na variável de condição caso o operador de seleção não pode receber individuos
	if (!this->EndOfGA())
	  this->_consumidorPt->ReadyToReceive();

	//adiciona ao vetor de indivíduos do operador de seleção
	this->_consumidorPt->PushBackIndividual(id);

	//sinaliza a variável de condição se encheu o vetor do operador de seleção e o algoritmo ainda não terminou
	if (this->_consumidorPt->FullPopulation() && !this->EndOfGA())
	{
	  //incrementa o contador de gerações
	  this->IncrementGenerationCount();

	  //acorda uma das threads do operador de seleção que acorda todas as outras threads
	  this->_consumidorPt->GetCondApplyOp().notify_one();

	}
      }

      

      /*********************************** fim da seção crítica do semáforo *****************************************************/
      if (!this->EndOfGA())
	this->_producerPt->GetSemProducer()->post();
    }
  }while (!this->EndOfGA());

}

/* ----------------------------------------*/
/**
 * @Synopsis Método que recebe como parâmetro um ponteiro para um indivíduo e aplica o operador de mutação sobre o mesmo. Basicamente o método caminha sobre cada objeto coordenada do indivíduo, e aplica o operador sobre cada coordenada. A probabilidade de mutação de cada bit é definidia no arquivo definitions.h
 *
 * @tparam _ty Tipo de cada coordenada para a representação binária
 * @tparam _realTy Tipo das coordenadas para a representação real
 * @Param id Ponteiro para o indivíduo no qual será aplicado o operador de mutação bit a bit.
 */
/* ----------------------------------------*/
template <typename _ty,typename _realTy>
void mutate_bit_by_bit_thread<_ty,_realTy>::ApplyMutateOperatorInIndividual (individual<_ty,_realTy>* id)
{
  for (typename individual<_ty,_realTy>::it_ it = id->begin(); it != id->end(); ++it)
    WalkOnIndividualHook(*(*it));
}

#endif //MUTATE_BIT_BY_BIT_THREAD_H
