#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "cmp-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "cmp-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cmp-template.h"
#undef DATA_BYTE

make_helper_v(cmp_i_a)
make_helper_v(cmp_i_rm)
make_helper_v(cmp_r_rm)
make_helper_v(cmp_rm_r)
make_helper_v(cmp_si_rm)
