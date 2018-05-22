#include <allegro.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

BITMAP *tablero;
BITMAP *barco2c;
BITMAP *barco3c;
BITMAP *barco4c;
BITMAP *barco5c;
BITMAP *barco2cv;
BITMAP *barco3cv;
BITMAP *barco4cv;
BITMAP *barco5cv;
BITMAP *fondo;
BITMAP *fondo_tab;
BITMAP *cursor;
BITMAP *menu0;
BITMAP *menu1;
BITMAP *menu2;
BITMAP *menu3;
BITMAP *menu4;
SAMPLE *selection;
BITMAP *barco_des;

void init();
void deinit();
int menu();
int ** reservaMemoria();
int Posiciona(int **Tab,char *nick);
void mover(int a,BITMAP * barco, BITMAP *barcov, BITMAP *fondo, int **tablero, int *,int *);
void imprime_barco(int **Tab);
void Tab_Bar_Rand();
void nicks(char *);
void copy(char *,char *);
void ataque(int **Tab,int **TabA);

int main() {
	barco_des=load_bitmap("dis\\barcos_des.bmp",NULL);
	int op=1;
	int *score1,*score2; score1=new int; score2=new int;
	int **Tab1,**TabA1; Tab1=reservaMemoria(); TabA1=reservaMemoria();
	int **Tab2,**TabA2; Tab2=reservaMemoria(); TabA2=reservaMemoria();
	char * nick1; nick1=new char[25];
	char * nick2; nick2=new char[25];
	init();
	while (op!=0) {
		switch(menu()){
			case 1:
				*score1=0; *score2=0;
				allegro_message("¡Turno del jugador 1!");
				nicks(nick1);
				Posiciona(Tab1,nick1);
				allegro_message("¡Turno del jugador 2!");
				nicks(nick2);
				Posiciona(Tab2,nick2);
				allegro_message("INICIA PARTIDA!");
				Tab_Bar_Rand();
				while(!key[KEY_ESC] && *score1!=25 && *score2!=25){
					allegro_message("¡Daños del jugador 1!");
					while(!key[KEY_O]){
							for(int i=0;i<10;i++){
								for(int j=0;j<10;j++){
									if(*(*(TabA2+i)+j)==6)
										draw_sprite(tablero, barco_des, i*60+75, j*60+75);
								}
							}
						imprime_barco(Tab1);
						textprintf(fondo,font,30,30,makecol(255,255,255),"O = ESTA BIEN");
						blit(tablero,fondo,0,0,45,45,660,660);
						blit(fondo,screen,0,0,0,0,1200,750);
					}
					clear(fondo);
					draw_sprite(fondo, fondo_tab, 0, 0);
					blit(fondo,screen,0,0,0,0,1200,750);
					allegro_message("¡Ataque del jugador 1!");
					ataque(Tab2,TabA2);
				}
			
				break;
			case 2:
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

	deinit();
	return 0;
}
END_OF_MAIN()

void ataque(int **Tab, int **TabA){
	int *x,*y; x=new int; y=new int;
	int band=1;
	cursor=load_bitmap("dis//cursor.bmp",NULL);  //imagen del cursor
	while(band!=0){
		imprime_barco(TabA);
		blit(tablero,fondo,0,0,45,45,660,660);
		if((mouse_x>75 && mouse_y>75) && (mouse_x<675 && mouse_y<675)){
			masked_blit(cursor,fondo,0,0,mouse_x,mouse_y,27,27);                  //imprime el cursor, respetando su transparencia en fondo
			if(mouse_b & 1) {
				*x=(mouse_x-75)/60;
				*y=(mouse_y-75)/60;
				if(*(*(Tab + *x)+*y)>1 && *(*(Tab +*x)+*y)<5){
					*(*(TabA+*x)+*y)=6;
					*(*(Tab+*x)+*y)=8;
					allegro_message("¡ACERTASTE TIRA DE NUEVO!");
				}else{
					*(*(TabA+*x)+*y)=7;
					allegro_message("¡TIRO AL AGUA!");
					band=0;
				}
			}
		}
		blit(fondo,screen,0,0,0,0,1200,750);
	}
}

void nicks(char * nickname){
	srand(time(NULL));
	FILE *archivo;
	archivo = fopen("utilidades\\nicks.txt", "r");
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
			strcat(nickname," ");
			strcat(nickname, aux2);
		}
		i++;
	}
	fclose(archivo);
	for(int i=0;*(nickname +i)!='\0';i++){
		if(*(nickname +i)>96 && *(nickname +i)<123){
			*(nickname +i)-=32;
		}
	}
}

