/**
 * @file ga_exception.h
 * @Synopsis Arquivo que contém as definições básicas para tratar as excessões do operador genético.
 * @author Pedro Pazzini
 * @version 0.0.1
 * @date 2010-10-30
 */
#ifndef GA_EXCEPTION_H
#define GA_EXCEPTION_H

#include <typeinfo>
#include <string>

/* ----------------------------------------*/
/**
 * @Synopsis Classe que trata as excessões do GA.
 */
/* ----------------------------------------*/
class ga_exception : public std::exception
{
  public:

    /* ----------------------------------------*/
    /**
     * @Synopsis Método construtor da excessão.
     *
     * @Param line Linha onde ocorreu a excessão.
     * @Param func Nome do método onde ocorreu a excessão.
     * @Param file Arquivo aonde ocorreu a excessão.
     */
    /* ----------------------------------------*/
     ga_exception(std::string line,std::string func, std::string file)
      :_line(line),_methodName(func),_fileName(file){}

     /* ----------------------------------------*/
     /**
      * @Synopsis Retorna a descrição detalhada da excessão.
      *
      * @return String com os detalhes da excessão.
      */
     /* ----------------------------------------*/
     std::string what (void) {return "Erro no método " + _methodName + ", do arquivo " + _fileName + " na linha " + _line;}

  private:
    
    //string com o nome do arquivo em que ocorreu o erro
    std::string _fileName;

    //nome do método em que ocorreu o erro
    std::string _methodName;

    //linha do arquivo em que ocorreu o erro
    std::string _line;
    
};

#endif //GA_EXCEPTION_H
