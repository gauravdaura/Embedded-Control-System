//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : MOSC.C
// FILE VERSION : 1.0
// PROGRAMMER   : [Student Name]
//----------------------------------------------------------------------------
// REVISION HISTORY
//----------------------------------------------------------------------------
//
// 1.0, 2019-09-19, [Student Name]
//   - Initial release
//
//----------------------------------------------------------------------------
// MODULE DESCRIPTION
//----------------------------------------------------------------------------
//
// Main oscillator configuration.
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include "global.h"
#include "hwdefs.h"
#include "mosc.h"

//----------------------------------------------------------------------------
// GLOBAL VARIABLES
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// FUNCTION : MOSC_Init( void )
// PURPOSE  : Initializes the main oscillator
//----------------------------------------------------------------------------

void MOSC_Init( void )
{
    uint32_t uiRCC, uiRCC2;

    uiRCC2 = HWREG( SYSCTL_RCC2 );      // Read current value of the RCC2 register
    uiRCC2 |= ( uint32_t )( 1 << 31 );  // Set bit 31 (USERCC2) - use a type cast to suppress
    uiRCC2 &= ~( 1 << 13 );             // Clear bit 13 (PWRDN2)
    uiRCC2 &= ~( 0x07 << 4 );           // Clear bits 6:4 (OSCSRC2)
    HWREG( SYSCTL_RCC2 ) = uiRCC2;      // Update the RCC2 register

    uiRCC = HWREG( SYSCTL_RCC );        // Read current value of the RCC register
    uiRCC &= ~( 1 << 13 );              // Clear bit 13 (PWRDN)
    uiRCC &= ~( 0x1F << 6 );            // Clear bits 10:6 (XTAL)
    uiRCC |= ( 0x15 << 6 );             // Configure for a 16 MHz crystal
    uiRCC &= ~( 0x03 << 4 );            // Clear bits 5:4 (OSCSRC)
    HWREG( SYSCTL_RCC ) = uiRCC;        // Update the RCC register

    while( !( HWREG( SYSCTL_PLLSTAT ) & 1 ) );  // Wait until the PLL locks

    uiRCC &= ~( 0x0F << 23 );           // Clear bits 26:23 (SYSDIV)
    uiRCC |= ( 2 << 23 );               // Update SYSDIV
    uiRCC |= ( 1 << 22 );               // Set bit 22 (USESYSDIV)
    uiRCC &= ~( 1 << 11 );              // Clear bit 11 (BYPASS)
    uiRCC &= ~( 1 << 0 );               // Clear bit 0 (MOSCDIS)
    HWREG( SYSCTL_RCC ) = uiRCC;        // Update the RCC register

    uiRCC2 |= ( 1 << 30 );              // Set bit 30 (DIV400)
    uiRCC2 &= ~( 0x7F << 22 );          // Clear bits 28:22 (SYSDIV2:SYSDIV2LSB)
    uiRCC2 |= ( 4 << 22 );              // Update SYSDIV2
    uiRCC2 &= ~( 1 << 11 );             // Clear bit 11 (BYPASS2)
    HWREG( SYSCTL_RCC2 ) = uiRCC2;      // Update the RCC2 register

    return;
}

//----------------------------------------------------------------------------
// END MOSC.C
//----------------------------------------------------------------------------
