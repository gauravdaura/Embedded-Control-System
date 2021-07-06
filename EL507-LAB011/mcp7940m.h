//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : MCP7940M.H
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
// INCLUSION LOCK
//----------------------------------------------------------------------------

#ifndef MCP7940M_H_
#define MCP7940M_H_

//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include <stdint.h>
#include <stdbool.h>

#include "i2c.h"

//----------------------------------------------------------------------------
// MACROS
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// STRUCTURES
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//----------------------------------------------------------------------------

void    MCP7940M_Init( void );
uint8_t MCP7940M_Sequencer( uint8_t uiCommand, uint8_t* puiData, uint8_t uiNumBytes );
void    MCP7940M_Read( uint8_t uiReg, uint8_t* puiData, uint8_t uiNum );
void    MCP7940M_Write( uint8_t uiReg, uint8_t* puiData, uint8_t uiNum );

//----------------------------------------------------------------------------
// CONSTANTS
//----------------------------------------------------------------------------

#define MCP7940M_SA 0x6f

enum MCP7940M_COMMANDS
{
    MCP7940M_CMD_RESET = 0,
    MCP7940M_CMD_WRITE,
    MCP7940M_CMD_READ,
    MCP7940M_CMD_PROCESS
};

enum MCP7940M_STATES
{
    MCP7940M_STATE_IDLE = 0,
    MCP7940M_STATE_START,
    MCP7940M_STATE_START_WFR,
    MCP7940M_STATE_RESTART,
    MCP7940M_STATE_RESTART_WFR,
    MCP7940M_STATE_DATA_WRITE,
    MCP7940M_STATE_DATA_WRITE_WFR,
    MCP7940M_STATE_DATA_READ,
    MCP7940M_STATE_DATA_READ_WFR
};

#endif // MCP7940M_H_

//----------------------------------------------------------------------------
// END MCP7940M.H
//----------------------------------------------------------------------------
