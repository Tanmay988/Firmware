#ifndef WC_H
#define WC_H

#include <stdint.h>
#include "FE.h"
#include "LBM.h"
#include "L2P.h"
#include "FIL.h"
#include "LUT.h"
#include "MCA.h"

void WCinit(MCA_t *mca);
uint32_t wc_write_request(uint32_t lca);

#endif // WC_H
