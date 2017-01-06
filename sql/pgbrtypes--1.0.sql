
-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION pgbrtypes" to load this file. \quit


create type cnpj;

create function cnpj_in(cstring, oid, integer) returns cnpj
as
'$libdir/libpgbrtypes', 'cnpj_in'
language c IMMUTABLE STRICT;

create function cnpj_out(cnpj) returns cstring
as
'$libdir/libpgbrtypes', 'cnpj_out'
language c IMMUTABLE STRICT;

create function cnpj_recv(internal) returns cnpj
as
'$libdir/libpgbrtypes', 'cnpj_recv'
language c IMMUTABLE STRICT;

create function cnpj_send(cnpj) returns bytea
as
'$libdir/libpgbrtypes', 'cnpj_send'
language c IMMUTABLE STRICT;



create function cnpj_to_bigint(cnpj) returns bigint
as
'$libdir/libpgbrtypes', 'cnpj_to_bigint'
language c IMMUTABLE STRICT;

create function is_cnpj(CSTRING) returns boolean
as
'$libdir/libpgbrtypes', 'cnpj_is_valid_cstring'
language c IMMUTABLE STRICT;

create function is_cnpj(text) returns boolean
as
$$
	select is_cnpj($1::cstring)
$$ language SQL IMMUTABLE STRICT;

create function is_cnpj(BIGINT) returns boolean
as
'$libdir/libpgbrtypes', 'cnpj_is_valid_bigint'
language c IMMUTABLE STRICT;



create type cnpj(
	INTERNALLENGTH = 8,
	INPUT = cnpj_in,
	OUTPUT = cnpj_out,
	RECEIVE  = cnpj_recv,
	SEND = cnpj_send
);



CREATE CAST (bigint AS cnpj)
    WITH INOUT
    AS implicit  ;

CREATE CAST (integer AS cnpj)
    WITH INOUT
    AS implicit  ;

CREATE CAST (cnpj as bigint)
    WITH FUNCTION cnpj_to_bigint(cnpj)
    AS ASSIGNMENT  ;


create function cnpj_cmp(cnpj, cnpj) returns integer
as
'$libdir/libpgbrtypes', 'cpf_cnpj_cmp'
language c IMMUTABLE STRICT;

create function cnpj_lt(cnpj, cnpj) returns BOOLEAN
as
'$libdir/libpgbrtypes', 'cpf_cnpj_lt'
language c IMMUTABLE STRICT;

create function cnpj_le(cnpj, cnpj) returns BOOLEAN
as
'$libdir/libpgbrtypes', 'cpf_cnpj_le'
language c IMMUTABLE STRICT;

create function cnpj_eq(cnpj, cnpj) returns BOOLEAN
as
'$libdir/libpgbrtypes', 'cpf_cnpj_eq'
language c IMMUTABLE STRICT;

create function cnpj_ge(cnpj, cnpj) returns BOOLEAN
as
'$libdir/libpgbrtypes', 'cpf_cnpj_ge'
language c IMMUTABLE STRICT;

create function cnpj_gt(cnpj, cnpj) returns BOOLEAN
as
'$libdir/libpgbrtypes', 'cpf_cnpj_gt'
language c IMMUTABLE STRICT;





------------------------------------------------------------------

create function cnpj_bi_cmp(cnpj, bigint) returns integer
as
'$libdir/libpgbrtypes', 'cpf_cnpj_cmp'
language c IMMUTABLE STRICT;

create function cnpj_bi_lt(cnpj, bigint) returns BOOLEAN
as
'$libdir/libpgbrtypes', 'cpf_cnpj_lt'
language c IMMUTABLE STRICT;

create function cnpj_bi_le(cnpj, bigint) returns BOOLEAN
as
'$libdir/libpgbrtypes', 'cpf_cnpj_le'
language c IMMUTABLE STRICT;

create function cnpj_bi_eq(cnpj, bigint) returns BOOLEAN
as
'$libdir/libpgbrtypes', 'cpf_cnpj_eq'
language c IMMUTABLE STRICT;

