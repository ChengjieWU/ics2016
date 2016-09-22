#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

//typedef struct watchpoint {
typedef struct watchpoint{
	int NO;
	struct watchpoint *next;

	/* TODO: Add more members if necessary */

	char expression[32];
	uint32_t value;
	bool occupied;

} WP;

#endif
