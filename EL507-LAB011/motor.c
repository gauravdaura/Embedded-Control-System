//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : MOTOR.C
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
// Configures the main oscillator.
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include "hwdefs.h"
#include "motor.h"

//----------------------------------------------------------------------------
// EXTERNAL REFERENCES
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// FUNCTION : MOTOR_Init( void )
// PURPOSE  : Initialize the motor interface
//----------------------------------------------------------------------------

void MOTOR_Init( void )
{
    // Configure the Pulse Width Modulation peripheral
    HWREG( SYSCTL_RCGCPWM )  |= 0x00000001; // Enable Clock for PWM 0
    HWREG( SYSCTL_RCGCGPIO ) |= 0x00000002; // Enable Clock for Port B

    // Configure PB6 and PB7 for PWM 0
    HWREG( GPIO_PORTB_BASE + GPIO_O_DEN ) |= 0xC0;
    HWREG( GPIO_PORTB_BASE + GPIO_O_DIR ) &= ~0xC0;
    HWREG( GPIO_PORTB_BASE + GPIO_O_AFSEL ) |= 0xC0;
    HWREG( GPIO_PORTB_BASE + GPIO_O_PCTL ) &= 0x00FFFFFF;
    HWREG( GPIO_PORTB_BASE + GPIO_O_PCTL ) |= 0x44000000;
    HWREG( SYSCTL_RCC ) &= ~0x001E0000;
    HWREG( SYSCTL_RCC ) |= 0x00100000;

    // System Clock / 2 = 40 MHz
    // Clear PWM0 Control Register
    HWREG( PWM0_BASE + PWM_O_0_CTL ) = 0;
    HWREG( PWM0_BASE + PWM_O_0_LOAD ) = 1000-1; // Set Period to 40 kHz (40 MHz / 1000)

    // Start with the motor off and ready for forward direction
    MOTOR_SetDutyCycle( 0.0f, 1 );

    // Enable PWM0 outputs
    HWREG( PWM0_BASE + PWM_O_0_CTL ) = 0x00000001;
    HWREG( PWM0_BASE + PWM_O_ENABLE ) = 0x00000003;

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : MOTOR_SetDutyCycle( float fMotorSpeed, bool bMotorDir )
// PURPOSE  : Sets the motor speed by changing the duty cycle
//----------------------------------------------------------------------------

void MOTOR_SetDutyCycle( float fMotorSpeed, bool bMotorDir )
{
    uint16_t uiPulse;
    uint16_t uiBSH = 50;

    // Bootstrap (High Side)
    uint16_t uiPulseMax = ( uint16_t )( HWREG( PWM0_BASE + PWM_O_0_LOAD ) );

    // Set Direction
    if( bMotorDir )
    {
        HWREG( PWM0_BASE + PWM_O_0_GENA ) = 0x00000022;
        HWREG( PWM0_BASE + PWM_O_0_GENB ) = 0x000000C2;
    }
    else
    {
        HWREG( PWM0_BASE + PWM_O_0_GENA ) = 0x000000C2;
        HWREG( PWM0_BASE + PWM_O_0_GENB ) = 0x00000022;
    }

    // Verify minimum and maximum
    fMotorSpeed = fMotorSpeed < 0.0f ? 0.0f : fMotorSpeed;
    fMotorSpeed = fMotorSpeed > 1.0f ? 1.0f : fMotorSpeed;

    // Calculate pulse width
    uiPulse = ( uint16_t )( uiPulseMax * fMotorSpeed + 0.5f );

    // Limit maximum pulse width
    if( uiPulse > ( uiPulseMax - uiBSH ) ) uiPulse = uiPulseMax - uiBSH;

    // Set motor speed
    HWREG( PWM0_BASE + PWM_O_0_CMPA ) = uiPulse;

    return;
}

//----------------------------------------------------------------------------
// END MOTOR.C
//----------------------------------------------------------------------------
