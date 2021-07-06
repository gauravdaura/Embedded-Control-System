;-----------------------------------------------------------------------------
; COMPANY      : Confederation College
; FILE         : LCD.ASM
; FILE VERSION : 1.0
; PROGRAMMER   : Programmer_Name
;-----------------------------------------------------------------------------
; REVISION HISTORY
;-----------------------------------------------------------------------------
;
; 1.0, YYYY-MM-DD, Programmer_Name
; - Initial release
;
;-----------------------------------------------------------------------------

            .global     LCD_WriteNibble
            .global     LCD_ReadNibble

            .global     LCD_IRWrite
            .global     LCD_IRRead

            .global     LCD_DRWrite
            .global     LCD_DRRead

            .global     LCD_WaitForReady
            .global     LCD_Init
            .global     LCD_SendMessage

            .global     SYSTICK_Delay

;-----------------------------------------------------------------------------
; REGISTER BASE AND OFFSET DEFINITIONS (EQUATES)
;-----------------------------------------------------------------------------

SYSCTL_BASE                 .equ    0x400fe000
SYSCTL_O_RCGCGPIO           .equ    0x608

GPIO_PORTA_BASE             .equ    0x40004000
GPIO_PORTE_BASE             .equ    0x40024000

GPIO_O_DATA                 .equ    0x000
GPIO_O_DIR                  .equ    0x400
GPIO_O_PUR                  .equ    0x510
GPIO_O_DEN                  .equ    0x51c

; Port A (Data)
LCD_DB7_DB3                 .equ    0x80
LCD_DB6_DB2                 .equ    0x40
LCD_DB5_DB1                 .equ    0x20
LCD_DB4_DB0                 .equ    0x10
LCD_BUS                     .equ    ( LCD_DB7_DB3 | LCD_DB6_DB2 | LCD_DB5_DB1 | LCD_DB4_DB0 )

; Port E (Control)
LCD_RS                      .equ    0x08
LCD_RW                      .equ    0x04
LCD_E                       .equ    0x02
LCD_PWR                     .equ    0x01
LCD_CONTROL                 .equ    ( LCD_RS | LCD_RW | LCD_E | LCD_PWR )

; Instruction Codes
LCD_IC_CLEAR                .equ    0x01 ; Clear Display
LCD_IC_HOME                 .equ    0x02 ; Return Home
LCD_IC_ENTRYMODE            .equ    0x04 ; Entry Mode Set
LCD_IC_DISPLAY              .equ    0x08 ; Display On/Off Control
LCD_IC_CURSORSHIFT          .equ    0x10 ; Cursor or Display Shift
LCD_IC_FUNCTION             .equ    0x20 ; Function Set
LCD_IC_CGRAMADDR            .equ    0x40 ; Set CGRAM Address
LCD_IC_DDRAMADDR            .equ    0x80 ; Set DDRAM Address

; Instruction Code Bits
LCD_IC_ENTRYMODE_INC        .equ    0x02
LCD_IC_ENTRYMODE_SHIFT      .equ    0x01

LCD_IC_DISPLAY_ON           .equ    0x04
LCD_IC_DISPLAY_CURSOR       .equ    0x02
LCD_IC_DISPLAY_BLINK        .equ    0x01

LCD_IC_CURSORSHIFT_DISPLAY  .equ    0x08
LCD_IC_CURSORSHIFT_RIGHT    .equ    0x04

LCD_IC_FUNCTION_8BIT        .equ    0x10
LCD_IC_FUNCTION_2LINE       .equ    0x08
LCD_IC_FUNCTION_ON          .equ    0x04

LCD_IC_STATUS_BUSY          .equ    0x80

;-----------------------------------------------------------------------------
; MACRO    : mov32
; PURPOSE  : Moves a 32-bit immediate value into the specified register
;-----------------------------------------------------------------------------

mov32       .macro      reg, imm
            movw        reg, #( imm & 0x0000ffff )
            movt        reg, #( imm >> 16 )
            .endm

;-----------------------------------------------------------------------------
; FUNCTION : LCD_WriteNibble
; PURPOSE  : Writes one nibble (bits 7-4) to the LCD module
;-----------------------------------------------------------------------------
;
; Description
;-------------------------------------
; This function writes a 4-bit value to the LCD module.
;
;
; Arguments:
;-------------------------------------
; R0: uiData (8-bits)
;
;
; Return:
;-------------------------------------
; None
;
;
			.text

