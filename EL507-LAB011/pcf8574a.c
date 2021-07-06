//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : PCF8574A.C
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
// PCF8574A Remote 8-Bit I/O Expander Interface.
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include "global.h"
#include "hwdefs.h"
#include "i2c.h"
#include "pcf8574a.h"

//----------------------------------------------------------------------------
// EXTERNAL REFERENCES
//----------------------------------------------------------------------------

extern uint32_t g_uiSYSFLAGS;

//----------------------------------------------------------------------------
// FUNCTION : PCF8574A_Init( void )
// PURPOSE  : Initializes the PCF8574A
//----------------------------------------------------------------------------

void PCF8574A_Init( void )
{
    uint8_t uiData;

    // Configure LEDs and switches
    uiData = 0xE7;
    PCF8574A_Write( PCF8574A_SA, &uiData );

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : PCF8574A_Read( uint8_t uiSA, uint8_t* puiData )
// PURPOSE  : Reads data from the PCF8574A
//----------------------------------------------------------------------------

void PCF8574A_Read( uint8_t uiSA, uint8_t* puiData )
{
    uint8_t uiMCS;

    // Ensure the bus is idle
    I2C_WaitForBusReady();

    // Write slave address and R/W = 1
    HWREG( I2C0_BASE + I2C_O_MSA ) = ( uiSA << 1 ) | 0x01;

    // Initiate I2C transaction
    uiMCS = I2C_MCS_RUN | I2C_MCS_START | I2C_MCS_STOP;
    HWREG( I2C0_BASE + I2C_O_MCS ) = uiMCS;

    // Wait until the controller is no longer busy
    I2C_WaitForControllerReady();

    // Read data from receive register
    *puiData = HWREG( I2C0_BASE + I2C_O_MDR );

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : PCF8574A_Write( uint8_t uiSA, uint8_t* puiData )
// PURPOSE  : Writes data to the PCF8574A
//----------------------------------------------------------------------------

void PCF8574A_Write( uint8_t uiSA, uint8_t* puiData )
{
    uint8_t uiMCS;

    // Ensure the bus is idle
    I2C_WaitForBusReady();

    // Write slave address and R/W = 0
    HWREG( I2C0_BASE + I2C_O_MSA ) = ( uiSA << 1 );

    // Write data to transmit register
    HWREG( I2C0_BASE + I2C_O_MDR ) = *puiData | PCF8574A_INPUTS;

    // Initiate I2C transaction
    uiMCS = I2C_MCS_RUN | I2C_MCS_START | I2C_MCS_STOP;
    HWREG( I2C0_BASE + I2C_O_MCS ) = uiMCS;

    // Wait until the controller is no longer busy
    I2C_WaitForControllerReady();

    return;
}

//----------------------------------------------------------------------------
// END PCF8574A.C
//----------------------------------------------------------------------------
