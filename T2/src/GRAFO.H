#if ! defined( GRAFO_ )
#define GRAFO_
/***************************************************************************
*
*  $MCD M�dulo de defini��o: Grafo dirigido
*
*  Arquivo gerado:              GRAFO.H
*  Letras identificadoras:      GRA
*
*  Projeto: T2 INF1301 2013.2
*  Autores: gb - Guilherme Berger
*           fl - Felipe Luiz
*           jv - Jo�o Vicente
*
*  $ED Descri��o do m�dulo
*     Implementa fun��es para criar e explorar grafos dirigidos.

*
***************************************************************************/
 
#if defined( GRAFO_OWN )
   #define GRAFO_EXT
#else
   #define GRAFO_EXT extern
#endif

#include "LISTA.H"
#include "VERTICE.H"

/***** Declara��es exportadas pelo m�dulo *****/

/* Tipo refer�ncia para uma grafo */

typedef struct GRA_tagGrafo * GRA_tppGrafo ;

/***********************************************************************
*
*  $TC Tipo de dados: GRA Condicoes de retorno
*
*  $ED Descri��o do tipo
*     Condi��es de retorno das fun��es da grafo
*
***********************************************************************/

   typedef enum {

      GRA_CondRetOK ,
         /* Executou corretamente */

		GRA_CondRetGrafoVazio ,
		   /* Opera��o inv�lida pois o grafo est� vazio */

		GRA_CondRetVerticeInvalido ,
			/* Vertice referenciado n�o existe ou n�o � acess�vel */
			   
		GRA_CondRetErroVertice ,
			/* Opera��o no v�rtice resultou em erro */

      GRA_CondRetPonteiroNulo ,
         /* Foi passado um ponteiro para NULL */

      GRA_CondRetFaltouMemoria ,
         /* Faltou mem�ria ao alocar dados */

		GRA_CondRetArestaJaExiste ,
			/* J� existe uma aresta com a mesma origem e destino */

      GRA_CondRetArestaInvalida
         /* N�o existe uma aresta com tal identificador */

   } GRA_tpCondRet ;
   

/***********************************************************************
*
*  $FC Fun��o: GRA Criar grafo
*
*  $ED Descri��o da fun��o
*     Cria um novo grafo.
*     O ponteiro para o novo grafo ser� fornecido no valor do ponteiro
*     de ponteiro para grafo.
*
*  $EP Par�metros
*     $P ppGrafo - ponteiro para o ponteiro de grafo.
*
*  $FV Valor retornado
*     GRA_CondRetOK
*     GRA_CondRetFaltouMemoria
*
*  $EAE Assertivas de entrada
*     ExcluirValor ponteiro para uma fun��o que encarregue-se de excluir os
*        valores que ser�o usados nos v�rtices do grafo
*
*  $EAE Assertivas de sa�da
*     SE GRA_CondRetOk
*        ppGrafo conter� um ponteiro para um grafo
*     SE GRA_CondRetFaltouMemoria
*        ppGrafo n�o ser� modificado
*
***********************************************************************/

   GRA_tpCondRet GRA_CriarGrafo( GRA_tppGrafo * ppGrafo, void ( * ExcluirValor ) ( void * pDado ) ) ;


/***********************************************************************
*
*  $FC Fun��o: GRA Destruir grafo
*
*  $ED Descri��o da fun��o
*     Destr�i o grafo, liberando o espa�o ocupado
*     pela grafo, pelos v�rtices e pelas listas.
*
*  $EP Par�metros
*     $P pGrafo - ponteiro para a grafo.
*
*  $FV Valor retornado
*     GRA_CondRetOK
*     GRA_CondRetPonteiroNulo
*
*  $EAE Assertivas de entrada
*     pGrafo != NULL
*     pGrafo deve ser ponteiro v�lido para um grafo do m�dulo
*
*  $EAE Assertivas de sa�da
*     SE GRA_CondRetOk
*        pGrafo ter� sido liberado e ser� agora um ponteiro inv�lido
*     SE GRA_CondRetPonteiroNulo
*        pGrafo era NULL
*
***********************************************************************/

   GRA_tpCondRet GRA_DestruirGrafo( GRA_tppGrafo pGrafo ) ;


