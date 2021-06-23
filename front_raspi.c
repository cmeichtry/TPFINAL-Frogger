// INCLUDES
#include "globalstuff.h"

//DEFINES LOCALES
#define THRESHOLD 40

// FUNCTION PROTOTYPES
int init_raspi (void);
action_t get_input_raspi (void);            //devuelve la accion realizada
void output_world_raspi (mundo_t * mundo);  //muestra el mundo en un momento dado en el display
void output_frog_raspi (rana_t * rana);     //muestra la rana parpadeante en el display
action_t output_initmenu_raspi (void);      //muestra el menu de inicio en el display
action_t output_gamepaused_raspi (void);    //muestra el menu de pausa en el display
//action_t output_topscores_raspi (void);     //muestra los top scores en el display (FALTA!!!)
void output_dead_raspi (void);              //muestra cuando muere la rana en el display
void output_gameover_raspi (void);          //muestra la imagen de game over
dcoord_t get_disp_coord (mundo_t * disp);   //trae la coordenada del display donde esta el jugador

// FUNCION DEFINITIONS

/********************************* INIT RPI **************************************/
int init_raspi (void) {

    disp_init();    //inicializa el display
    disp_clear();   //limpia todo el display
    joy_init();     //inicializo el joystick

    return 0;
}

/********************************* GET INPUT **************************************/
action_t get_input_raspi (void){ //devuelve la accion realizada, action_t es una estructura que guarda acciones
    
    jcoord_t myCoords;
    action_t accion = NONE;

    //primero actualizo las coordenadas medidas
    joy_update();
    //luego las guardo en myCoords
    myCoords = joy_get_coord();

    if (joy_get_switch()== J_PRESS) {
        accion = PLAY;
    }
    else if (joy_get_switch()== J_NOPRESS) {

        if ( (myCoords.y < 64) && (myCoords.y > -64) && (myCoords.x > 64) ){
            accion = RIGHT;
        }
        else if ( (myCoords.y < 64) && (myCoords.y > -64) && (myCoords.x < -64) ){
            accion = LEFT;
        }
        else if ( (myCoords.x < 64) && (myCoords.x > -64) && (myCoords.y > 64) ){
            accion = UP;
        }
        else if ( (myCoords.x < 64) && (myCoords.x > -64) && (myCoords.y < -64) ){
            accion = DOWN;
        }
        else {
            accion = NONE;
        }
    }

    return  accion;
}

/********************************* OUTPUT WORLD **************************************/
void output_world_raspi (mundo_t * mundo){  //muestra el mundo en un momento dado en el display
    
    dcoord_t myPoint = {};
    int i,j;

    for (i=0, myPoint.y = DISP_MIN ; i<CANTFILS; i++, myPoint.y++){
        for (j=0, myPoint.x = DISP_MIN ; j<CANTCOLS; j++, myPoint.x++){
            switch ((*mundo)[i][j]){
                case STREET:
                case WATER:
                    disp_write(myPoint, D_OFF);
                    break;
                case CAR:
                case TRUCK:
                case SAFE:
                case LOG:
                case TORTOISE:
                case DEAD:
                    disp_write(myPoint, D_ON);
                    break;
            }
        }
    }
    disp_update();
}

/********************************* OUTPUT FROG **************************************/
void output_frog_raspi (rana_t * rana){ //se debe invocar cada una cantidad establecida de tiempo, menor que la de actualizacion del display
    if (rana->estado == VIVA){
        if (rana->encendida == ENCENDIDA){
            disp_write(rana->coords, D_OFF);
        }
        else {
            disp_write(rana->coords, D_ON);
        }
        disp_update();
    }
    else {
        output_dead_raspi();
    }
}