create function cnpj_bi_ge(cnpj, bigint) returns BOOLEAN
as
'$libdir/libpgbrtypes', 'cpf_cnpj_ge'
language c IMMUTABLE STRICT;

create function cnpj_bi_gt(cnpj, bigint) returns BOOLEAN
as
'$libdir/libpgbrtypes', 'cpf_cnpj_gt'
language c IMMUTABLE STRICT;

-------

create function bi_cnpj_cmp(bigint, cnpj) returns integer
as
'$libdir/libpgbrtypes', 'cpf_cnpj_cmp'
language c IMMUTABLE STRICT;

create function bi_cnpj_lt(bigint, cnpj) returns BOOLEAN
as
'$libdir/libpgbrtypes', 'cpf_cnpj_lt'
language c IMMUTABLE STRICT;

create function bi_cnpj_le(bigint, cnpj) returns BOOLEAN
as
'$libdir/libpgbrtypes', 'cpf_cnpj_le'
language c IMMUTABLE STRICT;

create function bi_cnpj_eq(bigint, cnpj) returns BOOLEAN
as
'$libdir/libpgbrtypes', 'cpf_cnpj_eq'
language c IMMUTABLE STRICT;

create function bi_cnpj_ge(bigint, cnpj) returns BOOLEAN
as
'$libdir/libpgbrtypes', 'cpf_cnpj_ge'
language c IMMUTABLE STRICT;


create function bi_cnpj_gt(bigint, cnpj) returns BOOLEAN
as
'$libdir/libpgbrtypes', 'cpf_cnpj_gt'
language c IMMUTABLE STRICT;

------------------------------------------------------------------


CREATE OPERATOR < (
   leftarg = cnpj, rightarg = cnpj, procedure = cnpj_lt,
   commutator = > , negator = >= ,
   restrict = scalarltsel, join = scalarltjoinsel
);

CREATE OPERATOR <= (
   leftarg = cnpj, rightarg = cnpj, procedure = cnpj_le,
   commutator = >= , negator = > ,
   restrict = scalarltsel, join = scalarltjoinsel
);

CREATE OPERATOR = (
   leftarg = cnpj, rightarg = cnpj, procedure = cnpj_eq,
   commutator = = ,
   restrict = eqsel, join = eqjoinsel
);

CREATE OPERATOR >= (
   leftarg = cnpj, rightarg = cnpj, procedure = cnpj_ge,
   commutator = <= , negator = < ,
   restrict = scalargtsel, join = scalargtjoinsel
);

CREATE OPERATOR > (
   leftarg = cnpj, rightarg = cnpj, procedure = cnpj_gt,
   commutator = < , negator = <= ,
   restrict = scalargtsel, join = scalargtjoinsel
);


------------------------------------------------------------------

CREATE OPERATOR < (
   leftarg = cnpj, rightarg = bigint, procedure = cnpj_bi_lt,
   commutator = > , negator = >= ,
   restrict = scalarltsel, join = scalarltjoinsel
);

CREATE OPERATOR <= (
   leftarg = cnpj, rightarg = bigint, procedure = cnpj_bi_le,
   commutator = >= , negator = > ,
   restrict = scalarltsel, join = scalarltjoinsel
);

CREATE OPERATOR = (
   leftarg = cnpj, rightarg = bigint, procedure = cnpj_bi_eq,
   commutator = = ,
   restrict = eqsel, join = eqjoinsel
);

CREATE OPERATOR >= (
   leftarg = cnpj, rightarg = bigint, procedure = cnpj_bi_ge,
   commutator = <= , negator = < ,
   restrict = scalargtsel, join = scalargtjoinsel
);

CREATE OPERATOR > (
   leftarg = cnpj, rightarg = bigint, procedure = cnpj_bi_gt,
   commutator = < , negator = <= ,
   restrict = scalargtsel, join = scalargtjoinsel
);


--------------------

CREATE OPERATOR < (
   leftarg = bigint, rightarg = cnpj, procedure = bi_cnpj_lt,
   commutator = > , negator = >= ,
   restrict = scalarltsel, join = scalarltjoinsel
);

