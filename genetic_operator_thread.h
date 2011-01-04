/**
 * @file genetic_operator_thread.h
 * @Synopsis Arquivo que contém a definição básica do operador genético paralelizado.
 * @author Pedro Pazzini
 * @version 0.0.1
 * @date 2010-10-30
 */
#ifndef GENETIC_OPERATOR_THREAD_H
#define GENETIC_OPERATOR_THREAD_H

#include "population.h"
#include "definitions.h"
#include "semaphore.h"
//#include "ga_exception.h"
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <boost/thread/condition.hpp>
#include <typeinfo>
#include <fstream>

/* ----------------------------------------*/
/**
 * @Synopsis Valores default templates, definidos no arquivo definitions.h
 *
 * @tparam _ty
 * @tparam _realTy
 */
/* ----------------------------------------*/
template <typename _ty = GAtype,typename _realTy = GAreal_type> class genetic_operator_thread;

/* ----------------------------------------*/
/**
 * @Synopsis Classe que possui a definição básica de um operador genético paralelizado.
 *
 * @tparam _ty
 * @tparam _realTy
 */
/* ----------------------------------------*/
template<typename _ty,typename _realTy> 
class genetic_operator_thread
{
  public:

    /* --------------------------------------------------------------- *
     * --- Definições básicas da classe                                *
     * --------------------------------------------------------------- */

    /* ----------------------------------------*/
    /**
     * @Synopsis Definição do padrão scoped lock.
     */
    /* ----------------------------------------*/
    typedef boost::mutex::scoped_lock scoped_lock;//padrão scoped_lock

    /* ----------------------------------------*/
    /**
     * @Synopsis Iterator para percorrer o vetor de threads.
     */
    /* ----------------------------------------*/
    typedef std::vector<boost::thread*>::iterator it_;//definição do iterator

    /* --------------------------------------------------------------- *
     * --- Destrotor e Construtor                                      *
     * --------------------------------------------------------------- */

    /* ----------------------------------------*/
    /**
     * @Synopsis Método construtor.
     *
     * @Param popPt Ponteiro para a população em que se vai aplicar o operador.
     */
    /* ----------------------------------------*/
    genetic_operator_thread(population<>* popPt, const short int& maskOutPut=0);
    
    /* ----------------------------------------*/
    /**
     * @Synopsis Método destrutor.
     */
    /* ----------------------------------------*/
    ~genetic_operator_thread();

    /* ----------------------------------------*/
    /**
     * @Synopsis Construtor de cópia.
     *
     * @Param got Operador genético a ser copiado.
     */
    /* ----------------------------------------*/
    genetic_operator_thread(const genetic_operator_thread<_ty,_realTy>& got);

