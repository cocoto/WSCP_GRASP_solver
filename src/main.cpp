#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <list>
#include <set>
#include <vector>
#include <fstream>
#include<unistd.h>
bool est_admissible(const std::vector<bool> &solution,const  std::vector<std::vector <int> > &coefs_contraintes );

struct Couple{
  int numero_variable;
  float rapport;
};

struct Solution{
  int valeur;
  std::vector<bool> detail;
};

int calcul_solution(const std::vector<bool> &solution, const std::vector<int> &couts)
{
  int i;
  int resultat=0;
  for(i=0;i<(int)solution.size();i++)
  {
    resultat += (solution[i])?couts[i]:0;
  }
  return resultat;
}

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
  bool admissible;
  int i,j,k;
  int nb_coefs;
  int coef;
  float alpha;
  int n=2;
  std::ifstream fichier;
  int nb_variables,nb_contraintes;
  int sum;
  int borne;
  Solution meilleure_solution;
  Solution solution;
  std::set<Couple, Comcouple>::reverse_iterator rit;
  std::set<Couple, Comcouple> liste_rapports;
  std::ofstream fichier_sortie("test.res");
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
    //FIN DU PARSING
    std::vector<std::vector <int> > init_coefs_contraintes=coefs_contraintes;
    int cpt=0;
    while(cpt<n)
    {
		solution.detail=std::vector<bool>(nb_variables,0);
		Couple couple;
		coefs_contraintes=init_coefs_contraintes;
		do{
		  
		    liste_rapports.clear();
		    //Etablissement des rapports
		    for(i=0;i<nb_variables;i++)
		    {
			sum=0;
			for(j=0;j<nb_contraintes;j++)
			{
			  sum+=coefs_contraintes[j][i];
			}
			couple.numero_variable=i+1;
			couple.rapport=(float)sum/couts[i];
			if(couple.rapport>0)
			{
			//std::cout<<"ajout de "<<couple.numero_variable<<"\n";
			liste_rapports.insert(couple); 
			}
		    }
		    if(!liste_rapports.empty())
		    {
		      
			//Calcul de la borne par borne=min + alpha * (max - min)
			rit=liste_rapports.rbegin();
			borne=alpha* rit->rapport;
			
			//Recherche du nombre de candidats
			i=0;
			while((*rit).rapport>=borne && rit!=liste_rapports.rend())
			{
			  rit++;
			  i++;
			}
			//std::cout<<"Nombre de candidats :"<<i<<"\n";
			//Tirage aléatoire d'un candidat
			if(i!=1)
			{
			  i=rand()%(i-1);
			}
			else
			{
			  i=0;
			}
			rit=liste_rapports.rbegin();
			for(j=0;j<i;j++)
			{
			  rit++;
			}
			i=rit->numero_variable;
			//std::cout<<"Variable choisie "<<i<<"\n";
			
			solution.detail[i-1]=true;
			//Mise à jour de la matrice actuelle
			for(j=0;j<nb_contraintes;j++)
			{
			  if(coefs_contraintes[j][i-1]==1)
			  {
			    for(k=0;k<nb_variables;k++)
			    {
			      coefs_contraintes[j][k]=0;
			    }
			  }
			}
		    }//Boucle de selection d'un candidat et mise à jour
		}while(!liste_rapports.empty());
		
		//Première solution admissible OK !
		solution.valeur=calcul_solution(solution.detail,couts);
		meilleure_solution=solution;


		    for(i=0;i<nb_variables;i++)
		    {
		      if(solution.detail[i]==true)
		      {
			//Vérification de l'admissibilité de la solution
			admissible=true;
			j=0;
			while(j<nb_contraintes && admissible)
			{
			  if(init_coefs_contraintes[j][i]>0)
			  {
			    sum=0;
			    for(k=0;k<nb_variables;k++)
			    {
			      if(k!=i)
			      {
				sum+= (solution.detail[k])?coefs_contraintes[j][k]:0;
			      }
			    }
			    if(sum==0)
			    {
			      admissible=false;
			    }
			  }
			  j++;
			}
			if(admissible)
			{
			  //std::cout<<"admissible\n";
			  solution.detail[i]=false;
			  solution.valeur-=couts[i];
			  if(solution.valeur<meilleure_solution.valeur)
			  {
			    std::cout<<solution.valeur<<"\n";
			    meilleure_solution=solution;
			  }
			}
		      }
		    }//Fin de la rercherche locale
	std::cout<<"Run no "<<cpt<<" valeur="<<meilleure_solution.valeur<<"\n";
	fichier_sortie<<cpt<<"  "<<meilleure_solution.valeur<<"\n";
	cpt++;
	
    }//while RUN
   fichier.close();
   fichier_sortie.close();
   /* ------------------------------------------------------- */
	// Ouverture du shell et lancement de gnuplot
	FILE* f = popen("gnuplot", "w");
	
	// exécution de la commande gnuplot
	fprintf(f, " set grid xtics \n");
	fprintf(f, " set grid ytics \n");
	fprintf(f, " set xlabel \"iteration\" \n");
	fprintf(f, " set ylabel \"Valeur optimale\" \n");
	fprintf(f, " set title \"Évolution du run GRASP 1-0\" \n");
	fprintf(f, " plot         \"test.res\" u 1:2\n");	
	fflush(f);
	// terminer l'envoi de commandes et fermer gnuplot
	//sleep(10000);
	std::cin>>i;
	pclose(f);
	/* ------------------------------------------------------- */
	
  }
  else
  {
    std::cout<<"Not enought Arguments\n";
  }
}



bool est_admissible(const std::vector<bool> &solution,const  std::vector<std::vector <int> > &coefs_contraintes )
{
  int nb_variables=solution.size();
  int nb_contraintes=coefs_contraintes.size();
  int i;
  int sum;
  int j;
  for(i=0;i<nb_contraintes;i++)
  {
    sum=0;
    for(j=0;j<nb_variables;j++)
    {
      sum+=solution[j]?coefs_contraintes[i][j]:0;
    }
    if(sum==0)
    {
      return false;
    }
  }
  return true;
}