/***********************************************************************
*
*  $FC Fun��o: GRA Esvaziar
*
*  $ED Descri��o da fun��o
*     Esvazia a grafo, isso �, libera todos os v�rtices e listas.
*
*  $EP Par�metros
*     $P pGrafo - ponteiro para a grafo.
*
*  $FV Valor retornado
*     GRA_CondRetOK
*     GRA_CondRetPonteiroNulo
*
*  $EAE Assertivas de entrada
*     pGrafo != NULL
*     pGrafo deve ser ponteiro v�lido para um grafo do m�dulo
*
*  $EAE Assertivas de sa�da
*     SE GRA_CondRetOk
*        pGrafo ter� sido esvaziado: todos os v�rtices, arestas e origens
*           ter�o sido exclu�dos
*        v�rtice corrente ser� NULL
*     SE GRA_CondRetPonteiroNulo
*        pGrafo era NULL
*
***********************************************************************/

   GRA_tpCondRet GRA_EsvaziarGrafo( GRA_tppGrafo pGrafo ) ;

   
/***********************************************************************
*
*  $FC Fun��o: GRA Obter valor do v�rtice corrente
*
*  $ED Descri��o da fun��o
*     O valor do v�rtice corrente � retornado pelo par�metro ppValor.
*
*  $EP Par�metros
*     $P pGrafo - ponteiro para a grafo.
*     $P ppValor - ponteiro para o ponteiro de vertice, a ser modificado
*
*  $FV Valor retornado
*     GRA_CondRetOK
*     GRA_CondRetPonteiroNulo
*
*  $EAE Assertivas de entrada
*     pGrafo != NULL
*     pGrafo deve ser ponteiro v�lido para um grafo do m�dulo
*     ppValor != NULL
*
*  $EAE Assertivas de sa�da
*     SE GRA_CondRetOk
*        ppValor conter� ponteiro para o valor contido no v�rtice corrente
*     SE GRA_CondRetPonteiroNulo
*        pGrafo era NULL ou ppValor era NULL
*
***********************************************************************/

   GRA_tpCondRet GRA_ObterValorCorrente( GRA_tppGrafo pGrafo, void ** ppValor ) ;

   
/***********************************************************************
*
*  $FC Fun��o: GRA Alterar valor do v�rtice corrente
*
*  $ED Descri��o da fun��o
*     O v�rtice corrente � alterado para o valor de pValor.
*
*  $EP Par�metros
*     $P pGrafo - ponteiro para a grafo.
*     $P pValor - ponteiro para um valor de vertice
*
*  $FV Valor retornado
*     GRA_CondRetOK
*     GRA_CondRetPonteiroNulo
*
*  $EAE Assertivas de entrada
*     pGrafo != NULL
*     pGrafo deve ser ponteiro v�lido para um grafo do m�dulo
*
*  $EAE Assertivas de sa�da
*     SE GRA_CondRetOk
*        pGrafo ter� em seu n� corrente o valor pValor
*     SE GRA_CondRetPonteiroNulo
*        pGrafo era NULL
*
***********************************************************************/

   GRA_tpCondRet GRA_AlterarValorCorrente( GRA_tppGrafo pGrafo, void * pValor ) ;


/***********************************************************************
*
*  $FC Fun��o: GRA Ir para vertice
*
*  $ED Descri��o da fun��o
*     O v�rtice corrente torna-se o v�rtice cujo valor � o dado, se
*		existir tal v�rtice no grafo.
*
*  $EP Par�metros
*     $P pGrafo - ponteiro para a grafo.
*     $P idVertice - identificador do v�rtice
*
*  $FV Valor retornado
*     GRA_CondRetOK
*     GRA_CondRetPonteiroNulo
*	   GRA_CondRetVerticeInvalido
*
*  $EAE Assertivas de entrada
*     pGrafo != NULL
*     pGrafo deve ser ponteiro v�lido para um grafo do m�dulo
*     idVertice deve ser um id de um v�rtice v�lido existente no grafo
*
*  $EAE Assertivas de sa�da
*     SE GRA_CondRetOk
*        pGrafo ter� como v�rtice corrente o v�rtice cujo id � idVertice
*     SE GRA_CondRetPonteiroNulo
*        pGrafo era NULL
*     SE GRA_CondRetVerticeInvalido
*        idVertice n�o era um id de um v�rtice v�lido existente no grafo
*
***********************************************************************/

   GRA_tpCondRet GRA_IrParaVertice( GRA_tppGrafo pGrafo, char idVertice ) ;


