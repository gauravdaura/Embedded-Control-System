//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : HWDEFS.H
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

#ifndef HWDEFS_H_
#define HWDEFS_H_

//----------------------------------------------------------------------------
// CONSTANTS
//----------------------------------------------------------------------------

#define SYSCTL_RCC      0x400FE060 // hw_sysctl
#define SYSCTL_RCGCGPIO 0x400FE608 // hw_sysctl
#define SYSCTL_RCGCI2C  0x400FE620 // hw_sysctl
#define SYSCTL_RCGCPWM  0x400FE640 // hw_sysctl

#define SYSCTL_RCC      0x400FE060 // hw_sysctl
#define SYSCTL_RCC2     0x400FE070 // hw_sysctl
#define SYSCTL_PLLSTAT  0x400FE168 // hw_sysctl

#define GPIO_PORTA_BASE 0x40004000 // hw_memmap
#define GPIO_PORTB_BASE 0x40005000 // hw_memmap
#define GPIO_PORTD_BASE 0x40007000 // hw_memmap
#define GPIO_PORTF_BASE 0x40025000 // hw_memmap

#define GPIO_O_DATA     0x00000000 // hw_gpio
#define GPIO_O_DIR      0x00000400 // hw_gpio
#define GPIO_O_AFSEL    0x00000420 // hw_gpio
#define GPIO_O_ODR      0x0000050C // hw_gpio
#define GPIO_O_DEN      0x0000051C // hw_gpio
#define GPIO_O_PCTL     0x0000052C // hw_gpio


#define I2C0_BASE       0x40020000 // hw_memmap

#define I2C_O_MSA       0x00000000 // hw_i2c
#define I2C_O_MCS       0x00000004 // hw_i2c
#define I2C_O_MDR       0x00000008 // hw_i2c
#define I2C_O_MTPR      0x0000000C // hw_i2c
#define I2C_O_MIMR      0x00000010 // hw_i2c
#define I2C_O_MRIS      0x00000014 // hw_i2c
#define I2C_O_MMIS      0x00000018 // hw_i2c
#define I2C_O_MICR      0x0000001C // hw_i2c
#define I2C_O_MCR       0x00000020 // hw_i2c
#define I2C_O_MCLKOCNT  0x00000024 // hw_i2c
#define I2C_O_MBMON     0x0000002C // hw_i2c
#define I2C_O_MCR2      0x00000038 // hw_i2c


#define PWM0_BASE       0x40028000 // hw_memmap

#define PWM_O_ENABLE    0x00000008 // hw_pwm
#define PWM_O_0_CTL     0x00000040 // hw_pwm
#define PWM_O_0_GENA    0x00000060 // hw_pwm
#define PWM_O_0_GENB    0x00000064 // hw_pwm
#define PWM_O_0_LOAD    0x00000050 // hw_pwm
#define PWM_O_0_CMPB    0x0000005C // hw_pwm
#define PWM_O_0_CMPA    0x00000058 // hw_pwm


#define NVIC_ST_CTRL    0xE000E010 // hw_nvic
#define NVIC_ST_RELOAD  0xE000E014 // hw_nvic
#define NVIC_ST_CURRENT 0xE000E018 // hw_nvic

#define NVIC_EN0        0xE000E100 // hw_nvic
#define NVIC_DIS0       0xE000E180 // hw_nvic

#define HWREG( x ) ( *( ( volatile uint32_t * )( x ) ) )
#endif // HWDEFS_H_

//----------------------------------------------------------------------------
// END HWDEFS.H
//----------------------------------------------------------------------------
