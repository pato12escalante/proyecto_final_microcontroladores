//---DUCKYLIB DEFINES-----
#define _PA0 0x00 
#define _PA1 0x01
#define _PA2 0x02
#define _PA3 0x03
#define _PA4 0x04
#define _PA5 0x05

#define _PB0 0x10
#define _PB1 0x11
#define _PB2 0x12
#define _PB3 0x13
#define _PB4 0x14
#define _PB5 0x15
#define _PB6 0x16
#define _PB7 0x17

#define _PC0 0x20
#define _PC1 0x21
#define _PC2 0x22
#define _PC4 0x24
#define _PC5 0x25
#define _PC6 0x26

#define _PD0 0x30
#define _PD1 0x31
#define _PD2 0x32
#define _PD3 0x33
#define _PD4 0x34
#define _PD5 0x35
#define _PD6 0x36
#define _PD7 0x37

#define _PE0 0x40
#define _PE1 0x41
#define _PE2 0x42

#define INPUT 0
#define OUTPUT 1

#define HIGH 1
#define LOW 0

#define _AN0 1
#define _AN1 2
#define _AN2 3
#define _AN3 4
#define _AN4 5
#define _AN5 6
#define _AN6 7
#define _AN7 8
#define _AN8 9
#define _AN9 10
#define _AN10 11
#define _AN11 12
#define _AN12 13

#define _PORTB 1

#ifndef LCD_H
#define LCD_H
#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
 volatile unsigned char* PORT; // Pointer to the LCD port e.g &PORTC
 unsigned RS :3; // The RS bit of the LCD PORT e.g. 2
 unsigned EN :3; // The EN bit of the LCD PORT e.g. 3
 unsigned D4 :3; // The D4 bit of the LCD PORT e.g. 4
 unsigned D5 :3; // The D5 bit of the LCD PORT e.g. 5
 unsigned D6 :3; // The D6 bit of the LCD PORT e.g. 6
 unsigned D7 :3; // The D7 bit of the LCD PORT e.g. 7
} LCD;
extern LCD lcd;
// Macros that correspond to LCD commands
#define LCD_Clear() LCD_Cmd(0x01)
#define LCD_Decrement_Cursor() LCD_Cmd(0x04)
#define LCD_Increment_Cursor() LCD_Cmd(0x05)
#define LCD_Shift_Display_Right() LCD_Cmd(0x06)
#define LCD_Shift_Display_Left() LCD_Cmd(0x07)
#define LCD_Shift_Right() LCD_Cmd(0x1C)
#define LCD_Shift_Left() LCD_Cmd(0x18)
// Sets the LCD cursor position
#define LCD_Set_Cursor( x, y ) \
do { \
 if ( x == 0 ) { \
 LCD_Cmd(0x80 + y); \
 } \
 else if ( x == 1 ) { \
 LCD_Cmd(0xC0 + y); \
 } \
 else if ( x == 2 ) { \
 LCD_Cmd(0x94 + y); \
 } \
 else if ( x == 3 ) { \
 LCD_Cmd(0xD4 + y); \
 } \
} while ( false )
// [Internal use only] Sets the display on/off, the cursor on/off and if it will blink or not
#define LCD_Display( on, cursor, blink ) \
do { \
 unsigned char cmd = 0x08; \
 \
 if ( on == true ) { \
 cmd |= 1 << 2; \
 } \
 \
 if ( cursor == true ) { \
 cmd |= 1 << 1; \
 } \
 \
 if ( blink == true ) { \
 cmd |= 1; \
 } \
 \
 LCD_Cmd(cmd); \
} while ( false )
// [Internal use only] Sends a command to the LCD
#define LCD_Cmd( c ) \
do { \
 LCD_Write( (c & 0xF0) >> 4 ); \
 LCD_Write( c & 0x0F); \
} while ( false )
// Initializes the LCD (See LCD struct)
bool LCD_Init ( LCD display );
// Prints a character on the LCD
void LCD_putc ( char a );
// Prints a string on the LCD that is allocated on the RAM
void LCD_puts ( char *a );
// Prints a string on the LCD that is allocated on the program memory
void LCD_putrs ( const char *a );
// [Internal use only] Write a byte to the LCD
void LCD_Write ( unsigned char c );
// [Internal use only] Outputs the data to the LCD Port
void LCD_Out ( char a );
#ifdef __cplusplus
}
#endif
#endif /* LCD_H */



void tmr0init (char time);
void PWMinit(void);
void pwmON(int freq, char ciclot);
void ADCinit(int Canal);
void printNumber(unsigned int nu);
int analogRead(int Canal);
void pwmOFF(void);
void pinMode(char terminal, char mode);
char digitalRead(char terminal);
void digitalWrite(char terminal, char mode);
void keypad4x4init(char puerto);
char keypadread(void);
void serialinit(void);
void serialout(char datax);




char F;
char salida;
char time;
char converted_time;
char unidades;
char decenas;
char centenas;
char milesimas;
int contador;
void pwmON(int freq, char ciclot);  
char keypadread(void);
char valor[4][4];
char tecla[4][4];
long map(long x, long in_min, long in_max, long out_min, long out_max);
volatile unsigned char *punteroaux0;  //AGREGARLO TAMBIÉN EN ARCHIVO .h
volatile unsigned char *punteroaux1; //AGREGARLO TAMBIÉN EN ARCHIVO .h
char puerto;
char datax;