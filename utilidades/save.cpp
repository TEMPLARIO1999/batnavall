#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct TRecord{
    int sec;
    char nickname[40];
};

int main(){
	TRecord records[10], prueba[10];
	int j=0;
	FILE *archivo = fopen("save.dat", "rb+");
	for(int i=0; i<10; i++){
		records[i].sec = 0;
		strcpy(records[i].nickname, "SinRegistro");
		fwrite(&records[i], sizeof(TRecord), 1, archivo);
	}
	rewind(archivo);
	fread(&prueba[j], sizeof(TRecord), 1, archivo);
	while(!feof(archivo)){
		printf("- %s %i\n", prueba[j].nickname, prueba[j].sec);
		j++;
		fread(&prueba[j], sizeof(TRecord), 1, archivo);
	}
	printf("Finished.");
}
