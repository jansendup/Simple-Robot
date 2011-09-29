.equ DAT1_P, PORTF
.equ DAT1_B, RF5

.equ STR1_P, PORTF
.equ STR1_B, RF1

.equ CLK1_P, PORTC
.equ CLK1_B, RF13

.equ R1SIZE, #16
.equ R2SIZE, #8

.macro sreg_out reg,i
         bclr STR&i&_P, STR&i&_B
shift_loop:
         DO R&i&SIZE-1,end_shift_loop
         bclr CLK&i&_P, CLK&i&_B
         rlc.b \reg
         bra C,set_bit
         bclr DAT&i&_P, DAT&i&_B
         bra end_shift_loop
set_bit:
         bset DAT&i&_P, DAT&i&_B
end_shift_loop:
         bset CLK&i&_P, CLK&i&_B
strobe_data:
         bset STR&i&_P, STR&i&_B
.endm

_write_reg1:
         sreg_out w0, 1
         return