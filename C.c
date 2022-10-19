//******************************************************************************
//   UNIVERSIDAD DEL VALLE DE GUATEMALA
//   IE2023 PROGRAAMACIÓN DE MICROCONTROLADORES 
//   AUTOR: MICHELLE SERRANO
//   COMPILADOR: XC8 (v1.41), MPLAB X IDE (v6.00)
//   PROYECTO: LABORATORIO 4
//   HARDWARE: PIC16F887
//   CREADO: 07/10/2022
//   ÚLTIMA MODIFCACIÓN: 17/10/2022

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT
#pragma config WDTE = OFF       
#pragma config PWRTE = ON      
#pragma config MCLRE = OFF      
#pragma config CP = OFF        
#pragma config CPD = OFF        
#pragma config BOREN = OFF      
#pragma config IESO = OFF       
#pragma config FCMEN = OFF     
#pragma config LVP = OFF       

// CONFIG2
#pragma config BOR4V = BOR40V   
#pragma config WRT = OFF    

#include <xc.h>

#define _XTAL_FREQ 4000000 //4mHz 

void setup(void);
void setupADC(void);
void main (void);

//******************************************************************************
// Variables
//******************************************************************************
int i = 0; 
int valor = 0;
unsigned int display1=0;
unsigned int display2=0;
unsigned char hexadecimal[] = {
    0b00111111, //0
    0b00000110, //1
    0b01011011, //2
    0b01001111, //3
    0b01100110, //4
    0b01101101, //5
    0b01111101, //6
    0b00000111, //7
    0b01111111, //8
    0b01100111, //9
    0b01110111, //A
    0b01111100, //B
    0b00111001, //C
    0b01011110, //D
    0b01111001, //E
    0b01110001, //F
};
//******************************************************************************
// CÃ³digo Principal
//******************************************************************************

void main(void) {
    
    setup(); //llamamos la configuración de puertos
    setupADC(); //lamamos la configuración de ADC 
    
    while(1){
    
        if (PORTCbits.RC0 == 1) { //C2 aumenta
            i++; //suma 1
            if (i>=128){ //si ya llegó a 15
                i= 128;}  //permanece ahí 
            PORTB = i;  //se muestra en el puerto B
            __delay_ms(500);
        }   
        
        if (PORTCbits.RC1 == 1) { //C1 decrementa
            i--; //resta 1
            if (i<0){  //si es menor que 0 
                i= 0;}
            PORTB = i; //mostrar en el puerto B
            __delay_ms(500);
        }
        
        ADCON0bits.GO = 1;
        while(ADCON0bits.GO == 1);
        ADIF = 0;
        valor = ADRESH;
        __delay_ms(10);
        
        
      display1 = (valor%16); 
      display2 = (valor/16);

        //display 1 
        PORTD = hexadecimal[display1];
        PORTEbits.RE1 = 1;
        PORTEbits.RE2 = 0;

        __delay_ms(5); 

        //display 2
        PORTD = hexadecimal[display2];
        PORTEbits.RE1 = 0;
        PORTEbits.RE2 = 1;

        __delay_ms(5); 
      {

        if (valor >= i ) //alarma 
        {
          PORTEbits.RE0= 1; 
        }
        else
        {
          PORTEbits.RE0 = 0; 
        }
          } 
     }
    return;
}

//******************************************************************************
// FunciÃ³n para configurar puertos
//******************************************************************************
void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    
    TRISB = 0; //puerto B como salida
    PORTB = 0;  //limpiamos el puerto
    
    TRISC = 0b00000011; // RC0 y RC1 como entradas
    PORTC = 0; //limpiamos el puerto   
    
    
    TRISD = 0; //puerto D como salida (7 seg)
    PORTD = 0; //limpiamos el puerto D
    
    TRISE = 0; //puerto E para la alarma
    PORTE = 0; //limpiamos el puerto E
    
}

//******************************************************************************
// FunciÃ³n para configurar ADC
//******************************************************************************
void setupADC(void){
    
    //TRISAbits.TRISA0 = 1;
    TRISA = TRISA | 0x01;
    ANSEL = ANSEL | 0x01;
    
    // Configurar mÃ³dulo ADC
    
    ADCON0bits.ADCS1 = 0;
    ADCON0bits.ADCS0 = 1;       // Fosc/ 8
    
    ADCON1bits.VCFG1 = 0;       // Ref VSS
    ADCON1bits.VCFG0 = 0;       // Ref VDD
    
    ADCON1bits.ADFM = 0;        // Justificado hacia izquierda
    
    ADCON0bits.CHS3 = 0;
    ADCON0bits.CHS2 = 0;
    ADCON0bits.CHS1 = 0;
    ADCON0bits.CHS0 = 0;        // Canal AN0
    
    ADCON0bits.ADON = 1;        // Habilitamos el ADC
    __delay_us(100);
    
}


