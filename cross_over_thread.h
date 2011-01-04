#ifndef CROSS_OVER_THREAD_H
#define CROSS_OVER_THREAD_H

#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/condition.hpp>
#include <boost/bind.hpp>

#include "genetic_operator_thread.h"
#include "cross_over.h"
#include "semaphore.h"

template<typename _ty,typename _realTy> 
class cross_over_thread : public cross_over <_ty,_realTy> , public genetic_operator_thread<_ty,_realTy>
{
  public:

    /* --------------------------------------------------------------- *
     * --- Construtores/destrutores                                    *
     * --------------------------------------------------------------- */

    /* ----------------------------------------*/
    /**
     * @Synopsis Método construtor.
     *
     * @Param popPt Ponteiro para a população que irá realizar o cruzamento.
     */
    /* ----------------------------------------*/
    cross_over_thread (population<_ty,_realTy>* popPt);

    /* ----------------------------------------*/
    /**
     * @Synopsis Método destrutor.
     */
    /* ----------------------------------------*/
    ~cross_over_thread ();

    /* ----------------------------------------*/
    /**
     * @Synopsis Métod que realmente executa o cruzamento, ou seja que é executado pelas threads.
     *
     * @return Um ponteiro NULL.
     */
    /* ----------------------------------------*/
    void* ConsumeAndProduceIndividuals (void);

  private:

    /* --------------------------------------------------------------- *
     * --- Definição básica                                            *
     * --------------------------------------------------------------- */

    /* ----------------------------------------*/
    /**
     * @Synopsis Definição do padrão scoped lock
     */
    /* ----------------------------------------*/
    typedef boost::mutex::scoped_lock scoped_lock;

    /* --------------------------------------------------------------- *
     * --- Variáveis de sincronização                                  *
     * --------------------------------------------------------------- */

    //mutexes para realizar o post e wait dos semáforos
    //como saõ necessários 2 posts e dois waits, é necessária que a opereação seja atômica
    boost::mutex _mutexOnPost;
    boost::mutex _mutexOnWait;

    /* ----------------------------------------*/
    /**
     * @Synopsis Método que realiza o wait no semáforo consumidor do cross over.
     */
    /* ----------------------------------------*/
    void WaitOnCrossOver (void);

    /* ----------------------------------------*/
    /**
     * @Synopsis Método que realiza o post no semáforo produtor do operador de seleção.
     */
    /* ----------------------------------------*/
    void PostOnCrossOver (void);

    /* ----------------------------------------*/
    /**
     * @Synopsis Método estático para chamar as threads.
     *
     * @Param v Ponteiro para um cross_over_threads que se deseja executar as threads.
     *
     * @return Ponteiro NULL.
     */
    /* ----------------------------------------*/
    static void* CallConsumeAndProduce (void* v){cross_over_thread<_ty,_realTy>* co = (cross_over_thread<_ty,_realTy>*)v; co->ConsumeAndProduceIndividuals();return 0;}

    /* --------------------------------------------------------------- *
     * --- Fim das variáveis de sincronização                          *
     * --------------------------------------------------------------- */

    /* ----------------------------------------*/
    /**
     * @Synopsis Realiza o cruzamento entre os indivíduos
     *
     * @Param id1 O primeiro indivíduo do par a ser cruzado.
     * @Param id2 O segundo indivíduo do par a ser cruzado
     *
     * @return True se o cruzamento ocorreu com sucesso, false caso contrário.
     */
    /* ----------------------------------------*/
    bool CrossOver (individual<_ty,_realTy>* id1,individual<_ty,_realTy>* id2);

};

template <typename _ty,typename _realTy>
cross_over_thread<_ty,_realTy>::cross_over_thread (population<_ty,_realTy>* popPt)
:genetic_operator_thread<_ty,_realTy>(popPt) //chama o construtor da classe mãe
,cross_over<_ty,_realTy>() //chama o construtor da calsse mãe
{
  //inicia os semáforos
  this->_semConsumer = new semaphore (0,def::population::population_size);
  this->_semProducer = new semaphore (def::population::population_size,def::population::population_size);

  //cria e inicia as threads consumidoras
  for (int i = 0; i != genetic_operator_thread<_ty,_realTy>::_coreNumbers; ++i)
  { 
    boost::thread* t = new boost::thread(boost::bind(&CallConsumeAndProduce,this));
    this->_threadVec.push_back(t);
  }

}

