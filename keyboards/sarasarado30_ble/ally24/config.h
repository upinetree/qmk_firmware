#ifndef MASTER_CONFIG_H_
#define MASTER_CONFIG_H_

#include "custom_board.h"

/* key matrix size */
#define MATRIX_ROWS 4
#define MATRIX_COLS 11

#define THIS_DEVICE_ROWS 4
#define THIS_DEVICE_COLS 11

#define MATRIX_ROW_PINS { PIN17, PIN16, PIN15, PIN14 }
#define MATRIX_COL_PINS { PIN12, PIN11, PIN10, PIN9, PIN8, PIN7, PIN2, PIN1, PIN20, PIN19, PIN18 }
#define IS_LEFT_HAND  true
#define ENABLE_STARTUP_ADV_NOLIST

#define LAYOUT( \
  K00, K01, K02, K03, K04, K05,      K07, K08, K09, K0A, K0B, K0C, \
  K10, K11, K12, K13, K14, K15, K16, K17, K18, K19, K1A, K1B,	   \
  K20, K21, K22, K23, K24, K25, K26, K27, K28, K29, K2A,	   \
            J00, J01, J02,      J10, J11, J12                 \
) \
{ \
  { K00,   K01,   K02,   K03, K04, K05, K07, K08, K09, K0A, K0B, },	\
  { K10,   K11,   K12,   K13, K14, K15, K16, K17, K18, K19, K1A, }, \
  { K20,   K21,   K22,   K23, K24, K25, K26, K27, K28, K29, K2A, },	\
  { KC_NO, KC_NO, KC_NO, J00, J01, J02, J10, J11, J12, K0C, K1B, },	\
}

#endif /* MASTER_CONFIG_H_ */