void Tab_Bar_Rand(){
	srand(time(NULL));
	switch(rand()%4){
		case 0:
			tablero=load_bitmap("dis\\tableros\\fondo-0.bmp",NULL);
			barco2c=load_bitmap("dis\\barcos\\barco2c-0.bmp",NULL);
			barco3c=load_bitmap("dis\\barcos\\barco3c-0.bmp",NULL);
			barco4c=load_bitmap("dis\\barcos\\barco4c-0.bmp",NULL);
			barco5c=load_bitmap("dis\\barcos\\barco5c-0.bmp",NULL);
			barco2cv=load_bitmap("dis\\barcos\\barco2cv-0.bmp",NULL);
			barco3cv=load_bitmap("dis\\barcos\\barco3cv-0.bmp",NULL);
			barco4cv=load_bitmap("dis\\barcos\\barco4cv-0.bmp",NULL);
			barco5cv=load_bitmap("dis\\barcos\\barco5cv-0.bmp",NULL);
			break;
		case 1:
			tablero=load_bitmap("dis\\tableros\\fondo-1.bmp",NULL);
			barco2c=load_bitmap("dis\\barcos\\barco2c-1.bmp",NULL);
			barco3c=load_bitmap("dis\\barcos\\barco3c-1.bmp",NULL);
			barco4c=load_bitmap("dis\\barcos\\barco4c-1.bmp",NULL);
			barco5c=load_bitmap("dis\\barcos\\barco5c-1.bmp",NULL);
			barco2cv=load_bitmap("dis\\barcos\\barco2cv-1.bmp",NULL);
			barco3cv=load_bitmap("dis\\barcos\\barco3cv-1.bmp",NULL);
			barco4cv=load_bitmap("dis\\barcos\\barco4cv-1.bmp",NULL);
			barco5cv=load_bitmap("dis\\barcos\\barco5cv-1.bmp",NULL);
			break;
		case 2:
			tablero=load_bitmap("dis\\tableros\\fondo-2.bmp",NULL);
			barco2c=load_bitmap("dis\\barcos\\barco2c-2.bmp",NULL);
			barco3c=load_bitmap("dis\\barcos\\barco3c-2.bmp",NULL);
			barco4c=load_bitmap("dis\\barcos\\barco4c-2.bmp",NULL);
			barco5c=load_bitmap("dis\\barcos\\barco5c-2.bmp",NULL);
			barco2cv=load_bitmap("dis\\barcos\\barco2cv-2.bmp",NULL);
			barco3cv=load_bitmap("dis\\barcos\\barco3cv-2.bmp",NULL);
			barco4cv=load_bitmap("dis\\barcos\\barco4cv-2.bmp",NULL);
			barco5cv=load_bitmap("dis\\barcos\\barco5cv-2.bmp",NULL);
			break;
		case 3:
			tablero=load_bitmap("dis\\tableros\\fondo-3.bmp",NULL);
			barco2c=load_bitmap("dis\\barcos\\barco2c-3.bmp",NULL);
			barco3c=load_bitmap("dis\\barcos\\barco3c-3.bmp",NULL);
			barco4c=load_bitmap("dis\\barcos\\barco4c-3.bmp",NULL);
			barco5c=load_bitmap("dis\\barcos\\barco5c-3.bmp",NULL);
			barco2cv=load_bitmap("dis\\barcos\\barco2cv-3.bmp",NULL);
			barco3cv=load_bitmap("dis\\barcos\\barco3cv-3.bmp",NULL);
			barco4cv=load_bitmap("dis\\barcos\\barco4cv-3.bmp",NULL);
			barco5cv=load_bitmap("dis\\barcos\\barco5cv-3.bmp",NULL);
			break;
	}
}

