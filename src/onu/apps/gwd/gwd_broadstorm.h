#ifndef __GWD_BROADSTORM_H__
#define __GWD_BROADSTORM_H__
#include "gwd_types.h"

#ifdef	HAVE_GWD_BC_LIMIT
#define BC_STORM_THRESHOLD_MAX 2000000
#define BC_STORM_THRESHOLD_LAS 10

typedef struct{
	gw_uint64 gulBcStormThreshold;
	gw_uint64 gulBcStormStat;
}broadcast_storm_s;

void gw_broadcast_storm_init();
#endif /*(#ifdef	HAVE_GWD_BC_LIMIT)*/

#endif	/*end of the file*/

