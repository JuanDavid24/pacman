#include <allegro.h>
#include <cstdio>
#define MAXFILAS 21
#define MAXCOLS 30


/*Tareas:

-bug: pac muere cuando el fantasama lo sigue de cerca
-sonido
-clase pacman
-2 players
-you lose √
-vidas system
-mejora IA
-you win
-mejora resolucion

*/

BITMAP *buffer;
BITMAP *roca;
BITMAP *pacbmp;
BITMAP *pacman;
BITMAP *comida;
BITMAP *muertebmp;
FONT *retryFont;

int dir=0;
int px=30*10;
int py=30*11;
int px_anterior;
int py_anterior;

char mapa[MAXFILAS][MAXCOLS]={
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  "Xo o ! o o oXXXXXo o o ! o oX",
  "XoXXXoXXXXXoXXXXXoXXXXXoXXXoX",
  "XoXXXoXXXXXoXXXXXoXXXXXoXXXoX",
  " t o ! oto o o o o oto ! o t ",
  "XoXXXoXX XXXXXXXXXXX XXoXXXoX",
  "Xt o tXXo o oXXXo o oXXt o tX",
  "XoXXXoXXXXXX XXX XXXXXXoXXXoX",
  "XoXXXoXX    t   t    XXoXXXoX",
  " t o tXX XXXXXXXXXXX XXo o o ",
  "XoXXXoXX XXXXXXXXXXX XXoXXXoX",
  "XoXXXoXX    t   t    XXoXXXoX",
  "XoXXXoXXXXXX XXX XXXXXXoXXXoX",
  " ! o !XXo o oXXXo o oXX! o ! ",
  "XoXXXoXXoXXXXXXXXXXXoXXoXXXoX",
  "XoXXXto !to o o o ot! o!XXXoX",
  "XoXXXoXXX XXXXXXXXX XXXoXXXoX",
  "XoXXXoXXXto o o o otXXXoXXXoX",
  "Xo o ! o oXXXXXXXXXo o ! o oX",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
};

void you_lose(bool &game_over, BITMAP *screen){
        textout_centre_ex(screen, retryFont, "GAME OVER",
                        SCREEN_W / 2, SCREEN_H /3,
                        makecol(255, 0, 0), makecol(0, 0, 0));
        textout_centre_ex(screen, retryFont, "press 'R' key to retry or 'ESC' to exit",
                        SCREEN_W / 2, SCREEN_H *2/3,
                        makecol(255, 0, 0), makecol(0, 0, 0));


    while(!key[KEY_ESC] && !key[KEY_R]){
            if (key [KEY_R])
                game_over = false;
                else if (key[KEY_ESC])
                    game_over = true;
              }

}

bool hay_bolitas;
bool game_over = false;

void dibujar_mapa(){
    int fila,col;
    hay_bolitas=false;
    for(fila=0;fila<MAXFILAS;fila++) {
        for (col=0;col<MAXCOLS;col++){
            if(mapa[fila][col]=='X'){
                draw_sprite(buffer, roca, col*30, fila*30);
            }
            else if(mapa[fila][col]=='o' || mapa[fila][col]=='!'){
                draw_sprite(buffer, comida, col*30, fila*30);
                hay_bolitas=true;
                if(px/30==col && py/30==fila){
                    mapa[fila][col] =' ';
                }
            }
        }
    }
}
void pantalla (){  //imprime todo en la pantalla
    blit(buffer, screen, 0, 0, 0, 0, 880, 600);
}
void dibujar_personaje(){ //Dibuja a pacman
    blit(pacbmp,pacman,dir*33,0,0,0,33,33);
    draw_sprite(buffer,pacman,px,py);
}

class fantasma{
    BITMAP *enemigobmp, *enemigo;
    int fx, fy, fdir;

public:
    fantasma(int x, int y); //constructor
    void mover_fantasma();
    void dibujar_fantasma() const;
    void choque_pacman();
};

fantasma::fantasma(int x, int y){
    fx = x*30;
    fy = y*30;
    fdir = rand()%4;
    enemigobmp = load_bitmap("enemigo.bmp",NULL);
    enemigo = create_bitmap(30,30);
}

