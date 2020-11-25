/************************************************************/
/* Basisfunktionen f�r die Steuerung verschiedener Displays */
/*															*/
/* Autor: ZKS												*/
/*															*/
/*															*/
/* Versionsinfos:											*/
/* 26.8.2020, Initial release V2021.					    */
/************************************************************/
#include <stdint.h>
#define ZKSLIBUART 202105

// Defines f�r die Definition der Baudrate
#define N_BAUDRATES 5
#define UART_BAUDRATE_1200 0
#define UART_BAUDRATE_2400 1
#define UART_BAUDRATE_9600 2
#define UART_BAUDRATE_57600 3
#define UART_BAUDRATE_115200 4

// Defines f�r die Konfiguration
#define UART_CONFIG_8N1 0
#define UART_CONFIG_8N2 1
#define UART_CONFIG_8E1 2
#define UART_CONFIG_8E2 3
#define UART_CONFIG_8O1 4
#define UART_CONFIG_8O2 5

// R�ckmeldecodes
#define UART_ERR 0
#define UART_OK 1

// Einstellungen des Timeouts f�r das Warten auf ankommende Daten
// Das Delay wird 1000x abgewartet
#define TIMEOUT_N_US 100

// Defines f�r Labels und Sonderzeichen
#define UART_CR 13
#define UART_LF 10

#define UART_YES 1
#define UART_NO 0

//------------------------------------------------------------------------
//HW Unabh�ngige Funktionsprototypen f�r die Uart-Steuerung

//Initialisiert die Schnittstelle f�r einen gegebenen Parameter-Satz
// F�r die Parameter m�ssen die Defines aus diesem .h File verwendet werden
// UartBaudRate: �bergabe der Baudrate
// UartMode: definiert den Mode f�r die Uart Schnittstelle
void uart_Init(uint8_t UartBaudRate, uint8_t UartMode);

// Pr�ft ob neue Daten empfangen wurden
// Liefert als Ergebnis TRUE wenn neue Daten vorhanden andernfalls FALSE 
uint8_t uart_NewData(void);

// Pr�ft ob die letzte Sendung komplett ausgef�hrt wurde ohne zu warten
// TRUE: Sendebuffer ist leer
// FALSE: Sendung l�uft noch
uint8_t uart_SendComplete(void);

// Warten auf empfangene Daten mit Timeout
// TRUE: Daten wurden empfangen
// FALSE: Timeout
// Die Dauer des Timeouts wird als #define festgelegt
uint8_t uart_WaitForNewData(void);

// Wartet bis eine aktuell laufende Kommunikation abgeschlossen ist
void uart_WaitForSendComplete(void);

// Gibt das zuletzt empfangene Datenbyte aus dem Empfangsbuffer zur�ck
// (Ohne Interrupt kann nur eine Byte empfangen werden) 
// Wenn keine Daten empfangen wurden wird 0x00 zur�ckgegeben
// Vorsicht: die Daten sind nur g�ltig, wenn zuvor auf neue Daten gepr�ft wurde
uint8_t uart_GetData(void);

// Sendet ein einzelnes Datenbyte und wartet (optional) bis die Daten vollst�ndig gesendet sind 
// Data: das zu sendende Datenbyte
// WaitYesNo: Flag das anzeigt ob gewartet werden soll bis die �bertragung komplett ist oder nicht
void uart_SendByte(uint8_t Data, uint8_t WaitYesNo);

// Sendet einen Text �ber die Schnittstelle, optional mit nachfolgendem CRLF
// TextOut muss ein Null-terminierter String sein
void uart_SendTextWait(char  * TextOut,  uint8_t SendCrLf);
//------------------------------------------------------------------------