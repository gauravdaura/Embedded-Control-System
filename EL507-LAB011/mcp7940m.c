//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : MCP7940M.C
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
// Inter-integrated circuit interface.
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include "global.h"
#include "hwdefs.h"
#include "i2c.h"
#include "mcp7940m.h"

//----------------------------------------------------------------------------
// EXTERNAL REFERENCES
//----------------------------------------------------------------------------

extern uint32_t g_uiSYSFLAGS;

//----------------------------------------------------------------------------
// FUNCTION : MCP7940M_Init( void )
// PURPOSE  : Initializes the MCP7940M RTC
//----------------------------------------------------------------------------

void MCP7940M_Init( void )
{
    uint8_t uiData;

    uiData = 0x80; // Seconds
    MCP7940M_Write( 0x00, &uiData, 1 );

    uiData = 0x30; // Minute
    MCP7940M_Write( 0x01, &uiData, 1 );

    uiData = 0x51; // Hour
    MCP7940M_Write( 0x02, &uiData, 1 );

    uiData = 0x02; // Weekday
    MCP7940M_Write( 0x03, &uiData, 1 );

    uiData = 0x07; // Day
    MCP7940M_Write( 0x04, &uiData, 1 );

    uiData = 0x12; // Month
    MCP7940M_Write( 0x05, &uiData, 1 );

    uiData = 0x20; // Year
    MCP7940M_Write( 0x06, &uiData, 1 );

    uiData = 0x43; // Control
    MCP7940M_Write( 0x07, &uiData, 1 );

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : MCP7940M_Sequencer( uint8_t uiCommand, uint8_t* puiData, uint8_t uiNumBytes )
// PURPOSE  : Sequences access to the MCP7940M RTC
//----------------------------------------------------------------------------

uint8_t MCP7940M_Sequencer( uint8_t uiCommand, uint8_t* puiData, uint8_t uiNumBytes )
{
    static uint8_t* auiData = 0;
    static uint8_t uiIndex = 0;
    static uint8_t uiBytesToTransfer = 0;
    static uint8_t uiRW = 0;
    static uint8_t uiState = MCP7940M_STATE_IDLE;

    switch( uiCommand )
    {
        case MCP7940M_CMD_RESET:
            uiState = MCP7940M_STATE_IDLE;
            auiData = 0;
            uiIndex = 0;
            uiBytesToTransfer = 0;
            uiRW = 0;
            break;
        case MCP7940M_CMD_WRITE:
            uiCommand = MCP7940M_CMD_PROCESS;
            uiState = MCP7940M_STATE_START;
            auiData = puiData;
            uiIndex = 0;
            uiBytesToTransfer = uiNumBytes;
            uiRW = 0;
            break;
        case MCP7940M_CMD_READ:
            uiCommand = MCP7940M_CMD_PROCESS;
            uiState = MCP7940M_STATE_START;
            auiData = puiData;
            uiIndex = 0;
            uiBytesToTransfer = uiNumBytes;
            uiRW = 1;
            break;
        default:
            break;
    }

    // Process I2C transaction phase
    if( uiCommand == MCP7940M_CMD_PROCESS )
    {
        switch( uiState )
        {
            case MCP7940M_STATE_IDLE:
                uiIndex = 0;
                break;
            case MCP7940M_STATE_START:

                // Ensure the bus is idle
                I2C_WaitForBusReady();

                // Write slave address and R/W = 0
                HWREG( I2C0_BASE + I2C_O_MSA ) = ( MCP7940M_SA << 1 );

                // Write register address to the transmit register
                HWREG( I2C0_BASE + I2C_O_MDR ) = auiData[ uiIndex++ ];

                // Initiate I2C transaction
                HWREG( I2C0_BASE + I2C_O_MCS ) = I2C_MCS_RUN | I2C_MCS_START;

                // Prepare to wait for completion of transaction phase
                uiState = MCP7940M_STATE_START_WFR;

                break;
            case MCP7940M_STATE_START_WFR:

                // Wait for controller ready
                if( I2C_IsControllerReady() )
                {
                    // Halt if the slave did not acknowledge its address
                    if( ( HWREG( I2C0_BASE + I2C_O_MCS ) & I2C_MCS_ADRACK ) )
                    {
                        while( 1 );
                    }

                    if( uiRW & 0x01 )
                    {
                        uiState = MCP7940M_STATE_RESTART;
                        uiState = MCP7940M_Sequencer( MCP7940M_CMD_PROCESS, 0, 0 );
                    }
                    else
                    {
                        uiState = MCP7940M_STATE_DATA_WRITE;
                        uiState = MCP7940M_Sequencer( MCP7940M_CMD_PROCESS, 0, 0 );
                    }
                }

                break;
            case MCP7940M_STATE_RESTART:

                // Wait for bus ready
                //if( I2C_IsBusReady() )
                {
                    // Write slave address and R/W
                    HWREG( I2C0_BASE + I2C_O_MSA ) = ( MCP7940M_SA << 1 ) | 0x01;

                    // Continue with the I2C transaction
                    HWREG( I2C0_BASE + I2C_O_MCS ) = I2C_MCS_RUN | I2C_MCS_START | ( ( uiBytesToTransfer - uiIndex ) == 1 ? I2C_MCS_STOP : I2C_MCS_ACK );

                    // Prepare to wait for completion of transaction phase
                    uiState = MCP7940M_STATE_RESTART_WFR;
                }

                break;
            case MCP7940M_STATE_RESTART_WFR:

                // Wait for controller ready
                if( I2C_IsControllerReady() )
                {
                    // Halt if the slave did not acknowledge its address
                    if( ( HWREG( I2C0_BASE + I2C_O_MCS ) & I2C_MCS_ADRACK ) )
                    {
                        while( 1 );
                    }

                    // Read data from slave
                    auiData[ uiIndex++ ] = HWREG( I2C0_BASE + I2C_O_MDR );

                    // Prepare to continue
                    uiState = ( uiBytesToTransfer - uiIndex ) ? MCP7940M_STATE_DATA_READ : MCP7940M_STATE_IDLE;
                    uiState = MCP7940M_Sequencer( MCP7940M_CMD_PROCESS, 0, 0 );
                }

                break;
            case MCP7940M_STATE_DATA_WRITE:

                // Write data
                HWREG( I2C0_BASE + I2C_O_MDR ) = auiData[ uiIndex++ ];

                // Continue with the I2C transaction
                HWREG( I2C0_BASE + I2C_O_MCS ) = I2C_MCS_RUN | ( uiBytesToTransfer - uiIndex ? 0 : I2C_MCS_STOP );

                // Prepare to wait for completion of transaction phase
                uiState = MCP7940M_STATE_DATA_WRITE_WFR;

                break;
            case MCP7940M_STATE_DATA_WRITE_WFR:

                // Wait for controller ready
                if( I2C_IsControllerReady() )
                {
                    // Halt if the slave did not acknowledge the data
                    if( ( HWREG( I2C0_BASE + I2C_O_MCS ) & I2C_MCS_DATACK ) )
                    {
                        while( 1 );
                    }

                    uiState = ( uiBytesToTransfer - uiIndex ) ? MCP7940M_STATE_DATA_WRITE : MCP7940M_STATE_IDLE;
                    uiState = MCP7940M_Sequencer( MCP7940M_CMD_PROCESS, 0, 0 );
                }

                break;
            case MCP7940M_STATE_DATA_READ:

                // Continue with the I2C transaction
                HWREG( I2C0_BASE + I2C_O_MCS ) = I2C_MCS_RUN | ( ( uiBytesToTransfer - uiIndex ) == 1 ? I2C_MCS_STOP : I2C_MCS_ACK );

                // Prepare to wait for completion of transaction phase
                uiState = MCP7940M_STATE_DATA_READ_WFR;

                break;
            case MCP7940M_STATE_DATA_READ_WFR:

                // Wait for controller ready
                if( I2C_IsControllerReady() )
                {
                    // Read data from slave
                    auiData[ uiIndex++ ] = HWREG( I2C0_BASE + I2C_O_MDR );

                    // Prepare to continue
                    uiState = ( uiBytesToTransfer - uiIndex ) ? MCP7940M_STATE_DATA_READ : MCP7940M_STATE_IDLE;
                    uiState = MCP7940M_Sequencer( MCP7940M_CMD_PROCESS, 0, 0 );
                }

                break;
            default:
                uiState = MCP7940M_STATE_IDLE;
                uiState = MCP7940M_Sequencer( MCP7940M_CMD_PROCESS, 0, 0 );
                break;
        }
    }

    return uiState;
}

//----------------------------------------------------------------------------
// FUNCTION : MCP7940M_Read( uint8_t uiReg, uint8_t* puiData, uint8_t uiNum )
// PURPOSE  : Reads one register from the MCP7940M RTC
//----------------------------------------------------------------------------

void MCP7940M_Read( uint8_t uiReg, uint8_t* puiData, uint8_t uiNum )
{
    return;
}

//----------------------------------------------------------------------------
// FUNCTION : MCP7940M_Write( uint8_t uiRegAddr, uint8_t* puiData, uint8_t uiNumRegs )
// PURPOSE  : Writes one register to the MCP7940M RTC
//----------------------------------------------------------------------------

void MCP7940M_Write( uint8_t uiRegAddr, uint8_t* puiData, uint8_t uiNumRegs )
{
    uint8_t uiMCS;
    uint8_t uiRegsWritten = 0;

    // Ensure the bus is idle
    I2C_WaitForBusReady();

    if( uiNumRegs )
    {
        // Write slave address and R/W = 0
        HWREG( I2C0_BASE + I2C_O_MSA ) = ( MCP7940M_SA << 1 );

        // Write register address to the transmit register
        HWREG( I2C0_BASE + I2C_O_MDR ) = uiRegAddr;

        // Initiate I2C transaction
        uiMCS = I2C_MCS_RUN | I2C_MCS_START;
        HWREG( I2C0_BASE + I2C_O_MCS ) = uiMCS;

        // Wait until the controller is no longer busy
        I2C_WaitForControllerReady();

        do
        {
            // Write register data
            HWREG( I2C0_BASE + I2C_O_MDR ) = *puiData++;

            // Continue with the I2C transaction
            uiMCS = I2C_MCS_RUN | ( ( uiNumRegs - uiRegsWritten ) == 1 ? I2C_MCS_STOP : 0 );
            HWREG( I2C0_BASE + I2C_O_MCS ) = uiMCS;

            // Wait until the controller is no longer busy
            I2C_WaitForControllerReady();

            // Another register written
            uiRegsWritten++;

        } while( uiNumRegs - uiRegsWritten );
    }

    return;
}

//----------------------------------------------------------------------------
// END MCP7940M.C
//----------------------------------------------------------------------------
