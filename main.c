/*
 * File:   main.c
 * Author: Patricio Escalante
 *
 * Created on 26 de noviembre de 2021, 05:42 PM
 */


#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <stdbool.h>
#include "configbits.h"
#include "duckylib.h"

#define _XTAL_FREQ 4000000

#define led_rojo _PA2
#define led_verde _PA3
#define servo _PC2
#define switchh _PA4
#define buzzer _PA1




int puerta=0, flag1=0, flag2=0, flag3=0, flag4=0;
int contador=0, contador2=0, contador3=0, contador4=0;
int j=0;
int tecla_oprimida=0;
int contrasena_ingresada[5];
int contrasena_real[5]={1,2,7,0,1};

void mostrar_temperatura(){
        int temp_analog, temp;
        temp_analog=analogRead(_AN0);
        
        temp=(temp_analog*10)/2.05;
        
        printNumber(temp);
        LCD_Set_Cursor(0,0);
        if(temp<100){
            LCD_putrs(""); //Escribir los millares
            LCD_putrs("");// Escribir las centenas
            LCD_putc(decenas); //Escribir las decenas
            LCD_putrs(".");
            LCD_putc(unidades); //Escribir las unidades
            LCD_putc(223);
            LCD_putrs("C");
        }else{
            if(temp>=100 && temp<1000){
                LCD_putrs(""); //Escribir los millares
                LCD_putc(centenas); // Escribir las centenas
                LCD_putc(decenas); //Escribir las decenas
                LCD_putrs(".");
                LCD_putc(unidades); //Escribir las unidades
                LCD_putc(223);
                LCD_putrs("C");
            }else{
                LCD_putc(milesimas); //Escribir los millares
                LCD_putc(centenas); // Escribir las centenas
                LCD_putc(decenas); //Escribir las decenas
                LCD_putrs(".");
                LCD_putc(unidades); //Escribir las unidades
                LCD_putc(223);
                LCD_putrs("C");
            }
    }
}


void modo_desarmado(){
        pwmON(250, 29);
        LCD_Set_Cursor(0,0); 
        LCD_putrs("             ");
        tecla_oprimida=000;
        digitalWrite(led_rojo, LOW);
        digitalWrite(led_verde, HIGH);
        LCD_Set_Cursor(1,0); 
        LCD_putrs("CAJA DESARMADA");
        LCD_Set_Cursor(2,0);
        LCD_putrs("                     ");
        
        puerta=digitalRead(switchh);
        if(puerta==1){
            flag1=0;
            flag2=0;
            flag3=0;
        }
    }
    
    
void esperar_on(){
        pwmON(250, 29);
        LCD_Set_Cursor(0,0); 
        LCD_putrs("             ");
        digitalWrite(led_rojo, LOW);
        digitalWrite(led_verde, HIGH);
        LCD_Set_Cursor(1,0); 
        LCD_putrs("CAJA DESARMADA");
        LCD_Set_Cursor(2,0); 
        LCD_putrs("TECLA * PARA ARMAR");
        
        tecla_oprimida=keypadread();
        if(tecla_oprimida==44){
            flag1=0;
            flag2=0;
            flag3=1;
            digitalWrite(buzzer, HIGH);
            __delay_ms(80);
            digitalWrite(buzzer, LOW);
        }
        
        
}

void modo_armado(){
    pwmON(250, 46);
    digitalWrite(led_rojo, HIGH);
    digitalWrite(led_verde, LOW);
    LCD_Set_Cursor(1,0); 
    LCD_putrs("CAJA ARMADA     ");
    LCD_Set_Cursor(2,0); 
    LCD_putrs("                    ");
    
    mostrar_temperatura();
    
    puerta=digitalRead(switchh);
    if(puerta==0){
        serialout('A');
        serialout('L');
        serialout('E');
        serialout('R');
        serialout('T');
        serialout('A');
        serialout(' ');
        flag2=1;
        flag1=0;
        flag3=0;
    }
    
    tecla_oprimida=keypadread();
    if(tecla_oprimida==66){
        digitalWrite(buzzer, HIGH);
            __delay_ms(80);
            digitalWrite(buzzer, LOW);
            flag1=1;
            flag2=0;
            flag3=1;
        }
}

