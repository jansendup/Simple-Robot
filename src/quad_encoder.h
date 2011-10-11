#ifndef QUAD_ENCODER
#define QUAD_ENCODER

#define ENC_CNT1 POSCNT
#define ENC_CNT2 poscnt2

#define ENC_DIR1 _UPDN
#define ENC_DIR2 updn2

#define ENC_TICKS_PER_REV  36
#define ENC_COUNT_CENTER   (4*ENC_TICKS_PER_REV)
#define ENC_MAX_COUNT      (2*ENC_COUNT_CENTER)

void init_quad_encoder(void);

#endif

