#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

#define tailleMaxCMD 100
#define mille 1000 //pour convertir des secondes en milisecondes
#define million 1000000 //pour convertir des nanosecondes en milisecondes

char clavierIN[tailleMaxCMD];
char signalProcFils[tailleMaxCMD];
const char bienvenue[]="\nBienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n";
const char prompt[]="enseash >> "; //prompt normal
const char promptRAS[]="enseash [exit:0] >> ";//
const char promptAVous[]=" >> ";//sigifie l'allente d'instuctions
const char exitToken[]="exit";
const char msgSortie[]="à tantôt !\n";
const char *separateur=" ";
char sig[tailleMaxCMD] = "enseash[sign:";
int status; //status du processus fils
char promptStatut[tailleMaxCMD];
char promptTemps[tailleMaxCMD];
char promptCR[2*tailleMaxCMD];

//config cronomètre
int deltaTms;//durée d'exécution en ms
clock_t startCLK,endCLK;
#define clocksPerMiliSec CLOCKS_PER_SEC/mille

//fonctions
void affiche(const char*msg){write(STDOUT_FILENO,msg, strlen(msg));}

//programme principal
int main (){
	
	affiche(bienvenue);
	affiche(prompt);

	size_t tailleCMD;
	
	while(1){
		tailleCMD = read(STDIN_FILENO, clavierIN, tailleMaxCMD);
		clavierIN[tailleCMD-1]='\0'; //on transforme l'entrée clavier en chaine de caractère string.
        char *strToken;
        char *cmdDuplicated;
		
		if(strncmp(clavierIN,exitToken,strlen(exitToken))==0 || tailleCMD==0){//détection de "exit" ou de Ctrl+d
			affiche(msgSortie);
			break;
		}
        //cf diapo 45/2.
		pid_t pid = fork(); //on duplique le processus pour l'execution d'une commande
		startCLK=clock();
		if(pid==0){ //si on se trouve dans le processus fils
                char *liste[20];
                cmdDuplicated=strdup(clavierIN);//on duplique la commande pour pouvoir travailler sur des éléments

                int i,j;
                for(i=0;;i++,cmdDuplicated=NULL){
					strToken = strtok(cmdDuplicated," "); //enregistre le premier argument de la commande
					if (strToken==NULL) break;
                    liste[i]=strToken;//on ajoute un argument à chaque occurence de la liste
                    j+=1;
                }

                free(cmdDuplicated); //permet de libérrer l'allocation de la mémoire
				liste[j] = (char *)NULL; //le dernier mot de la liste correspond au dernier argument de la fonction execvp

                execvp(liste[0],&liste[0]);
				affiche(prompt);
			    exit(EXIT_FAILURE);
		}
		else{//processus père
			wait(&status);
			if(WIFEXITED(status)){
					signalProcFils[0] = (char) WEXITSTATUS(status);
					sprintf(promptStatut,"exit:%d",signalProcFils[0]);
				}
			else if(WIFSIGNALED(status)){
					signalProcFils[0] = (char) WTERMSIG(status);
					sprintf(promptStatut,"signal:%d",signalProcFils[0]);
				}
			endCLK=clock();
			deltaTms=(endCLK-startCLK)/mille;
			sprintf(promptCR,"enseash [%s|%ims] >> ",promptStatut, deltaTms);
        	affiche(promptCR);
		}	
	}
	exit(EXIT_SUCCESS);
}