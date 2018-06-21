#include <allegro.h>   //libreria de allegro (modo grafico)
#include <time.h>      //libreria utilizada para numeros random y para el tiempo
#include <stdlib.h>    //Libreria estandar con funciones como srand
#include <stdio.h>     //Libreria de entrada y salida de datos, funciones como fopen, fclose, etc
#include <string.h>    //Libreria utilizada para las funciones strcat, etc de los nicknames

//Bitmap para el tablero donde se posicionan los barcos
BITMAP *tablero;
//Bitmap para los barcos verticales y horizontales.
BITMAP *barco2c;
BITMAP *barco3c;
BITMAP *barco4c;
BITMAP *barco5c;
BITMAP *barco2cv;
BITMAP *barco3cv;
BITMAP *barco4cv;
BITMAP *barco5cv;
//Bitmap para el fondo en color negro (buffer) y fondo en degradado azul.
BITMAP *fondo;
BITMAP *fondo_tab;
//Bitmap para el cursor.
BITMAP *cursor;
//Bitmaps para el menu, son 5 identificando la seleccion de cada uno.
BITMAP *menu0;
BITMAP *menu1;
BITMAP *menu2;
BITMAP *menu3;
BITMAP *menu4;
BITMAP *record;
//Bitmaps representando los tiros.
BITMAP *barco_des;
BITMAP *disp_agua;
BITMAP *fuego;
//Bitmap representando el status de cada jugador y el tiempo.
BITMAP *status;
//Bitmaps donde se expondra la ayuda.
BITMAP *ayuda_pos;
BITMAP *ayuda_ata;
//Archivos .wav para reproduccion de sonido.
SAMPLE *selection;
SAMPLE *posicion;
SAMPLE *main_theme;
SAMPLE *agua;
SAMPLE *explosion;

// Estructura de datos que guarda la informacion esencial para
// guardar y cargar partidas.
struct Tjuego{
	int Tab1[10][10];
	int Tab2[10][10];
	int TabA1[10][10];
	int TabA2[10][10];
	int score1;
	int score2;
	int turno;
	int tiempo[4];
	char nick1[25];
	char nick2[25];
};

// Estructura auxiliar en el posicionamiento y dibujado de barcos.
struct Barco {
	int x;
	int y;
	int tipo;
};

// Estructura que permitira guardar y cargar records.
struct TRecord{
    int sec;
    char nickname[40];
};

void init(); //Prototipo para la inicializacion de Allegro.
void deinit(); //Prototipo para la funcion que se encarga de finalizar la ejecucion de Allegro.
int menu(); //Prototipo que conmuta al inicio, records y final del juego.
int ** reservaMemoria(); //Prototipo que reservara la memoria necesaria para cada matriz principal.
int Posiciona(int **Tab,char *nick, Barco *barcos);  //Prototipo que se encargara de la posicion de cada uno de los barcos en el juego.
void mover(int a,BITMAP * barco, BITMAP *barcov, BITMAP *fondo, int **tablero, int *,int *, Barco*,int n); //Prototipo de la funcion que movera los barcos con las teclas.
void imprime_danio(int **tab, int modo); //Prototipo que se encargara de imprimir parte dañada de los barcos
void imprime_barco(Barco*);//Prototipo que imprime los barcos en el tablero.
int Tab_Bar_Rand(int);//Funcion que selecciona aleatoriamente los bitmaps.
char * nicks(char *);//Le dara un nickname a cada uno de los 2 jugadores.
int ataque(int**Tab,int**TabA,int**Tab1,int**Tab2,int jugador,char*nick1,char*nick2,int*score1,int*score2,int*tiempo, int, Barco*, Barco*);//Definira la validez de los ataques e intercambiara turnos.
void operar_juego(); //Funcion que inicializar y finalizar el juego.
void Copy_Mat(int **Mat,int Mat1[10][10]);  //copia la matriz de juego en la matriz de la estructura
void Copy_Mat2(int Mat[10][10],int **Mat1); //compia la matriz del archivo en la matriz de juego
void guarda(int **Tab1, int **Tab2,int **TabA1, int **TabA2, int jugador,char *nick1,char *nick2,int *score1,int *score2,int*tiempo, Barco *j1, Barco *j2);  //guarda el juego
int carga(int **Tab1, int **Tab2,int **TabA1, int **TabA2,char *nick1,char *nick2,int *score1,int *score2,int*tiempo, Barco *j1, Barco *j2);  //carga los datos guardados en un archivo
void limpia(int **Tab1, int **Tab2,int **TabA1, int **TabA2,int*tiempo, Barco *j1, Barco *j2); //Limpia cada una de las variables al terminar un juego
void limbar(Barco j[8]); // Limpia la estructura barco
void limMat(int **mat); // Limpia las matrices principales del juego
void finalizar_juego(int turno, int *tiempo, char *nick1, char *nick2); // Funcion que determina si es un record y finaliza el juego.
void mostrar_records(); // Funcion que muestra los records al usuario.

int main() {
	srand(time(NULL)); //Necesitamos numeros aleatorios para diseños y nicknames.
	init(); //Inicializamos allegro.
	operar_juego(); //Inicializamos el administrador del juego.
	deinit(); //Desinicializamos allegro.
	return 0; //Retornamos cero. Finaliza la ejecucion.
}
END_OF_MAIN() //Funcion de Allegro que marca el fin del main. Sin el el juego no se ejecuta.

// Funcion que limpia todas las variables esenciales antes de iniciar una partida nueva.
void limpia(int **Tab1, int **Tab2,int **TabA1, int **TabA2,int*tiempo, Barco *j1, Barco *j2){
	limMat(Tab1); limMat(Tab2); limMat(TabA1); limMat(TabA2);
	tiempo[0]=0; tiempo[1]=0; tiempo[2]=0; tiempo[3]=0; tiempo[4]=0;
	limbar(j1);
	limbar(j2);
}

// Funcion que es llamada para limpiar las matrices de juego.
void limMat(int **mat){
	for(int i=0;i<10;i++){
		for(int j=0;j<10;j++){
			*(*(mat+i)+j)=0;
		}
	}
}

// Funcion que devuelve la variable de control de los barcos a -1
// señalando que no ha sido posicionado.
void limbar(Barco j[8]){
	for(int i=0;i<8;i++){
		j[i].x=-1;
	}
}