void fantasma::dibujar_fantasma() const{
    blit(enemigobmp,enemigo,0,0,0,0,30,30);
    draw_sprite(buffer,enemigo,fx,fy);
}

void fantasma::choque_pacman(){
    if ((px == fx && py == fy) || (px_anterior == fx && py_anterior == fy)){

        for (int j=0; j<5 ; j++){
            clear (pacman);
            clear (buffer);
            dibujar_mapa();

            blit(muertebmp, pacman, j*33,0,0,0,33,33);
            draw_sprite(buffer,pacman,px,py);

            pantalla();
            rest(80);
        }

        px=30*10;   //posicion inicial de pacman
        py=30*11;
        px_anterior=px; //reset de las variables x e y anteriores
        py_anterior=py;
        you_lose(game_over, screen);
    }
}

void fantasma::mover_fantasma(){
    dibujar_fantasma();
    choque_pacman();

    if (fdir==0){
        if (mapa[fy/30][(fx-30)/30] != 'X') fx-=30;
        else fdir = rand()%4;
        }
    if (fdir==1){
        if (mapa[fy/30][(fx+30)/30] != 'X') fx+=30;
        else fdir = rand()%4;
        }
    if (fdir==2){
        if (mapa[(fy-30)/30][fx/30] != 'X') fy-=30;
        else fdir = rand()%4;
        }
    if (fdir==3){
        if (mapa[(fy+30)/30][fx/30] != 'X') fy+=30;
        else fdir = rand()%4;
        }
    if (mapa[fy/30][fx/30]=='t' || mapa[fy/30][fx/30]=='!') fdir = rand()%4; //En los casos en los que hay mas de un camino posible

    if (fx <= -30) fx=29*30; //rutina atajo
        else if (fx >= 29*30) fx=-30;

}

int main() {
    allegro_init();
	install_keyboard();
	set_color_depth(32);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED,880,600,0,0);

	buffer = create_bitmap(880,600);
    roca = load_bitmap("roca.bmp",NULL);
    pacbmp = load_bitmap("pacman.bmp",NULL);
    pacman = create_bitmap (33,33);
    comida = load_bitmap ("Comida.bmp",NULL);
    muertebmp = load_bitmap("muerte.bmp", NULL);
    retryFont = load_font("pacFont.pcx", NULL, NULL);

    fantasma A = fantasma(5,14);
    fantasma B = fantasma(5,3);
    fantasma C = fantasma(25,1);
    fantasma D = fantasma(25,18);

    while(!key[KEY_ESC]&&!game_over){

        dibujar_mapa();
        pantalla();

        px_anterior=px;
        py_anterior=py;

        //rutina mover pacman-----------------------------
        if (key[KEY_A]) dir=0;
        else if (key[KEY_D]) dir=1;
        else if (key[KEY_W]) dir=2;
        else if (key[KEY_S]) dir=3;

        if (dir==0){  //Incremento en x e y para cada direccion haciendo que se mueva.
            if(mapa[py/30][(px-30)/30]!= 'X')
                px -= 30;
            else dir=4;
            }

        if (dir==1){
            if(mapa[py/30][(px+30)/30]!= 'X')
                px += 30;
            else dir=4;
            }

        if (dir==2){
            if(mapa[py/30-1][px/30]!= 'X')
                py -= 30;
            else dir=4;
            }

        if (dir==3){
            if(mapa[py/30+1][px/30]!= 'X')
                py += 30;
            else dir=4;
           }

        if (px <= -30) px=29*30;
        else if (px >= 29*30) px=-30;

        clear(buffer);
        dibujar_mapa();
        dibujar_personaje();
        A.mover_fantasma();
        B.mover_fantasma();
        C.mover_fantasma();
        D.mover_fantasma();

        pantalla();
        rest(90);

        //Animacion pacman
        blit(pacbmp,pacman,4*33,0,0,0,33,33);
        draw_sprite(buffer,pacman,px,py);
        pantalla();
        rest(70);

        if (hay_bolitas==false) game_over=true;
    }
    return(0);
}
END_OF_MAIN();
