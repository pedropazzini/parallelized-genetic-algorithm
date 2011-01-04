/**
 * @file individual.h
 * @Synopsis Arquivo que define a calsse indivíduo, que encapsula os conceitos de um indivíduo do algoritmo genético.
 * @author Pedro Pazzini
 * @version 0.0.1
 * @date 2010-10-29
 */
#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H


#include <vector>
#include <iostream>

#include "coordinate.h"
#include "definitions.h"//definições básicas/default

/* ----------------------------------------*/
/**
 * @Synopsis O tipo default é definido no arquivo definitions.h
 *
 * @tparam _ty
 * @tparam _realTy
 */
/* ----------------------------------------*/
template <typename _ty = GAtype,typename _realTy = GAreal_type> class individual;

/* ----------------------------------------*/
/**
 * @Synopsis Classe que possui os atributos e operações que podem ser realizados com um indivíduo do algoritmo genético. O indivíduo possui como atributo principal um vetor de coordenadas que contém uma coordenada para cada grau de liberdade do problema a ser otimizado.
 *
 * @tparam _ty
 * @tparam _realTy
 */
/* ----------------------------------------*/
template <typename _ty,typename _realTy>
class individual
{
  public:
    
    /* --------------------------------------------------------------- *
     * --- Algumas definições básicas da classe                        *
     * --------------------------------------------------------------- */

    /* ----------------------------------------*/
    /**
     * @Synopsis Tipo de dados do vetor que armazena as coordenadas que compõem o indivíduo.
     */
    /* ----------------------------------------*/
    typedef typename std::vector<coordinate<_ty,_realTy>*> posTy_;

    /* ----------------------------------------*/
    /**
     * @Synopsis Tipo de dado do iterator para percorrer o vetor de coordenadas.
     */
    /* ----------------------------------------*/
    typedef typename std::vector<coordinate<_ty,_realTy>*>::iterator it_;

    /* ----------------------------------------*/
    /**
     * @Synopsis Tipo de dado de iterator constante para percorrer o vetor de coordenadas.
     */
    /* ----------------------------------------*/
    typedef typename std::vector<coordinate<_ty,_realTy>*>::const_iterator const_it_;
    
    /* --------------------------------------------------------------- *
     * --- Construtores/Destrutores                                    *
     * --------------------------------------------------------------- */

    /* ----------------------------------------*/
    /**
     * @Synopsis Método construtor.
     *
     * @Param id Identificador que identifica o indivíduo.
     * @Param dimension Número de dimensões da função a ser otimizada (graus de liberdade do problema).
     * @Param size Tamanho da representação em binário do problema.
     */
    /* ----------------------------------------*/
    individual (const int& id,
		const int& dimension = def::individual::dimension,
		const int& size = def::individual::size);

    /* ----------------------------------------*/
    /**
     * @Synopsis Método construtor de cópia.
     *
     * @Param id O indivíduo a ser copiado.
     */
    /* ----------------------------------------*/
    individual (const individual<_ty,_realTy>& id);
		
    /* ----------------------------------------*/
    /**
     * @Synopsis Método destrutor.
     */
    /* ----------------------------------------*/
    ~individual (void);
    
