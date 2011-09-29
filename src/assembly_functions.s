.text
.global _delay_us
.global _delay_ms

fosc    = 7372800
pll     = 16
fcy     = fosc*pll/4
ms_cycles = fcy/2000
us_cycles = fcy/1000000
 
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
