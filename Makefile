
MODULE_big = libpgbrtypes
MODULE = libpgbrtypes
OBJS = pgBRTypes.o cpf/cpf_type.o cnpj/cnpj_type.o commons/cpf_cnpj_operators.o


EXTENSION = pgbrtypes

DATA = sql/pgbrtypes--1.0.sql
PGFILEDESC = "pgbrtypes - Brazilian data types as CNPJ and CPF"

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
