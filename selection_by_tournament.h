/**
 * @file selection_by_tournament.h
 * @Synopsis Arquivo que contém as deinições para a calsse de seleção por torneio.
 * @author Pedro Pazzini
 * @version 0.0.1
 * @date 2010-10-30
 */
#ifndef SELECTION_BY_TOURNAMENT_H
#define SELECTION_BY_TOURNAMENT_H

#include "definitions.h"
#include "individual.h"
#include "genetic_operator_thread.h"
#include "semaphore.h"

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/bind.hpp>


/* ----------------------------------------*/
/**
 * @Synopsis Classe que implementa a interface de um operador genético de seleção por torneio. As threads do operador executam o método Select(). Como o operador de seleção é um gargalo na implementação atual ele espera
 *
 * @tparam _ty
 * @tparam _realTy
 */
/* ----------------------------------------*/
template <typename _ty,typename _realTy>
class selection_by_tournament : public genetic_operator_thread<_ty,_realTy>
{

  public:

    /* ----------------------------------------*/
    /**
     * @Synopsis Definição do padrão scoped lock.
     */
    /* ----------------------------------------*/
    typedef boost::mutex::scoped_lock scoped_lock;

    /* --------------------------------------------------------------- *
     * --- Construtores/Destrutores
     * --------------------------------------------------------------- */

    /* ----------------------------------------*/
    /**
     * @Synopsis Método construtor.
     *
     * @Param popPt Ponteiro para a população onde será aplicado o operador.
     */
    /* ----------------------------------------*/
    selection_by_tournament(population<_ty,_realTy>* popPt,const short int& mask);

    /* ----------------------------------------*/
    /**
     * @Synopsis Método destrutor.
     */
    /* ----------------------------------------*/
    ~selection_by_tournament();

    /* ----------------------------------------*/
    /**
     * @Synopsis Realiza o torneio efetivamente após a avaliação da funções objetivo
     */
    /* ----------------------------------------*/
    void Select (void);

    /* ----------------------------------------*/
    /**
     * @Synopsis Método que chama as threads.
     *
     * @Param v Ponteiro para o operador.
     *
     * @return Ponteiro NULL.
     */
    /* ----------------------------------------*/
    static void* CallSelect (selection_by_tournament<_ty,_realTy>* v);

    /* ----------------------------------------*/
    /**
     * @Synopsis Função que implementa a variável de condição para receber indivíduos
     */
    /* ----------------------------------------*/
    void ReadyToReceive (void);

  protected:

  private:


    /* ----------------------------------------*/
    /**
     * @Synopsis Contador de individuos selecionados, é zerado em cada geração
     */
    /* ----------------------------------------*/
    int _countId;

    /* ----------------------------------------*/
    /**
     * @Synopsis Mutex para acesso a variável acima
     */
    /* ----------------------------------------*/
    boost::mutex _mutexCountId;

    /* ----------------------------------------*/
    /**
     * @Synopsis Variável booleana que diz se as threads foram finalizadas corretamente
     */
    /* ----------------------------------------*/
    bool _threadsFinished;

    /* ----------------------------------------*/
    /**
     * @Synopsis Método que espera por uma variável de condição para o operador começar a executar
     */
    /* ----------------------------------------*/
    void WaitForInit (void);

    /* ----------------------------------------*/
    /**
     * @Synopsis Retorna o valor de count id
     *
     * @return O número de indivíduos que já foram selecionados.
     */
    /* ----------------------------------------*/
    const int& GetCountID (void);

    /* ----------------------------------------*/
    /**
     * @Synopsis Incrementa o contador de indivíduos selecionados
     */
    /* ----------------------------------------*/
    void IncrementCountID (void);

    /* ----------------------------------------*/
    /**
     * @Synopsis Tenta limpar apopulação de indivíduos e zerar o contadotor de indivíduos
     */
    /* ----------------------------------------*/
    void TryCleanPopulation (void);

    /* ----------------------------------------*/
    /**
     * @Synopsis Zera o contador de individuos selecionados após uma nova geração
     */
    /* ----------------------------------------*/
    void RestartCountID (void);

    /* ----------------------------------------*/
    /**
     * @Synopsis Diz se todos os indivíduos foram selecionados
     *
     * @return True caso todos tenham sido selecionados, false caso contrário.
     */
    /* ----------------------------------------*/
    bool AllSelected (void);

    /* ----------------------------------------*/
    /**
     * @Synopsis Variável de condição para receber indivíduos.
     */
    /* ----------------------------------------*/
    boost::condition _condRedyToReceive;