/***********************************************************************
*
*  $FC Fun��o: GRA Ir para vertice
*
*  $ED Descri��o da fun��o
*     O v�rtice corrente torna-se o v�rtice cujo valor � o dado, desde
*		que tal v�rtice seja um dos sucessores do corrente.
*
*  $EP Par�metros
*     $P pGrafo - ponteiro para a grafo.
*     $P idVertice - identificador do v�rtice
*
*  $FV Valor retornado
*     GRA_CondRetOK
*     GRA_CondRetPonteiroNulo
*	   GRA_CondRetVerticeInvalido
*
*  $EAE Assertivas de entrada
*     pGrafo != NULL
*     pGrafo deve ser ponteiro v�lido para um grafo do m�dulo
*     idVertice deve ser um id de um v�rtice v�lido existente no grafo,
*       e tal v�rtice deve ser um dos sucessores do v�rtice corrente
*
*  $EAE Assertivas de sa�da
*     SE GRA_CondRetOk
*        pGrafo ter� como v�rtice corrente o v�rtice cujo id � idVertice
*     SE GRA_CondRetPonteiroNulo
*        pGrafo era NULL
*     SE GRA_CondRetVerticeInvalido
*        idVertice n�o era um id de um v�rtice v�lido existente no grafo
*           OU era id v�lido mas n�o era sucessor do v�rtice corrente
*
***********************************************************************/

   GRA_tpCondRet GRA_AndarParaVertice( GRA_tppGrafo pGrafo, char idVertice ) ;

   
/***********************************************************************
*
*  $FC Fun��o: GRA Inserir v�rtice
*
*  $ED Descri��o da fun��o
*     Cria um novo v�rtice a partir do valor dado.
*	  O novo v�rtice n�o possui arestas.
*
*  $EP Par�metros
*     $P pGrafo - ponteiro para o grafo.
*     $P pVertice - ponteiro para o conte�do a ser inserido no novo v�rtice criado.
*	   $P idVertice - identificador do v�rtice
*
*  $FV Valor retornado
*     GRA_CondRetOK
*     GRA_CondRetFaltouMemoria
*     GRA_CondRetPonteiroNulo
*
*  $EAE Assertivas de entrada
*     pGrafo != NULL
*     pGrafo deve ser ponteiro v�lido para um grafo do m�dulo
*
*  $EAE Assertivas de sa�da
*     SE GRA_CondRetOk
*        pGrafo ter� um novo v�rtice com o valor e identificador dados
*     SE GRA_CondRetFaltouMemoria
*        pGrafo n�o ser� modificado
*     SE GRA_CondRetPonteiroNulo
*        pGrafo era NULL
*
***********************************************************************/

   GRA_tpCondRet GRA_InserirVertice( GRA_tppGrafo pGrafo, void * pVertice, char idVertice ) ;

   
/***********************************************************************
*
*  $FC Fun��o: GRA Excluir v�rtice corrente
*
*  $ED Descri��o da fun��o
*     Exclui o v�rtice corrente.
*
*  $EP Par�metros
*     $P pGrafo - ponteiro para o grafo.
*
*  $FV Valor retornado
*     GRA_CondRetOK
*	   GRA_CondRetGrafoVazio
*
*  $EAE Assertivas de entrada
*     pGrafo != NULL
*     pGrafo deve ser ponteiro v�lido para um grafo do m�dulo
*
*  $EAE Assertivas de sa�da
*     SE GRA_CondRetOk
*        o antigo v�rtice corrente de pGrafo ser� exclu�do
*        o novo v�rtice corrente ser� o primeiro da lista de v�rtices
*        ou o novo v�rtice corrente ser� NULL, caso o grafo esteja vazio
*     SE GRA_CondRetPonteiroNulo
*        pGrafo era NULL
*
***********************************************************************/

   GRA_tpCondRet GRA_ExcluirVerticeCorrente( GRA_tppGrafo pGrafo ) ;

   
