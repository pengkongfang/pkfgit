#ifndef _BEEP_H
#define _BEEP_H

#include "ddl.h"
#include "stdbool.h"
#include "adt.h"
#include "lpm.h"
typedef struct 
{
	en_result_t (*init)(void);
	void (*write)(bool sta);
	en_adt_unit_t        enAdt;
}beep_str;
void beep_register(beep_str *beep);
extern beep_str Beep;

#endif


