#include "config.h"
#include "delay.h"
#include "pic18f4520.h"
#include "lcd.h"
#include "teclado.h"
#include "func.h"


typedef enum{
    false, 
    true
} logico;

void main(void) {
    unsigned char i;
    int tempo = 0, n_pedidos = 0, total = 1;
    int aux;
    logico controlador = false;
    inicia();
    lcd_init();
    TRISB = 0xF8;    

    for(;;){
        controlador = false;
        aux = espera(tempo%3);
        if (aux == 1) controlador = true;
        if(controlador==true){
            teclas();
            n_pedidos++;
        }
        if (tempo == 1){
            tempo = 0;
            if(n_pedidos>0){
                senha(total++);
                n_pedidos--;
                controlador = false;
            }
        }
        tempo++;
    }
    return;
}