/********************************* MENU PAUSA **************************************/
action_t output_gamepaused_raspi (void){  //muestra el menu de pausa en el display y devuelve que quizo hacer el jugador
    mundo_t gamepausedrpi = {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,1,1,0,1,0,0,1,1,1,0,1,0,1,0,0},
        {0,1,1,0,1,0,0,1,1,1,0,1,1,1,0,0},
        {0,1,0,0,1,0,0,1,0,1,0,0,1,0,0,0},
        {0,1,0,0,1,1,0,1,0,1,0,0,1,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,1,1,0,0,0,0,0,1,1,1,0,0,0},
        {0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,0},
        {0,0,0,1,1,0,0,0,0,0,1,1,1,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,1,1,0,1,0,0,1,0,1,0,1,1,1,0,0},
        {0,1,0,0,0,1,1,0,0,1,0,0,1,0,0,0},
        {0,1,1,0,0,1,1,0,0,1,0,0,1,0,0,0},
        {0,1,0,0,0,1,1,0,0,1,0,0,1,0,0,0},
        {0,1,1,0,1,0,0,1,0,1,0,0,1,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    };
    dcoord_t myPoint = {};
    action_t accion = NONE;
    dcoord_t click = {0,0};
    int i,j, repeat=0;

    for (i=0, myPoint.y = DISP_MIN ; i<CANTFILS; i++, myPoint.y++){
        for (j=0, myPoint.x = DISP_MIN ; j<CANTCOLS; j++, myPoint.x++){
            if (gamepausedrpi[i][j]==0){
                disp_write(myPoint, D_OFF);
            } 
            else if (gamepausedrpi[i][j]==1){
                disp_write(myPoint, D_ON);
            }
        }
    }
    disp_update();

    do{
        click = get_disp_coord (&gamepausedrpi);
        if ((click.x >= 3) && (click.x <= 5) && (click.y >= 6) && (click.y <= 8)){
            accion = PLAY;
            repeat = 1;
        }
        else if ((click.x >= 10) && (click.x <= 12) && (click.y >= 6) && (click.y <= 8)){
            accion = EXIT;
            repeat = 1;
        }
        else {
            repeat=0;
        }
        } while (repeat==0);

    return accion;
}

/********************************* MENU INICIO **************************************/
action_t output_initmenu_raspi (void){ //muestra el menu de inicio en el display y devuelve que quizo hacer el jugador

    mundo_t initmenurpi = {
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,1,1,1,0,0,1,1,1,0,0,0,1},
        {1,0,0,0,1,0,1,0,0,1,0,1,0,0,0,1},
        {1,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1},
        {1,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1},
        {1,0,0,0,1,1,1,0,0,1,1,1,0,0,0,1},
        {1,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,1,1,0,0,1,0,1,0,0,0,0,1},
        {1,0,0,0,1,1,1,0,0,1,0,0,0,0,0,1},
        {1,0,0,0,1,1,0,0,1,0,1,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,1,1,1,0,1,1,0,1,1,0,0,0,1},
        {1,0,0,0,1,0,0,1,1,0,1,1,0,0,0,1},
        {1,0,0,0,1,0,0,1,1,0,1,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}
    };
    dcoord_t myPoint = {};
    action_t accion = NONE;
    dcoord_t click = {0,0};
    int i,j,repeat=0;

    for (i=0, myPoint.y = DISP_MIN ; i<CANTFILS; i++, myPoint.y++){
        for (j=0, myPoint.x = DISP_MIN ; j<CANTCOLS; j++, myPoint.x++){
            if (initmenurpi[i][j]==0){
                disp_write(myPoint, D_OFF);
            } 
            else if (initmenurpi[i][j]==1){
                disp_write(myPoint, D_ON);
            }
        }
    }
    disp_update();
    
    do{
        click = get_disp_coord (&initmenurpi);
        if ((click.x >= 4) && (click.x <= 6) && (click.y >= 8) && (click.y <= 10)){
            accion = PLAY;
            repeat = 1;
        }
        else if ((click.x >= 8) && (click.x <= 10) && (click.y >= 8) && (click.y <= 10)){
            accion = EXIT;
            repeat = 1;
        }
        else if ((click.x >= 3) && (click.x <= 11) && (click.y >= 12) && (click.y <= 14)){
            accion = TOPSCORES;
            repeat = 1;
        }
        else {
            repeat=0;
        }
    } while (repeat==0);
    
    return accion;
}

