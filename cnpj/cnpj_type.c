/*
 * cnpj_type.c
 *
 *  Created on: 20/12/2016
 *      Author: Marcone Peres - DBA
 *      	@ :	marconeperes@gmail.com
 *     Twitter: @marconeperes
 *
 *  This code is a implementation of a simple CNPJ number data type extension in PostgreSQL.
 *
 *  Objectives;
 *  	1 - A custom data type to storage a CNPJ number efficiently.
 *  	2 - Check digits validation;
 *  	3 - Indexing and efficient search;
 *  	4 - Data masking by session variable
 */

#include <postgres.h>
#include <utils/palloc.h>
#include <libpq/pqformat.h>
#include "../pgBRTypes.h"


CPF_CNPJ cnpj_parser(char *the_cnpj, bool throws_error);

/*
 * cnpj_parser validate and create a binary representation of the_cnpj number
 */
CPF_CNPJ cnpj_parser(char *the_cnpj, bool throws_error) {
	//	  A CNPJ can be sent with less then 14 character, in case the firsts characters equals '0'. num_digits store the number of digits sent.
	//	  In the loop some characters index (idx) not necessarily represents the respective position of the character in the CNPJ, the position is
	//	  calculated and store in current_idx.
	short idx;
	short num_digts;
	short current_idx = 0;
	int sum[2] = { 0, 0 };
	CPF_CNPJ cnpj = 0;

	// makes a copy of sent CNPJ
	char *the_cnpj_bkp = palloc(sizeof(the_cnpj));
	strcpy(the_cnpj_bkp, the_cnpj);

	// Check and remove format characters
	if (the_cnpj[2] == '.' && the_cnpj[6] == '.' && the_cnpj[10] == '/'
			&& the_cnpj[15] == '-') {
		memcpy(the_cnpj + 2, the_cnpj + 3, 3);
		memcpy(the_cnpj + 5, the_cnpj + 7, 3);
		memcpy(the_cnpj + 8, the_cnpj + 11, 4);
		memcpy(the_cnpj + 12, the_cnpj + 16, 3);
	}

	num_digts = strlen(the_cnpj);

	// without format characters a CNPJ has a maximum of 14 characters, if not, throws a error.
	if (strlen(the_cnpj) > 14) {
		if (!throws_error)
			return -1;

		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION), errmsg(INVALID_REPRESENTATION_MSG, "CNPJ", the_cnpj_bkp)));
	}

	for (idx = 0; idx < num_digts - 2; idx++) {
		current_idx = (14 - num_digts) + idx; 				// set the current idx in cnpj sent number
		sum[0] += (current_idx > 3 ? (13 - current_idx) : (5 - current_idx))
				* (the_cnpj[idx] - ASCII_0);
		sum[1] += (current_idx > 4 ? (14 - current_idx) : (6 - current_idx))
				* (the_cnpj[idx] - ASCII_0);

		cnpj = cnpj * 10 + (the_cnpj[idx] - ASCII_0);
	}

	current_idx++;
	sum[1] += (current_idx > 4 ? (14 - current_idx) : (6 - current_idx))
			* (the_cnpj[idx] - ASCII_0);

	// Check if the check digits it's OK.
	for (idx = 0; idx < 2; idx++) {

		sum[idx] = ((sum[idx] % 11) < 2) ? 0 : 11 - (sum[idx] % 11);

		if (sum[idx] != (the_cnpj[num_digts - (2 - idx)] - ASCII_0)) {
			// If check digits are not valid, throws a error
			if (!throws_error)
				return -1;

			ereport(ERROR,
					(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION), errmsg(INVALID_REPRESENTATION_MSG, "CNPJ", the_cnpj_bkp)));
		}

		cnpj = cnpj * 10 + sum[idx];

	}

	return cnpj;
}


/*
 * cnpj_in receive a CNPJ's CSTRING representation and call cnpj_parser and return a CNPJ binary representation
 */
PG_FUNCTION_INFO_V1(cnpj_in);
Datum cnpj_in( PG_FUNCTION_ARGS) {
	char *the_cnpj = PG_GETARG_CSTRING(0); // The first argument is in CSTRING format (*char)

	CPF_CNPJ *cnpj = palloc(sizeof(CPF_CNPJ));
	*cnpj = cnpj_parser(the_cnpj, true);

	PG_RETURN_POINTER(cnpj);
}