    /* ----------------------------------------*/
    /**
     * @Synopsis Mutex associado à variável de condição _condReadyToReceive.
     */
    /* ----------------------------------------*/
    boost::mutex _mutexCondReadyToReceive;

    /* ----------------------------------------*/
    /**
     * @Synopsis Limpa o vetor de indivíduos
     */
    /* ----------------------------------------*/
    void CleanPopulation (void);

    /* ----------------------------------------*/
    /**
     * @Synopsis Termina todas as threads
     */
    /* ----------------------------------------*/
    void FinishAllThreads (void);

};

template <typename _ty,typename _realTy>
selection_by_tournament<_ty,_realTy>::selection_by_tournament (population<_ty,_realTy>* popPt,const short int& mask)
:genetic_operator_thread<_ty,_realTy>(popPt,mask) //chama o construtor da classe mãe
{

  //inicia os semáforos
  this->_semConsumer = new semaphore(def::population::population_size,def::population::population_size);
  this->_semProducer = new semaphore(def::population::population_size,def::population::population_size);

  //cria as threads
  for (int i = 0; i != genetic_operator_thread<_ty,_realTy>::_coreNumbers; ++i)
  {
    boost::thread* t = new boost::thread(boost::bind(&CallSelect,this));
    this->_threadVec.push_back(t);
  }

  //alimenta o gerador de números aleatórios
  srand(time(NULL));

  _countId = 0;

  _threadsFinished = false;

}

template <typename _ty,typename _realTy>
void selection_by_tournament<_ty,_realTy>::Select (void)
{

  WaitForInit();

  do
  {

    /* --------------------------------------------------------------- *
     * --- Espera todos os indivíduos serem avaliados                  *
     * --------------------------------------------------------------- */

    if (true)
    {
      scoped_lock lock(this->_mutexCondApplyOp);
      /* --------------------------------------------------------------- *
       * --- início da seção crítica da variável de condição             *
       * --------------------------------------------------------------- */


      //cai na variável de condição se o vetor auxiliar nao se contra cheio, 
      while ((this->_popOperatorPt->GetNumerOfIndividuals() < def::population::population_size))
      {
	this->_condApplyOperator.wait(this->_mutexCondApplyOp);
	
      }

      /* --------------------------------------------------------------- *
       * --- fim da seção crítica da variável de condição                *
       * --------------------------------------------------------------- */
    }

    //confere se o algoritmo já não temrminou
    if (!this->EndOfGA())
    {
    
      //esepra pelo semáforo do consumidor
      if (!this->EndOfGA())
	this->_semConsumer->wait();


      //indivíduo a ser adicionado no vetor do operador de cruzamento
      individual<_ty,_realTy>* idPt;

      //gera os índices aleatórios para comparar dois indivíduos(com a mesma probabilidade)
      int indice_1 = rand() % this->_popOperatorPt->GetNumerOfIndividuals();
      int indice_2 = rand() % this->_popOperatorPt->GetNumerOfIndividuals();
      
      //faz o torneio propriamente dito
      individual<_ty,_realTy>* id_1 = (*this->_popOperatorPt)[indice_1];
      individual<_ty,_realTy>* id_2 = (*this->_popOperatorPt)[indice_2];
      if (id_1->GetValue() <= id_2->GetValue())
	idPt = new individual<_ty,_realTy>(*id_1);
      else
	idPt = new individual<_ty,_realTy>(*id_2);

      //seta o id do indivíduo com o contador de indivíduos selecionados
      idPt->SetID(GetCountID());

      //adiciona ao vetor do operador de cruzamento
      this->_consumidorPt->AddIndividual(idPt);

      //incrementa o cantador de indivíduos selecionados na geração atual
      IncrementCountID();

      //Verfica se todos os indivíduos foram selecionados, e limpa o vetor e caso afirmativo
      TryCleanPopulation(); 

      //incrementa o semáforo produtor do operador de mutação
      if (!this->EndOfGA())
	this->_producerPt->GetSemProducer()->post();

    }
  } while (!this->EndOfGA());

  FinishAllThreads();
}

template <typename _ty,typename _realTy>
void* selection_by_tournament<_ty,_realTy>::CallSelect (selection_by_tournament<_ty,_realTy>* v)
{
  v->Select();
  return NULL;
}

