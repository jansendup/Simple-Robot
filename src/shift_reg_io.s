.equ DAT1_P, PORTF
.equ DAT1_B, 5
.equ STR1_P, PORTF
.equ STR1_B, 1
.equ CLK1_P, PORTF
.equ CLK1_B, 4

.equ CLK2_P, PORTC
.equ CLK2_B, 13
.equ DAT2_P, PORTC
.equ DAT2_B, 14
.equ STR2_P, PORTD
.equ STR2_B, 3

.equ R1SIZE, 16
.equ R2SIZE, 8

.macro sreg_out reg,i,s
         bclr STR&i&_P, #STR&i&_B
         DO #R&i&SIZE,3f              ; Loop for each bit in SR.
		 REPEAT #4000
		 NOP
		 bclr CLK&i&_P, #CLK&i&_B   ; Make CLK low.
         REPEAT #4000
		 NOP
		 rlc.&s& \reg,\reg              ; Shift current bit to be sent to C-flag.
         bra C,1f
         bclr DAT&i&_P, #DAT&i&_B       ; C-flag = 0: Set DATi line low.
		 bra 2f
1:
         bset DAT&i&_P, #DAT&i&_B       ; C-flag = 1: Set DATi high.
2:
		 REPEAT #4000
		 NOP 
3:       bset CLK&i&_P, #CLK&i&_B       ; Make CLK high to shift DATi into SREGi        
		 bset STR&i&_P, #STR&i&_B       ; All bits are shifted in...Strobe Latches.
.endm

.macro sreg_w i s
         and.&s& _sreg&i&,WREG    ; AND reg value with mask(w0).
         ior.&s& w1, w0, w0      ; Or the bits in that should change.
         mov.&s& WREG,_sreg&i&    ; Update sreg.
         sreg_out w0,&i&,&s&     ; Output new value to shift reg.
.endm

.macro sreg_s i s
         mov.&s& WREG,_sreg&i&
         sreg_out w0,&i&,&s&     ; Output new value to shift reg.
.endm

.global _sreg1
.global _sreg2
.bss
_sreg1:   .space 2
_sreg2:   .space 1

.global _write_sreg1
.global _write_sreg2
.global _set_sreg1
.global _set_sreg2
.text
_write_sreg1:
         sreg_w 1, w
         return
_write_sreg2:
         sreg_w 2, b
         return
_set_sreg1:
         sreg_s 1, w
         return
_set_sreg2:
         sreg_s 2, b
         return
.end