/***********************************************************************
*
*  $FC Fun��o: GRA Inserir aresta
*
*  $ED Descri��o da fun��o
*     Insere uma aresta no grafo
*
*  $EP Par�metros
*     $P pGrafo - ponteiro para o grafo.
*     $P idVerticeOrigem - identificador do v�rtice origem da aresta
*     $P idVerticeDestino - identificador do v�rtice destino da aresta
*     $P idAresta - identificador da aresta a ser criada
*
*  $FV Valor retornado
*     GRA_CondRetOK
*     GRA_CondRetPonteiroNulo
*	   GRA_CondRetArestaJaExiste
*     GRA_CondRetVerticeInvalido
*     GRA_CondRetFaltouMemoria
*
*  $EAE Assertivas de entrada
*     pGrafo != NULL
*     pGrafo deve ser ponteiro v�lido para um grafo do m�dulo
*     idVerticeOrigem deve ser um identificador de v�rtice v�lido
*     idVerticeDestino deve ser um identificador de v�rtice v�lido
*     idAresta deve ser uma string de no m�ximo 9 caracteres v�lidos
*
*  $EAE Assertivas de sa�da
*     SE GRA_CondRetOk
*        pGrafo ter� uma nova aresta apontando do v�rtice origem 
*           ao v�rtice destino
*     SE GRA_CondRetPonteiroNulo
*        pGrafo era NULL
*     SE GRA_CondRetArestaJaExiste
*        j� existia uma aresta ligando o v�rtice origem ao v�rtice destino
*     SE GRA_CondRetVerticeInvalido
*        idVerticeOrigem n�o � um identificador de v�rtice v�lido
*        OU idVerticeDestino n�o � um identificador de v�rtice v�lido
*     SE GRA_CondRetFaltouMemoria
*        pGrafo n�o ser� modificado
*
***********************************************************************/

   GRA_tpCondRet GRA_InserirAresta( GRA_tppGrafo pGrafo, char idVerticeOrigem, char idVerticeDestino, char * idAresta ) ;


/***********************************************************************
*
*  $FC Fun��o: GRA Inserir aresta no corrente
*
*  $ED Descri��o da fun��o
*     Insere uma aresta a partir do v�rtice corrente, dirigida
*     ao v�rtice informado como destino
*
*  $EP Par�metros
*     $P pGrafo - ponteiro para o grafo.
*     $P idVerticeDestino - identificador do v�rtice destino da aresta
*     $P idAresta - identificador da aresta a ser criada, possui no
*           m�ximo 9 caracteres v�lidos.
*
*  $FV Valor retornado
*     GRA_CondRetOK
*     GRA_CondRetPonteiroNulo
*     GRA_CondRetArestaJaExiste
*     GRA_CondRetVerticeInvalido
*     GRA_CondRetFaltouMemoria
*
*  $EAE Assertivas de entrada
*     pGrafo != NULL
*     pGrafo deve ser ponteiro v�lido para um grafo do m�dulo
*     idVerticeDestino deve ser um identificador de v�rtice v�lido
*     idAresta deve ser uma string de no m�ximo 9 caracteres v�lidos
*
*  $EAE Assertivas de sa�da
*     SE GRA_CondRetOk
*        pGrafo ter� uma nova aresta apontando do v�rtice corrente 
*           ao v�rtice destino
*     SE GRA_CondRetPonteiroNulo
*        pGrafo era NULL
*     SE GRA_CondRetArestaJaExiste
*        j� existia uma aresta ligando o v�rtice corrente ao v�rtice destino
*     SE GRA_CondRetVerticeInvalido
*        idVerticeDestino n�o � um identificador de v�rtice v�lido
*     SE GRA_CondRetFaltouMemoria
*        pGrafo n�o ser� modificado
*
***********************************************************************/

   GRA_tpCondRet GRA_InserirArestaOrigemCorrente( GRA_tppGrafo pGrafo, char idVerticeDestino, char * idAresta ) ;

   