/********************************* DEAD FROG **************************************/
void output_dead_raspi (void){
    mundo_t deadfrog = {
        {0,0,0,0,1,1,1,0,0,1,1,1,0,0,0,0},
        {0,0,0,0,1,0,1,1,1,1,0,1,0,0,0,0},
        {0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0},
        {0,0,0,0,1,1,1,0,0,1,1,1,0,0,0,0},
        {0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,1,1,0,0,1,1,0,1,1,1,0,1,1,0,0},
        {0,1,0,1,0,1,0,0,1,0,1,0,1,0,1,0},
        {0,1,0,1,0,1,1,0,1,1,1,0,1,0,1,0},
        {0,1,0,1,0,1,0,0,1,0,1,0,1,0,1,0},
        {0,1,1,0,0,1,1,0,1,0,1,0,1,1,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,1,0,1,1,0,1,1,0,0},
        {0,0,0,0,0,0,1,1,0,1,1,1,1,1,0,0},
        {0,0,0,1,1,0,0,1,0,0,1,1,1,0,0,0},
        {0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0}
    };
    dcoord_t myPoint = {};
    int i,j;

    for (i=0, myPoint.y = DISP_MIN ; i<CANTFILS; i++, myPoint.y++){
        for (j=0, myPoint.x = DISP_MIN ; j<CANTCOLS; j++, myPoint.x++){
            if (deadfrog[i][j]==0){
                disp_write(myPoint, D_OFF);
            } 
            else if (deadfrog[i][j]==1){
                disp_write(myPoint, D_ON);
            }
        }
    }
    disp_update();
}

/********************************* GAME OVER **************************************/
void output_gameover_raspi (void){
    mundo_t gameover = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {1,1,1,0,1,1,1,0,1,0,1,0,1,1,1,0},
        {1,0,0,0,1,0,1,0,1,1,1,0,1,0,0,0},
        {1,0,1,0,1,1,1,0,1,0,1,0,1,1,0,0},
        {1,0,1,0,1,0,1,0,1,0,1,0,1,0,0,0},
        {1,1,1,0,1,0,1,0,1,0,1,0,1,1,1,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {1,1,1,0,1,0,1,0,1,1,1,0,1,1,1,0},
        {1,0,1,0,1,0,1,0,1,0,0,0,1,0,1,0},
        {1,0,1,0,1,0,1,0,1,1,0,0,1,1,1,0},
        {1,0,1,0,1,0,1,0,1,0,0,0,1,1,0,0},
        {1,1,1,0,0,1,0,0,1,1,1,0,1,0,1,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };
    dcoord_t myPoint = {};
    int i,j;

    for (i=0, myPoint.y = DISP_MIN ; i<CANTFILS; i++, myPoint.y++){
        for (j=0, myPoint.x = DISP_MIN ; j<CANTCOLS; j++, myPoint.x++){
            if (gameover[i][j]==0){
                disp_write(myPoint, D_OFF);
            } 
            else if (gameover[i][j]==1){
                disp_write(myPoint, D_ON);
            }
        }
    }
    disp_update();
}

/********************************* GET DISPLAY COORD **************************************/
dcoord_t get_disp_coord (mundo_t * disp){

    dcoord_t pos = {DISP_MAX_X>>1 , DISP_MAX_Y>>1};	//pos es la posición actual, empieza en el centro de la matriz
	dcoord_t npos = pos;                            //npos es la próxima posición
	jcoord_t coord = {0,0};							//coordenadas medidas del joystick
    dlevel_t aux = D_OFF;                           //para guardar el estado del display original

	do
	{
		disp_update();	//Actualiza el display con el contenido del buffer
		joy_update();	//Mide las coordenadas del joystick
		coord = joy_get_coord();	//Guarda las coordenadas medidas
		
		//Establece la próxima posición según las coordenadas medidas
		if(coord.x > THRESHOLD && npos.x < DISP_MAX_X)
		{
			npos.x++;
		}
		if(coord.x < -THRESHOLD && npos.x > DISP_MIN)
		{
			npos.x--;
		}
		if(coord.y > THRESHOLD && npos.y > DISP_MIN)
		{
			npos.y--;
		}
		if(coord.y < -THRESHOLD && npos.y < DISP_MAX_Y)
		{
			npos.y++;
		}

        // trae lo que deberia haber en el led modificado del display matricial
        if ((*disp)[pos.y][pos.x]== 0){
            aux = D_OFF;
        }
        else if ((*disp)[pos.y][pos.x]== 1){
            aux = D_ON;
        }

        disp_write(pos,aux);    //reestablece lo que habia previamente en la posicion donde esta el mouse
		disp_write(npos,D_ON);	//enciende la posición nueva en el buffer
		pos = npos;				//actualiza la posición actual
		
	} while( joy_get_switch() == J_NOPRESS );	//termina si se presiona el switch

    return npos;
}