// Cuando se deja una partida guardada la funcion es encargada de dejar todo como estaba cuando se guardo.
int carga(int **Tab1, int **Tab2,int **TabA1, int **TabA2,char *nick1,char *nick2,int *score1,int *score2,int*tiempo, Barco *j1, Barco *j2){
	FILE *archivo;
	// Accedemos al archivo que contiene la partida.
	if((archivo=fopen("utilidades/juego.dat","rb+"))==NULL){
		allegro_message("No existe una partida guardada");
		return 5;
	}
	Tjuego juego;
	// Le damos a cada jugador los barcos que tenian.
	for(int i=0;i<8;i++){
		fread(&j1[i],sizeof(Barco),1,archivo);
		fread(&j2[i],sizeof(Barco),1,archivo);
	}
	// Leemos la variable de tipo juego que contiene los datos esenciales.
	fread(&juego,sizeof(Tjuego),1,archivo);
	// Recuperamos los datos de las matrices principales.
	Copy_Mat2(juego.Tab1,Tab1);
	Copy_Mat2(juego.Tab2,Tab2);
	Copy_Mat2(juego.TabA1,TabA1);
	Copy_Mat2(juego.TabA2,TabA2);
	// Recuperamos las puntuaciones.
	*score1=juego.score1;
	*score2=juego.score2;
	// Recuperamos el turno en juego.
	int jugador=juego.turno;
	// Recuperamos los tiempos en hh:mm:ss
	tiempo[0]=juego.tiempo[0];
	tiempo[1]=juego.tiempo[1];
	tiempo[2]=juego.tiempo[2];
	tiempo[3]=juego.tiempo[3];
	// Recuperamos los nicknames de cada jugador.
	strcpy(nick1,juego.nick1);
	strcpy(nick2,juego.nick2);
	// Cerramos el archivo para evitar la corrupcion del mismo.
	fclose(archivo);
	return jugador;
}

// Funciones que copian una matriz definida a una de punteros
// y una de punteros a una definida.
void Copy_Mat2(int Mat[10][10],int **Mat1){
	for(int i=0;i<10;i++)
		for(int j=0;j<10;j++)
			*(*(Mat1+i)+j)=Mat[i][j];
}

void Copy_Mat(int **Mat,int Mat1[10][10]){
	for(int i=0;i<10;i++)
		for(int j=0;j<10;j++)
			Mat1[i][j]=*(*(Mat+i)+j);
}

void operar_juego(){
	//VARIABLES PRINCIPALES:
	//Operacion es la variable de control del menu, 1 es para iniciar juego, 2 para cargar juego, 3 para records y 0 para salir.
	//El turno es una variable que se define aleatoriamente, 1 y 2 para cada jugador respectivamente.
	//rand_bmp decidira el disenio para el tablero, hay 4 disponibles.
	//tiempo es un vector de 4 espacios, uno para el acumulado, los otros 3 para HH:MM:SS.
	//4 matrices que definira los tableros de puntuacion y ataque, 2 para cada jugador.
	//Nick1 y Nick 2 le dara a cada jugador un nickname.
	//bar_j1 y bar_j2 son las posiciones y tipo de barco de cada jugador.
	int op=1, turno, *score1=new int, *score2=new int, rand_bmp = -1, tiempo[4] = {0,0,0,0};
	int **Tab1=reservaMemoria(), **TabA1=reservaMemoria(), **Tab2=reservaMemoria(), **TabA2=reservaMemoria();
	Barco bar_j1[8], bar_j2[8];
	char *nick1,*nick2;
	// A cada barco en su posicion x se le asigna -1 para identificar que esa posicion aun no ha sido usada.
	for(int i=0; i<8; i++){
		bar_j1[i].x = -1;
		bar_j2[i].x = -1;
	}
	int i;
	// While hasta que la operacion sea 0.
	// La operacion estara definida por lo que menu retorne.
	while (op!=0) {
		// Llamanos un switch condicionado a menu.
		switch(menu()){
			case 1:
				// OPCION PARA INICIAR NUEVA PARTIDA.
				// se le da un numero a rand_bmp para seleccionar el disenio aleatorio.
				rand_bmp = Tab_Bar_Rand(rand_bmp);
				// Vaciamos scores, nicknames y determinamos un turno aleatorio para posterior uso.
				nick1=new char[25],nick2=new char[25];
				turno=(rand()%2)+1;
				*score1=0; *score2=0;
				// El J1 posiciona sus barcos.
				allegro_message("Jugador 1 posicione sus barcos!");
				Tab_Bar_Rand(rand_bmp);
				// Se le da un nickname al J1 antes de posicionar sus barcos.
				strcpy(nick1, nicks(nick1));
				Posiciona(Tab1,nick1,bar_j1);
				rest(500);
				// El J2 posiciona sus barcos.
				allegro_message("Jugador 2 posicione sus barcos!");
				Tab_Bar_Rand(rand_bmp);
				// Se le da un nickname al J2 antes de posicionar sus barcos.
				strcpy(nick2, nicks(nick2));
				Posiciona(Tab2,nick2,bar_j2);
				// Se llama esta funcion de vuelta para definir los bitmaps porque se hace una limpieza
				// despues de cada posicionamiento de barcos.
				Tab_Bar_Rand(rand_bmp);
				rest(500);
				// Basado en el sorteo anterior avisamos que jugador iniciara la partida.
				allegro_message("Se hara un sorteo para determinar que jugador inicia, el resultado es...");
				if(turno==1)
					allegro_message("INICIA EL JUGADOR 1 (%s)", nick1);
				else
					allegro_message("INICIA EL JUGADOR 2 (%s)", nick2);
				// Inicializamos nuestro contador de tiempo.
				tiempo[0] = time(0);
				// Mientras los puntajes sean distintos de 25 (puntaje para ganar), o que el jugador aun no haya decidido salir
				// se estara ejecutando la funcion de ataque, donde cada jugador atacara al otro.
				while(*score1!=25 && *score2!=25 && turno!=5){
					clear_bitmap(tablero);
					Tab_Bar_Rand(rand_bmp);
					turno = ataque(Tab1, Tab2, TabA1, TabA2, turno,nick1,nick2,score1,score2,tiempo,rand_bmp, bar_j1, bar_j2);
				}
				// Mientas alguno de los jugadores haya finalizado el juego porque gano y no porque se haya salido
				// la funcion se ejecutara.
				if(turno!=5)
					finalizar_juego(turno, tiempo, nick1, nick2);
				// Finalmente limpiamos cada variable esencial para poder iniciar un nuevo juego si el usuario desea.
				limpia(Tab1, Tab2,TabA1,TabA2,tiempo,bar_j1,bar_j2);
				rand_bmp=-1;
				clear_bitmap(tablero);
				break;
			case 2:
				// OPCION PARA CARGAR PARTIDA.
				nick1=new char[25]; nick2=new char[25];
				// Cargamos nuestras variables principales para dejar el juego como se habia guardado.
				turno=carga(Tab1,Tab2,TabA1,TabA2,nick1,nick2,score1,score2,tiempo,bar_j1,bar_j2); 
				rand_bmp = Tab_Bar_Rand(rand_bmp);
				// Generamos un nuevo fondo para el tablero y estatus.
				fondo = create_bitmap(SCREEN_W, SCREEN_H);
				fondo_tab = load_bitmap("dis/pantalla.bmp",NULL);
				// Inicializamos rutinas de tiempo.
				tiempo[0] = time(0);
				// Mientras los puntajes sean distintos de 25 (puntaje para ganar), o que el jugador aun no haya decidido salir
				// se estara ejecutando la funcion de ataque, donde cada jugador atacara al otro.
				while(*score1!=25 && *score2!=25 && turno!=5){
					clear_bitmap(tablero);
					Tab_Bar_Rand(rand_bmp);
					turno = ataque(Tab1, Tab2, TabA1, TabA2, turno,nick1,nick2,score1,score2,tiempo, rand_bmp, bar_j1, bar_j2);
				}
				// Si el usuario gano y no forzo la salida entra a la funcion encargada de terminar el juego.
				if(turno!=5){
					finalizar_juego(turno, tiempo, nick1, nick2);
					remove("utilidades\\juego.dat");
				}
                // Se vuelven a limpiar las variables principales del juego por si el usuario desea iniciar una nueva partida.
				limpia(Tab1, Tab2,TabA1,TabA2,tiempo,bar_j1,bar_j2);
				rand_bmp=-1;
				clear_bitmap(tablero);
				break;
			case 3:
				// Llamamos la función que mostrara en pantalla los records.
				mostrar_records();
				break;
			case 4:
				op=0;
				break;
			default:
				op=0;
				break;
		}
	}
}