void imprime_barco(int **Tab){
	int band=0,x,y;
	for(int a=2;a<6;a++){
		for(int i=0;i<10;i++){
			for(int j=0;j<10;j++){
				if(*(*(Tab+j)+i)==a){
					band++;
					if(band==5 && a==5){
						blit(barco5cv,tablero,0,0,x,y,60,300);
						band=0;
					}
					if(band==1){
						x=((i-1)*60)+90; y=((j)*60)+30;
					}
				}else{
					if(band!=a) band=0;
					switch(band){
						case 1:
							break;
						case 2:
							blit(barco2cv,tablero,0,0,x,y,60,120);
							break;
						case 3:
							blit(barco3cv,tablero,0,0,x,y,60,180);
							break;
						case 4:
							blit(barco4cv,tablero,0,0,x,y,60,240);
							break;
						case 5:
							blit(barco5cv,tablero,0,0,x,y,60,300);
							break;
					}
					band=0;
				}
			}
		}
		for(int i=0;i<10;i++){
			for(int j=0;j<10;j++){
				if(*(*(Tab+i)+j)==a){
					band++;
					if(band==5 && a==5){
						blit(barco5c,tablero,0,0,y,x,300,60);
						band=0;
					}
					if(band==1){
						x=((i-1)*60)+90; y=((j)*60)+30;
					}
				}else{
					if(band!=a) band=0;
					switch(band){
						case 1:
							break;
						case 2:
							blit(barco2c,tablero,0,0,y,x,120,60);
							break;
						case 3:
							blit(barco3c,tablero,0,0,y,x,180,60);
							break;
						case 4:
							blit(barco4c,tablero,0,0,y,x,240,60);
							break;
						case 5:
							blit(barco5c,tablero,0,0,y,x,300,60);
							break;
					}
					band=0;
				}
			}
		}
	}
}

