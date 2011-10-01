.global _delay_us
.global _delay_ms

.equ fosc, #7372800
.equ pll,  #8
.equ fcy,  fosc*pll/4
.equ ms_cycles, fcy/2000
.equ us_cycles, fcy/1000000

 .text

 _delay_us:
         repeat  #us_cycles-4
         nop
         dec     w0,w0
         bra     NZ,_delay_us
         return

_delay_ms:
         repeat  #ms_cycles-1
         nop
         repeat  #ms_cycles-4
         nop
         dec     w0,w0
         bra     NZ,_delay_ms
         return
         
.end