CREATE OPERATOR <= (
   leftarg = bigint, rightarg = cnpj, procedure = bi_cnpj_le,
   commutator = >= , negator = > ,
   restrict = scalarltsel, join = scalarltjoinsel
);

CREATE OPERATOR = (
   leftarg = bigint, rightarg = cnpj, procedure = bi_cnpj_eq,
   commutator = = ,
   restrict = eqsel, join = eqjoinsel
);

CREATE OPERATOR >= (
   leftarg = bigint, rightarg = cnpj, procedure = bi_cnpj_ge,
   commutator = <= , negator = < ,
   restrict = scalargtsel, join = scalargtjoinsel
);

CREATE OPERATOR > (
   leftarg = bigint, rightarg = cnpj, procedure = bi_cnpj_gt,
   commutator = < , negator = <= ,
   restrict = scalargtsel, join = scalargtjoinsel
);

------------------------------------------------------------------

CREATE OPERATOR CLASS cnpj_op_class
    DEFAULT FOR TYPE cnpj USING btree AS
	OPERATOR        1       < (cnpj, cnpj),
	OPERATOR        2       <= (cnpj, cnpj),
	OPERATOR        3       = (cnpj, cnpj),
	OPERATOR        4       >= (cnpj, cnpj),
	OPERATOR        5       > (cnpj, cnpj),

	OPERATOR        1       < (cnpj, BIGINT),
	OPERATOR        2       <= (cnpj, BIGINT),
	OPERATOR        3       = (cnpj, BIGINT),
	OPERATOR        4       >= (cnpj, BIGINT),
	OPERATOR        5       > (cnpj, BIGINT),

	OPERATOR        1       < (BIGINT, cnpj),
	OPERATOR        2       <= (BIGINT, cnpj),
	OPERATOR        3       = (BIGINT, cnpj),
	OPERATOR        4       >= (BIGINT, cnpj),
	OPERATOR        5       > (BIGINT, cnpj),

	FUNCTION        1       cnpj_cmp(cnpj, cnpj),
	FUNCTION        1       cnpj_bi_cmp(cnpj, bigint),
	FUNCTION        1       bi_cnpj_cmp(bigint, cnpj);










create type cpf;

create function cpf_in(cstring, oid, integer) returns cpf
as
'$libdir/libpgbrtypes', 'cpf_in'
language c IMMUTABLE STRICT;

create function cpf_out(cpf) returns cstring
as
'$libdir/libpgbrtypes', 'cpf_out'
language c IMMUTABLE STRICT;

create function cpf_recv(internal) returns cpf
as
'$libdir/libpgbrtypes', 'cpf_recv'
language c IMMUTABLE STRICT;

create function cpf_send(cpf) returns bytea
as
'$libdir/libpgbrtypes', 'cpf_send'
language c IMMUTABLE STRICT;



create function cpf_to_bigint(cpf) returns bigint
as
'$libdir/libpgbrtypes', 'cpf_to_bigint'
language c IMMUTABLE STRICT;

create function is_cpf(CSTRING) returns boolean
as
'$libdir/libpgbrtypes', 'cpf_is_valid_cstring'
language c IMMUTABLE STRICT;

create function is_cpf(text) returns boolean
as
$$
	select is_cpf($1::cstring)
$$ language SQL IMMUTABLE STRICT;

create function is_cpf(BIGINT) returns boolean
as
'$libdir/libpgbrtypes', 'cpf_is_valid_bigint'
language c IMMUTABLE STRICT;



create type cpf(
	INTERNALLENGTH = 8,
	INPUT = cpf_in,
	OUTPUT = cpf_out,
	RECEIVE  = cpf_recv,
	SEND = cpf_send
);


------------------------------------------------------------------

CREATE CAST (bigint AS cpf)
    WITH INOUT
    AS implicit  ;

CREATE CAST (integer AS cpf)
    WITH INOUT
    AS implicit  ;

CREATE CAST (cpf as bigint)
    WITH FUNCTION cpf_to_bigint(cpf)
    AS ASSIGNMENT  ;


------------------------------------------------------------------