int Posiciona(int **Tab,char *nick){
	fondo = create_bitmap(SCREEN_W, SCREEN_H);
	fondo_tab = load_bitmap("dis\\pantalla.bmp",NULL);
	selection = load_wav("sonidos\\seleccion.wav");
	Tab_Bar_Rand();
	int *b5c_rest=new int,*b4c_rest=new int,*b3c_rest=new int,*b2c_rest=new int,*N_barcos=new int,a;
	*b5c_rest=1; *b4c_rest=2; *b3c_rest=2; *b2c_rest=3; *N_barcos=8; 
	while(*N_barcos!=0 && !key[KEY_ESC]){
		clear_bitmap(fondo);
		draw_sprite(fondo, fondo_tab, 0, 0);
		blit(tablero,fondo,0,0,45,45,660,660);
		blit(barco2c,fondo,0,0,800,600,120,60);
		blit(barco3c,fondo,0,0,800,440,180,60);
		blit(barco4c,fondo,0,0,800,280,240,60);
		blit(barco5c,fondo,0,0,800,120,300,60);
		text_mode(-1);                                                                //hace que el texto impreso sea sin fondo
		textprintf(fondo,font,800,50,makecol(255,255,255),"NOMBRE JUGADOR %s",nick);
		textprintf(fondo,font,800,100,makecol(255,255,255),"QUEDAN : %i",*b5c_rest);  //indican cuantos barcos le quedan al usuario
		textprintf(fondo,font,800,260,makecol(255,255,255),"QUEDAN : %i",*b4c_rest);
		textprintf(fondo,font,800,420,makecol(255,255,255),"QUEDAN : %i",*b3c_rest);
		textprintf(fondo,font,800,580,makecol(255,255,255),"QUEDAN : %i",*b2c_rest);
		textprintf(fondo,font,730,150,makecol(255,255,255),"TECLA 5");                //con que tecla selecciona cada barco
		textprintf(fondo,font,730,310,makecol(255,255,255),"TECLA 4");
		textprintf(fondo,font,730,470,makecol(255,255,255),"TECLA 3");
		textprintf(fondo,font,730,630,makecol(255,255,255),"TECLA 2");
		textprintf(fondo,font,45,20,makecol(255,255,255),"C = CANCELAR");
		textprintf(fondo,font,200,20,makecol(255,255,255),"R = ROTAR BARCO");
		textprintf(fondo,font,400,20,makecol(255,255,255),"A = AYUDA");
		blit(fondo,screen,0,0,0,0,1200,750);
		if(key[KEY_2] && *b2c_rest!=0){               //al presionar cualquier tecla permite al usuario posicionar un barco de n celdas
			(*b2c_rest)--; (*N_barcos)--;                  //se resta al numero de barcos general y de un tipo en concreto
			a=2; a*=60;
			mover(a, barco2c, barco2cv,fondo, Tab, b2c_rest,N_barcos);                  //funcion encargada del movimiento
		}
		if(key[KEY_3] && *b3c_rest!=0){
			(*b3c_rest)--; (*N_barcos)--;
			a=3; a*=60;
			mover(a, barco3c, barco3cv,fondo, Tab, b3c_rest,N_barcos);
		}
		if(key[KEY_4] && *b4c_rest!=0){
			(*b4c_rest)--; (*N_barcos)--;
			a=4; a*=60;
			mover(a, barco4c, barco4cv,fondo, Tab, b4c_rest,N_barcos);
		}
		if(key[KEY_5] && *b5c_rest!=0){
			(*b5c_rest)--; (*N_barcos)--;
			a=5; a*=60;
			mover(a, barco5c, barco5cv, fondo, Tab, b5c_rest,N_barcos);
		}
		if(key[KEY_A]){                                                        //ayuda 
			draw_sprite(fondo, fondo_tab, 0, 0);
			FILE * ayuda_barco;
			ayuda_barco=fopen("utilidades\\Como posicionar barcos.txt","r");
			char *aux0, *aux1; aux0=new char[120]; aux1=new char[120];
			int x=100,y=100;
			while(!key[KEY_B]){
				fscanf(ayuda_barco, "%s", aux0);
				while(!feof(ayuda_barco)){
					copy(aux0,aux1);
			 		textprintf(fondo,font,x,y,makecol(255,255,255),"%s",aux1);
					fscanf(ayuda_barco, "%s ", aux0);
					y+=45;
				}
			blit(fondo,screen,0,0,0,0,1200,750);
			}
			fclose(ayuda_barco);
		}
	}
	clear_bitmap(fondo);
	clear_bitmap(tablero);
	draw_sprite(fondo, fondo_tab, 0, 0);
	blit(fondo,screen,0,0,0,0,1200,750);
}

void copy(char *aux,char *aux0){
	for(int i=0;i<120;i++) *(aux0 + i)=0;
	for(int i=0;*(aux +i)!='\0';i++){
		if(*(aux + i)==95) *(aux0 +i)=32;
	  	else if(*(aux +i)>96 && *(aux +i)<123) *(aux0 +i)=*(aux +i)-32;
		else *(aux0 +i)=*(aux +i);
	}
	
}

