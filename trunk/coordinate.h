/** 
 * @file coordinate.h
 * @brief Arquivo que contêm as definições da classe template coordinate.
 * @author Pedro Pazzini
 * @version 0.0.1
 * @date 2010-01-21
 */
#ifndef COORDINATE_H
#define COORDINATE_H


#include <string>//usado em ToString()
#include <cmath>//usado em pow(..), dentro de CroosOver(..) 
#include "definitions.h"//classe de definições básicas do problema
#include <iostream>//debug

//bibliotecas para gerar os números aleatórios
#include<stdio.h>
#include<stdlib.h>
#include<time.h>


//definições dos tipos default se encontram em definitions.h
template <typename _ty = GAtype, typename _realTy = GAreal_type> class coordinate;

/* ----------------------------------------*/
/**
 * @Synopsis Classe que define a interface de um objeto do tipo coordenada. Cada indivíduo do algoritmo genético possui um vetor de objetos do tipo coordenada. Como principal atributos da classe coordenada estão: o número de bits para representar a coordenada, a precisão da coordenada, os valores máximos e mínimos da coordenada. Os métodos principais são Code e Decode, que fazem a conversão entre a representação binária e real do valor da coordenada.
 *
 * @tparam _ty
 * @tparam _realTy
 */
/* ----------------------------------------*/
template <typename _ty,typename _realTy>
class coordinate
{
  
  public:
    
    ///Construtores/Destrutores
    
    #ifndef pc_cte
    
    /* ----------------------------------------*/
    /**
     * @Synopsis Método construtor com ponto de corte variável. Os valores defaults são definidos no arquivo definitions.h.
     *
     * @Param PC É o ponto de corte da coordenada, com ponto de corte variável.
     * @Param indice É o índice da coordenada. As coordenadas de funções com mais de um grau de liberdade possuem mais de um índice.
     * @Param size É o número de bist necessários para representar o valor real da coordenada em binário.
     * @Param precision É a precisão da representação em casas decimais. Ex: precision = 3, a precisão é de 0.001.
     * @Param max É o valor máximo da coordenada, em valor real.
     * @Param min É o valor mínimo da coordenada, em valor real.
     */
    /* ----------------------------------------*/
    coordinate (const int& PC = def_pc,const int& indice = def::coord::indice,const int& size=def::coord::size,
	const int& precision = def::coord::precision,const _realTy& max=def::coord::max, const _realTy& min=def::coord::min) 
        :_pc(PC),_indice(indice),_size(size),_precision(precision),_max(max),_min(min){GenerateSize();SetCodedBound();}
    #else

    /* ----------------------------------------*/
    /**
     * @Synopsis Método construtor para coordenada com ponto de corte constante e definido no arquivo definitions.h
     *
     * @Param indice É o índice da coordenada. As coordenadas de funções com mais de um grau de liberdade possuem mais de um índice.
     * @Param size É o número de bist necessários para representar o valor real da coordenada em binário.
     * @Param precision É a precisão da representação em casas decimais. Ex: precision = 3, a precisão é de 0.001.
     * @Param max É o valor máximo da coordenada, em valor real.
     * @Param min É o valor mínimo da coordenada, em valor real.
     */
    /* ----------------------------------------*/
    coordinate (const int& indice = def::coord::indice,const int& size=def::coord::size, const int& precision = def::coord::precision,
      	const _realTy& max=def::coord::max, const _realTy& min=def::coord::min )
        :_indice(indice),_size(size),_precision(precision),_pc(def_pc),_max(max),_min(min)
    	{GenerateSize();SetCodedBound();}
    #endif

    /* ----------------------------------------*/
    /**
     * @Synopsis Construtor de cópia.
     *
     * @Param É a coordenada a ser copiada.
     */
    /* ----------------------------------------*/
    coordinate(const coordinate<_ty,_realTy>& coo):_indice(0){_value = coo._value;_precision=coo._precision;_max=coo._max;
      _min=coo._min;_size = coo._size;this->GenerateSize();/*ajusta o tamanho da codificação*/this->SetCodedBound();}
    
    
      /* --------------------------------------------------------------- *
       * --- Métodos de interface get/set                                *
       * --------------------------------------------------------------- */

