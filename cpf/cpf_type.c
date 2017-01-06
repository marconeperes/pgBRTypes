/*
 * cpf_type.c
 *
 *  Created on: 07/12/2016
 *      Author: Marcone Peres - DBA
 *      	@ :	marconeperes@gmail.com
 *     Twitter: @marconeperes
 *
 *  This code is a implementation of a simple CPF number data type extension in PostgreSQL.
 *
 *  Objectives;
 *  	1 - A custom data type to storage a CPF number efficiently.
 *  	2 - Check digits validation;
 *  	3 - Indexing and efficient search;
 *  	4 - Data masking by session variable
 */

#include <postgres.h>
#include <utils/palloc.h>
#include <libpq/pqformat.h>
#include "../pgBRTypes.h"


CPF_CNPJ cpf_parser(char *the_cpf, bool throws_error);

/*
 * cpf_parser validate and create a binary representation of the_cpf number
 */
CPF_CNPJ cpf_parser(char *the_cpf, bool throws_error) {
	//	  A CNPJ can be sent with less then 14 character, in case the firsts characters equals '0'. num_digits store the number of digits sent
	short idx;
	short num_digts;
	int sum[2] = { 0, 0 };
	CPF_CNPJ cpf = 0;

	// makes a copy of sent CPF
	char *the_cpf_bkp = palloc(sizeof(the_cpf));
	strcpy(the_cpf_bkp, the_cpf);

	// Check and remove format characters
	if (the_cpf[3] == '.' && the_cpf[7] == '.' && the_cpf[11] == '-') {
		memcpy(the_cpf + 3, the_cpf + 4, 3);
		memcpy(the_cpf + 6, the_cpf + 8, 3);
		memcpy(the_cpf + 9, the_cpf + 12, 3);
	}

	num_digts = strlen(the_cpf);

	// without format characters a CPF has a maximum of 11 characters, if not, throws a error.
	if (strlen(the_cpf) > 11) {
		if (!throws_error)
			return -1;

		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION), errmsg(INVALID_REPRESENTATION_MSG, "CPF", the_cpf_bkp)));
	}

	for (idx = 0; idx < num_digts - 2; idx++) {
		sum[0] += (10 - idx - (11 - num_digts)) * (the_cpf[idx] - ASCII_0);
		sum[1] += (11 - idx - (11 - num_digts)) * (the_cpf[idx] - ASCII_0);

		cpf = cpf * 10 + (the_cpf[idx] - ASCII_0);
	}

	sum[1] += (11 - idx - (11 - num_digts)) * (the_cpf[idx] - ASCII_0);

	// Check if the check digits it's OK.
	for (idx = 0; idx < 2; idx++) {
		sum[idx] = 11 - (sum[idx] % 11);
		sum[idx] = (sum[idx] >= 10 ? 0 : sum[idx]);

		if (sum[idx] != (the_cpf[num_digts - (2 - idx)] - ASCII_0)) {
			// If check digits are not valid, throws a error
			if (!throws_error)
				return -1;

			ereport(ERROR,
					(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION), errmsg(INVALID_REPRESENTATION_MSG, "CPF", the_cpf_bkp)));
		}

		cpf = cpf * 10 + sum[idx];
	}

	return cpf;
}

/*
 * cpf_in receive a CPF's CSTRING representation and call cpf_parser and return a CPF binary representation
 */
PG_FUNCTION_INFO_V1(cpf_in);
Datum cpf_in( PG_FUNCTION_ARGS) {
	char *the_cpf = PG_GETARG_CSTRING(0); // The first argument is in CSTRING format (*char)

	CPF_CNPJ *cpf = palloc(sizeof(CPF_CNPJ));
	*cpf = cpf_parser(the_cpf, true);

	PG_RETURN_POINTER(cpf);
}

/*
 * cpf_out receive a CPF internal representation and convert into a "human" format
 */
PG_FUNCTION_INFO_V1(cpf_out);
Datum cpf_out(PG_FUNCTION_ARGS) {
	CPF_CNPJ *cpf = (CPF_CNPJ *) PG_GETARG_POINTER(0);
	char *cpf_char = psprintf("%011lld", *cpf);

	// If CPF_FORMATTED return the CPF with format (mask);
	if (CPF_FORMATTED) {
		char *cpf_formatted = palloc(sizeof(char) * 15);
		cpf_formatted[3] = '.';
		cpf_formatted[7] = '.';
		cpf_formatted[11] = '-';
		cpf_formatted[14] = '\0';

		memcpy(cpf_formatted, cpf_char, 3);
		memcpy(cpf_formatted + 4, cpf_char + 3, 3);
		memcpy(cpf_formatted + 8, cpf_char + 6, 3);
		memcpy(cpf_formatted + 12, cpf_char + 9, 2);

		PG_RETURN_CSTRING(cpf_formatted);
	}

	PG_RETURN_CSTRING(cpf_char);
}

/*
 *	cpf_recv perform a binary input for a CPF
 */

PG_FUNCTION_INFO_V1(cpf_recv);
Datum cpf_recv(PG_FUNCTION_ARGS) {
	StringInfo buffer = (StringInfo) PG_GETARG_POINTER(0);
	CPF_CNPJ *the_cpf = palloc(sizeof(CPF_CNPJ));
	char *ccpf;

	*the_cpf = pq_getmsgint64(buffer);

	ccpf = psprintf("%011lld", *the_cpf);

	cpf_parser(ccpf, true);

	PG_RETURN_POINTER(the_cpf);
}

/*
 *	cpf_send perform a binary output for a CPF
 */
PG_FUNCTION_INFO_V1(cpf_send);
Datum cpf_send(PG_FUNCTION_ARGS) {
	CPF_CNPJ *the_cpf = (CPF_CNPJ *) PG_GETARG_POINTER(0);
	StringInfoData buffer;

	pq_begintypsend(&buffer);
	pq_sendint64(&buffer, *the_cpf);

	PG_RETURN_BYTEA_P(pq_endtypsend(&buffer));
}

/*
 *	cpf_to_bigint returns a bigint from a CPF
 *
 */
PG_FUNCTION_INFO_V1(cpf_to_bigint);
Datum cpf_to_bigint(PG_FUNCTION_ARGS) {
	CPF_CNPJ *cpf = (CPF_CNPJ *) PG_GETARG_POINTER(0);
	int64 bigint_cpf = *cpf;

	PG_RETURN_INT64(bigint_cpf);
}

/*
 * 	cpf_is_valid_cstring checks if the CSTRING parameter can be a CPF
 */
PG_FUNCTION_INFO_V1(cpf_is_valid_cstring);
Datum cpf_is_valid_cstring(PG_FUNCTION_ARGS) {
	char *the_ccpf = PG_GETARG_CSTRING(0);

	PG_RETURN_BOOL((cpf_parser(the_ccpf, false) >= 0));
}

/*
 * 	cpf_is_valid_bigint checks if the BIGINT parameter  can be a CPF
 */
PG_FUNCTION_INFO_V1(cpf_is_valid_bigint);
Datum cpf_is_valid_bigint(PG_FUNCTION_ARGS) {
	int64 bi_cpf = PG_GETARG_INT64(0);
	char *cpf_char = psprintf("%011lld", bi_cpf);

	PG_RETURN_BOOL((cpf_parser(cpf_char, false) >= 0));
}