create function cpf_cmp(cpf, cpf) returns integer
as
'$libdir/libpgbrtypes', 'cpf_cnpj_cmp'
language c IMMUTABLE STRICT;

create function cpf_lt(cpf, cpf) returns BOOLEAN
as
'$libdir/libpgbrtypes', 'cpf_cnpj_lt'
language c IMMUTABLE STRICT;

create function cpf_le(cpf, cpf) returns BOOLEAN
as
'$libdir/libpgbrtypes', 'cpf_cnpj_le'
language c IMMUTABLE STRICT;

create function cpf_eq(cpf, cpf) returns BOOLEAN
as
'$libdir/libpgbrtypes', 'cpf_cnpj_eq'
language c IMMUTABLE STRICT;

create function cpf_ge(cpf, cpf) returns BOOLEAN
as
'$libdir/libpgbrtypes', 'cpf_cnpj_ge'
language c IMMUTABLE STRICT;


create function cpf_gt(cpf, cpf) returns BOOLEAN
as
'$libdir/libpgbrtypes', 'cpf_cnpj_gt'
language c IMMUTABLE STRICT;



------------------------------------------------------------------

create function cpf_bi_cmp(cpf, bigint) returns integer
as
'$libdir/libpgbrtypes', 'cpf_cnpj_cmp'
language c IMMUTABLE STRICT;

create functioN cpf_bi_lt(cpf, BIGINT) returns BOOLEAN
as
'$libdir/libpgbrtypes', 'cpf_cnpj_lt'
language c IMMUTABLE STRICT;

create functioN cpf_bi_le(cpf, BIGINT) returns BOOLEAN
as
'$libdir/libpgbrtypes', 'cpf_cnpj_le'
language c IMMUTABLE STRICT;

create functioN cpf_bi_eq(cpf, BIGINT) returns BOOLEAN
as
'$libdir/libpgbrtypes', 'cpf_cnpj_eq'
language c IMMUTABLE STRICT;

create functioN cpf_bi_ge(cpf, BIGINT) returns BOOLEAN
as
'$libdir/libpgbrtypes', 'cpf_cnpj_ge'
language c IMMUTABLE STRICT;


create functioN cpf_bi_gt(cpf, BIGINT) returns BOOLEAN
as
'$libdir/libpgbrtypes', 'cpf_cnpj_gt'
language c IMMUTABLE STRICT;

------------------------------------------------------------------

create function bi_cpf_cmp(BIGINT, cpf) returns integer
as
'$libdir/libpgbrtypes', 'cpf_cnpj_cmp'
language c IMMUTABLE STRICT;

create functioN bi_cpf_lt(BIGINT, cpf) returns BOOLEAN
as
'$libdir/libpgbrtypes', 'cpf_cnpj_lt'
language c IMMUTABLE STRICT;

create functioN bi_cpf_le(BIGINT, cpf) returns BOOLEAN
as
'$libdir/libpgbrtypes', 'cpf_cnpj_le'
language c IMMUTABLE STRICT;

create functioN bi_cpf_eq(BIGINT, cpf) returns BOOLEAN
as
'$libdir/libpgbrtypes', 'cpf_cnpj_eq'
language c IMMUTABLE STRICT;

create functioN bi_cpf_ge(BIGINT, cpf) returns BOOLEAN
as
'$libdir/libpgbrtypes', 'cpf_cnpj_ge'
language c IMMUTABLE STRICT;


create functioN bi_cpf_gt(BIGINT, cpf) returns BOOLEAN
as
'$libdir/libpgbrtypes', 'cpf_cnpj_gt'
language c IMMUTABLE STRICT;

------------------------------------------------------------------

CREATE OPERATOR < (
   leftarg = cpf, rightarg = cpf, procedure = cpf_lt,
   commutator = > , negator = >= ,
   restrict = scalarltsel, join = scalarltjoinsel
);

CREATE OPERATOR <= (
   leftarg = cpf, rightarg = cpf, procedure = cpf_le,
   commutator = >= , negator = > ,
   restrict = scalarltsel, join = scalarltjoinsel
);

