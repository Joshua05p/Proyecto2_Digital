/*
 * Laboratorio6.c
 *
 * Created: 12/03/2026 16:16:27
 * Author : masco
 */ 

/*
 * Lab3.c
 *
 * Created: 29/01/2026 10:12:50
 * Author : masco
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>


/****************************************/
// Function prototypes
void iniciar_USART(unsigned int ubrr);
void enviar(char data);
void cadena(char *caracteres);

// Main Function
int main(void)
{
	// Configurar PD2–PD7 como entrada
	DDRD &= ~((1<<PORTD2)|(1<<PORTD3)|(1<<PORTD4)|(1<<PORTD5)|(1<<PORTD6)|(1<<PORTD7));
	
	// Activar resistencias pull-up internas
	PORTD |= (1<<PORTD2)|(1<<PORTD3)|(1<<PORTD4)|(1<<PORTD5)|(1<<PORTD6)|(1<<PORTD7);
	
	
	PCICR |= (1<<PCIE2);  // Habilitar grupo PCINT[23:16]
	
	PCMSK2 |= (1<<PCINT18) | (1<<PCINT19) | (1<<PCINT20) | (1<<PCINT21) |	(1<<PCINT22) | (1<<PCINT23);

	sei();  // Habilitar interrupciones globales

	// Inicializar USART para 9600 baudios
	iniciar_USART(103);
	sei();
    while (1) 
    {
		

    }
}


// NON-Interrupt subroutines
void iniciar_USART(unsigned int ubrr) {
	UBRR0H = (ubrr >> 8);
	UBRR0L = ubrr;

	UCSR0B |= (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
}

void enviar(char data) {
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}

void cadena(char *caracteres) {
	while (*caracteres != '\0'){
		enviar(*caracteres);
		caracteres++;
	}
}


// Interrupt routines

ISR(PCINT2_vect)
{
	if (!(PIND & (1<<PIND2))) {
		cadena("1");
			
	}
	if (!(PIND & (1<<PIND3))) {
		cadena("2");
		
	}
	if (!(PIND & (1<<PIND4))) {
		cadena("3");
		
	}

	if (!(PIND & (1<<PIND5))) {
		cadena("4");
		
	}

	if (!(PIND & (1<<PIND6))) {
		cadena("5");
		
	}

	if (!(PIND & (1<<PIND7))) {
		cadena("6");
		
	}
}