/*
 * cnpj_out receive a CNPJ internal representation and convert into a "human" format
 */
PG_FUNCTION_INFO_V1(cnpj_out);
Datum cnpj_out(PG_FUNCTION_ARGS) {
	CPF_CNPJ *cnpj = (CPF_CNPJ *) PG_GETARG_POINTER(0);
	char *cnpj_char = psprintf("%014lld", *cnpj);

	// If CNPJ_FORMATTED return the CNPJ with format (mask);
	if (CNPJ_FORMATTED) {
		char *cnpj_formatted = palloc(sizeof(char) * 19);
		cnpj_formatted[2] = '.';
		cnpj_formatted[6] = '.';
		cnpj_formatted[10] = '/';
		cnpj_formatted[15] = '-';
		cnpj_formatted[18] = '\0';

		memcpy(cnpj_formatted, cnpj_char, 2);
		memcpy(cnpj_formatted + 3, cnpj_char + 2, 3);
		memcpy(cnpj_formatted + 7, cnpj_char + 5, 3);
		memcpy(cnpj_formatted + 11, cnpj_char + 8, 4);
		memcpy(cnpj_formatted + 16, cnpj_char + 12, 2);

		PG_RETURN_CSTRING(cnpj_formatted);
	}

	PG_RETURN_CSTRING(cnpj_char);
}

/*
 *	cnpj_recv perform a binary input for a CNPJ
 */

PG_FUNCTION_INFO_V1(cnpj_recv);
Datum cnpj_recv(PG_FUNCTION_ARGS) {
	StringInfo buffer = (StringInfo) PG_GETARG_POINTER(0);
	CPF_CNPJ *the_cnpj = palloc(sizeof(CPF_CNPJ));
	char *ccnpj;

	*the_cnpj = pq_getmsgint64(buffer);

	ccnpj = psprintf("%014lld", *the_cnpj);

	cnpj_parser(ccnpj, true);

	PG_RETURN_POINTER(the_cnpj);
}


/*
 *	cnpj_send perform a binary output for a CNPJ
 */
PG_FUNCTION_INFO_V1(cnpj_send);
Datum cnpj_send(PG_FUNCTION_ARGS) {
	CPF_CNPJ *the_cnpj = (CPF_CNPJ *) PG_GETARG_POINTER(0);
	StringInfoData buffer;

	pq_begintypsend(&buffer);
	pq_sendint64(&buffer, *the_cnpj);

	PG_RETURN_BYTEA_P(pq_endtypsend(&buffer));
}


/*
 *	cnpj_to_bigint returns a bigint from a CNPJ
 *
 */
PG_FUNCTION_INFO_V1(cnpj_to_bigint);
Datum cnpj_to_bigint(PG_FUNCTION_ARGS) {
	CPF_CNPJ *cnpj = (CPF_CNPJ *) PG_GETARG_POINTER(0);
	int64 bigint_cnpj = *cnpj;

	PG_RETURN_INT64(bigint_cnpj);
}


/*
 * 	cnpj_is_valid_cstring checks if the CSTRING parameter can be a CNPJ
 */
PG_FUNCTION_INFO_V1(cnpj_is_valid_cstring);
Datum cnpj_is_valid_cstring(PG_FUNCTION_ARGS) {
	char *the_ccnpj = PG_GETARG_CSTRING(0);

	PG_RETURN_BOOL((cnpj_parser(the_ccnpj, false) >= 0));
}


/*
 * 	cnpj_is_valid_bigint checks if the BIGINT parameter can be a CNPJ
 */
PG_FUNCTION_INFO_V1(cnpj_is_valid_bigint);
Datum cnpj_is_valid_bigint(PG_FUNCTION_ARGS) {
	int64 bi_cnpj = PG_GETARG_INT64(0);
	char *cnpj_char = psprintf("%014lld", bi_cnpj);

	PG_RETURN_BOOL((cnpj_parser(cnpj_char, false) >= 0));
}

