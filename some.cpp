#include <allegro.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
//Bitmaps para el men?, son 5 identificando la selecci? de cada uno.
BITMAP *menu0;
BITMAP *menu1;
BITMAP *menu2;
BITMAP *menu3;
BITMAP *menu4;
//Bitmaps representando los tiros.
BITMAP *barco_des;
BITMAP *disp_agua;
BITMAP *fuego;
//Bitmap representando el status de cada jugador y el tiempo.
BITMAP *status;
//Bitmaps donde se expondr?la ayuda.
BITMAP *ayuda_pos;
BITMAP *ayuda_ata;
//Archivos .wav para reproducci? de sonido.
SAMPLE *selection;
SAMPLE *posicion;
SAMPLE *main_theme;
SAMPLE *agua;
SAMPLE *explosion;

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

struct Barco {
	BITMAP *dibujo_barco;
	int x;
	int y;
};

void init(); //Prototipo para la inicializacion de Allegro.
void deinit(); //Prototipo para la funcion que se encarga de finalizar la ejecucion de Allegro.
int menu(); //Prototipo que conmuta al inicio, reords y final del juego.
int ** reservaMemoria(); //Prototipo que reservara la memoria necesaria para cada matriz principal.
int Posiciona(int **Tab,char *nick, Barco *barcos); //Prototipo que se encargara de la posicion de cada uno de los barcos en el juego.
void mover(int a,BITMAP * barco, BITMAP *barcov, BITMAP *fondo, int **tablero, int *,int *, Barco*); //Prototipo de la funcion que movera los barcos con las teclas.
void imprime_danio(int **tab, int modo);
void imprime_barco(Barco*);//Prototipo que imprime los barcos en el tablero.
int Tab_Bar_Rand(int);//Funci? que seleccionar?aleatoriamente los bitmaps.
void nicks(char *);//Le dar?un nickname a cada uno de los 2 jugadores.
int ataque(int**Tab,int**TabA,int**Tab1,int**Tab2,int jugador,char*nick1,char*nick2,int*score1,int*score2,int*tiempo, int, Barco*, Barco*);//Definira la validez de los ataques e intercambiar?turnos.
void operar_juego(); //Funcion que inicializar y finalizar el juego.
void Copy_Mat(int **Mat,int Mat1[10][10]);
void Copy_Mat2(int Mat[10][10],int **Mat1);

int main() {
	srand(time(NULL)); //Necesitamos numeros aleatorios para disenios y nicknames.
	init(); //Inicializamos allegro.
	operar_juego(); //Inicializamos el administrador del juego.
	deinit(); //Desinicializamos allegro.
	return 0; //Retornamos cero. Finaliza la ejecucion.
}
END_OF_MAIN() //Funcion de Allegro que marca el fin del main. Sin el el juego no se ejecuta.

void Copy_Mat2(int Mat[10][10],int **Mat1){
	for(int i=0;i<10;i++){
		for(int j=0;j<10;j++){
			*(*(Mat1+i)+j)=Mat[i][j];
		}
	}
}

void Copy_Mat(int **Mat,int Mat1[10][10]){
	for(int i=0;i<10;i++){
		for(int j=0;j<10;j++){
			Mat1[i][j]=*(*(Mat+i)+j);
		}
	}
}

