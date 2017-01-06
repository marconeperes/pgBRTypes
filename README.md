pgBRTypes
=========

This is a simple implementation of some brazilians data types as CNPJ and CPF into PostgreSQL. The documentation is avaliable only in portuguese.

Tipos de dados comumente utilizados no Brasil. Por enquanto os tipos disponíveis são CPF e CNPJ, mas em breve serão implementados outras tipagens como título de eleitor, RG (alguns estados), certidões, etc.

I. Pré-requisitos
-----------------

- PostgreSQL >= 9.1 (Testado somente na versão 9.6, porém acreditamos que rode tranquilamente em versões anteriores);
- pg_config acessível diretamente via shell (caminho dos binários do postgres configurados no PATH);
- Pacote make e suas dependências;

II. Instalação
--------------

**Windows**

Não há suporte/documentação para utilização neste sistema operacional, mas é possível instalação seguindo-se os procedimentos padrões de instalação de extensions para postgres no windows, mas estes procedimentos não foram verificados e, portanto, não serão descritos aqui.

**Linux**

Após download do pacote e dentro da pasta do mesmo execute:

```sh
make && make install
```

Logado no PostgreSQL no banco de dados desejado execute:

```sh
CREATE EXTENSION pgbrtypes;
```

Pronto! PgBRTypes já estará pronta para uso.

III. Utilização
---------------

**Criando CPF's / CNPJ's:**

```sh
SELECT 
	20787875660::cpf,
	'021.516.186-68'::cpf,
	508581060::cpf,
	56808167000138::cnpj,
	'14.320.135/0001-49'::cnpj,
	4283548000147::cnpj;

-- * CPFs e CNPJs gerados aleatoriamente.

-- Convertendo colunas de tabelas:
CREATE TABLE teste_pgbrtypes(
	cpf_numerico BIGINT NOT NULL,
	cpf_texto VARCHAR NOT NULL,
	cpf_texto_formatado VARCHAR NOT NULL
); 

INSERT INTO teste_pgbrtypes
VALUES (20787875660, '02151618668', '005.085.810-60');

SELECT *
FROM teste_pgbrtypes;

 cpf_numerico |  cpf_texto  | cpf_texto_formatado
--------------+-------------+---------------------
  20787875660 | 02151618668 | 005.085.810-60
(1 row)

ALTER TABLE teste_pgbrtypes
ALTER COLUMN cpf_numerico TYPE cpf,
ALTER COLUMN cpf_texto TYPE cpf USING (cpf_texto::cpf),
ALTER COLUMN cpf_texto_formatado TYPE cpf using (cpf_texto_formatado::cpf);

SELECT *
FROM teste_pgbrtypes;

  cpf_numerico  |   cpf_texto    | cpf_texto_formatado
----------------+----------------+---------------------
 207.878.756-60 | 021.516.186-68 | 005.085.810-60
(1 row)
```

Observe que podem ser feitas conversões de dados para CPF oriundos de tipos numéricos ou de caracteres, com ou sem zeros à esquerda. O mesmo se aplica a CNPJ.


IV. Indexação
-------------

pgBRTypes provê alguns operadores e funções para a criação de índices do tipo B-Tree. Segue um exemplo:

```sh
CREATE INDEX idx_teste_pgbrtypes_cpf on teste_pgbrtypes (cpf_numerico);

SET enable_seqscan TO OFF;

EXPLAIN ANALYSE
SELECT cpf_numerico
FROM teste_pgbrtypes
WHERE cpf_numerico = 20787875660;

                                                                  QUERY PLAN
----------------------------------------------------------------------------------------------------------------------------------------------
 Index Only Scan using idx_teste_pgbrtypes_cpf on teste_pgbrtypes  (cost=0.12..8.14 rows=1 width=8) (actual time=0.265..0.269 rows=1 loops=1)
   Index Cond: (cpf_numerico = '20787875660'::bigint)
   Heap Fetches: 1
 Planning time: 0.725 ms
 Execution time: 0.628 ms
(5 rows)
```

Observe que não é necessário fazer um cast para que o filtro (número 20787875660) seja verificado como CPF internamente na busca, o postgres fará o cast automaticamente. Poderíamos ter utilizado como filtro o mesmo número como caractere com ou sem formatação ('20787875660' ou '207.878.756-60') o resultado seria o mesmo.

**A mesma funcionalidade aplica-se a CNPJ!**

V. Funções de verificação
-------------------------

Utilize a função **is_cpf({BIGINT | TEXT | CSTRING})** para verificar se um CPF é válido e **is_cnpj({BIGINT | TEXT | CSTRING})** para CNPJ. 


VI. Controlando formatação
-------------------------

São disponibilizadas duas configurações para controlar se a saída deve ser ou não formatada, uma para CPF e outra para CNPJ:

```sh
SHOW cpf.enable_format;
SHOW cnpj.enable_format;
```

Isso ajuda a minimizar impactos de utilização de pgBRTypes uma vez que pode ser controlada a saída conforme determinada aplicação ou usuário espera.

Para alterar a exibição faça:

```sh
SET cpf.enable_format TO OFF;	 CPF's serão exibidos sem formatação
SET cpf.enable_format TO ON;	 CPF's serão exibidos com formatação

SET cnpj.enable_format TO OFF;	 CNPJ's serão exibidos sem formatação
SET cnpj.enable_format TO ON;	 CNPJ's serão exibidos com formatação
```

É possível alterar estas configurações especificamente para um usuário, utilize:

```sh
ALTER USER <nome usuário> SET {cpf.enable_format | cnpj.enable_format} to {ON | OFF};
```
Ex.:
```sh
ALTER USER usr_oreia SET cpf.enable_format to OFF;
```
No exemplo acima o usuário usr_oreia não receberá os CPF's com formatação nos retornos.

VII. Encontrou um BUG??!!
------------------------

Favor reportar para:
marconeperes@gmail.com
@marconeperes

VIII. Posso utilizar em Produção?
--------------------------------

Claro!!!

IX. E se der problema?
------------------------

pgBRTypes é fornecida como está e sua utilização é por conta e risco de quem decide utilizá-la. Faça teste em seu ambiente, pois não terá quem responsabilizar caso ocorra alguma falha, perda ou corrupção de dados.

Agora se der tudo certo, favor dar os créditos! kkkk