/***********************************************************************
*
*  $FC Fun��o: GRA Excluir aresta
*
*  $ED Descri��o da fun��o
*	  Exclui aresta que tem id igual ao fornecido
*
*  $EP Par�metros
*     $P pGrafo - ponteiro para o grafo.
*     $P idAresta - identificador da aresta
*
*  $FV Valor retornado
*     GRA_CondRetOK
*     GRA_CondRetPonteiroNulo
*     GRA_CondRetArestaInvalida
*
*  $EAE Assertivas de entrada
*     pGrafo != NULL
*     pGrafo deve ser ponteiro v�lido para um grafo do m�dulo
*     idAresta deve ser uma string de no m�ximo 9 caracteres
*     idAresta deve ser um identificador v�lido de uma aresta existente
*
*  $EAE Assertivas de sa�da
*     SE GRA_CondRetOk
*        pGrafo ter� uma nova aresta apontando do v�rtice corrente 
*           ao v�rtice destino
*     SE GRA_CondRetPonteiroNulo
*        pGrafo era NULL
*     SE GRA_CondRetArestaInvalida
*        idAresta n�o era um identificado v�lido de uma aresta existente
*
***********************************************************************/

   GRA_tpCondRet GRA_ExcluirAresta( GRA_tppGrafo pGrafo, char * idAresta ) ;


/***********************************************************************
*
*  $FC Fun��o: GRA Adicionar origem
*
*  $ED Descri��o da fun��o
*     Inclui v�rtice na lista de origens do grafo.
*
*  $EP Par�metros
*     $P pGrafo - ponteiro para o grafo.
*     $P idVertice - identificador do v�rtice a ser definido como origem
*
*  $FV Valor retornado
*     GRA_CondRetOK
*     GRA_CondRetPonteiroNulo
*     GRA_CondRetVerticeInvalido
*     GRA_CondRetFaltouMemoria
*
*  $EAE Assertivas de entrada
*     pGrafo != NULL
*     pGrafo deve ser ponteiro v�lido para um grafo do m�dulo
*     idV�rtice deve ser um id v�lido de um v�rtice existente
*
*  $EAE Assertivas de sa�da
*     SE GRA_CondRetOk
*        pGrafo ter� uma nova origem
*        v�rtice cujo id � idVertice ser� uma origem
*     SE GRA_CondRetPonteiroNulo
*        pGrafo era NULL
*     SE GRA_CondRetVerticeInvalido
*        idVertice n�o era um identificado v�lido de uma v�rtice existente
*     SE GRA_CondRetFaltouMemoria
*        pGrafo n�o ser� modificado
*
***********************************************************************/

   GRA_tpCondRet GRA_AdicionarOrigem( GRA_tppGrafo pGrafo, char idVertice ) ;

/***********************************************************************
*
*  $FC Fun��o: GRA Remover origem
*
*  $ED Descri��o da fun��o
*     Remove v�rtice da lista de origens do grafo.
*
*  $EP Par�metros
*     $P pGrafo - ponteiro para o grafo.
*     $P idVertice - identificador do v�rtice a n�o ser mais origem
*
*  $FV Valor retornado
*     GRA_CondRetOK
*     GRA_CondRetPonteiroNulo
*     GRA_CondRetVerticeInvalido
*
*  $EAE Assertivas de entrada
*     pGrafo != NULL
*     pGrafo deve ser ponteiro v�lido para um grafo do m�dulo
*     idV�rtice deve ser um id v�lido de um v�rtice existente
*
*  $EAE Assertivas de sa�da
*     SE GRA_CondRetOk
*        v�rtice cujo id � idVertice n�o ser� mais uma origem
*     SE GRA_CondRetPonteiroNulo
*        pGrafo era NULL
*     SE GRA_CondRetVerticeInvalido
*        idVertice n�o era um identificado v�lido de uma v�rtice existente
*
***********************************************************************/

   GRA_tpCondRet GRA_RemoverOrigem( GRA_tppGrafo pGrafo, char idVertice ) ;
   

#undef GRAFO_EXT

/********** Fim do m�dulo de defini��o: M�dulo grafo **********/

#else
#endif
