/**
 * @file semaphore.h
 * @Synopsis Arquivo que define a classe que encapsula os conceitos do objeto de sincronização semáforo.
 * @author Pedro Pazzini
 * @version 0.0.1
 * @date 2010-10-29
 */
#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <iostream>

/* ----------------------------------------*/
/**
 * @Synopsis Classe que encapsula os conceitos de um semáforo. Foi necessário a realização desta classe pois não existe um semáforo na boost, com apenas os métodos post() e wait() como na pthreads.
 */
/* ----------------------------------------*/
class semaphore
{
  public:

    /* ----------------------------------------*/
    /**
     * @Synopsis Definição do padrão de projeto scoped lock.
     */
    /* ----------------------------------------*/
    typedef boost::mutex::scoped_lock scoped_lock;

    /* ----------------------------------------*/
    /**
     * @Synopsis Método construtor.
     *
     * @Param initCount Valor inicial do semáforo.
     * @Param maxCount Valor máximo do semáforo.
     */
    /* ----------------------------------------*/
    semaphore (const unsigned int& initCount,const unsigned int& maxCount)
      :_myCount(initCount),_myMaxCount(maxCount){}

    /* ----------------------------------------*/
    /**
     * @Synopsis Método que incremnta o semáforo até o valor máximo.
     */
    /* ----------------------------------------*/
   void wait (void)
   {
     try
     {
       scoped_lock lock(_myMutex);
       if (_myCount > 0 )
       {
	 _myCount--;
	 if (_myCount==_myMaxCount-1)
	   _myCondition.notify_all();
       }
       else
       {
	 scoped_lock(_myMutexCondition);
	 while(_myCount <= 0 )
	   _myCondition.wait(_myMutex);
	 _myCount--;
       }
     }
     catch (std::exception& ex)
     {
       std::cout << "Erro: " << ex.what() << std::endl;
       exit(1);
     }
   }

   /* ----------------------------------------*/
   /**
    * @Synopsis Método que decrementa o semáfor, até o valor zero.
    */
   /* ----------------------------------------*/
   void post (void)
   {
     try
     {
       scoped_lock lock(_myMutex);
       if (_myCount+1 <= _myMaxCount)
       {
	 _myCount++;
	 if (_myCount==1)
	   _myCondition.notify_all();
       }
       else
       {
	 scoped_lock (_myMutexCondition);
	 while(_myCount+1 >= _myMaxCount)
	   _myCondition.wait(_myMutex);
	 _myCount++;
       }
     }
     catch (std::exception& ex)
     {
       std::cout << "Erro: " << ex.what() << std::endl;
       exit(1);
     }
   }

  private:

    boost::mutex _myMutex;
    boost::condition _myCondition;
    unsigned int _myCount;
    const unsigned int _myMaxCount;
};

#endif //SEMAPHORE_H
