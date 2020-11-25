/************************************************************/
/* Basisfunktionen f�r die Steuerung verschiedener Displays */
/*															*/
/* Autor: ZKS												*/
/*															*/
/*															*/
/* Versionsinfos:											*/
/* 19.8.2020, Initial release V1							*/
/* 8.9.2020, Code f�r Nokia Display dazugef�gt      (.2)    */
/************************************************************/ 
#define ZKSLIBDISPLAY 202105

#include <stdint.h>

#define ASCII_CR 0x0d
#define ASCII_LF 0x0a

#ifdef DISP_MEGACARD

/* Defines f�r das Megacard LC Display */
#define DISP_PORTENRS PORTA
#define DISP_PORTDATA PORTB

#define DISP_RS				6
#define DISP_EN				4
#define DISP_DB4				2
#define DISP_DB5				5
#define DISP_DB6				6
#define DISP_DB7				7

#define DISP_LINES 2
#define DISP_COLS 8
#define DISP_LEN (DISP_LINES*DISP_COLS)

#endif

#ifdef DISP_NOKIA

/* Defines f�r das Nokia LC Display */
#define NOKIA_SPI_CHANNEL 0
#define NOKIA_CONTROL_PORT PORTD
#define NOKIA_DDR_PORT DDRD
#define NOKIA_CD_BIT 5
#define NOKIA_CS_BIT 6
#define NOKIA_RST_BIT 7


#define DISP_LINES 6
#define DISP_COLS 9
#define DISP_LEN (DISP_LINES*DISP_COLS)

#endif


//HW Unabh�ngige Funktionsprototypen f�r die Display-Steuerung
//Diese Funktionen sind immer gleich, unabh�ngig vom Typ des Displays.

// Konfigurieren des Displays und der Leitungen
void display_Init(void);

// Den Cursor an die Position (0,0) verschieben
void display_Home(void);

// Den Cursor an eine beliebige Position im Display verschieben
// Bei �berschreiten der Limite bleibt der Cursor unver�ndert
// X-Position von links nach rechts, y-Position von oben nach unten
// Z�hler beginnt bei 0
void display_Pos(unsigned char x, unsigned char y);

// Das Display l�schen und den Cursor an die Position (0,0) verschieben
void display_Clear();

// Ein Zeichen an der aktuellen Cursor-Position ausgeben und
// den Cursor um eine Stelle weiterr�cken. 
// Bei Erreichen der letzten Stelle wird das Display gescrollt.
void display_CharToDisplay (uint8_t c);

// Debugging only
void display_Test(void);


// Eine 8-Bit Zahl als Wert mit drei Ziffern (000 .. 255) an der aktuellen Cursor-Position ausgeben.
//void lcd_BinToDisplay(unsigned char x);

// Eine 8-Bit Zahl als Wert mit N Ziffern (000 .. 255) an der aktuellen Cursor-Position ausgeben.
// Ist die �bergebene Zahl gr��er als der m�gliche Zahlenbereich, werden die f�hrenden Stellen abgeschnitten.
//void lcd_BinToDisplayN(unsigned char x, char N);

// Eine beliebige Zahl als Wert mit N Ziffern an der aktuellen Cursor-Position ausgeben.
// Ist die �bergebene Zahl gr��er als der m�gliche Zahlenbereich, werden die f�hrenden Stellen abgeschnitten.
// Es wird ein entsprechendes Vorzeichen mit ausgegeben
void display_IntToDisplay(int32_t x, char N);

// Eine positive Zahl als Wert mit N Ziffern an der aktuellen Cursor-Position ausgeben.
// Ist die �bergebene Zahl gr��er als der m�gliche Zahlenbereich, werden die f�hrenden Stellen abgeschnitten.
void display_UintToDisplay(uint32_t x, char N);

// Einen Text der als array of char �bergeben wird an der aktuellen Cursor-Position ausgeben.
// txt: das ist der Text der ausgegeben wird. Es kann eine Variable sein oder direkt eine Zeichenfolge mit Anf�hrungsstrichen
// len: Die Anzahl Zeichen die ausgegeben werden sollen.
// Zeilenumbruch und Scrollen
void display_TxtToDisplay(char *txt, unsigned char len);


