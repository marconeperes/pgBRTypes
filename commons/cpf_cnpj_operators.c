/*
 * cpf_operators.c
 *
 *  Created on: 08/12/2016
 *      Author: Marcone
 */

#include <postgres.h>
#include <utils/palloc.h>
#include <utils/guc.h>
#include <lib/stringinfo.h>
#include <libpq/pqformat.h>
#include <string.h>


#include "../pgBRTypes.h"

/*
 *	cpf_cmp_internals compare two instances of CPF for indexing
 *
 */
int32 cpf_cnpj_cmp_internals(CPF_CNPJ cpf_cnpj0, CPF_CNPJ cpf_cnpj1);
int32 cpf_cnpj_cmp_internals(CPF_CNPJ cpf_cnpj0, CPF_CNPJ cpf_cnpj1){
	if ((cpf_cnpj0 - cpf_cnpj1) < 0)
		return -1;
	else if ((cpf_cnpj0 - cpf_cnpj1) > 0)
		return 1;
	else
		return 0;
}

/*
 *	cpf_cnpj_cmp compare two instances of CPF_CNPJ for indexing
 *
 */
PG_FUNCTION_INFO_V1(cpf_cnpj_cmp);
Datum cpf_cnpj_cmp(PG_FUNCTION_ARGS){
	CPF_CNPJ *cpf_cnpj0 = (CPF_CNPJ *) PG_GETARG_POINTER(0);
	CPF_CNPJ *cpf_cnpj1 = (CPF_CNPJ *) PG_GETARG_POINTER(1);

	PG_RETURN_INT32(cpf_cnpj_cmp_internals(*cpf_cnpj0, *cpf_cnpj1));
}


/*
 * 	cpf_cnpj_lt returns true if cpf_cnpj0 < cpf_cnpj1. Operator <
 */
PG_FUNCTION_INFO_V1(cpf_cnpj_lt);
Datum cpf_cnpj_lt(PG_FUNCTION_ARGS){
	CPF_CNPJ *cpf_cnpj0 = (CPF_CNPJ *) PG_GETARG_POINTER(0);
	CPF_CNPJ *cpf_cnpj1 = (CPF_CNPJ *) PG_GETARG_POINTER(1);

	PG_RETURN_BOOL(cpf_cnpj_cmp_internals(*cpf_cnpj0, *cpf_cnpj1) < 0);
}

/*
 * 	cpf_cnpj_le returns true if cpf_cnpj0 <= cpf_cnpj1. Operator <=
 */
PG_FUNCTION_INFO_V1(cpf_cnpj_le);
Datum cpf_cnpj_le(PG_FUNCTION_ARGS){
	CPF_CNPJ *cpf_cnpj0 = (CPF_CNPJ *) PG_GETARG_POINTER(0);
	CPF_CNPJ *cpf_cnpj1 = (CPF_CNPJ *) PG_GETARG_POINTER(1);

	PG_RETURN_BOOL(cpf_cnpj_cmp_internals(*cpf_cnpj0, *cpf_cnpj1) <= 0);
}

/*
 * 	cpf_cnpj_eq returns true if cpf_cnpj0 = cpf_cnpj1. Operator =
 */
PG_FUNCTION_INFO_V1(cpf_cnpj_eq);
Datum cpf_cnpj_eq(PG_FUNCTION_ARGS){
	CPF_CNPJ *cpf_cnpj0 = (CPF_CNPJ *) PG_GETARG_POINTER(0);
	CPF_CNPJ *cpf_cnpj1 = (CPF_CNPJ *) PG_GETARG_POINTER(1);

	PG_RETURN_BOOL(cpf_cnpj_cmp_internals(*cpf_cnpj0, *cpf_cnpj1) == 0);
}


/*
 * 	cpf_cnpj_ge returns true if cpf_cnpj0 >= cpf_cnpj1. Operator >=
 */
PG_FUNCTION_INFO_V1(cpf_cnpj_ge);
Datum cpf_cnpj_ge(PG_FUNCTION_ARGS){
	CPF_CNPJ *cpf_cnpj0 = (CPF_CNPJ *) PG_GETARG_POINTER(0);
	CPF_CNPJ *cpf_cnpj1 = (CPF_CNPJ *) PG_GETARG_POINTER(1);

	PG_RETURN_BOOL(cpf_cnpj_cmp_internals(*cpf_cnpj0, *cpf_cnpj1) >= 0);
}


/*
 * 	cpf_cnpj_gt returns true if cpf_cnpj0 > cpf_cnpj1. Operator >
 */
PG_FUNCTION_INFO_V1(cpf_cnpj_gt);
Datum cpf_cnpj_gt(PG_FUNCTION_ARGS){
	CPF_CNPJ *cpf_cnpj0 = (CPF_CNPJ *) PG_GETARG_POINTER(0);
	CPF_CNPJ *cpf_cnpj1 = (CPF_CNPJ *) PG_GETARG_POINTER(1);

	PG_RETURN_BOOL(cpf_cnpj_cmp_internals(*cpf_cnpj0, *cpf_cnpj1) > 0);
}