// FUNCION PARA MOSTRAR LOS RECORDS EN PANTALLA
void mostrar_records(){
	// i es para recuperar las 10 posiciones en los records.
	// m y h son las posiciones HH:MM para poder imprimir el tiempo.
	int i=0, m[10], h[10];
	// Declaramos un vector de 10 posiciones para records, porque es lo que tiene almacenado el
	// archivo binario para records.
    TRecord records[10];
    // Abrimos nuestro archivo como lectura de binarios.
	FILE *archivo = fopen("utilidades/save.dat", "rb");
    if(archivo==NULL){
        allegro_message("Error interno!");
        exit(1);
    }
    // Leemos las 10 posiciones de los records. Asi como los minutos, horas y segundos de cada uno para 
    // posterior uso en la impresion.
    fread(&records[i], sizeof(TRecord), 1, archivo);
    while(!feof(archivo)){
    	h[i] = records[i].sec/3600;
        records[i].sec%=3600;
        m[i] = records[i].sec/60;
        records[i].sec%=60;
    	i++;
    	fread(&records[i], sizeof(TRecord), 1, archivo);
    }
    // Modo de texto transparente
    text_mode(-1);
    rewind(archivo);
    // Cargamos el bitmap para records.
    record = load_bitmap("dis/records.bmp", NULL);
    // Imprimimos en pantalla cada uno de los records.
    for(i=0; i<10; i++)
    	textprintf(record,font,450,280+(i*40),makecol(255, 255, 255), "- %s               %02d:%02d:%02d", records[i].nickname, h[i], m[i], records[i].sec);
    // Dibujamos el texto con el bitmap en pantalla.
    draw_sprite(screen, record, 0, 0);
    // Ciclo infinito hasta que el usuario presione ESC, con esto saldra de los records.
    while(!key[KEY_ESC]){
	}
	// Cerramos el archivo para evitar perdida de datos.
	fclose(archivo);
}

// FUNCION QUE DETERMINA SI HAY RECORD Y FINAL
void finalizar_juego(int turno, int *tiempo, char *nick1, char *nick2){
	// Declaramos un total de tiempo en SEGUNDOS para poder ser comparado sin complicaciones.
    int tot=tiempo[1]+tiempo[2]*60+tiempo[3]*60*60, i=0;
    // Definimos un vector de records de 10 posiciones para poder comparar.
    TRecord records[10];
	// Abrimos nuestro archivo de records. Si no existe cierra la ejecucion.
    FILE *archivo = fopen("utilidades/save.dat", "rb+");
    if(archivo==NULL){
        allegro_message("Error interno!");
        exit(1);
	}
	// El turno en el que estaba en juego representa quien ha ganado los 25 puntos
	// y gano el juego. Se envia un mensaje de quien gano.
	if (turno==1)
		allegro_message("JUGADOR 1 (%s) ES EL GANADOR", nick1);
	else
		allegro_message("JUGADOR 2 (%s) ES EL GANADOR", nick2);
	// Recuperamos todos los records.
    fread(&records[i], sizeof(TRecord), 1, archivo);
    while(!feof(archivo)){
    	i++;
        fread(&records[i], sizeof(TRecord), 1, archivo);
    }
    // Rebobinamos el archivo de records.
    rewind(archivo);
    // Hacemos las comparaciones de tiempos. La condicion esta dada por:
    // - Si el record esta en ceros, significa que puede modificarse por estar vacio.
    // - Si el tiempo es menor a alguno de los records.
    for(i=0; i<10; i++){
    	if(tot<records[i].sec||records[i].sec==0){
    		// Cuando un record es generado, los demas se desplazaran una posicion abajo
    		// desde la posicion del nuevo record.
    		for(int j=i; j<9; j+=2){
    			records[j+1].sec=records[j].sec;
    			strcpy(records[j+1].nickname, records[j].nickname);
    		}
    		// Escribimos el tiempo y el nombre del jugador ganador.
			records[i].sec = tot;
    		if(turno==1)
    			strcpy(records[i].nickname, nick1);
    		else
    			strcpy(records[i].nickname, nick2);
    		// Felicitamos al usuario.
    		allegro_message("Excelente, ha generado un nuevo record, consultelo en el menu!");
    		break;
		}
	}
	// Escribimos los 10 records al archivo para que se guarden.
	for(int i=0; i<10; i++)
        fwrite(&records[i], sizeof(TRecord), 1, archivo);
    // Cerramos el archivo para evitar perdida de datos.
	clear(screen);
    fclose(archivo);
}

