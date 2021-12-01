#define _XTAL_FREQ 4000000
#include <stdbool.h>
#include <xc.h>
#include "duckylib.h"


//-----LCD------
LCD lcd;
void LCD_Out ( char c ) {

 if ( c & 1 ){
 *(lcd.PORT) |= 1 << lcd.D4;
 }
 else {
 *(lcd.PORT) &= ~(1 << lcd.D4);
 }
 if ( c & 2 ) {
 *(lcd.PORT) |= 1 << lcd.D5;
 }
 else {
 *(lcd.PORT) &= ~(1 << lcd.D5);
 }
 if ( c & 4 ) {
 *(lcd.PORT) |= 1 << lcd.D6;
 }
 else {
 *(lcd.PORT) &= ~(1 << lcd.D6);
 }
 if ( c & 8 ) {
 *(lcd.PORT) |= 1 << lcd.D7;
 }
 else {
 *(lcd.PORT) &= ~(1 << lcd.D7);
 }
}
void LCD_Write ( unsigned char c ) {
 *(lcd.PORT) &= ~(1 << lcd.RS); // => RS = 0
 LCD_Out(c);
 *(lcd.PORT) |= 1 << lcd.EN; // => E = 1
 __delay_ms(4);
 *(lcd.PORT) &= ~(1 << lcd.EN); // => E = 0
}
bool LCD_Init ( LCD display ) {
 lcd = display;
 /*
 * TODO:
 * The function should clear only the appropriate bits, not the whole PORT
 *
 * TODO:
 * "#if defined" needs to support more microcontrollers that have PORTD and PORTE
 */
 if ( lcd.PORT == &PORTA ) {
 TRISA = 0x00;
 }
 else if ( lcd.PORT == &PORTB ) {
 TRISB = 0x00;
 }
 else if ( lcd.PORT == &PORTC ) {
 TRISC = 0x00;
 }
 // #if defined(_16F877) || defined(_16F877A)
 else if ( lcd.PORT == &PORTD ) {
 TRISD = 0x00;
 }
 else if ( lcd.PORT == &PORTE ) {
 TRISE = 0x00;
 }
 // #endif
 else {
 return false;
 }
 // Give some time to the LCD to start function properly
 __delay_ms(20);
 // Send reset signal to the LCD
 LCD_Write(0x03);
 __delay_ms(5);
 LCD_Write(0x03);
 __delay_ms(16);
 LCD_Write(0x03);
 // Specify the data lenght to 4 bits
 LCD_Write(0x02);
 // Set interface data length to 8 bits, number of display lines to 2 and font to 5x8 dots
 LCD_Cmd(0x28);
 // Set cursor to move from left to right
 LCD_Cmd(0x06);
 LCD_Display(true, false, false); // Turn on display and set cursor off
 LCD_Clear();

 return true;
}
void LCD_putc ( char c ) {
 *(lcd.PORT) |= 1 << lcd.RS; // => RS = 1
 LCD_Out((c & 0xF0) >> 4); //Data transfer
 *(lcd.PORT) |= 1 << lcd.EN;
 __delay_us(40);
 *(lcd.PORT) &= ~(1 << lcd.EN);
 LCD_Out(c & 0x0F);
 *(lcd.PORT) |= 1 << lcd.EN;
 __delay_us(40);
 *(lcd.PORT) &= ~(1 << lcd.EN);
}
void LCD_puts ( char *a ) {
 for ( int i = 0; a[i] != '\0'; ++i ) {
 LCD_putc(a[i]);
 }
}
void LCD_putrs ( const char *a ) {
 for ( int i = 0; a[i] != '\0'; ++i ) {
 LCD_putc(a[i]);
 }
}