      /* ----------------------------------------*/
      /**
       * @Synopsis Método de interface (get).
       *
       * @return O número de bits necessários para representar o valor real da coordenada em binário.
       */
      /* ----------------------------------------*/
    const int& GetSize (void) const {return _size;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (set)
     *
     * @Param new_size É o novo valor do número de bits necessárioos para representar o valor real da coordenada em binário.
     */
    /* ----------------------------------------*/
    void SetSize (const int& new_size) {_size = new_size;}
    
    /* ----------------------------------------*/
    /**
     * @Synopsis Métod de interface (get).
     *
     * @return O índice da coordeanada.
     */
    /* ----------------------------------------*/
    const int& GetIndice (void) const {return _indice;}
    
    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (get)
     *
     * @return Referência constante para o valor, na representação em binário, da variável.
     */
    /* ----------------------------------------*/
    const _ty& GetValue (void) const {return _value;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (get). 
     *
     * @return Referência para o valor, na representação em binário, da variável.
     */
    /* ----------------------------------------*/
    _ty& GetValue (void) {return _value;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (set). Antes de ser setado, é checado se o valor se encontra dentro do intervalo definido pelos valores máximo e mínimo da variável.
     *
     * @Param new_value É o novo valor, em binário do valor da coordenada.
     */
    /* ----------------------------------------*/
    void SetValue (const _ty& new_value) {if(!IsOutOfBound(new_value)) _value = new_value;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (set). Antes de ser setado, é checado se o valor se encontra dentro do intervalo definido pelos valores máximo e mínimo da variável. Após o novo valor ter sido validado ele é codificado para binário e o novo valor é setado.
     *
     * @Param new_value É o novo valor, em real do valor da coordenada.
     */
    /* ----------------------------------------*/
    void SetValue (const _realTy& new_value) {if(!IsOutOfBound(new_value))
      					      SetValue(coordinate<_ty,_realTy>::Code(*this,new_value));}

    /* ----------------------------------------*/
    /**
     * @Synopsis Seta o valor da coordenada como o valor mínimo definido para a mesma.
     */
    /* ----------------------------------------*/
    void SetValueMinValue (void) {_value = _minCoded;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Seta o valor da coordenada como o valor máximo definido para a mesma.
     */
    /* ----------------------------------------*/
    void SetValueMaxValue (void) {_value = _maxCoded;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (get).
     *
     * @return A precisão da coordenada
     */
    /* ----------------------------------------*/
    const int& GetPrecision (void) const {return _precision;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (set). 
     *
     * @Param new_precision É o novo valor da precisão da coordenada.
     */
    /* ----------------------------------------*/
    void SetPrecision (const int& new_precision) {_precision=new_precision;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (get). 
     *
     * @return O valor mínimao, em valor real, da coordenada.
     */
    /* ----------------------------------------*/
    const _realTy& GetMin (void) const {return _min;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (get).
     *
     * @return O valor mínimo, em valor real, da coordenada.
     */
    /* ----------------------------------------*/
    const _realTy& GetMax (void) const {return _max;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (get). 
     *
     * @return O valor máximo, em binário, da coordenada.
     */
    /* ----------------------------------------*/
    const _ty& GetMaxCoded (void) const {return _maxCoded;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (get). 
     *
     * @return O valor mínimo, em binário, da coordenada.
     */
    /* ----------------------------------------*/
    const _ty& GetMinCoded (void) const {return _minCoded;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (set). 
     *
     * @Param new_min É o valor mínimo, em valor real, da coordenada.
     */
    /* ----------------------------------------*/
    void SetMin (const _realTy& new_min) {_min=new_min;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (set). 
     *
     * @Param new_max É o valor máximo, em valor real, da coordenada.
     */
    /* ----------------------------------------*/
    void SetMax (const _realTy& new_max) {_max=new_max;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (set). 
     *
     * @Param new_minCoded É o valor mínimo, em binário, da coordenada.
     */
    /* ----------------------------------------*/
    void SetMinCoded (const _ty& new_minCoded) {_minCoded = new_minCoded;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (set). 
     *
     * @Param new_maxCoded É o valor máximo, em binário, da coordenada.
     */
    /* ----------------------------------------*/
    void SetMaxCoded (const _ty& new_maxCoded) {_maxCoded = new_maxCoded;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Método que verifica se os valores mínimos e máximos da coordenadas são consistentes, ou seja, se o valor mínimo é menor que o valor máximo.
     *
     * @return True caso o vamor mínimo da coordenada seja menor que o valor máximo, False caso contrário.
     */
    /* ----------------------------------------*/
    bool IsConsistent (void) {return _max>_min;}//retorna se as coordenadas de máximo e mínimo são coerentes

    /* ----------------------------------------*/
    /**
     * @Synopsis Método que calcula o tamanho do intervalo da coordenada.
     *
     * @return O tamanho do intervalo da coordenada.
     */
    /* ----------------------------------------*/
    _realTy GetInterval (void) {return _max-_min;}
    
    /* ----------------------------------------*/
    /**
     * @Synopsis Método que checa se um valor, em binário, se encontra dentro do itervalo da coordenada.
     *
     * @Param value O valor a ser checado.
     *
     * @return True caso value esteja dentro do intervalo, e False caso contrário.
     */
    /* ----------------------------------------*/
    bool IsOutOfBound (const _ty& value) const {return ((value<_minCoded) || (value>_maxCoded));}

    /* ----------------------------------------*/
    /**
     * @Synopsis Retorna se um valor passado como parâmetro se encontra fora do intervalo da coordenada.
     *
     * @Param value O Valor real a ser conferido se se encontra dentro dos limites mínimos e máximos da coordenada.
     *
     * @return true caso o valor se encontra fora do intervalo, e false caso se encontre dentro do intervalo.
     */
    /* ----------------------------------------*/
    bool IsOutOfBound (const _realTy& value) const {return IsOutOfBound(coordinate<_ty,_realTy>::Code(*this,value));}
    
    #ifdef pc_cte

    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (get). 
     *
     * @return O valor do ponto de corte.
     */
    /* ----------------------------------------*/
    const int& GetPC (void) const {return _pc;}
    #else
    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (get). 
     *
     * @return O valor do ponto de corte.
     */
    /* ----------------------------------------*/
    int& GetPC (void) const {return _pc;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (set). 
     *
     * @Param new_pc O novo valor do ponto de corte.
     */
    /* ----------------------------------------*/
    void SetPc (const int& new_pc) {_pc=new_pc;}
    #endif
    
    /* ----------------------------------------*/
    /**
     * @Synopsis Método estático que recebe um objeto do tipo coordenada como parâmetro, e retorna o valor da coordenada nas representação real do ga.
     *
     * @tparam _ty Tipo da representação em binário das coordenadas do GA.
     * @tparam _realTy Tipo da representação real cas coordeandas do GA
     * @Param coo Referência para a coordenada a ser decodifcada.
     *
     * @return O valor da representação real da coordenada.
     */
    /* ----------------------------------------*/
    static _realTy Decode (const coordinate<_ty,_realTy>& coo);

    /* ----------------------------------------*/
    /**
     * @Synopsis Método estático que recebe um objeto do tipo coordenada, e um valor na representação real do GA. Com base nos atributos da coordenada passada como parâmetro, como precisão, máximos, e mínimos, o método retorna a representação em binário do valor real passado como parâmetro.
     *
     * @tparam _ty Tipo de dado da representação em binário da coordenada do GA.
     * @tparam _realTy Tipo de dado da representação real da coordeanda do GA.
     * @Param coo Objeto do tipo coordenda que contém as informações para realizar a codificação.
     * @Param value O valor real a ser codificado
     *
     * @return O representação binária do GA de value.
     */
    /* ----------------------------------------*/
    static _ty Code (const coordinate<_ty,_realTy>& coo,const _realTy& value);

    /* ----------------------------------------*/
    /**
     * @Synopsis Método que gera um número aleatório de coordenada dentro dos valores máximos e mínimos da coordenada. O valor é codificado e depois setado no valor da coordenada.
     * 
     *
     * @tparam _ty Tipo de dado da representação em binário da coordenada do GA.
     * @tparam _realTy Tipo de dado da representação real da coordeanda do GA.
     */
    /* ----------------------------------------*/
    void GenerateCoordinate (void);
    
    /* ----------------------------------------*/
    /**
     * @Synopsis Retorna uma string com a representação em binário do valor da coordenada.
     *
     * @tparam _ty Tipo de dado da representação em binário da coordenada do GA.
     * @tparam _realTy Tipo de dado da representação real da coordeanda do GA.
     *
     * @return Uma string representado o valor binário do valor da coordenada.
     */
    /* ----------------------------------------*/
    std::string ToString (void) const;

    /* ----------------------------------------*/
    /**
     * @Synopsis Método que recebe um objeto do tipo coordenada como parâmetro, e retorna o valor da coordenada nas representação real do ga
     *
     * @tparam _ty Tipo da representação em binário das coordenadas do GA.
     * @tparam _realTy Tipo da representação real cas coordeandas do GA
     * @Param coo Referência para a coordenada a ser decodifcada.
     *
     * @return O valor da representação real da coordenada.
     */
    /* ----------------------------------------*/
    _realTy Decode (void) const {return coordinate<_ty,_realTy>::Decode(*this);}

    /* --------------------------------------------------------------- *
     * --- Sobrecarga de operadores                                    *
     * --------------------------------------------------------------- */

    /* ----------------------------------------*/
    /**
     * @Synopsis Operador de atribuição. Ao ser utlizado, os atributos da coordenada à esquerda do operador tem seus valores alterados, e se tornam idênticos aos atributos da coordenada à direita do operador.
     *
     * @tparam _ty
     * @tparam _realTy
     * @Param coo A coordenada à direia do operador de atribuição, com os atributos a serem copiados.
     *
     * @return A coordenada à esquerda do operador de atribuição, com os atributos a serem alterados.
     */
    /* ----------------------------------------*/
    coordinate<_ty,_realTy>& operator = (const coordinate<_ty,_realTy>& coo) ;
    
    /* ----------------------------------------*/
    /**
     * @Synopsis Operador de igualdade. Confere se o valor de duas coordenadas são iguais.
     *
     * @Param coo Coordenada a ser checada se o valor é igual.
     *
     * @return True caso os valores das coordenadas sejam iguais, False caso contrário.
     */
    /* ----------------------------------------*/
    bool operator == (const coordinate<_ty,_realTy>& coo) const {return this->_value == coo.GetValue();}
    
    /* ----------------------------------------*/
    /**
     * @Synopsis Operador de diferença. Confere se o valor de duas coordenadas são diferentes.
     *
     * @Param coo Coordenada a ser checada se o valor é diferente.
     *
     * @return True caso os valores das coordenadas sejam iguais, False caso contrário.
     */
    /* ----------------------------------------*/
    bool operator != (const coordinate<_ty,_realTy>& coo) const {return this->_value != coo.GetValue();}
    
    /* ----------------------------------------*/
    /**
     * @Synopsis Operador de comparação. Confere se o valor de uma coordenada é maior que o valor da outra.
     *
     * @Param coo Coordenada a ser checada se o valor é maior.
     *
     * @return True caso o valor da coordenada seja maior que o da coordenada de coo, False caso contrário.
     */
    /* ----------------------------------------*/
    bool operator > (const coordinate<_ty,_realTy>& coo) const {return this->_value > coo.GetValue();}
    
    /* ----------------------------------------*/
    /**
     * @Synopsis Operador de comparação. Confere se o valor de uma coordenada é menor que o valor da outra.
     *
     * @Param coo Coordenada a ser checada se o valor é menor.
     *
     * @return True caso o valor da coordenada seja menor que o da coordenada de coo, False caso contrário.
     */
    /* ----------------------------------------*/
    bool operator < (const coordinate<_ty,_realTy>& coo) const {return this->_value < coo.GetValue();}
    
    /* ----------------------------------------*/
    /**
     * @Synopsis Operador de comparação. Confere se o valor de uma coordenada é maior ou igual ao valor da outra.
     *
     * @Param coo Coordenada a ser checada se o valor é maior ou igual.
     *
     * @return True caso o valor da coordenada seja maior ou igual ao da coordenada de coo, False caso contrário.
     */
    /* ----------------------------------------*/
    bool operator >= (const coordinate<_ty,_realTy>& coo) const {return this->_value >= coo.GetValue();}
    
    /* ----------------------------------------*/
    /**
     * @Synopsis Operador de comparação. Confere se o valor de uma coordenada é menor ou igual ao valor da outra.
     *
     * @Param coo Coordenada a ser checada se o valor é menor ou igual.
     *
     * @return True caso o valor da coordenada seja menor ou igual ao da coordenada de coo, False caso contrário.
     */
    /* ----------------------------------------*/
    bool operator <= (const coordinate<_ty,_realTy>& coo) const {return this->_value <= coo.GetValue();}

    
    /* ----------------------------------------*/
    /**
     * @Synopsis Operador de fluxo de entrada. Recebe os atributos do objeto coordenada da stream, e os seta no objeto do tipo coordenada passado como parâmetro
     *
     * @tparam T
     * @tparam U
     * @Param os O objeto stream que carrega os atributos.
     * @Param coo O objeto coordenada que recebe os atributos da stream
     *
     * @return O mesmo objeto stream passado como parâmetro, para possibilitar o encadeamento do operador
     */
    /* ----------------------------------------*/
    template <typename T,typename U> friend
    std::ostream& operator << (std::ostream& os, const coordinate<T,U>& coo);
    
    /* ----------------------------------------*/
    /**
     * @Synopsis Operador de fluxo de saída. Imprime na stream os atributos do objeto coordenada.
     *
     * @tparam T
     * @tparam U
     * @Param is A stream que recebe os atributos
     * @Param coo A coordenada que possui os atributos que serão passados para a stream
     *
     * @return A mesma stream passada como parâmetro, para permitir o uso encadeado do operador
     */
    /* ----------------------------------------*/
    template <typename T,typename U> friend
    std::istream& operator >> (std::istream& is, coordinate<T,U>& coo);
    
  private:

    /* ----------------------------------------*/
    /**
     * @Synopsis Calcula com base nos parâmetros da coordenada (mínimo, máximo, precisão) o número de bits necessários para representar a coordeada. Após o calculo, o método atualiza este atributo no objeto coordeanda.
     *
     * @tparam _ty
     * @tparam _realTy
     */
    /* ----------------------------------------*/
    void GenerateSize (void);

    /* ----------------------------------------*/
    /**
     * @Synopsis Seta os limites codificados a partir dos não codificados.
     */
    /* ----------------------------------------*/
    void SetCodedBound (void) {SetMinCoded(coordinate<_ty,_realTy>::Code(*this,this->_min));
      			       SetMaxCoded(coordinate<_ty,_realTy>::Code(*this,this->_max));}

    
    const int _indice;//indice da coordeanda. equivalente a qual diensão ela representa
    _ty _value;//valor da coordenada
    int _precision;//precisão exigida da coordenada
    int _size;//tamanho em bits da coordenada
    _realTy _max,_min;//mínimo e máximo da coordenada
    _ty _maxCoded,_minCoded;//máximo e mínimo da coordenada codificados

    //ponto de corte
    #ifdef pc_cte
    const int _pc;//se nas definições do problema o ponto de corte for constante
    #else
    int _pc;//ponto de corte definido como variável ou aleatório
    #endif
    
};

/* --------------------------------------------------------------- *
 * --- Métodos estáticos                                           *
 * --------------------------------------------------------------- */

template <typename _ty,typename _realTy>
_realTy coordinate<_ty,_realTy>::Decode (const coordinate<_ty,_realTy>& coo )
{
	_realTy decoded;
	decoded = _realTy(coo._value)*((coo._max-coo._min)/(pow(2,coo._size)-_realTy(1)))+coo._min;
	return decoded;
}


template <typename _ty,typename _realTy>
_ty coordinate<_ty,_realTy>::Code (const coordinate<_ty,_realTy>& coo,const _realTy& value)
{
	_ty coded;
	_realTy to_convert;
	to_convert = value-coo._min;
	to_convert *= (pow(_realTy(2),coo._size)-1);
	to_convert /= (coo._max-coo._min);
	coded = int(to_convert);
	return coded;
}

/* --------------------------------------------------------------- *
 * --- Métodos públicos                                            *
 * --------------------------------------------------------------- */

template <typename _ty,typename _realTy>
void coordinate<_ty,_realTy>::GenerateCoordinate (void)
{

  //Se o valor de máximo é maior que o de mínimo
  if (this->IsConsistent())
  {
    _realTy interval = this->GetInterval();

    //transforma o intervalo para um número inteiro
    _ty max_value = _ty(interval*pow(10,this->GetPrecision()));

    _ty random = rand() % max_value;

    //volta para valor decimal e grava na coordenada
    _realTy new_value = _realTy(random)*_realTy(pow(10,-this->GetPrecision()));
    new_value += this->GetMin();
    _ty value = coordinate<_ty,_realTy>::Code(*this,new_value);
    this->SetValue(value);

  }
}

template <typename _ty,typename _realTy>
inline std::string coordinate<_ty,_realTy>::ToString (void) const
{
  std::string str;
  
  _ty mask = 1;//mascara para pegar os bits
  int shift = coordinate<_ty,_realTy>::_size-1;
  mask = mask << shift;//bit mais alto
  
  for (int i= coordinate<_ty,_realTy>::_size; i!=0; --i)
  {//pega bit a bit da coordenada
  
    if (mask & _value)//pega um bit
      str += "1";
    else
      str +="0";
    
    shift--;
    mask = 1;
    mask = mask << shift;//desloca para o próximo bit
    
  }
  
  return str;
}


/* --------------------------------------------------------------- *
 * --- Métodos privados                                            *
 * --------------------------------------------------------------- */

template<typename _ty,typename _realTy>
void coordinate<_ty,_realTy>::GenerateSize (void)
{
	_realTy temp = _realTy(_max)-_realTy(_min);
	temp *= pow (_realTy(10),_realTy(_precision));
	temp = _realTy(log10(temp)/log10(_realTy(2)));
	temp = ceil (temp);
	_size = int(temp);
}

/* --------------------------------------------------------------- *
 * --- Sobrecarga de operadores                                    *
 * --------------------------------------------------------------- */

template <typename _ty,typename _realTy>
coordinate<_ty,_realTy>& coordinate<_ty,_realTy>::operator = (const coordinate<_ty,_realTy>& coo) 
{
	if (this != &coo)
	{
		
		this->_value = coo._value;
		this->_min = coo._min;
		this->_max = coo._max;
		this->_precision = coo._precision;
		this->_size = coo._size;
		this->GenerateSize();//ajusta o tamanho da codificação
	}
	
	return *this;
}

template <typename T,typename U>
std::istream& operator >> (std::istream& is, coordinate<T,U>& coo)
{

  is >> coo._value;
  is >> coo._precision;
  is >> coo._size;
  is >> coo._max;
  is >> coo._min;

  //Ajusta o tamanho da palavra com os novos valores de máximo 
  //mínimo e precisão(caso o size fornecido nao estaja correto
  coo.GenerateSize();

  //ajusta os limites codificados
  coo.SetCodedBound();

  return is;
}

template <typename T,typename U> 
std::ostream& operator << (std::ostream& os, const coordinate<T,U>& coo)
{
  os << coo._value << " " << coo._precision << " " << coo._size << " " << coo._max << " " << coo._min;
  return os;
}

#endif //COORDINATE_H
