//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : LED.C
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
// Controls the system LED.
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include "led.h"
#include "hwdefs.h"

//----------------------------------------------------------------------------
// EXTERNAL REFERENCES
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// FUNCTION : LED_Init( void )
// PURPOSE  : Initializes the system LED interface
//----------------------------------------------------------------------------

void LED_Init( void )
{
    // GPIO Port F Run Mode Clock Gating Control
    HWREG( SYSCTL_RCGCGPIO ) |= 0x20;

    // Configure PF1 as a digital signal
    HWREG( GPIO_PORTF_BASE + GPIO_O_DEN ) |= 0x0e;

    // Configure PF1 as an output
    HWREG( GPIO_PORTF_BASE + GPIO_O_DIR ) |= 0x0e;

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : LED_Control( bool bState )
// PURPOSE  : Controls the system LED
//----------------------------------------------------------------------------

void LED_Control( bool bState )
{
    if( bState )
    {
        // Turn on LED
        HWREG( GPIO_PORTF_BASE + GPIO_O_DATA + ( 0x02 << 2 ) ) = 0x02;
    }
    else
    {
        // Turn off LED
        HWREG( GPIO_PORTF_BASE + GPIO_O_DATA + ( 0x02 << 2 ) ) = 0;
    }

    return;
}

//----------------------------------------------------------------------------
// END LED.C
//----------------------------------------------------------------------------