//-----PIN MODE----
void pinMode(char terminal, char mode){
    
    char aux0 = (terminal & 0xF0) >> 4; //identifica el puerto
    char aux1 = 1 << (terminal & 0x0F); //identifica la terminal
    
    volatile unsigned char *puerto;
    puerto = &TRISA + aux0;
    
    if (mode == OUTPUT){
        char aux2 = *puerto & (~aux1);
        *puerto = aux2;
    }
    else {
        char aux2 = *puerto | aux1;
        *puerto = aux2;
    }
    
    switch(terminal){
        case _PA0:
                ADCON1bits.PCFG = 0b1111;
                break;
        
        case _PA1:
            if (ADCON1bits.PCFG < 14){
                ADCON1bits.PCFG = 14;
            }
            break;
    }
}



//-----ANALOG---
void ADCinit(int Canal){
    ADCON2 = 0b10000100; //Justificación a la derecha / Reloj de conversión FOSC/4
    
    switch(Canal){
        case 1:
            ADCON1bits.PCFG= 0b1110;
            TRISAbits.TRISA0=1;
            break;
        case 2:
            ADCON1bits.PCFG= 0b1101;
            TRISAbits.TRISA0=1;
            TRISAbits.TRISA1=1;
            break;
        case 3:
            ADCON1bits.PCFG= 0b1100;
            TRISAbits.TRISA0=1;
            TRISAbits.TRISA1=1;
            TRISAbits.TRISA2=1;
            break;
        case 4:
            ADCON1bits.PCFG= 0b1011;
            TRISAbits.TRISA0=1;
            TRISAbits.TRISA1=1;
            TRISAbits.TRISA2=1;
            TRISAbits.TRISA3=1;
            break;
        case 5:
            ADCON1bits.PCFG= 0b1010;
            TRISAbits.TRISA0=1;
            TRISAbits.TRISA1=1;
            TRISAbits.TRISA2=1;
            TRISAbits.TRISA3=1;
            TRISAbits.TRISA5=1;
            break;
        case 6:
            ADCON1bits.PCFG= 0b1001;
            TRISAbits.TRISA0=1;
            TRISAbits.TRISA1=1;
            TRISAbits.TRISA2=1;
            TRISAbits.TRISA3=1;
            TRISAbits.TRISA5=1;
            TRISEbits.TRISE0=1;
            break;
        case 7:
            ADCON1bits.PCFG= 0b1000;
            TRISAbits.TRISA0=1;
            TRISAbits.TRISA1=1;
            TRISAbits.TRISA2=1;
            TRISAbits.TRISA3=1;
            TRISAbits.TRISA5=1;
            TRISEbits.TRISE0=1;
            TRISEbits.TRISE1=1;
            break;
        case 8:
            ADCON1bits.PCFG= 0b0111;
            TRISAbits.TRISA0=1;
            TRISAbits.TRISA1=1;
            TRISAbits.TRISA2=1;
            TRISAbits.TRISA3=1;
            TRISAbits.TRISA5=1;
            TRISEbits.TRISE0=1;
            TRISEbits.TRISE1=1;
            TRISEbits.TRISE2=1;
            break;
        case 9:
            ADCON1bits.PCFG= 0b0110;
            TRISAbits.TRISA0=1;
            TRISAbits.TRISA1=1;
            TRISAbits.TRISA2=1;
            TRISAbits.TRISA3=1;
            TRISAbits.TRISA5=1;
            TRISEbits.TRISE0=1;
            TRISEbits.TRISE1=1;
            TRISEbits.TRISE2=1;
            TRISBbits.TRISB2=1;
            break;
        case 10:
            ADCON1bits.PCFG= 0b0101;
            TRISAbits.TRISA0=1;
            TRISAbits.TRISA1=1;
            TRISAbits.TRISA2=1;
            TRISAbits.TRISA3=1;
            TRISAbits.TRISA5=1;
            TRISEbits.TRISE0=1;
            TRISEbits.TRISE1=1;
            TRISEbits.TRISE2=1;
            TRISBbits.TRISB2=1;
            TRISBbits.TRISB3=1;
            break;
        case 11:
            ADCON1bits.PCFG= 0b0100;
            TRISAbits.TRISA0=1;
            TRISAbits.TRISA1=1;
            TRISAbits.TRISA2=1;
            TRISAbits.TRISA3=1;
            TRISAbits.TRISA5=1;
            TRISEbits.TRISE0=1;
            TRISEbits.TRISE1=1;
            TRISEbits.TRISE2=1;
            TRISBbits.TRISB2=1;
            TRISBbits.TRISB3=1;
            TRISBbits.TRISB1=1;
            break;
        case 12:
            ADCON1bits.PCFG= 0b0011;
            TRISAbits.TRISA0=1;
            TRISAbits.TRISA1=1;
            TRISAbits.TRISA2=1;
            TRISAbits.TRISA3=1;
            TRISAbits.TRISA5=1;
            TRISEbits.TRISE0=1;
            TRISEbits.TRISE1=1;
            TRISEbits.TRISE2=1;
            TRISBbits.TRISB2=1;
            TRISBbits.TRISB3=1;
            TRISBbits.TRISB1=1;
            TRISBbits.TRISB4=1;
            break;
        case 13:
            ADCON1bits.PCFG= 0b0010;
            TRISAbits.TRISA0=1;
            TRISAbits.TRISA1=1;
            TRISAbits.TRISA2=1;
            TRISAbits.TRISA3=1;
            TRISAbits.TRISA5=1;
            TRISEbits.TRISE0=1;
            TRISEbits.TRISE1=1;
            TRISEbits.TRISE2=1;
            TRISBbits.TRISB2=1;
            TRISBbits.TRISB3=1;
            TRISBbits.TRISB1=1;
            TRISBbits.TRISB4=1;
            TRISBbits.TRISB0=1;
            break;   
            
    }
    
    ADCON0bits.ADON = 1; //Convertidor AD encendido
    
}
int analogRead(int Canal){
    unsigned int dato=0;
    switch (Canal){
        case _AN0:
            ADCON0bits.CHS=0b0000;
            break;
        case _AN1:
           ADCON0bits.CHS=0b0001;
           break;
        case _AN2:
            ADCON0bits.CHS=0b0010;
            break;
        case _AN3:
           ADCON0bits.CHS=0b0011;
           break;
        case _AN4:
            ADCON0bits.CHS=0b0100;
            break;
        case _AN5:
           ADCON0bits.CHS=0b0101;
           break;
        case _AN6:
            ADCON0bits.CHS=0b0110;
            break;
        case _AN7:
           ADCON0bits.CHS=0b0111;
           break;
        case _AN8:
            ADCON0bits.CHS=0b1000;
            break;
        case _AN9:
           ADCON0bits.CHS=0b1001;
           break;
        case _AN10:
            ADCON0bits.CHS=0b1010;
            break;
        case _AN11:
           ADCON0bits.CHS=0b1011;
           break;
        case _AN12:
            ADCON0bits.CHS=0b1100;
            break;
    }
    
    
        ADCON0bits.GODONE = 1; 
        while (ADCON0bits.GODONE == 1){     
    }   
    dato = ADRESH << 8; 
    dato |= ADRESL; 
    
        return(dato);
    }