void mover(int a, BITMAP *barco, BITMAP *barcov, BITMAP *fondo, int **tab, int *rest,int *num){
	int x=75,y=75, vertical=-1, lim_sup=45, lim_inf=645, lim_der=705-a, lim_izq=45;
	while(!key[KEY_ENTER] && !key[KEY_C]){
		readkey();
		if(key[KEY_RIGHT]) {
			x+=60;
			if(x>lim_der) x-=60;
			stop_sample(selection);
			play_sample(selection, 255, 0, 2000, 0);
		}
		if(key[KEY_LEFT]) {
			x-=60;
			if(x<lim_izq) x+=60;
			stop_sample(selection);
			play_sample(selection, 255, 0, 2000, 0);
		}
		if(key[KEY_UP]) {
			y-=60;
			if(y<lim_sup) y+=60;
			stop_sample(selection);
			play_sample(selection, 255, 0, 2000, 0);
		}		
		if(key[KEY_DOWN]) {
			y+=60;
			if(y>lim_inf) y-=60;
			stop_sample(selection);
			play_sample(selection, 255, 0, 2000, 0);
		}
		if(key[KEY_R]){
			vertical*=-1;
			x=75,y=75;
			if(vertical>0){
				lim_sup=45, lim_inf=705-a, lim_der=645, lim_izq=45;
			} else {
				lim_sup=45, lim_inf=645, lim_der=705-a, lim_izq=45;
			}
		}
		blit(tablero,fondo,0,0,45,45,660,660);
		if(vertical<0)
			blit(barco,fondo,0,0,x,y,a,60);
		else
			blit(barcov,fondo,0,0,x,y,60,a);
		blit(fondo,screen,0,0,0,0,1200,750);
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
				for(int i=y_tab, j=0; j<a/60; i++, j++)
					tab[i][x_tab]=a/60;
			} else {
				allegro_message("¡No puedes ponerlo aquí!");
				(*rest)++; (*num)++;
			}
		} else {
			for(int i=x_tab, j=0; j<a/60; i++, j++)
				if(tab[y_tab][i])
					band=0;
			if(band){
				for(int i=x_tab, j=0; j<a/60; i++, j++)
					tab[y_tab][i]=a/60;
			} else {
				allegro_message("¡No puedes ponerlo aquí!");
				(*rest)++; (*num)++;
			}
		}
		for(int i=0; i<10; i++){
			for(int j=0; j<10; j++)
				printf("[%i]", tab[i][j]);
			printf("\n");
		}
		printf("\n");
	}
	imprime_barco(tab);
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
	cursor=load_bitmap("dis/cursor.bmp",NULL);  //imagen del cursor
	menu0=load_bitmap("menu/menu-0.bmp",NULL);    //imagenes del menu
	menu1=load_bitmap("menu/menu-1.bmp",NULL);
	menu2=load_bitmap("menu/menu-2.bmp",NULL);
	menu3=load_bitmap("menu/menu-3.bmp",NULL);
	menu4=load_bitmap("menu/menu-4.bmp",NULL);
	do {
		if(mouse_x>415 && mouse_x<795 && mouse_y>365 && mouse_y<400) {         
			blit(menu1,fondo,0,0,0,0,1200,750);                                //si el raton esta entre las coordenas anteriores se imprime menu1 en fondo
			if(mouse_b & 1) opcion=1;                                         //si hace clic izquierdo opcion=1
		} else if(mouse_x>390 && mouse_x<810 && mouse_y>440 && mouse_y<475) { 
			blit(menu2,fondo,0,0,0,0,1200,750);
			if(mouse_b & 1) opcion=2;
		} else if(mouse_x>495 && mouse_x<700 && mouse_y>515 && mouse_y<550) {
			blit(menu3,fondo,0,0,0,0,1200,750);
			if(mouse_b & 1) opcion=3;
		} else if(mouse_x>525 && mouse_x<665 && mouse_y>590 && mouse_y<620) {
			blit(menu4,fondo,0,0,0,0,1200,750);
			if(mouse_b & 1) opcion=4;
		} else 	blit(menu0,fondo,0,0,0,0,1200,750);
		masked_blit(cursor,fondo,0,0,mouse_x,mouse_y,27,27);                  //imprime el cursor, respetando su transparencia en fondo
		blit(fondo,screen,0,0,0,0,1200,750);                                 //imprime el fondo en pantalla
	} while(opcion==0);
	delete fondo;
	delete cursor;
	delete menu0;                                                           //elimina los bitmaps
	delete menu1;
	delete menu2;
	delete menu3;
	delete menu4;
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
}