    /* ----------------------------------------*/
    /**
     * @Synopsis Retorna uma referência de si mesmo
     *
     * @return Um ponteiro para si mesmo.
     */
    /* ----------------------------------------*/
    genetic_operator_thread* GetReference (void) {return this;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Seta qual operador consome os indivíduos produzidos.
     *
     * @Param new_prod_pt A nova referência para o operador produtor.
     */
    /* ----------------------------------------*/
    void SetProducer (genetic_operator_thread<_ty,_realTy>* new_prod_pt) {_producerPt = new_prod_pt;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Seta o outro operador genético que irá consumir os dados deste
     *
     * @Param cons_pt A nova referência para o operador produtor.
     */
    /* ----------------------------------------*/
    void SetConsumer (genetic_operator_thread *cons_pt) {_consumidorPt = cons_pt;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Retorna uma referência do semáforo do produtor
     *
     * @return Uma referêmcia para o objeto semáforo de produzir um indivíduo.
     */
    /* ----------------------------------------*/
    semaphore* GetSemProducer (void) {return _semProducer;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Retorna uma referência do semáforo do consumidor
     *
     * @return Uma referência para o objeto semáforo de consumir um indivíduo.
     */
    /* ----------------------------------------*/
    semaphore* GetSemConsumer (void) {return _semConsumer;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Método que dispara as threads que aplicam o operador genético
     */
    /* ----------------------------------------*/
    void ApplyGeneticOperator (void);

    /* ----------------------------------------*/
    /**
     * @Synopsis Método que adiciona um novo indivíduo ao vetor de indivíduos do operador
     *
     * @Param newId Ponteiro para o novo indivíduo a ser adicionado
     */
    /* ----------------------------------------*/
    void AddIndividual (individual<_ty,_realTy>* newId);

    /* ----------------------------------------*/
    /**
     * @Synopsis Método que adiciona um nov indivíduo independente do ponteiro de inserir
     *
     * @Param newId Ponteiro para o novo indivíduo a ser adicionado
     */
    /* ----------------------------------------*/
    void PushBackIndividual (individual<_ty,_realTy>* newId);

    /* ----------------------------------------*/
    /**
     * @Synopsis Retorna o melhor do indivíduo da população.
     *
     * @return Ponteiro para o melhor indivíduo da população.
     */
    /* ----------------------------------------*/
    individual<_ty,_realTy>* GetBestIndividual (void) {return _popOperatorPt->GetBestId();}

    /* ----------------------------------------*/
    /**
     * @Synopsis Atualiza os dados da população
     */
    /* ----------------------------------------*/
    void UpdatePopulation (void) {this->_popOperatorPt->UpdateData();}

    /* ----------------------------------------*/
    /**
     * @Synopsis Imprime a população ordenada pelo fitness
     */
    /* ----------------------------------------*/
    void PrintPopulation (void);

    /* ----------------------------------------*/
    /**
     * @Synopsis Método que retorna uma referência para a variável de condição para iniciar a aplicar o operador
     *
     * @return Referência para a variável de condição
     */
    /* ----------------------------------------*/
    boost::condition& GetCondApplyOp (void) {return _condApplyOperator;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Método que retorna uma referência do mutex da variável de condição para aplicar o operador genético.
     *
     * @return Uma referência para o mutec da variável de condição.
     */
    /* ----------------------------------------*/
    boost::mutex GetMutexCondApplyOp (void) {return _mutexCondApplyOp;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Confere se a população se eonctra cheia.
     *
     * @return True caso o número de indivíduos seja igual ao máximo, false caso contrário.
     */
    /* ----------------------------------------*/
    bool FullPopulation (void) const {return _popOperatorPt->GetNumerOfIndividuals() == def::population::population_size;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Retorna um iterator para o começo do vetor de threads
     *
     * @return Um iterator para o começo do vetor de threads.
     */
    /* ----------------------------------------*/
    it_ Begin (void) {return _threadVec.begin();}

    /* ----------------------------------------*/
    /**
     * @Synopsis Retorna um iterator para o fim do vetor de threads
     *
     * @return Um iterator para o fim do vetor de threads.
     */
    /* ----------------------------------------*/
    it_ End (void) {return _threadVec.end();}

    /* ----------------------------------------*/
    /**
     * @Synopsis Método virtual que confere se é possível que o operador receba indivíduos no seu vetor.
     */
    /* ----------------------------------------*/
    virtual void ReadyToReceive (void){}

  protected:


    /* --------------------------------------------------------------- *
     * --- Atributos protected                                         *
     * --------------------------------------------------------------- */
    
    /* ----------------------------------------*/
    /**
     * @Synopsis Ponteiro para a população do operador.
     */
    /* ----------------------------------------*/
    population<_ty,_realTy>* _popOperatorPt;

    /* ----------------------------------------*/
    /**
     * @Synopsis Ponteiro para o operador genético que consome os dados do operador genético.
     */
    /* ----------------------------------------*/
    genetic_operator_thread<_ty,_realTy>* _consumidorPt;

    /* ----------------------------------------*/
    /**
     * @Synopsis Ponteiro para o operador genético que produz os dados do operador genéticos
     */
    /* ----------------------------------------*/
    genetic_operator_thread<_ty,_realTy>* _producerPt;

    /* ----------------------------------------*/
    /**
     * @Synopsis Número de núcleos do procesador
     */
    /* ----------------------------------------*/
    int _coreNumbers;

    /* --------------------------------------------------------------- *
     * --- início dos atributos de sincronização                       *
     * --------------------------------------------------------------- */

    //iterator e mutex do produtor do vetor de populações
    typename population<_ty,_realTy>::it_ _inIterator;
    boost::mutex _inMutex;
    boost::mutex GetInMutex (void) {return _inMutex;}
    
    //iterator e mutex do consumidor do vetor de populações
    typename population<_ty,_realTy>::it_ _outIterator;
    boost::mutex _outMutex;
    boost::mutex GetOutMutex (void) {return _outMutex;}

    //semáforos do consumidor e produtor
    semaphore *_semConsumer,*_semProducer;

    //variável de condição para aplicar o operador, e o respectivo mutex associado
    boost::condition _condApplyOperator;
    boost::mutex _mutexCondApplyOp;
    


    //variável de condição para parar de aplicar o operador(condição de parada do ga), e o respectivo mutex associado
    boost::condition _condStopCondition;
    boost::mutex _mutexCondStopCond;

    //vetor de threads
    //boost::thread _threadVec[];
    std::vector<boost::thread*> _threadVec;
    
    /* --------------------------------------------------------------- *
     * --- Região de métodos/atributos estáticos                       *
     * --------------------------------------------------------------- */

    /* ----------------------------------------*/
    /**
     * @Synopsis Contador de número de gerações que já foram realizadas.
     */
    /* ----------------------------------------*/
    static int _genCount;
    
    /* ----------------------------------------*/
    /**
     * @Synopsis Mutex para acessar a variável _genCount.
     */
    /* ----------------------------------------*/
    static boost::mutex _MutexgenCount;

    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (get). 
     *
     * @return Retorna o número de geração ocorridas.
     */
    /* ----------------------------------------*/
    static int& GetGenerationCount (void); 

    /* ----------------------------------------*/
    /**
     * @Synopsis Incrementa o contador de gerações.
     */
    /* ----------------------------------------*/
    static void IncrementGenerationCount (void);

    /* ----------------------------------------*/
    /**
     * @Synopsis Retorna se o número de gerações chegou ao máximo, ou seja se o algoritmo já terminou.
     *
     * @return True caso o número de gerações já chegou ao máximo, false caso contrário.
     */
    /* ----------------------------------------*/
    static bool EndOfGA (void) {return _genCount >= def::genetic_operator::numberOfGenerations;}

    /* --------------------------------------------------------------- *
     * --- fim dos atributos de sincronização                          *
     * --------------------------------------------------------------- */

    /* ----------------------------------------*/
    /**
     * @Synopsis Incrementa um iteartor do vetor de populações, como em um vetor circular.
     *
     * @Param it O iterator a ser incrementado.
     */
    /* ----------------------------------------*/
    void IncrementIterator (typename population<_ty,_realTy>::it_& it);

    /* --------------------------------------------------------------- *
     * --- Atributos de sáida dos dados estatísticos do GA             *
     * --------------------------------------------------------------- */

    /* ----------------------------------------*/
    /**
     * @Synopsis Arquivo de saída aonde os dados recebidos serão copiados. Isso ocorrerá somente se _maskOutput !=0
     */
    /* ----------------------------------------*/
    std::ofstream _outputData;

    /* ----------------------------------------*/
    /**
     * @Synopsis Máscara que contém os dados que devem ser enviados ao arquivo de saída
     */
    /* ----------------------------------------*/
    const short int _maskOutput;

    /* ----------------------------------------*/
    /**
     * @Synopsis Método que envia os dados estatísticos da população para um arquivo de saída
     */
    /* ----------------------------------------*/
    void SendDataToOutput (void);
    

};


//inicia o contador das gerações, que é uma variável estática
template <typename _ty,typename _realTy>
int genetic_operator_thread<_ty,_realTy>::_genCount = 0;

//inicia o mutex que é uma variável estática
template <typename _ty,typename _realTy>
boost::mutex genetic_operator_thread<_ty,_realTy>::_MutexgenCount;

template <typename _ty,typename _realTy>
genetic_operator_thread<_ty,_realTy>::genetic_operator_thread (const genetic_operator_thread<_ty,_realTy>& got)
{
  this->_semProducer = got._semProducer;
  this->_semConsumer = got.semProduce;
  this->_consumidorPt = got._consumidorPt;
  this->_producerPt = got._producerPt;

  //begin debug
  std::cout << "Entrou no construtor de cópia" << std::endl;
 //end debug 

}

template <typename _ty,typename _realTy>
void genetic_operator_thread<_ty,_realTy>::ApplyGeneticOperator (void)
{

  /********************************** início da seção crítica *************************************/
  try
  {
    scoped_lock lock(_mutexCondApplyOp);
    _condApplyOperator.notify_all(); //dispara todas as threads que esperam pelo mutex
  }
  catch(std::exception ex)
  {
    std::cout << "Excessao no ga. ApplyGeneticOperator: " << ex.what() << std::endl;
  }

  /********************************** fim da seção crítica ****************************************/
}

template<typename _ty,typename _realTy> 
genetic_operator_thread<_ty,_realTy>::genetic_operator_thread (population<>* popPt,const short int& maskOutPut)
:_coreNumbers(sysconf( _SC_NPROCESSORS_ONLN ))//inicia o numero de núcleos do processador
,_maskOutput(maskOutPut)//inicia a máscara que diz quais dados serão enviados para o arquivo de saída
,_outputData("OutputGA_thread.dat")//inicia o arquivo de saída
{
  //variável para testar se a inicialização dos objetos de sincronização foi feita corretamente
  int status;

  //se não foi passada uma população, cria uma
  if (popPt == NULL)
  {
    _popOperatorPt = new population<_ty,_realTy>();
  }
  else
    _popOperatorPt = popPt;

  //gera a população aleatória
  _popOperatorPt->GeneratePopulation();

  //atualiza o fitness de todos os individuos da população
  _popOperatorPt->SetIndividualsValue();

  //seta os iterators dos consumidores e produtores bem como os mutexes de cada um
  _inIterator = _outIterator = _popOperatorPt->begin();

}

template <typename _ty,typename _realTy>
void genetic_operator_thread<_ty,_realTy>::AddIndividual (individual<_ty,_realTy>* newId)
{

  try
  {
    _producerPt->GetSemProducer()->wait();
  }
  catch (std::exception& ex)
  {
    std::cout << "Erro ao esperar por semáforo produtor. Método: AddIndividual" << ex.what() << std::endl;
  }
 
  try
  {
    scoped_lock lock(_inMutex);

    *_inIterator = newId; //depositda o indivíduo
    IncrementIterator(_inIterator);//e atualiza o iterator

  }
  catch (std::exception& ex)
  {

    std::cout << "Erro" << ex.what() << std::endl;
  }
   
  try
  {
    _semConsumer->post();
  }
  catch (std::exception& ex)
  {
    std::cout << "Erro" << ex.what() << std::endl;
  }
 
}

//deve ser usado apenas na seção crítica do iterator it
template <typename _ty,typename _realTy>
void genetic_operator_thread<_ty,_realTy>::IncrementIterator (typename population<_ty,_realTy>::it_& it)
{
  if (it+1 != _popOperatorPt->end())
    ++it;
  else
    it = _popOperatorPt->begin();
}

template<typename _ty,typename _realTy> 
genetic_operator_thread<_ty,_realTy>::~genetic_operator_thread(void)
{
  for ( population<>::it_ it = _popOperatorPt->begin(); it != _popOperatorPt->end(); ++it)
    delete *it;
}

template <typename _ty,typename _realTy>
void genetic_operator_thread<_ty,_realTy>::PushBackIndividual (individual<_ty,_realTy>* newId)
{

  try
  {
    _producerPt->GetSemProducer()->wait();
  }
  catch (std::exception& ex)
  {
    std::cout << "Erro ao esperar por semáforo produtor. Método: AddIndividual" << ex.what() << std::endl;
  }
 
  try
  {
    scoped_lock lock(_inMutex);
    _popOperatorPt->AddIndividualToPopulation(newId);
  }
  catch (std::exception& ex)
  {

    std::cout << "Erro" << ex.what() << std::endl;
  }
   
  try
  {
    _semConsumer->post();
  }
  catch (std::exception& ex)
  {
    std::cout << "Erro" << ex.what() << std::endl;
  }
}

template <typename _ty,typename _realTy>
void genetic_operator_thread<_ty,_realTy>::IncrementGenerationCount (void)
{
  scoped_lock lock(_MutexgenCount);
  _genCount++;
}

template <typename _ty,typename _realTy>
int& genetic_operator_thread<_ty,_realTy>::GetGenerationCount (void) 
{
  scoped_lock lock(_MutexgenCount);
  return _genCount;
}

template <typename _ty,typename _realTy>
void genetic_operator_thread<_ty,_realTy>::PrintPopulation (void)
{
  _popOperatorPt->SortPopulation();
  for (typename population<_ty,_realTy>::it_ it = _popOperatorPt->begin(); it != _popOperatorPt->end(); ++it)
  {
    std::cout << (*it)->GetID() << " " << (*it)->GetValue();
    for (typename individual<_ty,_realTy>::it_ it_coo = (*it)->begin(); it_coo != (*it)->end(); ++it_coo)
      std::cout << " " << (*it_coo)->GetValue() << "(" << (*it_coo)->Decode() << "),";
    std::cout << std::endl;
  }
}

template <typename _ty,typename _realTy>
void genetic_operator_thread<_ty,_realTy>::SendDataToOutput (void) 
{
  //se a mácara não for nula
  if (_maskOutput)
  {
    //geração na coluna 0
    _outputData << GetGenerationCount() << " ";

    //as próximas colunas são definidas a partir da máscara passada no construtor do operador
    if (_maskOutput && def::statistics::_maskBestIdFitness)
      _outputData << _popOperatorPt->GetBestId()->GetValue() << " ";
    if (_maskOutput && def::statistics::_maskWorseIdFitness)
      _outputData << _popOperatorPt->GetWorseId()->GetValue() << " ";
    if (_maskOutput && def::statistics::_maskAvaregeFitness)
      _outputData << _popOperatorPt->GetAveragePerformance() << " ";
    if (_maskOutput && def::statistics::_maskDeviationFitness)
      _outputData << _popOperatorPt->GetDeviation() << " ";

    //quebra a linha
    _outputData << std::endl;


  }
}

#endif //GENETIC_OPERATOR_THREAD_H