//----DIGITAL----
void digitalWrite(char terminal, char mode){
    char aux0 = (terminal & 0xF0) >> 4; //identifica el puerto
    char aux1 = 1 << (terminal & 0x0F); //identifica la terminal
    
    volatile unsigned char *puerto;
    puerto = &LATA + aux0;
    
     if (mode == LOW){
        char aux2 = *puerto & (~aux1);
        *puerto = aux2;
    }
    else {
        char aux2 = *puerto | aux1;
        *puerto = aux2;
    }
    
    
}
char digitalRead(char terminal){
    char resultado = 0;
    char aux0 = (terminal & 0xF0) >> 4; //identifica el puerto
    char aux1 = 1 << (terminal & 0x0F); //identifica la terminal
    
    volatile unsigned char *puerto;
    puerto = &PORTA + aux0;
    
    char aux2 = *puerto & aux1;
    if (aux2 > 0){
        resultado = 1;
    }
    else {
        resultado = 0;
    }
    
    return resultado;

}



//-----PRINT NUMBER-----
char unidades;
char decenas;
char centenas;
char milesimas;
void printNumber(unsigned int num){
     unidades=48;
     decenas=48;
     centenas=48;
     milesimas=48;

    for(num=num; num>999;num=num-1000){
        milesimas++;
    }
    
    for(num=num; num>99;num=num-100){
        centenas++;
    }
    
    for(num=num; num>9;num=num-10){
        decenas++;
    }
    
    for(num=num; num>0;num=num-1){
        unidades++;
    }
    
}