CREATE OPERATOR = (
   leftarg = cpf, rightarg = cpf, procedure = cpf_eq,
   commutator = = ,
   restrict = eqsel, join = eqjoinsel
);


CREATE OPERATOR >= (
   leftarg = cpf, rightarg = cpf, procedure = cpf_ge,
   commutator = <= , negator = < ,
   restrict = scalargtsel, join = scalargtjoinsel
);


CREATE OPERATOR > (
   leftarg = cpf, rightarg = cpf, procedure = cpf_gt,
   commutator = < , negator = <= ,
   restrict = scalargtsel, join = scalargtjoinsel
);



------------------------------------------------------------------

CREATE OPERATOR < (
   leftarg = cpf, rightarg = BIGINT, procedure = cpf_bi_lt,
   commutator = > , negator = >= ,
   restrict = scalarltsel, join = scalarltjoinsel
);

CREATE OPERATOR <= (
   leftarg = cpf, rightarg = BIGINT, procedure = cpf_bi_le,
   commutator = >= , negator = > ,
   restrict = scalarltsel, join = scalarltjoinsel
);

CREATE OPERATOR = (
   leftarg = cpf, rightarg = BIGINT, procedure = cpf_bi_eq,
   commutator = = ,
   restrict = eqsel, join = eqjoinsel
);


CREATE OPERATOR >= (
   leftarg = cpf, rightarg = BIGINT, procedure = cpf_bi_ge,
   commutator = <= , negator = < ,
   restrict = scalargtsel, join = scalargtjoinsel
);


CREATE OPERATOR > (
   leftarg = cpf, rightarg = BIGINT, procedure = cpf_bi_gt,
   commutator = < , negator = <= ,
   restrict = scalargtsel, join = scalargtjoinsel
);

------------------------------------------------------------------

CREATE OPERATOR < (
   leftarg = BIGINT, rightarg = cpf, procedure = bi_cpf_lt,
   commutator = > , negator = >= ,
   restrict = scalarltsel, join = scalarltjoinsel
);

CREATE OPERATOR <= (
   leftarg = BIGINT, rightarg = cpf, procedure = bi_cpf_le,
   commutator = >= , negator = > ,
   restrict = scalarltsel, join = scalarltjoinsel
);

CREATE OPERATOR = (
   leftarg = BIGINT, rightarg = cpf, procedure = bi_cpf_eq,
   commutator = = ,
   restrict = eqsel, join = eqjoinsel
);


CREATE OPERATOR >= (
   leftarg = BIGINT, rightarg = cpf, procedure = bi_cpf_ge,
   commutator = <= , negator = < ,
   restrict = scalargtsel, join = scalargtjoinsel
);


CREATE OPERATOR > (
   leftarg = BIGINT, rightarg = cpf, procedure = bi_cpf_gt,
   commutator = < , negator = <= ,
   restrict = scalargtsel, join = scalargtjoinsel
);
------------------------------------------------------------------


CREATE OPERATOR CLASS cpf_op_class
    DEFAULT FOR TYPE cpf USING btree AS
        OPERATOR        1       < (cpf, cpf),
        OPERATOR        2       <= (cpf, cpf),
        OPERATOR        3       = (cpf, cpf),
        OPERATOR        4       >= (cpf, cpf),
        OPERATOR        5       > (cpf, cpf),

        OPERATOR        1       < (cpf, bigint),
        OPERATOR        2       <= (cpf, bigint),
        OPERATOR        3       = (cpf, bigint),
        OPERATOR        4       >= (cpf, bigint),
        OPERATOR        5       > (cpf, bigint),

        OPERATOR        1       < (BIGINT, cpf),
        OPERATOR        2       <= (BIGINT, cpf),
        OPERATOR        3       = (BIGINT, cpf),
        OPERATOR        4       >= (BIGINT, cpf),
        OPERATOR        5       > (BIGINT, cpf),
        
        FUNCTION        1       cpf_cmp(cpf, cpf),
        FUNCTION        1       cpf_bi_cmp(cpf, bigint),
        FUNCTION        1       bi_cpf_cmp(bigint, cpf);




