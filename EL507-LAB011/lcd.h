//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : LCD.H
// FILE VERSION : 1.0
// PROGRAMMER   : Programmer Name
//----------------------------------------------------------------------------
// REVISION HISTORY
//----------------------------------------------------------------------------
//
// 1.0, YYYY-MM-DD, Programmer Name
// - Initial release
//
//----------------------------------------------------------------------------
// INCLUSION LOCK
//----------------------------------------------------------------------------

#ifndef LCD_H_
#define LCD_H_

//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include <stdint.h>
#include <stdbool.h>

//----------------------------------------------------------------------------
// CONSTANTS
//----------------------------------------------------------------------------

// Port A (Data)
#define LCD_BUS ( 0x80 | 0x40 | 0x20 | 0x10 )

// Port E (Control)
#define LCD_RS  0x08
#define LCD_RW  0x04
#define LCD_E   0x02
#define LCD_PWR 0x01

#define LCD_CONTROL ( LCD_RS | LCD_RW | LCD_E | LCD_PWR )

// Instruction Codes
#define LCD_IC_CLEAR       0x01 // Clear Display
#define LCD_IC_HOME        0x02 // Return Home
#define LCD_IC_ENTRYMODE   0x04 // Entry Mode Set
#define LCD_IC_DISPLAY     0x08 // Display On/Off Control
#define LCD_IC_CURSORSHIFT 0x10 // Cursor or Display Shift
#define LCD_IC_FUNCTION    0x20 // Function Set
#define LCD_IC_CGRAMADDR   0x40 // Set CGRAM Address
#define LCD_IC_DDRAMADDR   0x80 // Set DDRAM Address

// Instruction Code Bits
#define LCD_IC_ENTRYMODE_INC       0x02
#define LCD_IC_ENTRYMODE_SHIFT     0x01

#define LCD_IC_DISPLAY_ON          0x04
#define LCD_IC_DISPLAY_CURSOR      0x02
#define LCD_IC_DISPLAY_BLINK       0x01

#define LCD_IC_CURSORSHIFT_DISPLAY 0x08
#define LCD_IC_CURSORSHIFT_RIGHT   0x04

#define LCD_IC_FUNCTION_8BIT       0x10
#define LCD_IC_FUNCTION_2LINE      0x08
#define LCD_IC_FUNCTION_ON         0x04

#define LCD_IC_STATUS_BUSY         0x80

//----------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//----------------------------------------------------------------------------

// LCD Functions
void LCD_WriteNibble( uint8_t uiData );
void LCD_ReadNibble( uint8_t *puiData );

void LCD_IRWrite( uint8_t uiData );
void LCD_IRRead( uint8_t *puiData );

void LCD_DRWrite( uint8_t uiData );
void LCD_DRRead( uint8_t *puiData );

void LCD_WaitForReady( void );
void LCD_Init( void );
void LCD_SendMessage( uint8_t* sMessage );

#endif // LCD_H_

//----------------------------------------------------------------------------
// END LCD.H
//----------------------------------------------------------------------------
