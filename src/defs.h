#ifndef SRC_DEFS_H_
#define SRC_DEFS_H_


#define WINDOW_SIZE_HORIZONTAL 800
#define WINDOW_SIZE_VERTICAL 600


#define JUGADOR_SIZE_HORIZONTAL 230
#define JUGADOR_SIZE_VERTICAL 280

#define JUGADOR_POSICION_HORIZONTAL_INICIAL 100
#define JUGADOR_POSICION_VERTICAL_INICIAL 600-250-50

//Para el source
#define ANCHO_CAPA_PIXELES 2250
//Para el destino
#define ANCHO_CAPA_PIXELES_ESCALADA 2250 *2.05

#define MARGEN_IZQUIERDO 50
#define MARGEN_DERECHO 300
#define MARGEN_PISO_ARRIBA 390
#define MARGEN_PISO_ABAJO 270

#define VELOCIDAD_CODY 6
#define VELOCIDAD_SALTO_INICIAL 45
#define ALTURA_MAXIMA 200 //Lo maximo que puede saltar cody
#define GRAVEDAD -10
#define DELTA_T 0.16

#define SALTO_VERTICAL 0
#define SALTO_DERECHA 1
#define SALTO_IZQUIERDA 2

#define RIGHT 0
#define LEFT 1
#define UP 2
#define DOWN 3
#define STAND 4

#define ACCION_PARADO 0
#define ACCION_CAMINAR 1
#define ACCION_SALTO 2
#define ACCION_GOLPEAR 3
#define ACCION_SALTO_PATADA 4
#define ACCION_AGACHADO 5
#define ACCION_SALTO_VERTICAL 6

#define FRAME_DELAY 1000/60

#define MAX_CANT_ANIMADOS 25

#define MAX_CANT_ELEMENTOS 100

#define MAX_CLIENTES 4

#define MAX_USER_LEN 30

#define MAX_PASS_LEN 30


#endif /* SRC_DEFS_H_ */