int ataque(int **Tab1, int **Tab2,int **TabA1, int **TabA2, int jugador,char *nick1,char *nick2,int *score1,int *score2,int*tiempo, int rnd, Barco *j1, Barco *j2){
	// x y y determinaran donde se esta haciendo click.
	int *x=new int,*y = new int;
	// Cargamos nuestro cursor, la tabla de estatus, el fondo del juego, la ayuda para el usuario
	// y los sonidos para determinar ataques correctos y fallidos.
	cursor=load_bitmap("dis/mira.bmp",NULL);
	status=load_bitmap("dis/status.bmp",NULL);
	fondo_tab = load_bitmap("dis/pantalla.bmp",NULL);
	ayuda_ata = load_bitmap("dis/ayuda/ataque.bmp",NULL);
	agua = load_wav("sonidos/agua.wav");
	explosion = load_wav("sonidos/explosion.wav");
	int blanco = makecol(255, 255, 255);
	// Siempre que se cambia de turno actualizamos el tiempo.
	tiempo[1] += time(0) - tiempo[0];
	tiempo[0] = time(0);
	// Determinamos cuando es cambio de minuto.
	if(tiempo[1]>=60){
		tiempo[2]+=tiempo[1]/60;
		tiempo[1]%=60;
	}
	// Determinamos cuando es cambio de hora.
	if(tiempo[2]>=60){
		tiempo[3]+=tiempo[2]/60;
		tiempo[2]%=60;
	}
	// Ciclo infinito hasta que se cumpla una condicion valida de salida 
	// y se utilice break, es donde el usuario podra atacar al enemigo.
	while(1){
		// Imprimimos donde el usuario ha atacado correcta o incorrectamente.
		if(jugador==1)
			imprime_danio(TabA2, 0);	
		else
			imprime_danio(TabA1, 0);
		// Dibujamos el raton, los textos de ayuda, puntajes y tiempo transcurrido.
		if(mouse_x<645 && mouse_x>45 && mouse_y<645 && mouse_y>45)
			masked_blit(cursor,fondo,0,0,mouse_x,mouse_y,60,60);
		text_mode(-1);
		textprintf(fondo,font,50,25,blanco,"G = GUARDAR");
		textprintf(fondo,font,150,25,blanco,"A = AYUDA");
		textprintf(fondo,font,250,25,blanco,"M = MOSTRAR TABLERO");
		textprintf(fondo,font,450,25,blanco,"S = SALIR");
		textprintf(status,font,100,250,blanco,"JUGADOR 1 %s",nick1);
		textprintf(status,font,100,300,blanco," %i",*score1);
		textprintf(status,font,100,350,blanco,"JUGADOR 2 %s",nick2);
		textprintf(status,font,100,400,blanco," %i",*score2);
		textprintf(status,font,100,450,blanco,"Tiempo transcurrido:");
		textprintf(status, font, 100, 480, blanco, "%02d:%02d:%02d", tiempo[3], tiempo[2], tiempo[1]);
		//imprimimos la barra de informacion del juego en el fondo y a su vez este en la pantalla
		draw_sprite(fondo,status,700,0);
		draw_sprite(screen,fondo,0,0);
		//Si detecta un clic con el mouse confirma si el ataque es o no valido
		if((mouse_b &1) && (mouse_x<645 && mouse_x>45 && mouse_y<645 && mouse_y>45)){
			//Espera un momento
			for(int i=0; i<25000; i++)
				printf('\0');
			// Recupera las coordenadas del mouse y las convierte en numeros del 0 al 9
			*x= (mouse_x-45)/60, *y=(mouse_y-45)/60;
			if((*x>=0 && *x<10) && (*y>=0 && *y<10)){
				//Si juagdor = 1 utiliza los datos de jugador y si no de jugador 2
				if(jugador==1){
					//Si acierta el ataque llena el espacio en la matriz principal con un 10 
					if((*(*(Tab2+*y)+*x)>1 && *(*(Tab2+*y)+*x)<6) && (*(*(TabA2+*y)+*x)!=10 && *(*(TabA2+*y)+*x)!=9)){
						//Reproduce sonido de explosion y espera a que termine la reproduccion
						play_sample(explosion, 255, 0, 2000, 0);
						rest(1300);
						allegro_message("Jugador 1 tira de nuevo.");
						(*score1)++;
						*(*(TabA2+*y)+*x) = 10;
						return jugador;
					} else {
						//Si la casilla ya esta atacada manda un aviso al jugador
						if((*(*(TabA2+*y)+*x)==10 || *(*(TabA2+*y)+*x)==9)){
							allegro_message("Ya has atacado en esa casilla.");
							return jugador;
						//Si la casilla no tiene barcos ni esta atacada manda el mensaje AGUA = Cambio de turno
						}else{
							play_sample(agua, 255, 0, 2000, 0);
							rest(1000);
							allegro_message("AGUA. Turno del jugador 2.");
							//Da el valor a la casilla de 9 que significa tiro al agua
							*(*(TabA2+*y)+*x) = 9;
						}
						//Cambia el valor de turno = Cambio de jugador
						return jugador+1;
					}
				//Misma secuencia que para el jugador 1
				} else {
					//Si acierta el ataque llena el espacio en la matriz principal con un 10 
					if((*(*(Tab1+*y)+*x)>1 && *(*(Tab1+*y)+*x)<6) && (*(*(TabA1+*y)+*x)!=10 && *(*(TabA1+*y)+*x)!=9)){
						//Reproduce sonido de explosion y espera a que termine la reproduccion
						play_sample(explosion, 255, 0, 2000, 0);
						rest(1300);
						allegro_message("Jugador 2 tira de nuevo.");
						(*score2)++;
						*(*(TabA1+*y)+*x) = 10;
						return jugador;
					} else {
						//Si la casilla ya esta atacada manda un aviso al jugador
						if((*(*(TabA1+*y)+*x)==10 || *(*(TabA1+*y)+*x)==9)){
							allegro_message("Ya has atacado en esa casilla.");
							return jugador;
						//Si la casilla no tiene barcos ni esta atacada manda el mensaje AGUA = Cambio de turno 
						}else{
							play_sample(agua, 255, 0, 2000, 0);
							rest(1000);
							allegro_message("AGUA. Turno del jugador 1.");
							//Da el valor a la casilla de 9 que significa tiro al agua
							*(*(TabA1+*y)+*x) = 9;	
						}
						//Cambia el valor de turno = Cambio de jugador
						return jugador-1;
					}
				}
			}
		}
		//Si se oprime la tecla G guarda la partida
		if(key[KEY_G]){
			guarda(Tab1,Tab2,TabA1,TabA2,jugador,nick1,nick2,score1,score2,tiempo,j1,j2);
		}
		//Si se oprime la tecla A imprime la ayuda para el ataque
		if(key[KEY_A]){
			while(!key[KEY_ESC]){
				draw_sprite(fondo,ayuda_ata,0,0);
				draw_sprite(screen,fondo,0,0);
			}
		}
		//Si se oprime la tecla M muestra el tablero del jugador en curso
		if(key[KEY_M]){
            clear_bitmap(tablero);
            //Carga el tablero y los barcos
            Tab_Bar_Rand(rnd);
			while(!key[KEY_ESC]){
				if(jugador==1){
					//Imprime el daño y los barcos del jugador 1
					imprime_barco(j1);
					imprime_danio(TabA1, 1);	
				} else { 
					//Imprime el daño y los barcos del juagdor 2
					imprime_barco(j2);            
					imprime_danio(TabA2, 1);
				}
				//Imprime las estadistucas del juago  
				textprintf(fondo,font,50,25,blanco,"ESC = Salir de vista.");
				draw_sprite(fondo,status,700,0);
				draw_sprite(screen,fondo,0,0);
			}
			clear_bitmap(tablero);
			//Carga el tablero y los barcos
			Tab_Bar_Rand(rnd);
		}
		//Si se oprime la tecla S retorna un 5 con el que sale del ciclo del juego
		if(key[KEY_S]){
			return 5;
		}
	}                   
}