void alarma_8seg(){
    if(flag4==1){
        LCD_Set_Cursor(0,0); 
        LCD_putrs("        ");
        LCD_Set_Cursor(1,0); 
        LCD_putrs("ALARMA        ");
        LCD_Set_Cursor(2,0); 
        LCD_putrs("DEMASIADOS INTENTOS");
    }else{
        LCD_Set_Cursor(0,0); 
        LCD_putrs("        ");
        LCD_Set_Cursor(1,0); 
        LCD_putrs("ALARMA        ");
        LCD_Set_Cursor(2,0); 
        LCD_putrs("PUERTA ABIERTA     ");
    } 
    
    for( int i=0; i<16; i++){
        digitalWrite(buzzer, HIGH);
        __delay_ms(80);
        digitalWrite(buzzer, LOW);
        __delay_ms(420);
    }
    
    puerta=0;
    flag1=0;
    flag2=0;
    flag3=0;
    flag4=0;
}

void leer_tecla_mas(){
        mostrar_temperatura();
        tecla_oprimida=keypadread();
        
        LCD_Set_Cursor(1,0); 
        LCD_putrs("INGRESAR PSWD:");
        
        
        
        if(tecla_oprimida==0 || tecla_oprimida==1 || tecla_oprimida==2 || tecla_oprimida==3 || tecla_oprimida==4 || tecla_oprimida==5 || tecla_oprimida==6 || tecla_oprimida==7 || tecla_oprimida==8 || tecla_oprimida==9){
            digitalWrite(buzzer, HIGH);
            __delay_ms(80);
            digitalWrite(buzzer, LOW);
            __delay_ms(300);

            if(contador==5){
                contador=5;
            }else{
                contador++;
            }
            
            contrasena_ingresada[j]=tecla_oprimida;       
            j++;
            
        }
        
        for(int i=0;i<contador;i++){
            LCD_Set_Cursor(2,i); 
            LCD_putrs("*");
        }
        
        if(contador==5){
            for(int i=0;i<5;i++){
                if(contrasena_real[i]!=contrasena_ingresada[i]){
                    contador2++;
                }else{
                    contador3++;
                }
            }

            if(contador2>0){
                LCD_Set_Cursor(3,0); 
                LCD_putrs("ERROR      ");
                __delay_ms(1000);
                LCD_Set_Cursor(3,0); 
                LCD_putrs("           ");
                LCD_Set_Cursor(2,0); 
                LCD_putrs("           ");

                j=0;
                contador=0;
                contador2=0;
                contador3=0;
                contador4++;
            }else{
                if(contador3==5){
                    LCD_Set_Cursor(0,0); 
                    LCD_putrs("        ");
                    puerta=0;
                    flag1=0;
                    flag2=0;
                    flag3=0;
                    j=0;
                    contador=0;
                    contador2=0;
                    contador3=0;
                    contador4=0;
                }
                
            }
          
        }
        
        if(contador4==3){
            flag1=0;
            flag2=1;
            flag3=0;
            flag4=1;
            j=0;
            contador=0;
            contador2=0;
            contador3=0;
            contador4=0;
        }
      
}
    
    
void main() {
    PWMinit();
    ADCinit(1);
    serialinit();
    keypad4x4init(_PORTB);
    
    pinMode(led_rojo, OUTPUT);
    pinMode(led_verde, OUTPUT);
    pinMode(switchh, INPUT);
    pinMode(servo, OUTPUT);
    TRISB=0b11110000;
    pinMode(buzzer, OUTPUT);
    
    LCD lcd = { &PORTD, 5, 4, 0, 1, 2, 3 }; // PORT, RS, EN, D4, D5, D6, D7
    LCD_Init(lcd);
    LCD_Clear(); //LIMPIAR LCD
    
        
    while(1){
        
        puerta=digitalRead(switchh);
        if(puerta==0 && flag1==0 && flag2==0 && flag3==0){
            modo_desarmado();
        }
        
        puerta=digitalRead(switchh);
        if(puerta==1 && flag1==0 && flag2==0 && flag3==0){
            esperar_on();
        }
        
        puerta=digitalRead(switchh);
        if(puerta==1 && flag1==0 && flag2==0 && flag3==1){
            modo_armado();
        }
        
        puerta=digitalRead(switchh);
        if(puerta==1 && flag1==1 && flag2==0 && flag3==1){
            leer_tecla_mas();
        }
        
        puerta=digitalRead(switchh);
        if(flag2==1 && flag1==0 && flag3==0){
            alarma_8seg();
        }
    
    }
    
}
