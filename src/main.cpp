#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <list>
#include <set>
#include <vector>


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
  srand(time(NULL));
  int i,j,k;
  int nb_coefs;
  int coef;
  float alpha;
  int n;
  std::ifstream fichier;
  int nb_variables,nb_contraintes;
  int sum;
  int borne;
  
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
       alpha=atof(argv[i+1]);
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
    //std::vector< std::list<int> > coefs_contraintes(nb_contraintes);
    std::vector<std::vector <int> > coefs_contraintes(nb_contraintes,std::vector<int>(nb_variables,0));
    for(i=0;i<nb_contraintes;i++)
    {
      fichier>>nb_coefs;
      //coefs_contraintes(nb_variables,0);
      for(j=0;j<nb_coefs;j++)
      {
	fichier>>coef;
	coefs_contraintes[i][coef-1]=1;
      }
    }
    
    std::set<Couple, Comcouple> liste_rapports;
    Couple couple;
    for(i=0;i<nb_variables;i++)
    {
	sum=0;
	for(j=0;j<nb_contraintes;j++)
	{
	  sum+=coefs_contraintes[j][i];
	}
        couple.numero_variable=i+1;
	couple.rapport=(float)sum/couts[i];
	liste_rapports.insert(couple);
    }
    std::set<Couple, Comcouple>::reverse_iterator rit=liste_rapports.rbegin();
    borne=alpha* (*rit).rapport;
    i=0;
    while((*rit).rapport>=borne && rit!=liste_rapports.rend())
    {
      rit++;
      i++;
    }
    
    //i référence le nombre de variables potentielles
    i=rand()%i;
    std::cout<<"Variable choisie "<<i<<"\n";
    for(j=0;j<nb_contraintes;j++)
    {
      if(coefs_contraintes[j][i]==1)
      {
	for(k=0;k<nb_variables;k++)
	{
	  coefs_contraintes[j][k]=0;
	}
      }
    }
    
//     while(rit!=liste_rapports.rend())
//     {
//       std::cout<<"Variable "<<(*rit).numero_variable<<" valeur "<<(*rit).rapport<<"\n";
//       rit++;
//     }
    
    
  }
  else
  {
    std::cout<<"Not enought arguments\n";
  }
}