//------MATRIX KEYBOARD-----
volatile unsigned char *punteroaux0;  //AGREGARLO TAMBIÉN EN ARCHIVO .h
volatile unsigned char *punteroaux1; //AGREGARLO TAMBIÉN EN ARCHIVO .h
char valor[4][4]={{0,0,0,0},
                      {0,0,0,0},
                      {0,0,0,0},
                      {0,0,0,0}};

//4x4 KEYPAD FOR PROTEUS
//char tecla[4][4]={ {7,8,9,11},
//                   {4,5,6,22},
//                   {1,2,3,33},
//                   {44,0,55,66}};

//REAL 4x4 KEYPAD
char tecla[4][4]={ {1,2,3,11},
                   {4,5,6,22},
                   {7,8,9,33},
                   {44,0,55,66}};

void keypad4x4init(char puerto){  
  if (puerto == 1){
      TRISB = 0b11110000;
      punteroaux0 = &LATB;
      punteroaux1 = &PORTB;
  }  
  else {
      TRISD = 0b11110000;
      punteroaux0 = &LATD;
      punteroaux1 = &PORTD;
  }
}

char keypadread(){
        
            *punteroaux0 = 1;
            __delay_ms(5);    
            valor [0][0] = (*punteroaux1 & 0b00010000) > 0; /*Devuelve cero si la condición es falsa o 1 si es verdadera*/
            valor [0][1] = (*punteroaux1 & 0b00100000) > 0;
            valor [0][2] = (*punteroaux1 & 0b01000000) > 0;
            valor [0][3] = (*punteroaux1 & 0b10000000) > 0;
            

            *punteroaux0 = 2;
              __delay_ms(5);
             valor [1][0] = (*punteroaux1 & 0b00010000) > 0;
            valor [1][1] = (*punteroaux1 & 0b00100000) > 0;
            valor [1][2] = (*punteroaux1 & 0b01000000) > 0;
            valor [1][3] = (*punteroaux1 & 0b10000000) > 0;

            *punteroaux0 = 4;
             __delay_ms(5);
             valor [2][0] = (*punteroaux1 & 0b00010000) > 0;
            valor [2][1] = (*punteroaux1 & 0b00100000) > 0;
            valor [2][2] = (*punteroaux1 & 0b01000000) > 0;
            valor [2][3] = (*punteroaux1 & 0b10000000) > 0;

            *punteroaux0 = 8;
             __delay_ms(5);
             valor [3][0] = (*punteroaux1 & 0b00010000) > 0;
            valor [3][1] = (*punteroaux1 & 0b00100000) > 0;
            valor [3][2] = (*punteroaux1 & 0b01000000) > 0;
            valor [3][3] = (*punteroaux1 & 0b10000000) > 0;


            salida = 100;
            for(char i=0; i<4; i++){
                for (char j=0; j<4; j++){
                    if(valor[i][j] > 0){
                        salida = tecla[i][j];
                    }
                }
            }
      
            return(salida);
        }






//-----TIMER----
int contador = 0;

char time=0, converted_time=0;

void __interrupt() ISR(void) { 
 
 contador++;
 
  TMR0L = converted_time;
    
 INTCONbits.TMR0IF =0; 
 return; 
}