void operar_juego(){
	//VARIABLES PRINCIPALES:
	//Operaci? es la variable de control del menu, 1 es para iniciar juego, 2 para cargar juego, 3 para records y 0 para salir.
	//El turno es una variable que se define aleatoriamente, 1 y 2 para cada jugador respectivamente.
	//rand_bmp decidir?el dise? para el tablero, hay 4 disponibles.
	//tiempo es un vector de 4 espacios, uno para el acumulado, los otros 3 para HH:MM:SS.
	//4 matrices que definir? los tableros de puntuaci? y ataque, 2 para cada jugador.
	//Nick1 y Nick 2 le dar? a cada jugador un nickname.
	int op=1, turno=(rand()%2)+1, *score1=new int, *score2=new int, rand_bmp = -1, tiempo[4] = {0,0,0,0};
	int **Tab1=reservaMemoria(), **TabA1=reservaMemoria(), **Tab2=reservaMemoria(), **TabA2=reservaMemoria();
	Barco bar_j1[8], bar_j2[8];
	for(int i=0; i<8; i++){
		bar_j1[i].x = -1;
		bar_j2[i].x = -1;
	}
	char *nick1 = new char[25], *nick2=new char[25];
	nicks(nick1);
	nicks(nick2);
	rand_bmp = Tab_Bar_Rand(rand_bmp);
	while (op!=0) {
		switch(menu()){
			case 1:
				*score1=0; *score2=0;
				allegro_message("Turno del jugador 1!");
				Posiciona(Tab1,nick1,bar_j1);
				rest(500);
				allegro_message("Turno del jugador 2!");
				Tab_Bar_Rand(rand_bmp);
				Posiciona(Tab2,nick2,bar_j2);
				Tab_Bar_Rand(rand_bmp);
				rest(500);
				allegro_message("Se hara un sorteo para determinar que jugador inicia, el resultado es...");
				if(turno==1)
					allegro_message("INICIA EL JUGADOR 1 (%s)", nick1);
				else
					allegro_message("INICIA EL JUGADOR 2 (%s)", nick2);
				tiempo[0] = time(0);
				while(!key[KEY_ESC] && *score1!=2 && *score2!=2){
					clear_bitmap(tablero);
					Tab_Bar_Rand(rand_bmp);
					turno = ataque(Tab1, Tab2, TabA1, TabA2, turno,nick1,nick2,score1,score2,tiempo,rand_bmp, bar_j1, bar_j2);
				}
				if(turno==1){
					clear(screen);
					allegro_message("JUGADOR 1 %s ES EL GANADOR",nick1);
				}else{
					clear(screen);
					allegro_message("JUGADOR 2 %s ES EL GANADOR",nick2);
				}
				break;
			case 2:
				FILE *archivo;
				if((archivo=fopen("juego.dat","rb"))==NULL){
					exit(1);
				}
				Tjuego juego;
				fread(&juego,sizeof(Tjuego),1,archivo);
				Copy_Mat2(juego.Tab1,Tab1);
				Copy_Mat2(juego.Tab2,Tab2);
				Copy_Mat2(juego.TabA1,TabA1);
				Copy_Mat2(juego.TabA2,TabA2);
				*score1=juego.score1;
				*score2=juego.score2;
				turno=juego.turno;
				tiempo[0]=juego.tiempo[0];
				tiempo[1]=juego.tiempo[1];
				tiempo[2]=juego.tiempo[2];
				tiempo[3]=juego.tiempo[3];
				strcpy(nick1,juego.nick1);
				strcpy(nick2,juego.nick2);
				fclose(archivo);
				fondo = create_bitmap(SCREEN_W, SCREEN_H);
				fondo_tab = load_bitmap("dis/pantalla.bmp",NULL);
				Tab_Bar_Rand(rand_bmp);
				tiempo[0] = time(0);
				while(!key[KEY_ESC] && *score1!=25 && *score2!=25){
					clear_bitmap(tablero);
					Tab_Bar_Rand(rand_bmp);
					turno = ataque(Tab1, Tab2, TabA1, TabA2, turno,nick1,nick2,score1,score2,tiempo, rand_bmp, bar_j1, bar_j2);
				}
				if(turno==1){
					clear(screen);
					allegro_message("JUGADOR 1 %s ES EL GANADOR",nick1);
				}else{
					clear(screen);
					allegro_message("JUGADOR 2 %s ES EL GANADOR",nick2);
				}
				break;
			case 3:
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

int ataque(int **Tab1, int **Tab2,int **TabA1, int **TabA2, int jugador,char *nick1,char*nick2,int *score1,int *score2,int*tiempo, int rnd, Barco *j1, Barco *j2){
	int *x=new int,*y = new int;
	cursor=load_bitmap("dis/cursor.bmp",NULL);
	status=load_bitmap("dis/status.bmp",NULL);
	fondo_tab = load_bitmap("dis/pantalla.bmp",NULL);
	ayuda_ata = load_bitmap("dis/ayuda/ataque.bmp",NULL);
	agua = load_wav("sonidos/agua.wav");
	explosion = load_wav("sonidos/explosion.wav");
	int blanco = makecol(255, 255, 255);
	tiempo[1] += time(0) - tiempo[0];
	tiempo[0] = time(0);
	if(tiempo[1]>=60){
		tiempo[2]+=tiempo[1]/60;
		tiempo[1]%=60;
	}
	if(tiempo[2]>=60){
		tiempo[3]+=tiempo[2]/60;
		tiempo[2]%=60;
	}
	while(1){
		if(jugador==1)
			imprime_danio(TabA2, 0);	
		else
			imprime_danio(TabA1, 0);
		masked_blit(cursor,fondo,0,0,mouse_x,mouse_y,27,27);
		text_mode(-1);   
		textprintf(fondo,font,50,25,blanco,"G = GUARDAR");
		textprintf(fondo,font,150,25,blanco,"A = AYUDA");
		textprintf(fondo,font,250,25,blanco,"S = MOSTRAR TABLERO");
		textprintf(status,font,100,250,blanco,"JUGADOR 1 %s",nick1);
		textprintf(status,font,100,300,blanco," %i",*score1);
		textprintf(status,font,100,350,blanco,"JUGADOR 2 %s",nick2);
		textprintf(status,font,100,400,blanco," %i",*score2);
		textprintf(status,font,100,450,blanco,"Tiempo transcurrido:");
		textprintf(status, font, 100, 480, blanco, "%02d:%02d:%02d", tiempo[3], tiempo[2], tiempo[1]);
		draw_sprite(fondo,status,700,0);
		draw_sprite(screen,fondo,0,0);
		if(key[KEY_M]){
			for(int i=0; i<25000; i++)
				printf('\0');
			*x= (mouse_x-75)/60, *y=(mouse_y-75)/60;
			if((*x>=0 && *x<10) && (*y>=0 && *y<10)){
				if(jugador==1){
					if((*(*(Tab2+*y)+*x)>1 && *(*(Tab2+*y)+*x)<6) && (*(*(TabA2+*y)+*x)!=10 && *(*(TabA2+*y)+*x)!=9)){
						play_sample(explosion, 255, 0, 2000, 0);
						rest(1300);
						allegro_message("Jugador 1 tira de nuevo.");
						(*score1)++;
						*(*(TabA2+*y)+*x) = 10;
						return jugador;
					} else {
						if((*(*(TabA2+*y)+*x)==10 || *(*(TabA2+*y)+*x)==9)){
							allegro_message("Ya has atacado en esa casilla.");
							return jugador;
						}else{
							play_sample(agua, 255, 0, 2000, 0);
							rest(1000);
							allegro_message("AGUA. Turno del jugador 2.");
							*(*(TabA2+*y)+*x) = 9;
						}
						return jugador+1;
					}
				} else {
					if((*(*(Tab1+*y)+*x)>1 && *(*(Tab1+*y)+*x)<6) && (*(*(TabA1+*y)+*x)!=10 && *(*(TabA1+*y)+*x)!=9)){
						play_sample(explosion, 255, 0, 2000, 0);
						rest(1300);
						allegro_message("Jugador 2 tira de nuevo.");
						(*score2)++;
						*(*(TabA1+*y)+*x) = 10;
						return jugador;
					} else {
						if((*(*(TabA1+*y)+*x)==10 || *(*(TabA1+*y)+*x)==9)){
							allegro_message("Ya has atacado en esa casilla.");
							return jugador;
						}else{
							play_sample(agua, 255, 0, 2000, 0);
							rest(1000);
							allegro_message("AGUA. Turno del jugador 1.");
							*(*(TabA1+*y)+*x) = 9;	
						}
						return jugador-1;
					}
				}
			}
		}
		if(key[KEY_G]){
			Tjuego juego;
			Copy_Mat(Tab1,juego.Tab1);
			Copy_Mat(Tab2,juego.Tab2);
			Copy_Mat(TabA1,juego.TabA1);
			Copy_Mat(TabA2,juego.TabA2);
			juego.score1=*score1;
			juego.score2=*score2;
			juego.turno=jugador;
			juego.tiempo[0]=tiempo[0];
			juego.tiempo[1]=tiempo[1];
			juego.tiempo[2]=tiempo[2];
			juego.tiempo[3]=tiempo[3];
			strcpy(juego.nick1,nick1);
			strcpy(juego.nick2,nick2);
			FILE *archivo;
			if((archivo=fopen("juego.dat","ab+"))==NULL){
				exit(1);
			}
			fwrite(&juego,sizeof(Tjuego),1,archivo);
			fclose(archivo);
			rest(500);
		}
		if(key[KEY_A]){
			while(!key[KEY_ESC]){
				draw_sprite(fondo,ayuda_ata,0,0);
				draw_sprite(screen,fondo,0,0);
			}
		}
		if(key[KEY_S]){
			while(!key[KEY_ESC]){
				clear_bitmap(tablero);
				Tab_Bar_Rand(rnd);
				if(jugador==1){
					imprime_barco(j1);
					imprime_danio(TabA1, 1);	
				} else { 
					imprime_barco(j2);            
					imprime_danio(TabA2, 1);
				}
				textprintf(fondo,font,50,25,blanco,"ESC = Salir de vista.");
				draw_sprite(fondo,status,700,0);
				draw_sprite(screen,fondo,0,0);
			}
		}
		clear_bitmap(tablero);
		Tab_Bar_Rand(rnd);
	}                   
}

void nicks(char * nickname){
	FILE *archivo;
	archivo = fopen("utilidades/nicks.txt", "r");
	int rand1 = rand() % 19, rand2 = rand() % 19, i=0;
	char *aux1, *aux2; aux1=new char[25]; aux2=new char [25]; 
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
	fclose(archivo);
}

int Tab_Bar_Rand(int rand_bmp){
	if(rand_bmp<0)
		rand_bmp=rand()%4;
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

void imprime_danio(int **tab, int modo){
	int x, y;
	barco_des=load_bitmap("dis/barco_des.bmp",NULL);
	disp_agua=load_bitmap("dis/disp_agua.bmp",NULL);
	fuego=load_bitmap("dis/fuego.bmp",NULL);
	if(modo){
		for(int i=0; i<10; i++)
			for(int j=0; j<10; j++)
				if(*(*(tab+i)+j) == 10){
					x=((i-1)*60)+90; y=((j)*60)+30;
					masked_blit(fuego,tablero,0,0,y,x,60,60);
				}
	} else {		
		for(int i=0; i<10; i++)
			for(int j=0; j<10; j++){
				x=((i-1)*60)+90; y=((j)*60)+30;
				if(*(*(tab+i)+j) == 9)
					draw_sprite(tablero,disp_agua,y,x);
				else if(*(*(tab+i)+j) == 10)
					draw_sprite(tablero,barco_des,y,x);
			}
	}
	draw_sprite(fondo,fondo_tab,0,0);
	draw_sprite(fondo,tablero,45,45);
}

void imprime_barco(Barco *barcos){
	for(int i=0; i<8; i++){
		if(barcos[i].x<0)
			break;
		draw_sprite(tablero, barcos[i].dibujo_barco,  barcos[i].x-45,  barcos[i].y-45);
	}
	draw_sprite(fondo,fondo_tab,0,0);
	draw_sprite(fondo,tablero,45,45);
}

int Posiciona(int **Tab,char *nick, Barco *barcos){
	fondo = create_bitmap(SCREEN_W, SCREEN_H);
	fondo_tab = load_bitmap("dis/pantalla.bmp",NULL);
	selection = load_wav("sonidos/seleccion.wav");
	posicion = load_wav("sonidos/posicion.wav");
	ayuda_pos = load_bitmap("dis/ayuda/posicionar.bmp",NULL);
	int blanco = makecol(255, 255, 255);
	int *b5c_rest=new int,*b4c_rest=new int,*b3c_rest=new int,*b2c_rest=new int,*N_barcos=new int;
	*b5c_rest=1; *b4c_rest=2; *b3c_rest=2; *b2c_rest=3; *N_barcos=8; 
	while(*N_barcos!=0 && !key[KEY_ESC]){
		clear_bitmap(fondo);
		draw_sprite(fondo, fondo_tab, 0, 0);
		draw_sprite(fondo,tablero,45,45);
		draw_sprite(fondo,barco2c,800,600);
		draw_sprite(fondo,barco3c,800,440);
		draw_sprite(fondo,barco4c,800,280);
		draw_sprite(fondo,barco5c,800,120);
		text_mode(-1);                                                                //hace que el texto impreso sea sin fondo
		textprintf(fondo,font,800,50,blanco,"NOMBRE JUGADOR %s",nick);
		textprintf(fondo,font,800,100,blanco,"QUEDAN : %i",*b5c_rest);  //indican cuantos barcos le quedan al usuario
		textprintf(fondo,font,800,260,blanco,"QUEDAN : %i",*b4c_rest);
		textprintf(fondo,font,800,420,blanco,"QUEDAN : %i",*b3c_rest);
		textprintf(fondo,font,800,580,blanco,"QUEDAN : %i",*b2c_rest);
		textprintf(fondo,font,730,150,blanco,"TECLA 5");                //con que tecla selecciona cada barco
		textprintf(fondo,font,730,310,blanco,"TECLA 4");
		textprintf(fondo,font,730,470,blanco,"TECLA 3");
		textprintf(fondo,font,730,630,blanco,"TECLA 2");
		textprintf(fondo,font,45,20,blanco,"C = CANCELAR");
		textprintf(fondo,font,200,20,blanco,"R = ROTAR BARCO");
		textprintf(fondo,font,400,20,blanco,"A = AYUDA");
		draw_sprite(screen,fondo,0,0);
		if(key[KEY_2] && *b2c_rest!=0){               //al presionar cualquier tecla permite al usuario posicionar un barco de n celdas
			(*b2c_rest)--; (*N_barcos)--;                  //se resta al numero de barcos general y de un tipo en concreto
			mover(120, barco2c, barco2cv,fondo, Tab, b2c_rest,N_barcos, barcos);                  //funcion encargada del movimiento
		}
		if(key[KEY_3] && *b3c_rest!=0){
			(*b3c_rest)--; (*N_barcos)--;
			mover(180, barco3c, barco3cv,fondo, Tab, b3c_rest,N_barcos, barcos);
		}
		if(key[KEY_4] && *b4c_rest!=0){
			(*b4c_rest)--; (*N_barcos)--;
			mover(240, barco4c, barco4cv,fondo, Tab, b4c_rest,N_barcos, barcos);
		}
		if(key[KEY_5] && *b5c_rest!=0){
			(*b5c_rest)--; (*N_barcos)--;
			mover(300, barco5c, barco5cv, fondo, Tab, b5c_rest,N_barcos, barcos);
		}
		if(key[KEY_A]){                                                        //ayuda 
			while(!key[KEY_ESC]){
				draw_sprite(fondo,ayuda_pos,0,0);
				draw_sprite(screen,fondo,0,0);
			}
		}
	}
	clear_bitmap(fondo);
	clear_bitmap(tablero);
	draw_sprite(fondo,fondo_tab,0,0);
	draw_sprite(screen,fondo,0,0);
}


void mover(int a, BITMAP *barco, BITMAP *barcov, BITMAP *fondo, int **tab, int *rest,int *num, Barco *barcos){
	int x=75,y=75, vertical=-1, lim_sup=45, lim_inf=645, lim_der=705-a, lim_izq=45, ctrl=(8-(*num)-1);
	while(!key[KEY_ENTER] && !key[KEY_C]){
		readkey();
		if(key[KEY_RIGHT]) {
			x+=60;
			if(x>lim_der) x-=60;
		}
		if(key[KEY_LEFT]) {
			x-=60;
			if(x<lim_izq) x+=60;
		}
		if(key[KEY_UP]) {
			if(y<lim_sup) y+=60;
			y-=60;
		}		
		if(key[KEY_DOWN]) {
			y+=60;
			if(y>lim_inf) y-=60;
		}
		stop_sample(selection);
		play_sample(selection, 255, 0, 2000, 0);
		if(key[KEY_R]){
			vertical*=-1;
			x=75,y=75;
			if(vertical>0){
				lim_sup=45, lim_inf=705-a, lim_der=645, lim_izq=45;
			} else {
				lim_sup=45, lim_inf=645, lim_der=705-a, lim_izq=45;
			}
		}
		draw_sprite(fondo,tablero,45,45);
		if(vertical<0)
			draw_sprite(fondo,barco,x,y);
		else
			draw_sprite(fondo,barcov,x,y);
		draw_sprite(screen,fondo,0,0);
	}
	if(key[KEY_C]) {
		(*rest)++; (*num)++;
	} 
	if(key[KEY_ENTER]) {
		int band=1, x_tab=(x-75)/60, y_tab=(y-75)/60;
		if(vertical>0){
			for(int i=y_tab, j=0; j<a/60; i++, j++)
				if(tab[i][x_tab])
					band=0;
			if(band){
				play_sample(posicion, 255, 0, 2000, 0);
				for(int i=y_tab, j=0; j<a/60; i++, j++)
						tab[i][x_tab]=a/60;
				
			}
		} else {
			for(int i=x_tab, j=0; j<a/60; i++, j++)
				if(tab[y_tab][i])
					band=0;
			if(band){
				play_sample(posicion, 255, 0, 2000, 0);
				for(int i=x_tab, j=0; j<a/60; i++, j++)
					tab[y_tab][i]=a/60;
			}
		}
		if(band){
			if(vertical>0)
				barcos[ctrl].dibujo_barco = barcov;
			else
				barcos[ctrl].dibujo_barco = barco;
			barcos[ctrl].x = x;
			barcos[ctrl].y = y;
		} else {
			allegro_message("No puedes ponerlo aqui!");
			(*rest)++; (*num)++;
		}
	}
	imprime_barco(barcos);
}

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
	return Tab;
}

int menu(){
	int opcion=0;                                   //variable de control del menu
	fondo=create_bitmap(1200,750);
	main_theme=load_wav("sonidos/menu.wav");
	cursor=load_bitmap("dis/cursor.bmp",NULL);  //imagen del cursor
	menu0=load_bitmap("menu/menu-0.bmp",NULL);    //imagenes del menu
	menu1=load_bitmap("menu/menu-1.bmp",NULL);
	menu2=load_bitmap("menu/menu-2.bmp",NULL);
	menu3=load_bitmap("menu/menu-3.bmp",NULL);
	menu4=load_bitmap("menu/menu-4.bmp",NULL);
	play_sample(main_theme, 255, 0, 1000, 0);
	do {
		if(mouse_x>415 && mouse_x<795 && mouse_y>365 && mouse_y<400) {         
			draw_sprite(fondo,menu1,0,0);                                //si el raton esta entre las coordenas anteriores se imprime menu1 en fondo
			if(mouse_b & 1) opcion=1;                                         //si hace clic izquierdo opcion=1
		} else if(mouse_x>390 && mouse_x<810 && mouse_y>440 && mouse_y<475) { 
			draw_sprite(fondo,menu2,0,0);
			if(mouse_b & 1) opcion=2;
		} else if(mouse_x>495 && mouse_x<700 && mouse_y>515 && mouse_y<550) {
			draw_sprite(fondo,menu3,0,0);
			if(mouse_b & 1) opcion=3;
		} else if(mouse_x>525 && mouse_x<665 && mouse_y>590 && mouse_y<620) {
			draw_sprite(fondo,menu4,0,0);
			if(mouse_b & 1) opcion=4;
		} else 	draw_sprite(fondo,menu0,0,0);
		masked_blit(cursor,fondo,0,0,mouse_x,mouse_y,27,27);                  //imprime el cursor, respetando su transparencia en fondo
		draw_sprite(screen,fondo,0,0);                                 //imprime el fondo en pantalla
	} while(opcion==0);
	delete fondo;
	delete cursor;
	delete menu0;                                                           //elimina los bitmaps
	delete menu1;
	delete menu2;
	delete menu3;
	delete menu4;
	stop_sample(main_theme);
	rest(200);
	clear(screen);                                                           //al salir del menu se limpia la pantalla
	return opcion;
}

void init() {                                                                //crea pantalla e instala el mouse y teclado
	int depth, res;
	allegro_init();
	depth = desktop_color_depth();
	if (depth == 0) depth = 32;
	set_color_depth(depth);
	res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, 1200, 750, 0, 0);
	if (res != 0) {
		allegro_message(allegro_error);
		exit(-1);
	}
	install_keyboard();
	install_mouse();
	install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,NULL);
	
}

void deinit() {
	clear_keybuf();
	destroy_sample(selection);
	destroy_sample(posicion);
}
