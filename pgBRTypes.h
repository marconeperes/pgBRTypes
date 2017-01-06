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
 *	pgBRTypes will implements another documents data types from Brazil as "Título de Eleito", "Certidões" and others.
 *
 *
 */

#include <postgres.h>
#include <utils/guc.h>

#ifndef SRC_PGBRTYPES_H_
#define SRC_PGBRTYPES_H_

/*
 * ASCII_0 is a char representation for '0' (ASCII code = 60).
 * PostgreSQL send a type as CSTRING ("alias" to *char) and for each characters in the number we need decrease 60 to get the real number value.
 */
#define ASCII_0 '0'

/*
 * Intarnaly CPF or CNPJ is stored as an int64 (BIGINT)
 */
typedef int64 CPF_CNPJ;

/*
 * PostgreSQL sets CPF_FORMATTED and CNPJ_FORMATTED with superuser SET command. This variable controls the format (mask) show in output CPF or CNPJ data type.
 */
bool CPF_FORMATTED;
bool CNPJ_FORMATTED;


/*
 * INVALID_REPRESENTATION_MSG is a message of error in representation of a CPF ou CNPJ.
 */
#define INVALID_REPRESENTATION_MSG "Invalid representation for %s type: %s"


void _PG_init(void);


#endif /* SRC_PGBRTYPES_H_ */
