#include <fstream>
#include <cmath>
#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>

int main (int argc, char* argv[])
{
  if (argc != 4)
  {
    std::cout << "Erro na passagem por parâmetros!" << std::endl;
    std::cout << "Formato: ./a.out <nome_do_arquivo_> <numero_de_arquivos> <numero_de_linhas_em_cada_arquivo>" << std::endl;

    exit(0);

  }

  //definições de tipo
  typedef std::vector<std::ifstream*> myVec_;
  typedef std::vector<std::ifstream*>::iterator it_;

  //pega algumas das informações passadas com parâmetro
  int nlines = atoi(argv[3]);
  int nfiles = atoi(argv[2]);
  std::string fileTemplate(argv[1]);

  //constrói o vetor
  myVec_ vec;
  int aux_nfiles = nfiles;
  int count = 0;
  do
  {
    std::string str(fileTemplate);

    std::ostringstream numStr;
    numStr << count;

    str += numStr.str();
    count++;


    vec.push_back(new std::ifstream(str.c_str()));
  }while (--aux_nfiles);

  //variáveis a serem enviadas para o arquivo
  float melhorID,piorID,media,variancia;
  int geracao = 0;
  melhorID = 0;
  piorID = 0;
  media = 0;
  variancia = 0;

  aux_nfiles = nfiles;
  int aux_nlines = nlines;
  float val;
  std::ofstream out("saida_stat");
  do
  {
    for (it_ it = vec.begin(); it != vec.end(); ++it)
    {
      (**it) >> geracao;
      (**it) >> val;
      melhorID += val;
      (**it) >> val;
      piorID += val;
      (**it) >> val;
      media += val;
      (**it) >> val;
      variancia += val;
    }

    //joga para o arquivo de saída
    out << geracao << " ";
    out << melhorID/nfiles << " ";
    out << piorID/nfiles << " ";
    out << media/nfiles << " ";
    out << variancia/nfiles;
    out << std::endl;

    melhorID = 0;
    piorID = 0;
    media = 0;
    variancia = 0;

  }while (--aux_nlines);

  out.close();

  return 0;
}

