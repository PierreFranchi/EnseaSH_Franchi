#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define tailleMaxCMD 100

char clavierIN[tailleMaxCMD];
const char bienvenue[]="\nBienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n";
const char prompt[]="enseash % > ";
const char exitToken[]="exit";
const char msgAuRevoir[]="à tantôt !\n";

void affiche(const char*msg){write(STDOUT_FILENO,msg, strlen(msg));}

int main (){
	
	affiche(bienvenue);
	affiche(prompt);

	size_t tailleCMD;
	
	while(1){
		tailleCMD = read(STDIN_FILENO, clavierIN, tailleMaxCMD);
		clavierIN[tailleCMD-1]='\0'; //on transforme l'entrée clavier en chaine de caractère string.
		
		if(strncmp(clavierIN,exitToken,strlen(exitToken))==0 || tailleCMD==0){//détection de "exit" ou de Ctrl+d
			affiche(msgAuRevoir);
			break;
		}
		pid_t pid = fork(); //on duplique le processus sinon le processus actuel se ferme en cas d'execution d'une commande
		if(pid==0) //si on se trouve dans le processus fils afin d'éviter de clore le processus actuel qui correspond au shell
		{
			execlp(clavierIN,clavierIN,(char *)NULL);
			exit(EXIT_FAILURE);
		}
		else{
			wait(NULL);
		}
			
		affiche(prompt);
	}
	exit(EXIT_SUCCESS);
}

