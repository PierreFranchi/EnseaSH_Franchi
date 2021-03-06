#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define tailleMaxCMD 100

char clavierIN[tailleMaxCMD];
char signalProcFils[tailleMaxCMD];
const char bienvenue[]="\nBienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n";
const char prompt[]="enseash >> ";
const char promptRAS[]="enseash [exit:0] >> ";
const char exitToken[]="exit";
const char msgSortie[]="à tantôt !\n";
char sig[] = "enseash[sign:";
int status; //status du processus fils

void affiche(const char*msg){write(STDOUT_FILENO,msg, strlen(msg));}

int main (){
	
	affiche(bienvenue);
	affiche(prompt);

	size_t tailleCMD;
	
	while(1){
		tailleCMD = read(STDIN_FILENO, clavierIN, tailleMaxCMD);
		clavierIN[tailleCMD-1]='\0'; //on transforme l'entrée clavier en chaine de caractère string.
		
		if(strncmp(clavierIN,exitToken,strlen(exitToken))==0 || tailleCMD==0){//détection de "exit" ou de Ctrl+d
			affiche(msgSortie);
			break;
		}
        //cf diapo 45/2.
		pid_t pid = fork(); //on duplique le processus pour l'execution d'une commande

		if(pid==0){ //si on se trouve dans le processus fils
			execlp(clavierIN,clavierIN,(char *)NULL);
			exit(EXIT_FAILURE);
		}
		else{//processus père
			wait(&status);
			if(WIFEXITED(status)){
					affiche(promptRAS);
				}
			else if(WIFSIGNALED(status)){
					signalProcFils[0] = (char) WTERMSIG(status);
					write(STDOUT_FILENO,strcat(strcat(sig,&signalProcFils[0])," >> "),strlen(sig));
				}
		}
			
		
	}
	exit(EXIT_SUCCESS);
}