//Guarda las variables necesarias para continuar la partida como se dejo
void guarda(int **Tab1, int **Tab2,int **TabA1, int **TabA2, int jugador,char *nick1,char *nick2,int *score1,int *score2,int*tiempo, Barco *j1, Barco *j2){
	//Declara la estructura en la que se pueden guardar cada una de las variables
	Tjuego juego;
	//Copia las matrices principales del juego
	Copy_Mat(Tab1,juego.Tab1);
	Copy_Mat(Tab2,juego.Tab2);
	Copy_Mat(TabA1,juego.TabA1);
	Copy_Mat(TabA2,juego.TabA2);
	//Copia las puntuaciones
	juego.score1=*score1;
	juego.score2=*score2;
	//COpia el turno 
	juego.turno=jugador;
	//Copia el tiempo que ha transcurrido
	juego.tiempo[0]=tiempo[0];
	juego.tiempo[1]=tiempo[1];
	juego.tiempo[2]=tiempo[2];
	juego.tiempo[3]=tiempo[3];
	//Copia los nicks de ambos jugadores
	strcpy(juego.nick1,nick1);
	strcpy(juego.nick2,nick2);
	//Abrimos el archivo 
	FILE *archivo;
	if((archivo=fopen("utilidades/juego.dat","wb+"))==NULL){
		exit(1);
	}
	for(int i=0;i<8;i++){
		//Guardamos las estructuras de cada barco de ambos juagdores
		fwrite(&j1[i],sizeof(Barco),1,archivo);
		fwrite(&j2[i],sizeof(Barco),1,archivo);
	}
	//Guardamos la estructura que contiene las variables del juego
	fwrite(&juego,sizeof(Tjuego),1,archivo);
	fclose(archivo);
	rest(500);
}

//Retorna un nick 
char * nicks(char * nickname){
	//Abre el archivo que contiene cada nick
	FILE *archivo;
	//Copia a la variable principal la cadena nula
	strcpy(nickname, "");
	archivo = fopen("utilidades/nicks.txt", "r");
	//Obtiene dos numero random y declara los char para leer los nicks
	int rand1 = rand() % 19, rand2 = rand() % 19, i=0;
	char *aux1, *aux2; aux1=new char[25]; aux2=new char [25]; 
	//Busca y lee el nick random 1 y copia a la cadena principal
	while(!feof(archivo)){
		fscanf(archivo, "%s %s", aux1, aux2);
		if(i==rand1)
			strcat(nickname, aux1);
		i++;
	}
	//Regresa al inicio del archivo
	rewind(archivo);
	i=0;
	//Busca y lee el nick random 2 y copia a la cadena principal
	while(!feof(archivo)){
		fscanf(archivo, "%s %s", aux1, aux2);
		if(i==rand2)
			strcat(nickname, aux2);
		i++;
	}
	fclose(archivo);
	return nickname;
}