void tmr0init(char time){
    INTCONbits.GIE = 1; // habilitar las interrupciones globales (configurar PEIE = 0)
    INTCONbits.PEIE = 0; // Deshablitar Interrupciones de periféricos.
    INTCONbits.TMR0IE = 1; //Habilitar interrupción por desbordamiento de TMR0
    INTCONbits.INT0IE = 0; // Deshabilitar interrupciones externas.
    INTCONbits.RBIE = 0; //Deshabilitar interrupción por puerto RB
    INTCONbits.TMR0IF =0; // Limpiar la bandera de desbordamiento de TMR0
    RCONbits.IPEN = 0; // Deshabilitar prioridades en las interrupciones
    T0CONbits.TMR0ON = 1; // Habilitar tmr0
    T0CONbits.T08BIT = 1; //Configurar timer a 8 bits (puede ser de 16 bits)
    T0CONbits.T0CS = 0; // Seleccionar que el timer0 se incrementa por ciclo de reloj interno
    T0CONbits.PSA = 0; // Utilizar un prescaler (para hacer más largo el timpo de cuenta del
//timer).
//Los siguientes 3 bits controlan el preescaler, en este caso 1:256
    T0CONbits.T0PS0 = 1;
    T0CONbits.T0PS1 = 1;
    T0CONbits.T0PS2 = 1;
    
    converted_time=(256)-((time*1000)/(256));
    

    
    TMR0L = converted_time;
}




//-----PWM-----
void PWMinit(){
    CCP1CON=0b00001100;
    TRISCbits.RC2=0;
}
void pwmON(int freq, char ciclot){
    int aux=0;  
    char Presc=16;
    aux=(1000000/(freq*Presc))-1;
    if(aux<256){
    PR2=aux;        
    }
    else{
        int Presc=4;
        aux=(1000000/(freq*Presc))-1;
        if (aux<256){
            PR2=aux;           
          }
        else{
        int Presc=1;
        aux=(1000000/(freq*Presc))-1;
        if (aux<256){
         PR2=aux;            
            }
        else{
            aux=62;
            Presc=16;
            }
        }
        
    }
    PR2=aux;
    switch(Presc)
    {
        case 16:
            T2CONbits.T2CKPS=0b11;
            break;
        case 4:
            T2CONbits.T2CKPS=0b01;
            break;
        case 1:
            T2CONbits.T2CKPS=0b00;
            break;
    }
    aux=(40000*ciclot)/(freq*Presc);
    CCPR1L = aux >> 2;
    CCP1CONbits.DC1B = aux & 3;
    T2CONbits.TMR2ON = 1;
    return;
    
}
void pwmOFF(){
    T2CONbits.TMR2ON = 0;
}



//----MAP----
long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


//-----SERIAL COMUNICATION-----
void serialinit(){
TRISCbits.TRISC6 = 1;
TRISCbits.TRISC7 = 1;

//TXSTAbits.CSRC = 0; //Clock source select bit
TXSTAbits.TX9 = 0; //8 bit transmission
TXSTAbits.TXEN = 0; //Transmit enabled
TXSTAbits.SYNC = 0; //Asynchronous mode
TXSTAbits.SENDB = 1; //Send sync Break on next...
//...transmission (cleared by...
//...hardware upon completion).
TXSTAbits.BRGH = 0; //Low speed rate


RCSTAbits.SPEN = 1;
RCSTAbits.RX9 = 0;
RCSTAbits.CREN = 0; //enables receiver
RCSTAbits.ADDEN = 0; //Disables address detection
RCSTAbits.FERR = 0; //No overrun error


BAUDCONbits.ABDOVF = 0;
BAUDCONbits.RXDTP = 0;
BAUDCONbits.TXCKP = 0;
BAUDCONbits.BRG16 = 0;
BAUDCONbits.ABDEN = 0;

SPBRG = 51;
TXREG = 0b01110111;
}
//-----------------------------------------------------
//FUNCIÓN PARA ENVIAR UN CARACTER (CÓDIGO ASCII)
//POR PUERTO SERIAL. TIENE UN ÚNICO ARGUMENTO QUE ES
//EL NÚMERO DE CARACTER QUE SE DESEA ENVIAR, O EL PROPIO
//CARACTER ENTRE COMILLAS SIMPLES. POR EJEMPLO, ES EQUIVALENTE
//serialout(48); y serialout('0');
//EN AMBOS CASOS SE ENVIARÁ EL CARACTER "0";

void serialout(char datax){
TXSTAbits.TXEN = 1;
__delay_ms(1);
TXREG = datax;
__delay_ms(10);

}