    /* --------------------------------------------------------------- *
     * --- Métodos de interface (get/set)                              *
     * --------------------------------------------------------------- */

    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (get).
     *
     * @return O valor do fitness do indivíduo.
     */
    /* ----------------------------------------*/
    const _realTy& GetValue (void) const {return _val;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (set). 
     *
     * @Param new_val É o novo valor do fitness do indivíduo.
     */
    /* ----------------------------------------*/
    void SetValue (const _realTy& new_val) {_val = new_val;}
    
    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (get). 
     *
     * @return Um ponteiro para o indivíduo que é par do indivíduo. Utilizado durante o cruzamento dos indivíduos.
     */
    /* ----------------------------------------*/
    const individual<_ty,_realTy>* GetPair (void) const {return _pair;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (get). 
     *
     * @return Um ponteiro para o indivíduo que é par do indivíduo. Utilizado durante o cruzamento dos indivíduos.
     */
    /* ----------------------------------------*/
    individual<_ty,_realTy>* GetPair (void) {return _pair;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (set). 
     *
     * @Param new_pair Ponteiro para o par do indivíduo a ser setado.
     */
    /* ----------------------------------------*/
    void SetPair (individual<_ty,_realTy>* new_pair) {_pair = new_pair;}
    
    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (get). 
     *
     * @return Retorna o tamanho do indivíduo.
     */
    /* ----------------------------------------*/
    const int& GetSize (void) const {return _size;}
    
    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (get). 
     *
     * @return Número de graus de liberdade, ou dimensões do indivíduo.
     */
    /* ----------------------------------------*/
    const int& GetDimension (void) const {return _dimension;}
    
    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (get). 
     *
     * @return O número que identifica unicamente o indivíduo dentro de uma população.
     */
    /* ----------------------------------------*/
    const int& GetID (void) const {return _identifier;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Método de interface (set). 
     *
     * @Param new_id Seta o novo valor do identificador.
     */
    /* ----------------------------------------*/
    void SetID (const int& new_id) {_identifier = new_id;}

    /* ----------------------------------------*/
    /**
     * @Synopsis Retorna um iterator constante para o container que armazena as coordenadas do indivíduo.
     *
     * @return Um iterator constante para o início do container.
     */
    /* ----------------------------------------*/
    const_it_ begin (void) const {return _position.begin();}

    /* ----------------------------------------*/
    /**
     * @Synopsis Retorna um iterator constante para o container que armazena as coordenadas do indivíduo.
     *
     * @return Um iterator constante para o fim do container.
     */
    /* ----------------------------------------*/
    const_it_ end (void) const {return _position.end();}

    /* ----------------------------------------*/
    /**
     * @Synopsis Retorna um iterator para o container que armazena as coordenadas do indivíduo.
     *
     * @return Um iterator para o início do container.
     */
    /* ----------------------------------------*/
    it_ begin (void) {return _position.begin();}

    /* ----------------------------------------*/
    /**
     * @Synopsis Retorna um iterator para o container que armazena as coordenadas do indivíduo.
     *
     * @return Um iterator para o fim do container.
     */
    /* ----------------------------------------*/
    it_ end (void) {return _position.end();}
    
    /* ----------------------------------------*/
    /**
     * @Synopsis Método que adiciona uma coordenada ao vetor de coordenadas.
     *
     * @Param new_coord Nova coordenada a ser adicionada.
     *
     * @return True caso a inserção tenha sido realizada com sucesso, False caso contrário.
     */
    /* ----------------------------------------*/
    bool AddCoordinate (const coordinate<_ty,_realTy>& new_coord);

    /* ----------------------------------------*/
    /**
     * @Synopsis Método que retorna a posição, em representação real, do indivíduo.
     *
     * @return Um vetor contendo o valor real, em representação real, de cada coordenada do indivíduo.
     */
    /* ----------------------------------------*/
    std::vector<_realTy> GetRealPosition (void) const;
    
    /* ----------------------------------------*/
    /**
     * @Synopsis Gera uma posição aleatória para o indivíduo id, para as coordenada apontadas por begin e end
     */
    /* ----------------------------------------*/
    void GeneratePosition (void);

    /* ----------------------------------------*/
    /**
     * @Synopsis Imprime a posiçao, em valores reais, na saída padrão.
     */
    /* ----------------------------------------*/
    void PrintRealPosition (void) const
    {
      std::vector<_realTy> real = GetRealPosition();
      for(typename std::vector<_realTy>::iterator it = real.begin(); it != real.end(); ++it)
       	std::cout << *it << " " << std::endl;
    }
    
    /* --------------------------------------------------------------- *
     * --- Sobrecarga de operadores                                    *
     * --------------------------------------------------------------- */

    /* ----------------------------------------*/
    /**
     * @Synopsis Operador de acesso. Retorna a coordenada no índice passado como parâmetro.
     *
     * @Param pos A posição da coordenada.
     *
     * @return A coordenada na posição pos.
     */
    /* ----------------------------------------*/
    coordinate<_ty,_realTy>& operator [] (const int& pos) {if((pos >= 0) && (pos<_dimension)) return *_position[pos];}
    
    /* ----------------------------------------*/
    /**
     * @Synopsis Operador de acesso. Retorna a coordenada no índice passado como parâmetro.
     *
     * @Param pos A posição da coordenada.
     *
     * @return A coordenada na posição pos.
     */
    /* ----------------------------------------*/
    const coordinate<_ty,_realTy>& operator [] (const int& pos) const {if((pos >= 0) && (pos<_dimension)) return *_position[pos];}
    
    /* ----------------------------------------*/
    /**
     * @Synopsis Operador de igualdade. Retorna se o valor do fitness dos indivíduos são iguais.
     *
     * @Param id O indivíduo a ser comparado.
     *
     * @return True se os indivíduos são igauis, false caso contrário.
     */
    /* ----------------------------------------*/
    bool operator == (const individual<_ty,_realTy>& id) const {return _val == id.GetValue();}
    
    /* ----------------------------------------*/
    /**
     * @Synopsis Operador de desigualdade. retorna se o valor do fitness dos indivíduos são diferentes.
     *
     * @Param id O indivíduo a ser comparado.
     *
     * @return True caso os indivíduos são diferentes, false caso contrário.
     */
    /* ----------------------------------------*/
    bool operator != (const individual<_ty,_realTy>& id) const {return _val != id.GetValue();}
    
    /* ----------------------------------------*/
    /**
     * @Synopsis Operador de comparação. Retorna se o fitness de um indivíduo é menor que o do outro.
     *
     * @Param id O indivíduo a ser comparado.
     *
     * @return True se o fitness de id for menor, false caso contrário.
     */
    /* ----------------------------------------*/
    bool operator < (const individual<_ty,_realTy>& id) const {return _val < id.GetValue();}
    
    /* ----------------------------------------*/
    /**
     * @Synopsis Operador de comparação. Retorna se o fitness de um indivíduo é maior que o do outro.
     *
     * @Param id O indivíduo a ser comparado.
     *
     * @return True se o fitness de id for maior, false caso contrário.
     */
    /* ----------------------------------------*/
    bool operator > (const individual<_ty,_realTy>& id) const {return _val > id.GetValue();}
    
    /* ----------------------------------------*/
    /**
     * @Synopsis Operador de comparação. Retorna se o fitness de um indivíduo é maior ou igual que o do outro.
     *
     * @Param id O indivíduo a ser comparado.
     *
     * @return True se o fitness de id for maior ou igual, false caso contrário.
     */
    /* ----------------------------------------*/
    bool operator >= (const individual<_ty,_realTy>& id) const {return _val >= id.GetValue();}
    
    /* ----------------------------------------*/
    /**
     * @Synopsis Operador de comparação. Retorna se o fitness de um indivíduo é menor ou igual que o do outro.
     *
     * @Param id O indivíduo a ser comparado.
     *
     * @return True se o fitness de id for menor ou igual, false caso contrário
     */
    /* ----------------------------------------*/
    bool operator <= (const individual<_ty,_realTy>& id) const {return _val <= id.GetValue();}
    
    /* ----------------------------------------*/
    /**
     * @Synopsis Sobrecarga do operador de fluxo de entrada.
     *
     * @tparam T
     * @tparam U
     * @Param is Stream do fluxo de entrada.
     * @Param id Indivíduo a ser jogado no fluxo de entrada.
     *
     * @return A mesma stream passada como parâmetro.
     */
    /* ----------------------------------------*/
    template <typename T,typename U> friend
    std::istream& operator >> (std::istream& is, individual<T,U>& id);
    
    /* ----------------------------------------*/
    /**
     * @Synopsis Sobrecarga do operador de fluxo de saída.
     *
     * @tparam T
     * @tparam U
     * @Param is Stream do fluxo de saída.
     * @Param id Indivíduo a ser jogado no fluxo de saída.
     *
     * @return A mesma stream passada como parâmetro.
     */
    /* ----------------------------------------*/
    template <typename T,typename U> friend
    std::ostream& operator << (std::ostream& os, const individual<T,U>& id);

    /* --------------------------------------------------------------- *
     * --- Métodos estáticos                                           *
     * --------------------------------------------------------------- */

    /* ----------------------------------------*/
    /**
     * @Synopsis Método estático que faz o par entre dois indivíduos.
     *
     * @Param id_1 Um dos indivíduos do par.
     * @Param id_2 O outro indivíduo do par.
     */
    /* ----------------------------------------*/
    static void MakePair (individual<_ty,_realTy>& id_1,individual<_ty,_realTy>& id_2) {id_1.SetPair(&id_2);id_2.SetPair(&id_1);}

    /* ----------------------------------------*/
    /**
     * @Synopsis Método estático que desfaz um par de indivíduos.
     *
     * @Param id_1 O primeiro indivíduo do par a ser desfeito.
     * @Param id_2 O segundo indivíduo do par a ser desfeito.
     */
    /* ----------------------------------------*/
    static void SeparetePair (individual<_ty,_realTy>& id_1,individual<_ty,_realTy>& id_2) {id_1.SetPair(NULL);id_2.SetPair(NULL);}

  private:
    
    int _identifier;//identificador, que identifica o indivíduo
    int _size;//tamanho em bits da codificação
    int _dimension;//número de graus de liberdade do problema
    posTy_ _position;//vetor com as coordenadas do indivíduo
    _realTy _val;//valor da função objetivo no ponto em que o indivíduo se encontra
    individual<_ty,_realTy>* _pair;//parceiro do indivíduo em um futuro cruzamento
    
    
};

/* --------------------------------------------------------------- *
 * --- Construtores/Destrutores                                    *
 * --------------------------------------------------------------- */

template <typename _ty,typename _realTy>
individual<_ty,_realTy>::individual (const int& id, const int& dimension, const int& size )
:_size(size),_dimension(dimension),_identifier(id)
{
  //incialização das variáveis
  this->_pair = NULL;
  this->_val = 0;
  
  int indice = 0;
  
  do
  {
    _position.push_back(new coordinate<_ty,_realTy>(indice));
  } while (++indice != _dimension);
  
}

template <typename _ty,typename _realTy>
individual<_ty,_realTy>::individual (const individual<_ty,_realTy>& id)
{
  this->_pair = NULL;//por opção o par não é copiado
  this->_val = id._val;
  this->_size = id._size;
  this->_dimension = id._dimension;
  this->_identifier = id._identifier;

  for (const_it_ it = id._position.begin(); it != id._position.end(); ++it)
  {
    //prestar atenção no pc_cte da classe coordinate
    this->_position.push_back(new coordinate<_ty,_realTy>((*it)->GetIndice(),(*it)->GetSize(),(*it)->GetPrecision(),(*it)->GetMax(),(*it)->GetMin()));
    (this->_position.back())->SetValue((*it)->GetValue());
  }
}


template <typename _ty,typename _realTy>
individual<_ty,_realTy>::~individual (void)
{
  for ( it_ it = _position.begin(); it != _position.end(); ++it )
    delete *it;
}

/* --------------------------------------------------------------- *
 * --- Métodos públicos                                            *
 * --------------------------------------------------------------- */

template <typename _ty,typename _realTy>
void individual<_ty,_realTy>::GeneratePosition (void)
{
  for (it_ it = _position.begin(); it != _position.end(); ++it)
      (*it)->GenerateCoordinate();
}

template<typename _ty,typename _realTy>
std::vector<_realTy> individual<_ty,_realTy>::GetRealPosition (void) const
{
  std::vector<_realTy> real_pos;
  for (const_it_ it = _position.begin(); it != _position.end(); ++it)
    real_pos.push_back(coordinate<_ty,_realTy>::Decode(*(*it)));
  return real_pos;
}

template <typename _ty,typename _realTy>
bool individual<_ty,_realTy>::AddCoordinate (const coordinate<_ty,_realTy>& new_coord)
{
  //se ainda não preencheu o vetor de posições
  if (_dimension > _position.size())
  {
    coordinate<_ty,_realTy>* coo = new coordinate<_ty,_realTy>();
    *coo = new_coord;
    _position.push_back(coo);
    return true;
  }
  else
    return false;//o vetor já se encontra preenchido

}	

/* --------------------------------------------------------------- *
 * --- Sobrecarga de operadores                                    *
 * --------------------------------------------------------------- */

template <typename T,typename U>
std::istream& operator >> (std::istream& is, individual<T,U>& id)
{
  int identifier;
  is >> id._identifier;
  identifier = id._identifier;
  U val;
  is >> id._val;
  val = id._val;
  int dimension;
  is >> id._dimension;
  dimension = id._dimension;
  
  int i = id._dimension;
  while (i--)
    is >> id[i];
  return is;

}

template <typename T,typename U>
std::ostream& operator << (std::ostream& os, const individual<T,U>& id)
{
  os << id._identifier << " " << id._val << " " << id._dimension << " ";
  int i = id._dimension;
  while(i--)
    os << id[i] << " ";

  return os;
}


#endif //INDIVIDUAL_H
