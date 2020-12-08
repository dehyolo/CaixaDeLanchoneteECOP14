#include "lcd.h"
#include "teclado.h"
#include "pic18f4520.h"
#include "delay.h"

char display[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
unsigned int atrasoMin = 18;
unsigned int atrasoMed = 145;
unsigned int atrasoMax = 980;

void inicia(){
    ADCON1 = 0x06;
    TRISD = 0x00;
    TRISE = 0x00;
    TRISB = 0x01;
    TRISA = 0x00;
}

void cursor(){
    unsigned char i;
    for (i = 0; i < 16; i++) {
        atraso_ms(atrasoMed);
        lcd_cmd(0x10);
    }
}

void desloca(){
    unsigned char i;
    for (i = 0; i < 50; i++) {
        atraso_ms(atrasoMed);
        lcd_cmd(0x18);
    }
    atraso_ms(atrasoMax);
}

void apagaL1(){
    unsigned char i;
    lcd_cmd(L_L1);

    for (i = 0; i < 16; i++) {
        lcd_dat(i % 2);
        atraso_ms(atrasoMed);
    }
    atraso_ms(atrasoMax);
}

void apagaL2(){
    unsigned char i;
    lcd_cmd(L_L2);

    for (i = 0; i < 20; i++) {
        lcd_dat(i % 2);
        atraso_ms(atrasoMed);
    }
    atraso_ms(atrasoMax);
}

void escreveL1(char *frase){
    cursor();
    lcd_cmd(L_L1);
    lcd_str(frase);
}

void escreveL2(char *frase){
    cursor();
    lcd_cmd(L_L2);
    lcd_str(frase);
}

void senha(int pedido){
    char aux[4];
    int tempo, j;
    PORTD = 0x00;
    PORTA = 0x00;
    aux[0] = pedido/1000;
    aux[1] = (pedido % 1000)/100;
    aux[2] = (pedido % 100)/10;
    aux[3] = pedido % 10;
    for(tempo=0;tempo<1800;tempo++){
        PORTA = 0x04;
        PORTD = display[aux[0]];
        atraso_ms(1);
        
        PORTA = 0x08;
        PORTD = display[aux[1]];
        atraso_ms(1);        
        
        PORTA = 0x10;
        PORTD = display[aux[2]];
        atraso_ms(1);
        
        PORTA = 0x20;
        PORTD = display[aux[3]];
        atraso_ms(1);
    }
    PORTA = 0x00;
}

void apagaDigitado(){
    unsigned char i;
    unsigned char tmp;
    lcd_cmd(L_L2);
    for (i = 0; i < 16; i++) {
        lcd_dat(i % 2);
        atraso_ms(atrasoMin);
    }
    lcd_cmd(L_L1);
    for (i = 0; i < 16; i++) {
        lcd_dat(i % 2);
        atraso_ms(atrasoMin);
    }
    lcd_cmd(L_CLR);
}

void teclas(){
    unsigned char i;
    unsigned char tmp;
    unsigned char j=0;
    ADCON1 = 0x06;
    TRISB = 0x01;
    TRISD = 0x00;
    TRISE = 0x00;

    char op = 0;
    //Teclado numérico
    TRISB = 0xF8;
    while (1) {
        switch (op) {
        case 0:
            lcd_cmd(L_CLR);
            escreveL1("Lanche:");
            break;
        case 1:
            lcd_cmd(L_CLR);
            escreveL1("Bebida:");
            j=1;
            break;
        default:
            break;
        }
        TRISD = 0x0F;
        tmp = tc_tecla(0) + 0x30;
        TRISD = 0x00;
        atraso_ms(atrasoMax);
        if(tmp == 0x3A) apagaDigitado();
        if(tmp == 0x3C && j==1){
            apagaL1();
            cursor();
            escreveL1(" Enviando...");
            atraso_ms(atrasoMax);
            atraso_ms(atrasoMax);
            apagaL1();
            apagaL2();
            break;
        }
        lcd_dat(tmp);
        while(tmp!=0x3C){  
            TRISD = 0x0F;
            tmp = tc_tecla(0) + 0x30;
            TRISD = 0x00;
            lcd_dat(tmp);

         }
        lcd_cmd(0x18);  
        op++;
    }
}

void noticias(int k){
    cursor();
    char n_um[50] =   "PROMOCAO! X-BACON POR R$12.00";
    char n_dois[50] = "COCA-COLA 600ML: R$6.00";
    char n_tres[50] = "TEMPERATURA: 18 C";
    
    switch (k){
        case 0:
            lcd_cmd(L_CLR);
            escreveL1(n_um);
            escreveL2("Aperte 1 para fazer o pedido.");
            desloca();
            lcd_cmd(L_CLR);
            break;
        case 1:
            lcd_cmd(L_CLR);
            escreveL1(n_dois);
            escreveL2("Aperte 1 para fazer o pedido.");
            desloca();
            lcd_cmd(L_CLR);
            break;
        case 2:
            lcd_cmd(L_CLR);
            escreveL1(n_tres);
            escreveL2("Aperte 1 para fazer o pedido.");
            desloca();
            lcd_cmd(L_CLR);
            break;
        default:
            break;
            
    }
}

int pedido(){
    unsigned char i;
    unsigned char tmp;
    ADCON1 = 0x06;
    TRISB = 0x01;
    TRISD = 0x00;
    TRISE = 0x00;
    escreveL1("Deseja pedir?");
    escreveL2("1 - Sim");
    //Teclado numérico
    TRISB = 0xF8;;
    while (1) {
        TRISD = 0x0F;
        tmp = tc_tecla1(0, 0) + 0x30;
        TRISD = 0x00;
        if(tmp == 0x31){
            return 1;
        }else{
            return 0;
        }
    }
}

int espera(int cont){
    int resp;
    noticias(cont);
    resp = pedido();
    return resp;
}
