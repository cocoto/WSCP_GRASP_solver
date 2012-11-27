#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>

int main(int argc,char** argv)
{
  int i,j,k;
  int nb_coefs;
  int coef;
  int alpha,n;
  std::ifstream fichier;
  int nb_variables,nb_contraintes;
  
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
    }
    //Fin du scan des arguments
    
    /*
     * Parsing du fichier
     */
    fichier>>nb_variables;
    fichier>>nb_contraintes;
    std::vector<int> couts(nb_variables);
    for(i=0;i<nb_variables;i++)
    {
      fichier>>couts[i];
    }
    std::vector< std::list<int> > coefs_contraintes(nb_contraintes);
    for(i=0;i<nb_contraintes;i++)
    {
      fichier>>nb_coefs;
      for(j=0;i<nb_coefs;j++)
      {
	fichier>>coef;
	coefs_contraintes[i].push_back(coef);
      }
    }
    
  }
  else
  {
    std::cout<<"Not enought arguments\n";
  }
}