LCD_WriteNibble:

    push    {lr, r0-r3}

    mov32   r2,GPIO_PORTA_BASE
    mov32   r3,GPIO_PORTE_BASE

    ; Bring E strobe high
    movw    r1,#LCD_E
    strb    r1,[r3,#( GPIO_O_DATA + LCD_E << 2 ) ]

    ; Make 4-bit bus outputs
    ldrb    r1,[r2,#GPIO_O_DIR]
    orr     r1,r1,#LCD_BUS
    strb    r1,[r2,#GPIO_O_DIR]

    ; Output nibble
    strb    r0,[r2,#( GPIO_O_DATA + LCD_BUS << 2 ) ]

    ; Delay to meet timing requirements
    nop
    nop
    nop
    nop

    nop
    nop
    nop
    nop

    nop
    nop
    nop
    nop

    nop

    ; Bring E strobe low
    movw    r1,#0
    strb    r1,[r3,#( GPIO_O_DATA + LCD_E << 2 ) ]

    ; Make 4-bit bus inputs
    ldrb    r1,[r2,#GPIO_O_DIR]
    bic     r1,r1,#LCD_BUS
    strb    r1,[r2,#GPIO_O_DIR]

    ; Delay to meet timing requirements
    nop

    pop     {pc, r0-r3}

;-----------------------------------------------------------------------------
; FUNCTION : LCD_ReadNibble
; PURPOSE  : Writes one nibble (bits 7-4) to the LCD module
;-----------------------------------------------------------------------------
;
; Description
;-------------------------------------
; This function writes a 4-bit value to the LCD module.
;
;
; Arguments:
;-------------------------------------
; R0: address to deposit read data
;
;
; Return:
;-------------------------------------
; None
;
;
            .text

LCD_ReadNibble:
    push    {lr, r0-r3}

    mov32   r2,GPIO_PORTA_BASE
    mov32   r3,GPIO_PORTE_BASE

    ; Bring E strobe high
    movw    r1,#LCD_E
    strb    r1,[r3,#( GPIO_O_DATA + LCD_E << 2 ) ]

    ; Delay to meet timing requirements
    nop
    nop
    nop
    nop

    nop
    nop
    nop
    nop

    nop
    nop
    nop
    nop

    nop
    nop
    nop
    nop

    nop

    ; Read data
    ldrb    r1,[r2,#( GPIO_O_DATA + LCD_BUS << 2 ) ]

    ; Bring E strobe low
    movw    r2,#0
    strb    r2,[r3,#( GPIO_O_DATA + LCD_E << 2 ) ]

    ; Merge to lower nibble
    lsr     r1,r1,#4
    and     r1,r1,#0x0f
    ldrb    r2,[r0]
    and     r2,r2,#0xf0
    orr     r1,r1,r2
    strb    r1,[r0]

    pop     {pc, r0-r3}

;-----------------------------------------------------------------------------
; FUNCTION : LCD_IRWrite
; PURPOSE  : Writes one byte to the instruction register of the LCD module
;-----------------------------------------------------------------------------
;
; Description
;-------------------------------------
; This function writes one 8-bit value using two calls to the
; LCD_WriteNibble function.
;
;
; Arguments:
;-------------------------------------
; R0: 8-bit data to write
;
;
; Return:
;-------------------------------------
; None
;
;
            .text

LCD_IRWrite:
    push    {lr, r0-r2}

    ; Control port
    mov32   r2,GPIO_PORTE_BASE

    ; Ensure LCD module is ready
    bl      LCD_WaitForReady

    ; RS = 0, R/W = 0
    movw    r1,#0
    strb    r1,[r2,#( GPIO_O_DATA + ( LCD_RS | LCD_RW ) << 2 ) ]

    ; Write 8-bit data
    bl      LCD_WriteNibble
    lsl     r0,r0,#4
    bl      LCD_WriteNibble

    pop     {pc, r0-r2}

;-----------------------------------------------------------------------------
; FUNCTION : LCD_IRRead
; PURPOSE  : Reads one byte from the instruction register of the LCD module
;-----------------------------------------------------------------------------
;
; Description
;-------------------------------------
; This function reads one 8-bit value using two calls to the
; LCD_ReadNibble function.
;
;
; Arguments:
;-------------------------------------
; R0: address of 8-bit data where data will be placed
;
;
; Return:
;-------------------------------------
; None
;
;
            .text

LCD_IRRead:
    push    {lr, r0-r2}

    ; Control port
    mov32   r2,GPIO_PORTE_BASE

    ; RS = 0, R/W = 1
    movw    r1,#LCD_RW
    strb    r1,[r2,#( GPIO_O_DATA + ( LCD_RS | LCD_RW ) << 2 ) ]

    ; Read 8-bit data
    bl      LCD_ReadNibble
    ldrb    r1,[r0]
    lsl     r1,r1,#4
    strb    r1,[r0]
    bl      LCD_ReadNibble

    pop     {pc, r0-r2}

;-----------------------------------------------------------------------------
; FUNCTION : LCD_DRWrite
; PURPOSE  : Writes one byte to the data register of the LCD module
;-----------------------------------------------------------------------------
;
; Description
;-------------------------------------
; This function writes one 8-bit value using two calls to the
; LCD_WriteNibble function.
;
;
; Arguments:
;-------------------------------------
; R0: 8-bit data to write
;
;
; Return:
;-------------------------------------
; None
;
;
            .text

LCD_DRWrite:
    push    {lr, r0-r2}

    ; Control port
    mov32   r2,GPIO_PORTE_BASE

    ; Ensure LCD module is ready
    bl      LCD_WaitForReady

    ; RS = 1, R/W = 0
    movw    r1,#LCD_RS
    strb    r1,[r2,#( GPIO_O_DATA + ( LCD_RS | LCD_RW ) << 2 ) ]

    ; Write 8-bit data
    bl      LCD_WriteNibble
    lsl     r0,r0,#4
    bl      LCD_WriteNibble

    pop     {pc, r0-r2}

;-----------------------------------------------------------------------------
; FUNCTION : LCD_DRRead
; PURPOSE  : Reads one byte from the data register of the LCD module
;-----------------------------------------------------------------------------
;
; Description
;-------------------------------------
; This function reads one 8-bit value using two calls to the
; LCD_ReadNibble function.
;
;
; Arguments:
;-------------------------------------
; R0: address of 8-bit data where data will be placed
;
;
; Return:
;-------------------------------------
; None
;
;
            .text

LCD_DRRead:
    push    {lr, r0-r2}

    ; Control port
    mov32   r2,GPIO_PORTE_BASE

    ; Ensure LCD module is ready
    bl      LCD_WaitForReady

    ; RS = 1, R/W = 1
    movw    r1,#( LCD_RS | LCD_RW )
    strb    r1,[r2,#( GPIO_O_DATA + ( LCD_RS | LCD_RW ) << 2 ) ]

    ; Read 8-bit data
    bl      LCD_ReadNibble
    ldrb    r1,[r0]
    lsl     r1,r1,#4
    strb    r1,[r0]
    bl      LCD_ReadNibble

    pop     {pc, r0-r2}

;-----------------------------------------------------------------------------
; FUNCTION : LCD_WaitForReady
; PURPOSE  : Reads one byte from the data register of the LCD module
;-----------------------------------------------------------------------------
;
; Description
;-------------------------------------
; This function continually reads the instruction register until the module
; is ready for another function.
;
;
; Arguments:
;-------------------------------------
; None
;
;
; Return:
;-------------------------------------
; None
;
;
            .text

LCD_WaitForReady:
    push    {lr, r0-r1}

    ; Allocate space for one local variable on the stack
    sub     sp,sp,#4
    mov     r0,sp

    ; Wait until the LCD module is ready
LCD_WaitForReadyLoop:
    bl      LCD_IRRead
    ldrb    r1,[r0]
    ands    r1,r1,#LCD_IC_STATUS_BUSY
    bne     LCD_WaitForReadyLoop

    ; Deallocate local variable on the stack
    add     sp,sp,#4

    pop     {pc, r0-r1}

;-----------------------------------------------------------------------------
; FUNCTION : LCD_Init
; PURPOSE  : Initializes the interface to the LCD module
;-----------------------------------------------------------------------------
;
; Description
;-------------------------------------
; This function prepares the interface to the LCD module.
;
;
; Arguments:
;-------------------------------------
; None
;
;
; Return:
;-------------------------------------
; None
;
;
            .text

LCD_Init:
    push    {lr, r0-r1}

    ; GPIO Port A and E Run Mode Clock Gating Control
    mov32   r1,SYSCTL_BASE
    ldrb    r0,[r1,#SYSCTL_O_RCGCGPIO]
    orr     r0,r0,#0x11
    strb    r0,[r1,#SYSCTL_O_RCGCGPIO]

    ; Configure Data Bus (initially input)
    mov32   r1,GPIO_PORTA_BASE

    ldrb    r0,[r1,#GPIO_O_DEN]
    orr     r0,r0,#LCD_BUS
    strb    r0,[r1,#GPIO_O_DEN]

    movw    r0,#LCD_BUS
    strb    r0,[r1,#( GPIO_O_DATA + ( LCD_BUS << 2 ) ) ]

    ldrb    r0,[r1,#GPIO_O_DIR]
    bic     r0,r0,#LCD_BUS
    strb    r0,[r1,#GPIO_O_DIR]

    ; Configure Control Signals
    mov32   r1,GPIO_PORTE_BASE

    ldrb    r0,[r1,#GPIO_O_DEN]
    orr     r0,r0,#LCD_CONTROL
    strb    r0,[r1,#GPIO_O_DEN]

    movw    r0,#0
    strb    r0,[r1,#( GPIO_O_DATA + ( LCD_CONTROL << 2 ) ) ]

    ldrb    r0,[r1,#GPIO_O_DIR]
    orr     r0,r0,#LCD_CONTROL
    strb    r0,[r1,#GPIO_O_DIR]

    ; Delay prior to power-up
    movw	r0,#500
    push    {r1}
    bl		SYSTICK_Delay
    pop     {r1}

    ; Power-up the LCD module
    movw    r0,#LCD_PWR
    strb    r0,[r1,#( GPIO_O_DATA + ( LCD_PWR << 2 ) ) ]

	; Delay after power-up
    movw	r0,#50
    push    {r1}
    bl		SYSTICK_Delay
    pop     {r1}

    ; Wait for the LCD module to power-up
    bl      LCD_WaitForReady

    ; Set to 4-bit mode
    ; RS = 0, R/W = 0
    movw    r0,#0
    strb    r0,[r1,#( GPIO_O_DATA + ( LCD_RS | LCD_RW ) << 2 ) ]
    movw    r0,#LCD_IC_FUNCTION
    bl      LCD_WriteNibble

    ; Configure LCD Module
    movw    r0,#( LCD_IC_FUNCTION | LCD_IC_FUNCTION_2LINE | LCD_IC_FUNCTION_ON )
    bl      LCD_IRWrite
    movw    r0,#( LCD_IC_DISPLAY | LCD_IC_DISPLAY_ON );
    bl      LCD_IRWrite
    movw    r0,#( LCD_IC_CLEAR );
    bl      LCD_IRWrite
    movw    r0,#( LCD_IC_ENTRYMODE | LCD_IC_ENTRYMODE_INC );
    bl      LCD_IRWrite

    pop     {pc, r0-r1}


;-----------------------------------------------------------------------------
; FUNCTION : LCD_SendMessage
; PURPOSE  : Sends a NULL terminated string to the LCD module
;-----------------------------------------------------------------------------
;
; Description
;-------------------------------------
; This function writes all non-zero data to the LCD module.
;
;
; Arguments:
;-------------------------------------
; R0: address of the 8-bit data to send
;
;
; Return:
;-------------------------------------
; None
;
;
            .text

LCD_SendMessage:

    push    {lr, r0-r1}

LCD_SendMessageLoop:

    ldrb    r1,[r0]
    movs    r1,r1
    beq     LCD_SendMessageDone

    push    {r0}
    mov     r0,r1
    bl      LCD_DRWrite
    pop     {r0}

    add     r0,r0,#1
    b       LCD_SendMessageLoop

LCD_SendMessageDone:

    pop     {pc, r0-r1}

            .end

;-----------------------------------------------------------------------------
; END OF LCD.ASM
;-----------------------------------------------------------------------------