//Obtiene un numero random si este no existe y carga los bitmaps de acuerdo a dicho numero
int Tab_Bar_Rand(int rand_bmp){
	//Carga los bitmaps del barco destruido, disparo al agua y fuego 
    barco_des=load_bitmap("dis/barco_des.bmp",NULL);
    disp_agua=load_bitmap("dis/disp_agua.bmp",NULL);
    fuego=load_bitmap("dis/fuego.bmp",NULL);
	if(rand_bmp<0)
		//Obtiene un numero random
		rand_bmp=rand()%4;
	//De acuerdo al numero carga el tablero y cada barco
	switch(rand_bmp){
		case 0:
			tablero=load_bitmap("dis/tableros/fondo-0.bmp",NULL);
			barco2c=load_bitmap("dis/barcos/barco2c-0.bmp",NULL);
			barco3c=load_bitmap("dis/barcos/barco3c-0.bmp",NULL);
			barco4c=load_bitmap("dis/barcos/barco4c-0.bmp",NULL);
			barco5c=load_bitmap("dis/barcos/barco5c-0.bmp",NULL);
			barco2cv=load_bitmap("dis/barcos/barco2cv-0.bmp",NULL);
			barco3cv=load_bitmap("dis/barcos/barco3cv-0.bmp",NULL);
			barco4cv=load_bitmap("dis/barcos/barco4cv-0.bmp",NULL);
			barco5cv=load_bitmap("dis/barcos/barco5cv-0.bmp",NULL);
			break;
		case 1:
			tablero=load_bitmap("dis/tableros/fondo-1.bmp",NULL);
			barco2c=load_bitmap("dis/barcos/barco2c-1.bmp",NULL);
			barco3c=load_bitmap("dis/barcos/barco3c-1.bmp",NULL);
			barco4c=load_bitmap("dis/barcos/barco4c-1.bmp",NULL);
			barco5c=load_bitmap("dis/barcos/barco5c-1.bmp",NULL);
			barco2cv=load_bitmap("dis/barcos/barco2cv-1.bmp",NULL);
			barco3cv=load_bitmap("dis/barcos/barco3cv-1.bmp",NULL);
			barco4cv=load_bitmap("dis/barcos/barco4cv-1.bmp",NULL);
			barco5cv=load_bitmap("dis/barcos/barco5cv-1.bmp",NULL);
			break;
		case 2:
			tablero=load_bitmap("dis/tableros/fondo-2.bmp",NULL);
			barco2c=load_bitmap("dis/barcos/barco2c-2.bmp",NULL);
			barco3c=load_bitmap("dis/barcos/barco3c-2.bmp",NULL);
			barco4c=load_bitmap("dis/barcos/barco4c-2.bmp",NULL);
			barco5c=load_bitmap("dis/barcos/barco5c-2.bmp",NULL);
			barco2cv=load_bitmap("dis/barcos/barco2cv-2.bmp",NULL);
			barco3cv=load_bitmap("dis/barcos/barco3cv-2.bmp",NULL);
			barco4cv=load_bitmap("dis/barcos/barco4cv-2.bmp",NULL);
			barco5cv=load_bitmap("dis/barcos/barco5cv-2.bmp",NULL);
			break;
		case 3:
			tablero=load_bitmap("dis/tableros/fondo-3.bmp",NULL);
			barco2c=load_bitmap("dis/barcos/barco2c-3.bmp",NULL);
			barco3c=load_bitmap("dis/barcos/barco3c-3.bmp",NULL);
			barco4c=load_bitmap("dis/barcos/barco4c-3.bmp",NULL);
			barco5c=load_bitmap("dis/barcos/barco5c-3.bmp",NULL);
			barco2cv=load_bitmap("dis/barcos/barco2cv-3.bmp",NULL);
			barco3cv=load_bitmap("dis/barcos/barco3cv-3.bmp",NULL);
			barco4cv=load_bitmap("dis/barcos/barco4cv-3.bmp",NULL);
			barco5cv=load_bitmap("dis/barcos/barco5cv-3.bmp",NULL);
			break;
	}
	return rand_bmp;
}

//Imprime el daño, el barco destruido y el disparo al agua 
void imprime_danio(int **tab, int modo){
	int x, y;
	//Si mandamos un 1 queremos imprimir daño
	if(modo){
		for(int i=0; i<10; i++)
			for(int j=0; j<10; j++)
				//Si la casilla es igual a 10 imprime el fuego
				if(*(*(tab+i)+j) == 10){
					x=((i-1)*60)+90; y=((j)*60)+30;
					masked_blit(fuego,tablero,0,0,y,x,60,60);
				}
	} else {
	//Si mandamso un 0 queremos imprimir barco destruido y disparo al agua		
		for(int i=0; i<10; i++)
			for(int j=0; j<10; j++){
				x=((i-1)*60)+90; y=((j)*60)+30;
				//Si la casilla es igual a 9 imrpime disparo al agua o X
				if(*(*(tab+i)+j) == 9)
					draw_sprite(tablero,disp_agua,y,x);
				//Si la casilla es igual a 10 imprime barco des o palomita
				else if(*(*(tab+i)+j) == 10)
					draw_sprite(tablero,barco_des,y,x);
			}
	}
	//Imprime tod en el tablero
	draw_sprite(fondo,fondo_tab,0,0);
	draw_sprite(fondo,tablero,45,45);
}

//Recibe la estructura de los barcos e imprime estos
void imprime_barco(Barco *barcos){
	for(int i=0; i<8; i++){
		//Si x es menor a 0 sale del ciclo porque no hay mas barcos a imprimir 
		if(barcos[i].x<0)
			break;
		//De acuerdo al tipo de barco imprime en x,y la imagen
		switch(barcos[i].tipo){
			case 1: 	draw_sprite(tablero,barco2cv, barcos[i].x-45,  barcos[i].y-45);
				break;
			case 2: 	draw_sprite(tablero,barco2c,  barcos[i].x-45,  barcos[i].y-45);
				break;
			case 3: 	draw_sprite(tablero,barco3cv, barcos[i].x-45,  barcos[i].y-45);
				break;
			case 4: 	draw_sprite(tablero,barco3c,  barcos[i].x-45,  barcos[i].y-45);
				break;
			case 5: 	draw_sprite(tablero,barco4cv, barcos[i].x-45,  barcos[i].y-45);
				break;
			case 6: 	draw_sprite(tablero,barco4c,  barcos[i].x-45,  barcos[i].y-45);
				break;
			case 7: 	draw_sprite(tablero,barco5cv, barcos[i].x-45,  barcos[i].y-45);
				break;
			case 8: 	draw_sprite(tablero,barco5c,  barcos[i].x-45,  barcos[i].y-45);
				break;
		}
	}
	//Imprime todo en el tablero
	draw_sprite(fondo,fondo_tab,0,0);
	draw_sprite(fondo,tablero,45,45);
}

