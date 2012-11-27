#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <list>
#include <set>

struct Couple{
  int numero_variable;
  float rapport;
};

class Comcouple{
public:
  bool operator()(const Couple &a, const Couple &b)
  {
    if(a.rapport==b.rapport)
    {
      return a.numero_variable>b.numero_variable;
    }
    return a.rapport<b.rapport;
  }
};

int main(int argc,char** argv)
{
  int i,j;
  int nb_coefs;
  int coef;
  int alpha,n;
  std::ifstream fichier;
  int nb_variables,nb_contraintes;
  int sum;
  
  //Scan des arguments
  if(argc>2)
  {
    for(i=1;i<argc;i++)
    {
     if(strcmp(argv[i],"-instance")==0)
     {
       fichier.open(argv[i+1]);
       if(!fichier)
       {
	 std::cout<<"Error during opening file\n";
	 return 0;
       }
       i++;
     } 
     else if(strcmp(argv[i],"-alpha")==0)
     {
       alpha=atoi(argv[i+1]);
       i++;
     }
     else if(strcmp(argv[i],"-n")==0)
     {
       n=atoi(argv[i+1]);
       i++;
     }
     else
     {
       std::cout<<"Unreaded argument :"<<argv[i]<<"\n";
     }
    }
    //Fin du scan des arguments
    
    /*
     * Parsing du fichier
     */
    fichier>>nb_contraintes;
    fichier>>nb_variables;
    std::vector<int> couts(nb_variables);
    for(i=0;i<nb_variables;i++)
    {
      fichier>>couts[i];
    }
    std::vector< std::list<int> > coefs_contraintes(nb_contraintes);
    for(i=0;i<nb_contraintes;i++)
    {
      fichier>>nb_coefs;
      for(j=0;j<nb_coefs;j++)
      {
	fichier>>coef;
	coefs_contraintes[i].push_back(coef);
      }
    }
    
    std::set<Couple, Comcouple> liste_rapports;
    Couple couple;
    std::list<int>::iterator it;
    for(i=0;i<nb_variables;i++)
    {
	sum=0;
	for(j=0;j<nb_contraintes;j++)
	{
	  it=coefs_contraintes[j].begin();
	  while(it!=coefs_contraintes[j].end())
	  {
	    if((*it)==i+1)
	    {
	      sum++;
	      break;
	    }
	    it++;
	  }
	}
        couple.numero_variable=i+1;
	couple.rapport=(float)sum/couts[i];
	liste_rapports.insert(couple);
    }
    std::set<Couple, Comcouple>::reverse_iterator rit=liste_rapports.rbegin();
    while(rit!=liste_rapports.rend())
    {
      std::cout<<"Variable "<<(*rit).numero_variable<<" valeur "<<(*rit).rapport<<"\n";
      rit++;
    }
    
    
  }
  else
  {
    std::cout<<"Not enought arguments\n";
  }
}