/*
 * pgBRTypes.c
 *
 *  Created on: 22/12/2016
 *      Author: Marcone Peres - DBA
 *      	@ :	marconeperes@gmail.com
 *     Twitter: @marconeperes
 *
 *	This is a simple extension to PostgreSQL that implements a CPF and CNPJ data types so far.
 *
 *	pgBRTypes will implements another documents data types from Brazil as "Título de Eleitor", "Certidões" and others.
 *
 *
 */

#include "pgBRTypes.h"

#include <postgres.h>

#include <fmgr.h>
#include <utils/palloc.h>
#include <utils/array.h>
#include <utils/guc.h>
#include <catalog/pg_type.h>
#include <utils/catcache.h>

#include <strings.h>

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC
;
#endif


/*
 * _PG_init initialize some control variables that set if the respective document will be showed with or without format (mask).
 */
void _PG_init(void) {

	/*
	 * Creates customs variables to controls the CPF and CNPJ output with or without format (mask).
	 */

	DefineCustomBoolVariable("cpf.enable_format",
			"Controla a exibição de formatação dos CPFs",
			"Com esta configuração você pode visualizar o CPF com ou sem formatação. A formatação do CPF é a seguinte: 000.000.000-00",
			&CPF_FORMATTED,
			true, PGC_USERSET,
			GUC_LIST_INPUT | GUC_NOT_IN_SAMPLE,
			NULL,
			NULL, NULL);

	DefineCustomBoolVariable("cnpj.enable_format",
			"Controla a exibição de formatação dos CNPJs",
			"Com esta configuração você pode visualizar o CNPJ com ou sem formatação. A formatação do CNPJ é a seguinte: 00.000.000/0000-00",
			&CNPJ_FORMATTED,
			true, PGC_USERSET,
			GUC_LIST_INPUT | GUC_NOT_IN_SAMPLE,
			NULL,
			NULL, NULL);



}