//Funcion utilizada para posicionar los barcos al inicio de la partida
int Posiciona(int **Tab,char *nick, Barco *barcos){
	//Crea el fondo (buffer) y carga el fondo azul
	fondo = create_bitmap(SCREEN_W, SCREEN_H);
	fondo_tab = load_bitmap("dis/pantalla.bmp",NULL);
	//Carga los sonidos utilizados en el posicionamiento de los barcos
	selection = load_wav("sonidos/seleccion.wav");
	posicion = load_wav("sonidos/posicion.wav");
	//Carga la imagen de ayuda para posicionar barcos
	ayuda_pos = load_bitmap("dis/ayuda/posicionar.bmp",NULL);
	int blanco = makecol(255, 255, 255);
	//Declara las variables que controlan los barcos faltantes
	int *b5c_rest=new int,*b4c_rest=new int,*b3c_rest=new int,*b2c_rest=new int,*N_barcos=new int;
	*b5c_rest=1; *b4c_rest=2; *b3c_rest=2; *b2c_rest=3; *N_barcos=8; 
	//Entra a un ciclo que permite posicionar cada barco y despues se sale
	while(*N_barcos!=0 ){
		//Limpia el fondo
		clear_bitmap(fondo);
		//Imprime los barcos modelo para que el jugador elija, ademas del tablero en el buffer
		draw_sprite(fondo, fondo_tab, 0, 0);
		draw_sprite(fondo,tablero,45,45);
		draw_sprite(fondo,barco2c,800,600);
		draw_sprite(fondo,barco3c,800,440);
		draw_sprite(fondo,barco4c,800,280);
		draw_sprite(fondo,barco5c,800,120);
		text_mode(-1);                                                  //hace que el texto impreso sea sin fondo
		//Imprime el nombre del jugador y los barcos faltantes de cada tipo
		textprintf(fondo,font,800,50,blanco,"NOMBRE JUGADOR %s",nick);
		textprintf(fondo,font,800,100,blanco,"QUEDAN : %i",*b5c_rest); 
		textprintf(fondo,font,800,260,blanco,"QUEDAN : %i",*b4c_rest);
		textprintf(fondo,font,800,420,blanco,"QUEDAN : %i",*b3c_rest);
		textprintf(fondo,font,800,580,blanco,"QUEDAN : %i",*b2c_rest);
		//Imprime las teclas para elegir cada tipo de barco 
		textprintf(fondo,font,730,150,blanco,"TECLA 5");                
		textprintf(fondo,font,730,310,blanco,"TECLA 4");
		textprintf(fondo,font,730,470,blanco,"TECLA 3");
		textprintf(fondo,font,730,630,blanco,"TECLA 2");
		//Imprime las teclas para elegir opciones diversas
		textprintf(fondo,font,45,20,blanco,"C = CANCELAR");
		textprintf(fondo,font,200,20,blanco,"R = ROTAR BARCO");
		textprintf(fondo,font,400,20,blanco,"A = AYUDA");
		//Imprime el fondo en pantalla
		draw_sprite(screen,fondo,0,0);
		int ctrl=(8-(*N_barcos));
		 //Al presionar cualquier tecla permite al usuario posicionar un barco de n celdas
		 //Se resta al numero de barcos general y de un tipo en concreto
		if(key[KEY_2] && *b2c_rest!=0){              
			(*b2c_rest)--; (*N_barcos)--;                  
			mover(120, barco2c, barco2cv,fondo, Tab, b2c_rest,N_barcos, barcos,1);       //funcion encargada del movimiento
		}
		if(key[KEY_3] && *b3c_rest!=0){
			(*b3c_rest)--; (*N_barcos)--;
			mover(180, barco3c, barco3cv,fondo, Tab, b3c_rest,N_barcos, barcos,3);       //funcion encargada del movimiento
		}
		if(key[KEY_4] && *b4c_rest!=0){
			(*b4c_rest)--; (*N_barcos)--;
			mover(240, barco4c, barco4cv,fondo, Tab, b4c_rest,N_barcos, barcos,5);       //funcion encargada del movimiento
		}
		if(key[KEY_5] && *b5c_rest!=0){
			(*b5c_rest)--; (*N_barcos)--;
			mover(300, barco5c, barco5cv, fondo, Tab, b5c_rest,N_barcos, barcos,7);      //funcion encargada del movimiento
		}
		//Al presionar la tecla A proporciona ayuda al jugador
		if(key[KEY_A]){                                                         
			while(!key[KEY_ESC]){
				draw_sprite(fondo,ayuda_pos,0,0);
				draw_sprite(screen,fondo,0,0);
			}
		}
	}
	//Limpia cada bitmap dejandolo listo para el uso en cualquier otra funcion
	clear_bitmap(fondo);
	clear_bitmap(tablero);
	//Imprime el fondo en el buffer y el buffer en pantalla
	draw_sprite(fondo,fondo_tab,0,0);
	draw_sprite(screen,fondo,0,0);
}

//Funcion que se encarga del movimiento de los barcos al posicionarlos
void mover(int a, BITMAP *barco, BITMAP *barcov, BITMAP *fondo, int **tab, int *rest,int *num, Barco *barcos,int n){
	//Variables principales de coordenadas x,y y limites del tablero
	int x=75,y=75, vertical=-1, lim_sup=45, lim_inf=645, lim_der=705-a, lim_izq=45, ctrl=(8-(*num)-1);
	while(!key[KEY_ENTER] && !key[KEY_C]){ //Mientras no se cancele o establesca un barco continua en la funcion
		readkey(); //lee una llave
		if(key[KEY_RIGHT]) { //si es hacia la derecha aumenta x cuidando que no se salga del limite
			x+=60;
			if(x>lim_der) x-=60;
		}
		if(key[KEY_LEFT]) { //si es hacia la izquierda disminuye x cuidando que no se salga del limite
			x-=60;
			if(x<lim_izq) x+=60;
		}
		if(key[KEY_UP]) { //si es hacia arriba disminuye y cuidando que no se salga del limite
            y-=60;
			if(y<lim_sup) y+=60;
		}		
		if(key[KEY_DOWN]) { //si es hacia abajo aumenta y cuidando que no se salga del limite
			y+=60;
			if(y>lim_inf) y-=60;
		}
		//Secuencia de sonido del movimiento
		stop_sample(selection);
		play_sample(selection, 255, 0, 2000, 0);
		//Si se presiona R activa la variable vertical y cambia los limites
		if(key[KEY_R]){
			vertical*=-1;
			x=75,y=75; //regresa las coordenadas al inicio para evitar errores
			if(vertical>0){
				lim_sup=45, lim_inf=705-a, lim_der=645, lim_izq=45;
			} else {
				lim_sup=45, lim_inf=645, lim_der=705-a, lim_izq=45;
			}
		}
		//Imprime el tablero en el fondo
		draw_sprite(fondo,tablero,45,45);
		if(vertical<0) //Si es horizontal imprime barco horizontal
			draw_sprite(fondo,barco,x,y);
		else  //Si es vertical imprime barco vertical
			draw_sprite(fondo,barcov,x,y);
		draw_sprite(screen,fondo,0,0); //Imprime el fondo en pantalla
	}
	if(key[KEY_C]) {           //Si se oprime C se cancela el proceso 
		(*rest)++; (*num)++;   //Los barcos restantes y barcos del tipo elegido regresan al numero original
	} 
	if(key[KEY_ENTER]) { //Si se oprime enter
		int band=1, x_tab=(x-75)/60, y_tab=(y-75)/60; //Obtenemos los numeros de casilla
		if(vertical>0){ //Si es horizontal guardamos en la matriz numeros horizontales
			for(int i=y_tab, j=0; j<a/60; i++, j++)
				if(tab[i][x_tab]) //Si encuentra un 1 no posiciona
					band=0;
			if(band){
				//Secuencia de sonido de posicion
				play_sample(posicion, 255, 0, 2000, 0);
				for(int i=y_tab, j=0; j<a/60; i++, j++)
						tab[i][x_tab]=a/60; //Guardamos el numero que corresponde a la casilla del barco
				
			}
		} else { //Si es vertical guardamos en la matriz numeros verticales
			for(int i=x_tab, j=0; j<a/60; i++, j++)
				if(tab[y_tab][i]) //Si encuentra un 1 no posiciona
					band=0;
			if(band){
				//Secuencia de sonido de posicion
				play_sample(posicion, 255, 0, 2000, 0);
				for(int i=x_tab, j=0; j<a/60; i++, j++)
					tab[y_tab][i]=a/60; //Guardamos el numero que corresponde a la casilla del barco
			}
		}
		if(band){ //Guarda los barcos en la estructura 
			if(vertical>0)
				barcos[ctrl].tipo=n; //Que barco es en casillas
			else
				barcos[ctrl].tipo=n+1; //Que barco es en casillas
			barcos[ctrl].x = x; //Coordenada en x
			barcos[ctrl].y = y; //Coordenada en y
		} else { //Manda mensaje de error si no se posiciona
			allegro_message("No puedes ponerlo aqui!");
			(*rest)++; (*num)++;
		}
	}
	imprime_barco(barcos); //Va imprimiendo los barcos para que el jugador pueda verlos
}

