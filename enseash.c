#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

const char bienvenue[]="\nBienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n";
const char prompt[]="enseash % \n\n";


int main (){
	write(STDOUT_FILENO,bienvenue, sizeof(bienvenue));
	write(STDOUT_FILENO,prompt, sizeof(prompt));
	exit(EXIT_SUCCESS);
}
