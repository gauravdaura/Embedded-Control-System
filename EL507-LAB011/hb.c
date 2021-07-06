//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : HB.C
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
// Generates the heartbeat sequencing for the system. The HB_Sequencer()
// function must be called once every millisecond.
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include "hb.h"
#include "led.h"
#include "pcf8574a.h"

//----------------------------------------------------------------------------
// EXTERNAL REFERENCES
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// FUNCTION : HB_Sequencer( bool bReset )
// PURPOSE  : Sequences the heartbeat LED
//----------------------------------------------------------------------------

void HB_Sequencer( bool bReset )
{
    // State tracking
    static uint8_t uiState = HB_STATE_HB_ON;

    // Timer for LED duration and pauses
    static uint16_t uiTimer = 0;

    // Data for remote LEDs
    uint8_t uiData;

    // Reset all internal variables to known values
    if( bReset )
    {
        uiState = HB_STATE_HB_ON;
        uiTimer = 0;
    }

    // State sequencer
    switch( uiState )
    {
    case HB_STATE_HB_ON:
        if( uiTimer == 0 )
        {
            LED_Control( 1 );

            PCF8574A_Read( PCF8574A_SA, &uiData );
            uiData &= ~PCF8574A_LED4;
            PCF8574A_Write( PCF8574A_SA, &uiData );
        }
        if( ++uiTimer >= HB_DELAY_HB_ON )
        {
            uiState = HB_STATE_HB_OFF;
            uiTimer = 0;
        }
        break;
    case HB_STATE_HB_OFF:
        if( uiTimer == 0 )
        {
            LED_Control( 0 );

            PCF8574A_Read( PCF8574A_SA, &uiData );
            uiData |= PCF8574A_LED4;
            PCF8574A_Write( PCF8574A_SA, &uiData );
        }
        if( ++uiTimer >= HB_DELAY_HB_OFF )
        {
            uiState = HB_STATE_HB_ON;
            uiTimer = 0;
        }
        break;
    default:
        uiState = HB_STATE_HB_ON;
        LED_Control( 0 );
        break;
    }

    return;
}

//----------------------------------------------------------------------------
// END HB.C
//----------------------------------------------------------------------------