//Funcion creada para reservar memoria de una matriz de 10x10 e igualar a cero cada espacio
int ** reservaMemoria(){
	int **Tab; Tab=new int*[10];
	for(int i=0;i<10;i++){
		*(Tab+i)=new int[10];
	}
	for(int i=0;i<10;i++){
		for(int j=0;j<10;j++){
			*(*(Tab+i)+j)=0;
		}
	}
	return Tab; //Retorna la matriz declarada
}

//Funcion encargada del menu de inicio del juego 
int menu(){
	int opcion=0;                                //variable de control, de acuerdo al clic esta se modifica
	fondo=create_bitmap(1200,750);               //Buffer del fondo
	main_theme=load_wav("sonidos/menu.wav");     //Secuencia de sonido del menu
	cursor=load_bitmap("dis/papa.bmp",NULL);     //imagen del cursor, una PAPA!
	//Imagenes para cada caso en el menu
	menu0=load_bitmap("menu/menu-0.bmp",NULL);   
	menu1=load_bitmap("menu/menu-1.bmp",NULL);
	menu2=load_bitmap("menu/menu-2.bmp",NULL);
	menu3=load_bitmap("menu/menu-3.bmp",NULL);
	menu4=load_bitmap("menu/menu-4.bmp",NULL);
	//Inicia secuencia del sonido
	play_sample(main_theme, 255, 0, 1000, 0);
	do {
		//Condicionales que verifican las coordenadas del mouse y en base a ellas cambia la imagen del menu
		if(mouse_x>415 && mouse_x<795 && mouse_y>365 && mouse_y<400) {   
			draw_sprite(fondo,menu1,0,0);     //Imprime la imagen de menu seleccionada
			if(mouse_b & 1) opcion=1;         //Si se hace un clic la variable opcion toma el valor seleccionado
		} else if(mouse_x>390 && mouse_x<810 && mouse_y>440 && mouse_y<475) { 
			draw_sprite(fondo,menu2,0,0);     //Imprime la imagen de menu seleccionada
			if(mouse_b & 1) opcion=2;         //Si se hace un clic la variable opcion toma el valor seleccionado
		} else if(mouse_x>495 && mouse_x<700 && mouse_y>515 && mouse_y<550) {
			draw_sprite(fondo,menu3,0,0);     //Imprime la imagen de menu seleccionada
			if(mouse_b & 1) opcion=3;         //Si se hace un clic la variable opcion toma el valor seleccionado
		} else if(mouse_x>525 && mouse_x<665 && mouse_y>590 && mouse_y<620) {
			draw_sprite(fondo,menu4,0,0);     //Imprime la imagen de menu seleccionada
			if(mouse_b & 1) opcion=4;         //Si se hace un clic la variable opcion toma el valor seleccionado
		} else 	draw_sprite(fondo,menu0,0,0); //Imprime la imagen de menu seleccionada
		masked_blit(cursor,fondo,0,0,mouse_x,mouse_y,27,27); //imprime el cursor, respetando su transparencia en fondo
		draw_sprite(screen,fondo,0,0);                       //imprime el fondo en pantalla
	} while(opcion==0);                                      //Sale del menu hasta recibir una accion 
	//Elimina cada uno de los bitmaps utilizados, esto para liberar memoria ya que el menu se mostrara poco
	delete fondo;
	delete cursor;
	delete menu0;                                                           
	delete menu1;
	delete menu2;
	delete menu3;
	delete menu4;
	stop_sample(main_theme);                  //Detiene la secuencia de sonido del menu
	rest(200);                                //Espera un momento
	clear(screen);                            //Limpia la pantalla
	return opcion;
}

//Funcion encargada de las configuraciones iniciales de allegro
//Inicia allegro y crea una ventada de fondo negro de 1200 por 750 pixeles
void init() {                              
	int depth, res;
	allegro_init();         //Inicia allegro
	depth = desktop_color_depth(); 
	if (depth == 0) depth = 32;                                       
	set_color_depth(depth);                                  
	res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, 1200, 750, 0, 0);  //Crea una ventana de 1200 x 750 pixeles
	if (res != 0) {
		allegro_message(allegro_error);
		exit(-1);
	}
	install_keyboard();                                  //Instala el teclado
	install_mouse();                                     //Instala el mouse
	install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,NULL); //Instala el sonido
	
}

//Funcion encargada de terminar la ejecucion de allegro
//Limpia el keybuf y destruye las variables utilizadas para el sonido
void deinit() {
	clear_keybuf();
	destroy_sample(selection);
	destroy_sample(posicion);
}
