#include <allegro.h>
#include <cstdio>
#define MAXFILAS 21
#define MAXCOLS 30

/*Mejoras:

-clase pacman
-2 players
-vidas system
-mejora IA
-mejora resolucion mapa

Hecho:
-you lose √
-you win √
-sonido √
-bug: pac muere cuando el fantasama lo sigue de cerca √

*/

BITMAP *buffer;
BITMAP *roca;
BITMAP *pacbmp;
BITMAP *pacman;
BITMAP *comida;
BITMAP *muertebmp;
FONT *retryFont;
MIDI *musica;
SAMPLE *camina_wav;
SAMPLE *muerte_wav;
SAMPLE *bolita_wav;

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

bool hay_bolitas;
bool game_over = false;

void derrota(bool &game_over){
        textout_centre_ex(screen, retryFont, "PERDISTE :(",
                        SCREEN_W / 2, SCREEN_H /3,
                        makecol(255, 0, 0), makecol(0, 0, 0));
        textout_centre_ex(screen, retryFont, "Pulsa 'R' para reintentar o 'ESC' para salir",
                        SCREEN_W / 2, SCREEN_H *2/3,
                        makecol(255, 0, 0), makecol(0, 0, 0));

    while(!key[KEY_ESC] && !key[KEY_R]){
            if (key [KEY_R])
                game_over = false;
                else if (key[KEY_ESC])
                    game_over = true;
              }
}

void victoria(){
        textout_centre_ex(screen, retryFont, "Ganaste! :D",
                        SCREEN_W / 2, SCREEN_H /3,
                        makecol(255, 0, 0), makecol(0, 0, 0));
        textout_centre_ex(screen, retryFont, "Presiona 'ESC' para salir",
                        SCREEN_W / 2, SCREEN_H *2/3,
                        makecol(255, 0, 0), makecol(0, 0, 0));

        while(!key[KEY_ESC]){}

}

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
                    play_sample(bolita_wav,150,127,1000,0);
                    mapa[fila][col] =' ';
                }
            }
        }
    }
}
void pantalla (){  //imprime todo en la pantalla
    blit(buffer, screen, 0, 0, 0, 0, 880, 600);
}
void dibujar_pacman(){ //Dibuja a pacman
    blit(pacbmp,pacman,(dir-1)*33,0,0,0,33,33);
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
    fdir = (rand()%4)+1;
    enemigobmp = load_bitmap("./res/img/enemigo.bmp",NULL);
    enemigo = create_bitmap(30,30);
}

void fantasma::dibujar_fantasma() const{
    blit(enemigobmp,enemigo,0,0,0,0,30,30);
    draw_sprite(buffer,enemigo,fx,fy);
}

void fantasma::choque_pacman(){
    if ((px == fx && py == fy) ||
        (px_anterior == fx && py_anterior == fy && fdir==dir)){

        play_sample(muerte_wav,255,127,1000,0);

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
        derrota(game_over);
    }
}

void fantasma::mover_fantasma(){
    dibujar_fantasma();
    choque_pacman();

    if (fdir==2){
        if (mapa[fy/30][(fx-30)/30] != 'X') fx-=30; //izq
        else fdir = (rand()%4)+1;
        }
    if (fdir==1){
        if (mapa[fy/30][(fx+30)/30] != 'X') fx+=30; //der
        else fdir = (rand()%4)+1;
        }
    if (fdir==4){
        if (mapa[(fy-30)/30][fx/30] != 'X') fy-=30; //arriba
        else fdir = (rand()%4)+1;
        }
    if (fdir==3){
        if (mapa[(fy+30)/30][fx/30] != 'X') fy+=30; //abajo
        else fdir = (rand()%4)+1;
        }
    if (mapa[fy/30][fx/30]=='t' || mapa[fy/30][fx/30]=='!') fdir = (rand()%4)+1; //En los casos en los que hay mas de un camino posible

    if (fx <= -30) fx=29*30; //rutina atajo
        else if (fx >= 29*30) fx=-30;

}

void mover_pacman(){
        //rutina mover pacman-----------------------------
        if (key[KEY_A]) dir=1;
        else if (key[KEY_D]) dir=2;
        else if (key[KEY_W]) dir=3;
        else if (key[KEY_S]) dir=4;

        if (dir==1){  //Incremento en x e y para cada direccion haciendo que se mueva.
            if(mapa[py/30][(px-30)/30]!= 'X')
                px -= 30;
            else dir=0;
            }

        if (dir==2){
            if(mapa[py/30][(px+30)/30]!= 'X')
                px += 30;
            else dir=0;
            }

        if (dir==3){
            if(mapa[py/30-1][px/30]!= 'X')
                py -= 30;
            else dir=0;
            }

        if (dir==4){
            if(mapa[py/30+1][px/30]!= 'X')
                py += 30;
            else dir=0;
           }

        //Atajos
        if (px <= -30) px=29*30;
        else if (px >= 29*30) px=-30;
}

int main() {
    allegro_init();
	install_keyboard();
	set_color_depth(32);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED,880,600,0,0);

	//inicializa audio allegro
	 if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) != 0) {
       allegro_message("Error: inicializando sistema de sonido\n%s\n", allegro_error);
       return 1;
    }
    set_volume(100, 100);

	buffer = create_bitmap(880,600);
    roca = load_bitmap("./res/img/roca.bmp",NULL);
    pacbmp = load_bitmap("./res/img/pacman.bmp",NULL);
    pacman = create_bitmap (33,33);
    comida = load_bitmap ("./res/img/comida.bmp",NULL);
    muertebmp = load_bitmap("./res/img/muerte.bmp", NULL);
    retryFont = load_font("./res/font/pacFont.pcx", NULL, NULL);
    camina_wav = load_wav("./res/sound/walk.wav");
    bolita_wav = load_wav("./res/sound/ring.wav");
    muerte_wav = load_wav("./res/sound/death.wav");
    musica = load_midi("./res/sound/casino-night-zone.mid");

    fantasma A = fantasma(5,14);
    fantasma B = fantasma(5,3);
    fantasma C = fantasma(25,1);
    fantasma D = fantasma(25,18);

    play_midi(musica,1);

    while(!key[KEY_ESC] && !game_over){

        if (dir!=4) play_sample(camina_wav,100,127,1000,0); // sonido caminar pacman

        dibujar_mapa();
        pantalla();

        px_anterior=px; // guardo posicion previa de pacman en X e Y
        py_anterior=py;

        mover_pacman();

        clear(buffer);
        dibujar_mapa();
        dibujar_pacman();
        A.mover_fantasma();
        B.mover_fantasma();
        C.mover_fantasma();
        D.mover_fantasma();

        pantalla();
        rest(140);

        //Animacion pacman
        blit(pacbmp,pacman,4*33,0,0,0,33,33);
        draw_sprite(buffer,pacman,px,py);
        pantalla();
        rest(100);

        if (hay_bolitas==false) victoria();
    }
    return(0);
}
END_OF_MAIN();
