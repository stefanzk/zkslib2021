/************************************************************/
/* Basisfunktionen für die Steuerung der UART Schnittstelle */
/*															*/
/* Autor: ZKS												*/
/*															*/
/* zkslibuart.c                                             */
/*															*/
/* Versionsinfos:											*/
/* 26.8.2020, Initial release V1							*/
/************************************************************/
#include "zkslibuart.h"
#include <AVR/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

int	_uart_put(char c, FILE * f);

#ifdef STDOUT_UART
FILE uart_str = FDEV_SETUP_STREAM(_uart_put, NULL, _FDEV_SETUP_WRITE);
#endif


/*********************************************************************/
/* HW.Abhängige funktionen Funktionen							     */
/*********************************************************************/

/*********************************************************************/
// Low Level Hardware-Dependent Functions
#ifdef DEVICE_ATMEGA16

#define UART_RX_PIN 0
#define UART_TX_PIN 1
#define UART_PORT PORTD
#define UART_DDR DDRD
 


void _loc_Init(unsigned char UartBaudRate, unsigned char UartMode)
{
	
	unsigned long Baud[N_BAUDRATES] ={1200, 2400, 9600, 57600, 115200};
	unsigned long UbrrCalc;
	
	// Program the Pins
	// RX: PD0, TX: PD1
	// RX ist Eingang mit Pullup
	UART_DDR&=~(1<<UART_RX_PIN);
	UART_PORT|=(1<<UART_RX_PIN);
	
	// TX ist Ausgang auf high (Idle)
	UART_DDR|=(1<<UART_TX_PIN);
	UART_PORT|=(1<<UART_TX_PIN);
		
	
	// Berechnen des Ubrr Registers für die Baudrate
	UbrrCalc=F_CPU/(16*Baud[UartBaudRate])-1;
	
	// Set the BAUD Rate for the 12MHz System Clock
	UBRRL=(UbrrCalc & 0x000000ff);
	UBRRH=(UbrrCalc >> 8);
	
	 //Set Com Mode
	 //UCSRC: 0b10 P1 P0 T0 1 1 0
	 //T0: 0 1 Stopbit, 1 2 Stopbits
	 //P1 P0: 0 0 No Parity, 1 0 Even, 1 1 Odd
	 
	 switch(UartMode)
	 {
		 case UART_CONFIG_8N1:
		 UCSRC=0b10000110;
		 break;
		 
		 case UART_CONFIG_8N2:
		 UCSRC=0b10001110;
		 break;
		 
		 case UART_CONFIG_8E1:
		 UCSRC=0b10100110;
		 break;
		 
		 case UART_CONFIG_8E2:
		 UCSRC=0b10101110;
		 break;

		 case UART_CONFIG_8O1:
		 UCSRC=0b10110110;
		 break;
		 
		 case UART_CONFIG_8O2:
		 UCSRC=0b10111110;
		 break;

		 default:
		 UCSRC=0b10000110;
		 break;
		 
	 }
	
	// Enable Transmit and Receive Operation
	UCSRB=(1<<RXEN) | (1<<TXEN);
	
}

// Empfangsregister voll ?
uint8_t _loc_RxComplete(void)
{
	return ((UCSRA & (1<<RXC)) != 0);
}

// Senderegister übertragen ?
uint8_t _loc_TxComplete(void)
{
	uint8_t Result;
	
	Result=((UCSRA & (1<<TXC)) != 0);

	// clear the TXC Bit by writing 1 into it
	UCSRA|=(1<<TXC);
	return Result;
}

// Empfangsregister lesen und ausgeben
uint8_t _loc_GetRxData(void)
{
	return UDR;
}

// Datenbyte ausgeben
void _loc_SetTxData(uint8_t Data)
{
	UDR=Data;
}
#endif
/*********************************************************************/

#ifdef DEVICE_ATMEGA328
void uart_init(unsigned char UartBaudRate, unsigned char UartMode)
{
	
	unsigned long Baud[N_BAUDRATES] ={1200, 2400, 9600, 57600, 115200};
	unsigned long UbrrCalc;
	
	// Program the Pins
	// RX: PD0, TX: PD1
	DDRD&=0xfe;
	DDRD|=0x02;
	
	// Activate Pullup to keep RX in idle mode when nothing is connected
	PORTD&=0xfe;
	
	// UBRR = fosc / (16 BAUD) -1
	UbrrCalc=F_CPU/(16*Baud[UartBaudRate])-1;
	
	// Set the BAUD Rate 
	UBRR0L=(UbrrCalc & 0x000000ff);
	
	UBRR0H=(UbrrCalc >> 8);
	
	
	
	 //Set Com Mode for 328
	 //UCSRC: 0b00 P1 P0 T0 1 1 0
	 //T0: 0 1 Stopbit, 1 2 Stopbits
	 //P1 P0: 0 0 No Parity, 1 0 Even, 1 1 Odd
	 
	 switch(UartMode)
	 {
		 case UART_CONFIG_8N1:
		 UCSR0C=0b00000110;
		 break;
		 
		 case UART_CONFIG_8N2:
		 UCSR0C=0b00001110;
		 break;
		 
		 case UART_CONFIG_8E1:
		 UCSR0C=0b00100110;
		 break;
		 
		 case UART_CONFIG_8E2:
		 UCSR0C=0b00101110;
		 break;

		 case UART_CONFIG_8O1:
		 UCSR0C=0b00110110;
		 break;
		 
		 case UART_CONFIG_8O2:
		 UCSR0C=0b00111110;
		 break;
		 
		 default:
		 UCSR0C=0b00000110;
		 break;
		 
	 }
	
	// Enable Transmit and Receive Operation
	UCSR0B=(1<<RXEN0) | (1<<TXEN0);

}