//método executado pelas threads produtoras
template <typename _ty,typename _realTy>
void* cross_over_thread<_ty,_realTy>::ConsumeAndProduceIndividuals (void)
{
  do
  {
    //confere se o algoritmo já não acabou
    if (!this->EndOfGA())
    {

      //decrementa o semáforo
      if (!this->EndOfGA())
	WaitOnCrossOver();
      

      individual<_ty,_realTy>* id_1=NULL;
      individual<_ty,_realTy>* id_2=NULL;

      //tenta obter o mutex do ponteiro de saida
      if (true)
      {
	  scoped_lock pointerLock(this->_outMutex);
	/* --------------------------------------------------------------- *
	 * --- início da seção crítica                                     *
	 * --------------------------------------------------------------- */
	    

	//extrai os indivíduos para o cruzamento
	id_1 = *(this->_outIterator);
	this->IncrementIterator(this->_outIterator);
	id_2 = *(this->_outIterator);
	this->IncrementIterator(this->_outIterator);

      }
      /* --------------------------------------------------------------- *
       * --- fim da seção crítica                                        *
       * --------------------------------------------------------------- */

      //realiza o cruzamento dos indivíduos
      CrossOver(id_1,id_2);

      //e os deposita no vetor do operador consumidor (operador de mutação)
      this->_consumidorPt->AddIndividual(id_1);
      this->_consumidorPt->AddIndividual(id_2);


      //decrementa o semáforo
      if (!this->EndOfGA())
	PostOnCrossOver();
    }

    
  }while (!this->EndOfGA());

}

template <typename _ty,typename _realTy>
bool cross_over_thread<_ty,_realTy>::CrossOver (individual<_ty,_realTy>* id1,individual<_ty,_realTy>* id2)
{
  //confere se as dimensões são diferentes, ou seja se é possível realizar o cruzamento
  if (id1->GetSize() != id2->GetSize())
    return false;
  else
  {

    int i = 0;//variável para contar qual dimensão que se encontra a busca (usada somente no CrossOver(...)

    //percorre cada dimensão dos indivíduos
    for (typename individual<_ty,_realTy>::it_ idIt = id1->begin(); idIt != id1->end(); ++idIt)
    {
      //testa se o cruzamento irá acontecer
      if (this->_probability > genetic_operator<_ty,_realTy>::GenerateRandom())
      {
	int cuttOffpt; //ponto de corte

	//gera um ponto de corte aleatório para a dimensão do indivíduo
	do
	{
	  //gera um número aleatório dentro do intervalo
	  cuttOffpt = genetic_operator<_ty,_realTy>::GenerateRandom (_ty((*idIt)->GetSize()-1));
	}while ((cuttOffpt<0)&&(cuttOffpt>=(*idIt)->GetSize()));

	//realiza o cruzamento propriamente dito
	cross_over<_ty,_realTy>::CrossOver(*(*idIt),*(*(id2->begin()+i)),cuttOffpt);
      }
      i++;//incrementa para a próxima dimensão
    }
    return true;//as dimensões eram a mesma e foi possível realizar o cruzamento
  }
}

template <typename _ty,typename _realTy>
void cross_over_thread<_ty,_realTy>::WaitOnCrossOver (void)
{
  scoped_lock lock(_mutexOnWait);

  //decrementa o semáforo 2 vezes pois cada thread consome e produz dois indivíduos
  this->_semConsumer->wait();
  this->_semConsumer->wait();
}

template <typename _ty,typename _realTy>
void cross_over_thread<_ty,_realTy>::PostOnCrossOver (void)
{
  scoped_lock lock(_mutexOnPost);

  //incrementa o semáforo 2 vezes pois cada thread consome e produz dois indivíduos
  this->_producerPt->GetSemProducer()->post();
  this->_producerPt->GetSemProducer()->post();
}


#endif //CROSS_OVER_THREAD_H
