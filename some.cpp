#include <allegro.h>
#include<time.h>
#include<stdlib.h>

void init();
void deinit();
int menu();
int ** reservaMemoria();
int Posiciona(int **Tab);

int main() {
	int op=1;
	int **Tab1; Tab1=reservaMemoria();
	int **Tab2; Tab2=reservaMemoria();
	init();
	while (op!=0) {
		switch(menu()){
			case 1:
				Posiciona(Tab1);
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

int Posiciona(int **Tab){
	int i;
	BITMAP *fondo=load_bitmap("dis\\pantalla.bmp",NULL);
	BITMAP *tablero;
	BITMAP *barco2c;
	BITMAP *barco3c;
	BITMAP *barco4c;
	BITMAP *barco5c;
	srand(time(NULL));
	switch(rand()%4){
		case 0:
			tablero=load_bitmap("dis\\tableros\\fondo-0.bmp",NULL);
			barco2c=load_bitmap("dis\\barcos\\barco2c-0.bmp",NULL);
			barco3c=load_bitmap("dis\\barcos\\barco3c-0.bmp",NULL);
			barco4c=load_bitmap("dis\\barcos\\barco4c-0.bmp",NULL);
			barco5c=load_bitmap("dis\\barcos\\barco5c-0.bmp",NULL);
			break;
		case 1:
			tablero=load_bitmap("dis\\tableros\\fondo-1.bmp",NULL);
			barco2c=load_bitmap("dis\\barcos\\barco2c-1.bmp",NULL);
			barco3c=load_bitmap("dis\\barcos\\barco3c-1.bmp",NULL);
			barco4c=load_bitmap("dis\\barcos\\barco4c-1.bmp",NULL);
			barco5c=load_bitmap("dis\\barcos\\barco5c-1.bmp",NULL);
			break;
		case 2:
			tablero=load_bitmap("dis\\tableros\\fondo-2.bmp",NULL);
			barco2c=load_bitmap("dis\\barcos\\barco2c-2.bmp",NULL);
			barco3c=load_bitmap("dis\\barcos\\barco3c-2.bmp",NULL);
			barco4c=load_bitmap("dis\\barcos\\barco4c-2.bmp",NULL);
			barco5c=load_bitmap("dis\\barcos\\barco5c-2.bmp",NULL);
			break;
		case 3:
			tablero=load_bitmap("dis\\tableros\\fondo-3.bmp",NULL);
			barco2c=load_bitmap("dis\\barcos\\barco2c-3.bmp",NULL);
			barco3c=load_bitmap("dis\\barcos\\barco3c-3.bmp",NULL);
			barco4c=load_bitmap("dis\\barcos\\barco4c-3.bmp",NULL);
			barco5c=load_bitmap("dis\\barcos\\barco5c-3.bmp",NULL);
			break;
	}
	
	while(!key[KEY_ESC]){
		blit(tablero,fondo,0,0,50,50,660,660);
		blit(fondo,screen,0,0,0,0,1200,750);
		blit(barco2c,fondo,0,0,800,200,120,60);
		blit(barco3c,fondo,0,0,1000,200,180,60);
		blit(barco4c,fondo,0,0,800,400,240,60);
		blit(barco5c,fondo,0,0,800,600,300,60);
	}
	
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
	BITMAP *fondo=create_bitmap(1200,750);           
	BITMAP *cursor=load_bitmap("dis/cursor.bmp",NULL);  //imagen del cursor
	BITMAP *menu0=load_bitmap("menu/menu-0.bmp",NULL);    //imagenes del menu
	BITMAP *menu1=load_bitmap("menu/menu-1.bmp",NULL);
	BITMAP *menu2=load_bitmap("menu/menu-2.bmp",NULL);
	BITMAP *menu3=load_bitmap("menu/menu-3.bmp",NULL);
	BITMAP *menu4=load_bitmap("menu/menu-4.bmp",NULL);
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
	
}

void deinit() {
	clear_keybuf();
	
}