/* Prüft auf empfangene Daten */
unsigned char uart_RxComplete(void)
{
	
	return ((UCSR0A & (1<<RXC0)) != 0);
	
}

/* Prüft auf gesendete Daten */
unsigned char uart_TxComplete(void)
{
	uint8_t Result;
	
	
	Result=((UCSR0A & (1<<TXC0)) != 0);
	// clear the TXC Bit by writing 1 into it
	UCSR0A|=(1<<TXC0);
	return Result;
	
}

/* Gibt empfangene Daten zurück */
unsigned char uart_GetRxData(void)
{
	return UDR0;
}


/* Sendet ein Datenbyte und wartet bis die Daten vollständig gesendet sind */
void uart_SendTxDataWait(unsigned char Data)
{
	UDR0=Data;
	uart_WaitTx();
}
#endif

/*********************************************************************/
// Lokale HW Unabhängige FUnktionen
/*********************************************************************/
int	_uart_put(char c, FILE * f)
{
	uart_SendByte(c,UART_YES);
	return 0;
}

/*********************************************************************/


/*********************************************************************/
//HW Unabhängige Funktionsprototypen für die Uart-Steuerung
/*********************************************************************/

//Initialisiert die Schnittstelle für einen gegebenen Parameter-Satz
// Für die Parameter müssen die Defines aus diesem .h File verwendet werden
// UartBaudRate: Übergabe der Baudrate
// UartMode: definiert den Mode für die Uart Schnittstelle
void uart_Init(uint8_t UartBaudRate, uint8_t UartMode)
{
	_loc_Init(UartBaudRate, UartMode);
	// Send a Dummy Byte to Clear the Send Buffer 
	uart_SendByte(0,UART_YES);
	//uart_SendByte(UART_CR,UART_YES);
	//uart_SendByte(UART_LF,UART_YES);
	
	#ifdef STDOUT_UART
	uart_SendTextWait("Ready",UART_YES);
	stdout = &uart_str;
	#endif
	
	// Read Data to empty receive Buffer
	uart_GetData();
	
	
}

// Prüft ob neue Daten empfangen wurden
// Liefert als Ergebnis TRUE wenn neue Daten vorhanden andernfalls FALSE
uint8_t uart_NewData(void)
{
	return _loc_RxComplete();
}

// Prüft ob die letzte Sendung komplett ausgeführt wurde
// TRUE: Sendebuffer ist leer
// FALSE: Sendung läuft noch
uint8_t uart_SendComplete(void)
{
	return _loc_TxComplete();
}

// Warten auf empfangene Daten mit Timeout
// TRUE: Daten wurden empfangen
// FALSE: Timeout
uint8_t uart_WaitForNewData(void)
{
	uint16_t TimeOutCnt=0;
	uint8_t RxOk=0;
	
	while ((TimeOutCnt<1000) && !RxOk)
	{
		RxOk=_loc_RxComplete();
		_delay_us(TIMEOUT_N_US);
		TimeOutCnt++;
	}
	
	if (RxOk) return UART_OK;
	else return UART_ERR;	
}

// Wartet bis die aktuelle Kommunikation abgeschlossen ist
// Timeout nach 100ms
void uart_WaitForSendComplete(void)
{
	uint16_t TimeOutCnt=0;
	uint8_t TxOk=0;

	while ((TimeOutCnt<1000) && !TxOk)
	{
		TxOk=_loc_TxComplete();
		_delay_us(TIMEOUT_N_US);
		TimeOutCnt++;
	}
}

// Gibt das älteste empfangene Datenbyte aus dem Empfangsbuffer zurück
// (Ohne Interrupt kann nur eine Byte empfangen werden)
// Wenn keine Daten vorliegen wird 0x00 zurückgegeben
uint8_t uart_GetData(void)
{
	return _loc_GetRxData();
}

// Sendet ein einzelnes Datenbyte und wartet (optional) bis die Daten vollständig gesendet sind
// Data: das zu sendende Datenbyte
// WaitYesNo: Flag das anzeigt ob gewartet werden soll bis die Übertragung komplett ist oder nicht
void uart_SendByte(uint8_t Data, uint8_t WaitYesNo)
{
	_loc_SetTxData(Data);
	if (WaitYesNo)
	{
		uart_WaitForSendComplete();
	}
}


// Sendet einen Text über die Schnittstelle, optional mit nachfolgendem CRLF
void uart_SendTextWait(char  * TextOut,  uint8_t SendCrLf)
{
	uint8_t StrLen = strlen(TextOut);
	uint8_t Cnt;
	
	for(Cnt=0;Cnt<StrLen;Cnt++)
	{
		uart_SendByte(TextOut[Cnt],UART_YES);
	}	
	
	// Wenn gewünscht, CRLF nachsenden
	if(SendCrLf)
	{
		uart_SendByte(UART_CR, UART_YES);
		uart_SendByte(UART_LF, UART_YES);
	}
}