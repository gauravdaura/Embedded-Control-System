//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : MAIN.C
// FILE VERSION : 1.0
// PROGRAMMER   : Programmer Name
//----------------------------------------------------------------------------
// REVISION HISTORY
//----------------------------------------------------------------------------
//
// 1.0, YYYY-MM-DD, Programmer Name
//   - Initial release
//
//----------------------------------------------------------------------------
// MODULE DESCRIPTION
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "global.h"
#include "mosc.h"
#include "motor.h"
#include "systick.h"
#include "lcd.h"
#include "led.h"
#include "hb.h"
#include "i2c.h"
#include "pcf8574a.h"
#include "mcp7940m.h"

#include "hwdefs.h"

//----------------------------------------------------------------------------
// SYSTEM FLAGS
//----------------------------------------------------------------------------

uint32_t g_uiSYSFLAGS;
uint8_t  g_aData[ 97 ];
char     g_sBuffer[ 80 ];

//----------------------------------------------------------------------------
// GLOBAL VARIABLES
//----------------------------------------------------------------------------

uint32_t g_uiSYSFLAGS;

//----------------------------------------------------------------------------
// FUNCTION : Initialize( void )
// PURPOSE  : Initialize embedded system
//----------------------------------------------------------------------------

void Initialize( void )
{
    g_uiSYSFLAGS = 0;

    MOTOR_Init();
    MOSC_Init();
    SYSTICK_Init();

    LCD_Init();

    I2C_Init();
    PCF8574A_Init();
    MCP7940M_Init();

    LED_Init();
    HB_Sequencer( 1 );

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : main( void )
// PURPOSE  : Main program entry point - thread mode
//----------------------------------------------------------------------------

#define MAIN_UPDATERATE_TIME 1000
#define MAIN_UPDATERATE_LED5  150
#define MAIN_UPDATERATE_LED6   93

void main( void )
{
    Initialize();
    LCD_SendMessage( "Hello QEI!" );

    float fDutyCycle    = 0.0f;
    bool  bDirection    = 0;
    bool  bAcceleration = 1;

    while( 1 )
    {
        MOTOR_SetDutyCycle( fDutyCycle, bDirection );
        SYSTICK_Delay( 3 );
        if( bAcceleration )
        {
            if( fDutyCycle < 0.99f )
            {
              fDutyCycle += 0.01f;
            }
            else
            {
              fDutyCycle = 1.0f;
              bAcceleration = 0;
            }
        }
        else
        {
            if( fDutyCycle > 0.0f )
            {
              fDutyCycle -= 0.01f;
            }
            else
            {
              fDutyCycle = 0.0f;
              bAcceleration = 1;
              bDirection = !bDirection;
            }
        }
    }
}

//----------------------------------------------------------------------------
// END MAIN.C
//----------------------------------------------------------------------------