template <typename _ty,typename _realTy>
void selection_by_tournament<_ty,_realTy>::WaitForInit (void)
{
  scoped_lock lock(this->_mutexCondApplyOp);

  /* --------------------------------------------------------------- *
   * --- Ínicio da seção crítica da variável de condição             *
   * --------------------------------------------------------------- */

  while (this->_consumidorPt==NULL)
    this->_condApplyOperator.wait(this->_mutexCondApplyOp);

  /* --------------------------------------------------------------- *
   * --- fim da seção crítica da variável de condição                *
   * --------------------------------------------------------------- */

}

template <typename _ty,typename _realTy>
void selection_by_tournament<_ty,_realTy>::IncrementCountID (void)
{
  scoped_lock lock(_mutexCountId);

  /* --------------------------------------------------------------- *
   * --- início da seção crítica                                     *
   * --------------------------------------------------------------- */

  _countId++;

  /* --------------------------------------------------------------- *
   * --- fim da seção crítica                                        *
   * --------------------------------------------------------------- */

}

template <typename _ty,typename _realTy>
void selection_by_tournament<_ty,_realTy>::RestartCountID (void)
{
  scoped_lock lock(_mutexCountId);

  /* --------------------------------------------------------------- *
   * --- início da seção crítica                                     *
   * --------------------------------------------------------------- */

  _countId = 0;

  /* --------------------------------------------------------------- *
   * --- fim da seção crítica                                        *
   * --------------------------------------------------------------- */
}

template <typename _ty,typename _realTy>
bool selection_by_tournament<_ty,_realTy>::AllSelected (void)
{
  scoped_lock lock(_mutexCountId);

  /* --------------------------------------------------------------- *
   * --- início da seção crítica                                     *
   * --------------------------------------------------------------- */

  return _countId == this->_popOperatorPt->GetNumerOfIndividuals();

  /* --------------------------------------------------------------- *
   * --- fim da seção crítica                                        *
   * --------------------------------------------------------------- */
}

template <typename _ty,typename _realTy>
void selection_by_tournament<_ty,_realTy>::CleanPopulation (void)
{
  scoped_lock lock(this->_mutexCondApplyOp);

  this->_popOperatorPt->CleanPopulation();
}

template <typename _ty,typename _realTy>
void selection_by_tournament<_ty,_realTy>::ReadyToReceive (void)
{
  scoped_lock lock(this->_mutexCondApplyOp);

  //espera enquanto a população estiver cheia
  while (this->_popOperatorPt->GetNumerOfIndividuals() == def::population::population_size)
    _condRedyToReceive.wait(this->_mutexCondApplyOp);
}

template <typename _ty,typename _realTy>
void selection_by_tournament<_ty,_realTy>::TryCleanPopulation (void)
{
  scoped_lock lock(this->_mutexCountId);

  /* --------------------------------------------------------------- *
   * --- início da seção crítica                                     *
   * --------------------------------------------------------------- */

  //confere se todos os indivíduso já foram avaliados
  if (this->_countId == def::population::population_size)
  {
    this-> _countId = 0;

    //atualiza os dados da população
    this->_popOperatorPt->UpdateData();

    //envia os dados para o arquivo de saída (dependendo do construtor em genetic_operator_thread)
    if (this->_maskOutput)
      this->SendDataToOutput();

    CleanPopulation();//e limpa a população
    _condRedyToReceive.notify_all();//acorda as threads que estavam esperando para depositar no vetor do operador de seleção
  }


}

template <typename _ty,typename _realTy>
const int& selection_by_tournament<_ty,_realTy>::GetCountID (void) 
{
  scoped_lock lock(_mutexCountId);
  return _countId;
}

template <typename _ty,typename _realTy>
void selection_by_tournament<_ty,_realTy>::FinishAllThreads (void)
{
  if (!_threadsFinished)
  {//se as threads ainda não foram finalizadas

    //marca que as threads foram finalizadas
    _threadsFinished = true;


    //contador de quantas threads devem ser acordadas
    int counter = this->_coreNumbers;
    counter *= 2;//para cruzamento é o dobro
    while  (counter--)
    {

      //acorda as threads de cruzamento
      this->_consumidorPt->GetSemConsumer()->post();
    }

    counter = this->_coreNumbers-1;
    while (counter--)
    {
      //acorda as threads de mutação
      this->_producerPt->GetSemConsumer()->post();
    }

    //acorda todas as threads de seleção
    this->_condApplyOperator.notify_all();

  }

}

#endif //SELECTION_BY_TOURNAMENT_H 
