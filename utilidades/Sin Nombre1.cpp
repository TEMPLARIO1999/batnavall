#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

int main(){
	srand(time(NULL));
	FILE *archivo;
	archivo = fopen("nicks.txt", "r");
	
	int rand1 = rand() % 19, rand2 = rand() % 19, i=0;
	char nickname[25]="", aux1[25], aux2[25];
	
	while(!feof(archivo)){
		fscanf(archivo, "%s %s", aux1, aux2);
		if(i==rand1){
			strcat(nickname, aux1);
		}
		i++;
	}
	rewind(archivo);
	i=0;
	while(!feof(archivo)){
		fscanf(archivo, "%s %s", aux1, aux2);
		if(i==rand2){
			strcat(nickname, aux2);
		}
		i++;
	}
	printf("%s", nickname);
}
