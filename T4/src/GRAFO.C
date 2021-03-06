/***************************************************************************
*  $MCI M�dulo de implementa��o: M�dulo grafo dirigido
*
*  Arquivo gerado:              GRAFO.C
*  Letras identificadoras:      GRA
*
*  Projeto: T2 INF1301 2013.2
*  Autores: gb - Guilherme Berger
*           fl - Felipe Luiz
*           jv - Jo�o Vicente
*
***************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define GRAFO_OWN
#include "GRAFO.H"
#undef GRAFO_OWN

#ifdef _DEBUG
#include "CONTA.H"
#include "CESPDIN.H"
#endif

/***********************************************************************
*
*  $TC Tipo de dados: GRA Descritor da c�lula do grafo
*
*  $ED Descri��o do tipo
*     Descreve a organiza��o da c�lula
*
***********************************************************************/

   typedef struct GRA_tagVerticeGrafo {
	   LIS_tppLista pListaAnt;
	   LIS_tppLista pListaSuc;
	   char id;
	   void * pValor;
#ifdef _DEBUG
		int idTipo;
		GRA_tppGrafo pCabeca;
		int tamValor;
#endif
   } GRA_tpVerticeGrafo ;

   typedef GRA_tpVerticeGrafo * GRA_tppVerticeGrafo;

/***********************************************************************
*
*  $TC Tipo de dados: GRA Descritor da cabe�a de um grafo
*
*
*  $ED Descri��o do tipo
*     A cabe�a da grafo � o ponto de acesso para uma determinada grafo.
*     Por interm�dio da refer�ncia para a c�lula corrente pode-se navegar
*     pela grafo.
*
***********************************************************************/

   typedef struct GRA_tagGrafo {
	   LIS_tppLista pListaOrigens;
	   LIS_tppLista pListaVertices;
	   GRA_tppVerticeGrafo pVerticeCorrente;
	   void ( * ExcluirValor ) ( void * pValor ) ;
#ifdef _DEBUG
	   int totalElem;
	   int tamValores;
#endif
   } GRA_tpGrafo ;

/***********************************************************************
*
*  $TC Tipo de dados: GRA Descritor da aresta de um grafo
*
*
*  $ED Descri��o do tipo
*     A aresta do grafo � referenciada no valor do elemento da lista
*     de sucessores de um v�rtice.
*
***********************************************************************/

   typedef struct GRA_tagAresta {
	   char idAresta[10];
	   GRA_tppVerticeGrafo pVerticeApontado;
		#ifdef _DEBUG
			GRA_tppGrafo pCabeca;
		#endif
   } GRA_tpAresta ;



/***** Prot�tipo das fun��es encapuladas no m�dulo *****/

	static void DestruirConteudoListaDeAresta (void * pAresta) ;

	static GRA_tppVerticeGrafo CriarVerticeGrafo (void) ;

	static GRA_tpVerticeGrafo * PesquisaVertice( LIS_tppLista pLista, char idVertice ) ;

	static GRA_tpVerticeGrafo * PesquisaVerticeNaListaDeAresta( LIS_tppLista pLista, char idVertice ) ;

	static void RemoverVerticeFreeOpcional ( GRA_tppGrafo pGrafo, GRA_tppVerticeGrafo pVertice, int darFree);

	static void RemoverVertice ( GRA_tppGrafo pGrafo, GRA_tppVerticeGrafo pVertice ) ;

	static GRA_tpCondRet AdicionarAresta( GRA_tppVerticeGrafo pVerticeOrigem, GRA_tppVerticeGrafo pVerticeDestino, char * idAresta) ;


/*****  C�digo das fun��es exportadas pelo m�dulo  *****/

/***********************************************************************
*
*  $FC Fun��o: GRA Criar grafo
*
***********************************************************************/

   GRA_tpCondRet GRA_CriarGrafo( GRA_tppGrafo * ppGrafo, void ( * ExcluirValor ) ( void * pDado ) ) {
	   GRA_tppGrafo pGrafo = NULL;
	   
	   pGrafo = (GRA_tppGrafo) malloc(sizeof(GRA_tpGrafo));
	   if (pGrafo == NULL){
		   return GRA_CondRetFaltouMemoria;
	   }
   	
   	#ifdef _DEBUG
   	   CNT_CONTAR( "GRA_CriarGrafo" ) ;
   	#endif

	   pGrafo->pVerticeCorrente = NULL;
	   
	   pGrafo->pListaOrigens = LIS_CriarLista( NULL ) ;
	   if (pGrafo->pListaOrigens == NULL){
		   free(pGrafo);
		   return GRA_CondRetFaltouMemoria;
	   }

	   #ifdef _DEBUG
	      CNT_CONTAR( "GRA_CriarGrafo-pr0" ) ;
	   #endif

	   pGrafo->pListaVertices = LIS_CriarLista( NULL ) ;
	   if (pGrafo->pListaVertices == NULL){
		   LIS_DestruirLista(pGrafo->pListaOrigens);
		   free(pGrafo);
		   return GRA_CondRetFaltouMemoria;
	   }

	   #ifdef _DEBUG
	      CNT_CONTAR( "GRA_CriarGrafo-pr1" ) ;
	   #endif

	   pGrafo->ExcluirValor = ExcluirValor;

	   *ppGrafo = pGrafo;

#ifdef _DEBUG
	   CED_MarcarEspacoAtivo(pGrafo);
	   pGrafo->totalElem = 0;
	   pGrafo->tamValores = 0;
#endif

	   return GRA_CondRetOK;
   };


/***********************************************************************
*
*  $FC Fun��o: GRA Destruir grafo
*
***********************************************************************/

   GRA_tpCondRet GRA_DestruirGrafo( GRA_tppGrafo pGrafo ) {
	   if (pGrafo == NULL){
		   return GRA_CondRetPonteiroNulo;
	   }
	   
   	#ifdef _DEBUG
   	   CNT_CONTAR( "GRA_DestruirGrafo" ) ;
   	#endif

	   GRA_EsvaziarGrafo(pGrafo);
	   LIS_DestruirLista(pGrafo->pListaOrigens);
	   LIS_DestruirLista(pGrafo->pListaVertices);
	   free(pGrafo);

	   return GRA_CondRetOK;
   }


/***********************************************************************
*
*  $FC Fun��o: GRA Esvaziar
*
***********************************************************************/

   GRA_tpCondRet GRA_EsvaziarGrafo( GRA_tppGrafo pGrafo ) {
   	#ifdef _DEBUG
   	   CNT_CONTAR( "GRA_EsvaziarGrafo" ) ;
   	#endif

	   LIS_IrInicioLista( pGrafo->pListaVertices );
	   while (LIS_ObterValor( pGrafo->pListaVertices ) ) {
	   	#ifdef _DEBUG
	   	   CNT_CONTAR( "GRA_EsvaziarGrafo-while0" ) ;
	   	#endif
	   	RemoverVertice( pGrafo, (GRA_tppVerticeGrafo) LIS_ObterValor( pGrafo->pListaVertices ) );
	   	if(LIS_AvancarElementoCorrente(pGrafo->pListaVertices, 1) == LIS_CondRetFimLista){
		   	#ifdef _DEBUG
		   	   CNT_CONTAR( "GRA_EsvaziarGrafo-while0-if0" ) ;
		   	#endif
				break;
	   	}
	   }
	   LIS_EsvaziarLista( pGrafo->pListaVertices );
	   LIS_EsvaziarLista( pGrafo->pListaOrigens );
	   pGrafo->pVerticeCorrente = NULL;

	   return GRA_CondRetOK;
   }

   
/***********************************************************************
*
*  $FC Fun��o: GRA Obter valor do v�rtice corrente
*
***********************************************************************/

	GRA_tpCondRet GRA_ObterValorCorrente( GRA_tppGrafo pGrafo, void ** ppValor ) {

	   if (pGrafo == NULL){
		   return GRA_CondRetPonteiroNulo;
	   }

		#ifdef _DEBUG
		   CNT_CONTAR( "GRA_ObterValorCorrente" ) ;
		#endif

	   if (pGrafo->pVerticeCorrente == NULL){
   			#ifdef _DEBUG
				   CNT_CONTAR( "GRA_ObterValorCorrente-if0" ) ;
				#endif
		   return GRA_CondRetGrafoVazio;
	   }

	   #ifdef _DEBUG
	      CNT_CONTAR( "GRA_ObterValorCorrente-pr0" ) ;
	   #endif
	   
	   *ppValor = pGrafo->pVerticeCorrente->pValor;
	   	   
	   return GRA_CondRetOK;
   }

   
/***********************************************************************
*
*  $FC Fun��o: GRA Alterar valor do v�rtice corrente
*
***********************************************************************/

   GRA_tpCondRet GRA_AlterarValorCorrente( GRA_tppGrafo pGrafo, void * pValor ) {

	   if (pGrafo == NULL){
		   return GRA_CondRetPonteiroNulo;
	   }

   	#ifdef _DEBUG
   	   CNT_CONTAR( "GRA_AlterarValorCorrente" ) ;
   	#endif

	   if (pGrafo->pVerticeCorrente == NULL){
	   	 #ifdef _DEBUG
	   	    CNT_CONTAR( "GRA_AlterarValorCorrente-if0" ) ;
	   	 #endif
		   return GRA_CondRetGrafoVazio;
	   }

	   pGrafo->ExcluirValor(pGrafo->pVerticeCorrente->pValor);

	   #ifdef _DEBUG
	      CNT_CONTAR( "GRA_AlterarValorCorrente-pr0" ) ;
	   #endif

	   pGrafo->pVerticeCorrente->pValor = pValor;

#ifdef _DEBUG
		pGrafo->pVerticeCorrente->idTipo = CED_ObterTipoEspaco(pValor);
		pGrafo->tamValores -= pGrafo->pVerticeCorrente->tamValor;
		if(pValor != NULL){
			#ifdef _DEBUG
			   CNT_CONTAR( "GRA_AlterarValorCorrente-if1" ) ;
			#endif
			pGrafo->pVerticeCorrente->tamValor = CED_ObterTamanhoValor(pValor);
		} else {
			#ifdef _DEBUG
			   CNT_CONTAR( "GRA_AlterarValorCorrente-else0" ) ;
			#endif
			pGrafo->pVerticeCorrente->tamValor = 0;
		}
		pGrafo->tamValores += pGrafo->pVerticeCorrente->tamValor;
#endif

	   return GRA_CondRetOK;
   }


/***********************************************************************
*
*  $FC Fun��o: GRA Ir para vertice
*
***********************************************************************/

   GRA_tpCondRet GRA_IrParaVertice( GRA_tppGrafo pGrafo, char idVertice ) {
	   GRA_tppVerticeGrafo resultado;
	   if (pGrafo == NULL){
		   return GRA_CondRetPonteiroNulo;
	   }

   	#ifdef _DEBUG
   	   CNT_CONTAR( "GRA_IrParaVertice" ) ;
   	#endif

	   if (pGrafo->pVerticeCorrente == NULL){
	   	 #ifdef _DEBUG
	   	    CNT_CONTAR( "GRA_IrParaVertice-if0" ) ;
	   	 #endif
		   return GRA_CondRetGrafoVazio;
	   }

	   #ifdef _DEBUG
	      CNT_CONTAR( "GRA_IrParaVertice-pr0" ) ;
	   #endif

	   resultado = PesquisaVertice( pGrafo->pListaVertices, idVertice );
	   if(resultado == NULL) {
	   	 #ifdef _DEBUG
	   	    CNT_CONTAR( "GRA_IrParaVertice-if1" ) ;
	   	 #endif
		   return GRA_CondRetVerticeInvalido;
	   }

	   #ifdef _DEBUG
	      CNT_CONTAR( "GRA_IrParaVertice-pr1" ) ;
	   #endif

	   pGrafo->pVerticeCorrente = resultado;
	   
	   return GRA_CondRetOK;
   }


/***********************************************************************
*
*  $FC Fun��o: GRA Andar para vertice
*
***********************************************************************/

   GRA_tpCondRet GRA_AndarParaVertice( GRA_tppGrafo pGrafo, char idVertice ) {
	   GRA_tppVerticeGrafo resultado;
	   if (pGrafo == NULL){
		   return GRA_CondRetPonteiroNulo;
	   }

   	#ifdef _DEBUG
   	   CNT_CONTAR( "GRA_AndarParaVertice" ) ;
   	#endif

	   if (pGrafo->pVerticeCorrente == NULL){
	   	 #ifdef _DEBUG
	   	    CNT_CONTAR( "GRA_AndarParaVertice-if0" ) ;
	   	 #endif
		   return GRA_CondRetGrafoVazio;
	   }

	   #ifdef _DEBUG
	      CNT_CONTAR( "GRA_AndarParaVertice-pr0" ) ;
	   #endif

	   resultado = PesquisaVerticeNaListaDeAresta( pGrafo->pVerticeCorrente->pListaSuc, idVertice );
	   if(resultado == NULL) {
	   	 #ifdef _DEBUG
	   	    CNT_CONTAR( "GRA_AndarParaVertice-if1" ) ;
	   	 #endif
		   return GRA_CondRetVerticeInvalido;
	   }

	   #ifdef _DEBUG
	      CNT_CONTAR( "GRA_AndarParaVertice-pr1" ) ;
	   #endif

	   pGrafo->pVerticeCorrente = resultado;

	   return GRA_CondRetOK;
   }

   
/***********************************************************************
*
*  $FC Fun��o: GRA Inserir v�rtice
*
***********************************************************************/

   GRA_tpCondRet GRA_InserirVertice( GRA_tppGrafo pGrafo, void * pVertice, char idVertice ) {
	   GRA_tppVerticeGrafo vertice;
	   LIS_tpCondRet CondRetLis;
	   
	   if (pGrafo == NULL){
		   return GRA_CondRetPonteiroNulo;
	   }

	   vertice = CriarVerticeGrafo();
	   if(vertice == NULL) {
		   return GRA_CondRetFaltouMemoria;
	   }

   	#ifdef _DEBUG
   	   CNT_CONTAR( "GRA_InserirVertice" ) ;
   	#endif

	   vertice->pValor = pVertice;
	   vertice->id = idVertice;

#ifdef _DEBUG
		(pGrafo->totalElem)++;
		vertice->pCabeca = pGrafo;
		vertice->tamValor = 0;
		vertice->idTipo = CED_ObterTipoEspaco(pVertice);
		pGrafo->tamValores -= vertice->tamValor;
		if(pVertice != NULL){
			#ifdef _DEBUG
			   CNT_CONTAR( "GRA_InserirVertice-if0" ) ;
			#endif
			vertice->tamValor = CED_ObterTamanhoValor(pVertice);
		} else {
			#ifdef _DEBUG
			   CNT_CONTAR( "GRA_InserirVertice-else0" ) ;
			#endif
			vertice->tamValor = 0;
		}
		pGrafo->tamValores += vertice->tamValor;
#endif

	   LIS_IrFinalLista(pGrafo->pListaVertices);
	   CondRetLis = LIS_InserirElementoApos(pGrafo->pListaVertices, (void *) vertice);

	   if(pGrafo->pVerticeCorrente == NULL) {
	   	 #ifdef _DEBUG
	   	    CNT_CONTAR( "GRA_InserirVertice-if1" ) ;
	   	 #endif
		   pGrafo->pVerticeCorrente = vertice;
	   }

	   if (CondRetLis != LIS_CondRetOK){
		   return GRA_CondRetFaltouMemoria;
	   }

	   #ifdef _DEBUG
	      CNT_CONTAR( "GRA_InserirVertice-pr0" ) ;
	   #endif
	   
	   return GRA_CondRetOK;
   }

   
/***********************************************************************
*
*  $FC Fun��o: GRA Excluir v�rtice corrente
*
***********************************************************************/

   GRA_tpCondRet GRA_ExcluirVerticeCorrente( GRA_tppGrafo pGrafo ) {
	   LIS_tpCondRet condRetLis;

	   if (pGrafo == NULL){
		   return GRA_CondRetPonteiroNulo;
	   }

   	#ifdef _DEBUG
   	   CNT_CONTAR( "GRA_ExcluirVerticeCorrente" ) ;
   	#endif

	   RemoverVertice(pGrafo, pGrafo->pVerticeCorrente);

	   LIS_IrInicioLista(pGrafo->pListaVertices);
	   LIS_ProcurarValor(pGrafo->pListaVertices, (void *) pGrafo->pVerticeCorrente);
	   LIS_ExcluirElemento(pGrafo->pListaVertices);

	   LIS_IrInicioLista(pGrafo->pListaOrigens);
	   condRetLis = LIS_ProcurarValor(pGrafo->pListaOrigens, (void *) pGrafo->pVerticeCorrente);
	   if(condRetLis == LIS_CondRetOK) {
	   	 #ifdef _DEBUG
	   	    CNT_CONTAR( "GRA_ExcluirVerticeCorrente-if0" ) ;
	   	 #endif
		   LIS_ExcluirElemento(pGrafo->pListaOrigens);
	   }

	   // Reinicia corrente para o primeiro elemento da lista de v�rtices
	   LIS_IrInicioLista(pGrafo->pListaVertices);
	   pGrafo->pVerticeCorrente = (GRA_tppVerticeGrafo)LIS_ObterValor(pGrafo->pListaVertices);
	   
	   return GRA_CondRetOK;
   }

   
/***********************************************************************
*
*  $FC Fun��o: GRA Inserir aresta
*
***********************************************************************/

   GRA_tpCondRet GRA_InserirAresta( GRA_tppGrafo pGrafo, char idVerticeOrigem, char idVerticeDestino, char * idAresta ) {
	   GRA_tppVerticeGrafo pVerticeOrigem, pVerticeDestino;
	   if (pGrafo == NULL){
		   return GRA_CondRetPonteiroNulo;
	   }

   	#ifdef _DEBUG
   	   CNT_CONTAR( "GRA_InserirAresta" ) ;
   	#endif

	   if (pGrafo->pVerticeCorrente == NULL){
	   	 #ifdef _DEBUG
	   	    CNT_CONTAR( "GRA_InserirAresta-if0" ) ;
	   	 #endif
		   return GRA_CondRetGrafoVazio;
	   }

	   #ifdef _DEBUG
	      CNT_CONTAR( "GRA_InserirAresta-pr0" ) ;
	   #endif

	   pVerticeOrigem = PesquisaVertice(pGrafo->pListaVertices, idVerticeOrigem);

	   if(pVerticeOrigem == NULL) {
	   	 #ifdef _DEBUG
	   	    CNT_CONTAR( "GRA_InserirAresta-if1" ) ;
	   	 #endif
		   return GRA_CondRetVerticeInvalido;
	   }

	   #ifdef _DEBUG
	      CNT_CONTAR( "GRA_InserirAresta-pr1" ) ;
	   #endif

	   if(PesquisaVerticeNaListaDeAresta(pVerticeOrigem->pListaSuc, idVerticeDestino) != NULL) {
	   	 #ifdef _DEBUG
	   	    CNT_CONTAR( "GRA_InserirAresta-if2" ) ;
	   	 #endif
		   return GRA_CondRetArestaJaExiste;
	   }

	   #ifdef _DEBUG
	      CNT_CONTAR( "GRA_InserirAresta-pr2" ) ;
	   #endif

	   pVerticeDestino = PesquisaVertice(pGrafo->pListaVertices, idVerticeDestino);
	   if(pVerticeDestino == NULL) {
	   	 #ifdef _DEBUG
	   	    CNT_CONTAR( "GRA_InserirAresta-if3" ) ;
	   	 #endif
		   return GRA_CondRetVerticeInvalido;
	   }

	   #ifdef _DEBUG
	      CNT_CONTAR( "GRA_InserirAresta-pr3" ) ;
	   #endif

	   return AdicionarAresta(pVerticeOrigem,pVerticeDestino,idAresta);
   }

/***********************************************************************
*
*  $FC Fun��o: GRA Inserir aresta no corrente
*
***********************************************************************/

   GRA_tpCondRet GRA_InserirArestaOrigemCorrente( GRA_tppGrafo pGrafo, char idVerticeDestino, char * idAresta ) {
	   GRA_tppVerticeGrafo pVerticeDestino;
	   if (pGrafo == NULL){
		   return GRA_CondRetPonteiroNulo;
	   }
   	
   	#ifdef _DEBUG
   	   CNT_CONTAR( "GRA_InserirArestaOrigemCorrente" ) ;
   	#endif

	   if (pGrafo->pVerticeCorrente == NULL){
	   	 #ifdef _DEBUG
	   	    CNT_CONTAR( "GRA_InserirArestaOrigemCorrente-if0" ) ;
	   	 #endif
		   return GRA_CondRetGrafoVazio;
	   }

	   #ifdef _DEBUG
	      CNT_CONTAR( "GRA_InserirArestaOrigemCorrente-pr0" ) ;
	   #endif

	   if(PesquisaVerticeNaListaDeAresta(pGrafo->pVerticeCorrente->pListaSuc, idVerticeDestino) != NULL) {
	   	 #ifdef _DEBUG
	   	    CNT_CONTAR( "GRA_InserirArestaOrigemCorrente-if1" ) ;
	   	 #endif
		   return GRA_CondRetArestaJaExiste;
	   }

	   #ifdef _DEBUG
	      CNT_CONTAR( "GRA_InserirArestaOrigemCorrente-pr1" ) ;
	   #endif

	   pVerticeDestino = PesquisaVertice(pGrafo->pListaVertices, idVerticeDestino);
	   if(pVerticeDestino == NULL) {
	   	 #ifdef _DEBUG
	   	    CNT_CONTAR( "GRA_InserirArestaOrigemCorrente-if2" ) ;
	   	 #endif
		   return GRA_CondRetVerticeInvalido;
	   }

	   #ifdef _DEBUG
	      CNT_CONTAR( "GRA_InserirArestaOrigemCorrente-pr2" ) ;
	   #endif

	   return AdicionarAresta(pGrafo->pVerticeCorrente,pVerticeDestino,idAresta);
   }
   

/***********************************************************************
*
*  $FC Fun��o: GRA Excluir aresta
*
***********************************************************************/

   GRA_tpCondRet GRA_ExcluirAresta( GRA_tppGrafo pGrafo, char * idAresta ) {
	   GRA_tppVerticeGrafo pVertice, pVerticeDestino;
	   GRA_tpAresta * pAresta;
	   if (pGrafo == NULL){
		   return GRA_CondRetPonteiroNulo;
	   }

   	#ifdef _DEBUG
   	   CNT_CONTAR( "GRA_ExcluirAresta" ) ;
   	#endif

	   LIS_IrInicioLista( pGrafo->pListaVertices );
	   while (LIS_ObterValor( pGrafo->pListaVertices ) ) {
	   	#ifdef _DEBUG
	   	   CNT_CONTAR( "GRA_ExcluirAresta-while0" ) ;
	   	#endif
			pVertice = (GRA_tppVerticeGrafo) LIS_ObterValor( pGrafo->pListaVertices );

			LIS_IrInicioLista( pVertice->pListaSuc );
			while (LIS_ObterValor( pVertice->pListaSuc ) ) {
				#ifdef _DEBUG
				   CNT_CONTAR( "GRA_ExcluirAresta-while1" ) ;
				#endif
				pAresta = (GRA_tpAresta *) LIS_ObterValor( pVertice->pListaSuc );

				if(strcmp(idAresta, pAresta->idAresta) == 0) {
					#ifdef _DEBUG
					   CNT_CONTAR( "GRA_ExcluirAresta-if0" ) ;
					#endif
					pVerticeDestino = pAresta->pVerticeApontado;

					LIS_ExcluirElemento( pVertice->pListaSuc );

					LIS_IrInicioLista( pVerticeDestino->pListaAnt);
					LIS_ProcurarValor( pVerticeDestino->pListaAnt, pVertice);
					LIS_ExcluirElemento( pVerticeDestino->pListaAnt );
					return GRA_CondRetOK;
				} else if(LIS_AvancarElementoCorrente(pVertice->pListaSuc, 1) == LIS_CondRetFimLista){
					#ifdef _DEBUG
					   CNT_CONTAR( "GRA_ExcluirAresta-if1" ) ;
					#endif
					break;
				}

				#ifdef _DEBUG
				   CNT_CONTAR( "GRA_ExcluirAresta-br0" ) ;
				#endif
			}

   		if(LIS_AvancarElementoCorrente(pGrafo->pListaVertices, 1) == LIS_CondRetFimLista){
   			#ifdef _DEBUG
   			   CNT_CONTAR( "GRA_ExcluirAresta-if2" ) ;
   			#endif
				break;
   		}
	   }

	   #ifdef _DEBUG
	      CNT_CONTAR( "GRA_ExcluirAresta-nwhile0" ) ;
	   #endif
	   
	   return GRA_CondRetArestaInvalida;
   }


/***********************************************************************
*
*  $FC Fun��o: GRA Adicionar origem
*
***********************************************************************/

   GRA_tpCondRet GRA_AdicionarOrigem( GRA_tppGrafo pGrafo, char idVertice ) {
	   GRA_tppVerticeGrafo pVertice;
	   LIS_tpCondRet condRetLis;
	   if (pGrafo == NULL){
		   return GRA_CondRetPonteiroNulo;
	   }
   	
   	#ifdef _DEBUG
   	   CNT_CONTAR( "GRA_AdicionarOrigem" ) ;
   	#endif

	   pVertice = PesquisaVertice(pGrafo->pListaVertices, idVertice);
	   if(pVertice == NULL) {
	   	 #ifdef _DEBUG
	   	    CNT_CONTAR( "GRA_AdicionarOrigem-if0" ) ;
	   	 #endif
		   return GRA_CondRetVerticeInvalido;
	   }

	   #ifdef _DEBUG
	      CNT_CONTAR( "GRA_AdicionarOrigem-pr0" ) ;
	   #endif

	   LIS_IrFinalLista(pGrafo->pListaOrigens);
	   condRetLis = LIS_InserirElementoApos(pGrafo->pListaOrigens, (void *) pVertice);
	   if(condRetLis == LIS_CondRetFaltouMemoria) {
	   		return GRA_CondRetFaltouMemoria;
	   }

	   #ifdef _DEBUG
	      CNT_CONTAR( "GRA_AdicionarOrigem-pr1" ) ;
	   #endif

	   return GRA_CondRetOK;

   }


/***********************************************************************
*
*  $FC Fun��o: GRA Remover origem
*
***********************************************************************/

   GRA_tpCondRet GRA_RemoverOrigem( GRA_tppGrafo pGrafo, char idVertice ) {
	   GRA_tppVerticeGrafo pVertice;
	   if (pGrafo == NULL){
		   return GRA_CondRetPonteiroNulo;
	   }
   	
   	#ifdef _DEBUG
   	   CNT_CONTAR( "GRA_RemoverOrigem" ) ;
   	#endif

	   pVertice = PesquisaVertice(pGrafo->pListaOrigens, idVertice);
	   if(pVertice == NULL) {
	   	 #ifdef _DEBUG
	   	    CNT_CONTAR( "GRA_RemoverOrigem-if0" ) ;
	   	 #endif
		   return GRA_CondRetVerticeInvalido;
	   }

	   #ifdef _DEBUG
	      CNT_CONTAR( "GRA_RemoverOrigem-pr0" ) ;
	   #endif

	   LIS_ExcluirElemento( pGrafo->pListaOrigens );

	   return GRA_CondRetOK;
   }


#ifdef _DEBUG
/***********************************************************************
*
*  $FC Fun��o: GRA Verificar estrutura
*
***********************************************************************/


#define INCERROS (*numErros)++

   GRA_tpCondRet GRA_VerificarEstrutura( GRA_tppGrafo pGrafo, int * numErros ) {
	   int achou, qtd = 0, somaTam = 0;
	   GRA_tppVerticeGrafo pVertice, pVertice2;
	   GRA_tpAresta * pAresta;

	   *numErros = 0;

	   CNT_CONTAR( "GRA_VerificarEstrura" );
	   
	   LIS_IrInicioLista( pGrafo->pListaVertices );
	   if(LIS_ObterValor(pGrafo->pListaVertices) != NULL)
	   {
			CNT_CONTAR( "GRA_VerificarEstrura-if0" );
			if(pGrafo->pVerticeCorrente == NULL){
				CNT_CONTAR( "GRA_VerificarEstrura-if1" );
				INCERROS;
			}

			if(LIS_AvancarElementoCorrente(pGrafo->pListaOrigens, 1) != LIS_CondRetFimLista)
			{
				CNT_CONTAR( "GRA_VerificarEstrura-if2" );
				pVertice = (GRA_tppVerticeGrafo)LIS_ObterValor(pGrafo->pListaVertices);
				if(pVertice != pGrafo->pVerticeCorrente){
					CNT_CONTAR( "GRA_VerificarEstrura-if3" );
					INCERROS;
				}
			}
	   }

	   LIS_IrInicioLista(pGrafo->pListaOrigens);
	   while( 1 ) {
		   CNT_CONTAR( "GRA_VerificarEstrura-while0" );
		   pVertice = (GRA_tppVerticeGrafo) LIS_ObterValor( pGrafo->pListaOrigens );

		   if(pVertice != NULL && CED_ObterTamanhoValor(pVertice)!=-1) {
			   CNT_CONTAR( "GRA_VerificarEstrura-if21" );
			   LIS_IrInicioLista(pGrafo->pListaVertices);
			   if( LIS_ProcurarValor(pGrafo->pListaVertices, pVertice) != LIS_CondRetOK ){
				   CNT_CONTAR( "GRA_VerificarEstrura-if4" );
				   INCERROS;
			   }

		   } else {
			   CNT_CONTAR( "GRA_VerificarEstrura-else0" );
			   INCERROS;
		   }

			if( LIS_AvancarElementoCorrente( pGrafo->pListaOrigens, 1 ) != LIS_CondRetOK ){
				CNT_CONTAR( "GRA_VerificarEstrura-if5" );
				break;
			}
	   }

	   LIS_IrInicioLista(pGrafo->pListaVertices);
	   if(LIS_ProcurarValor(pGrafo->pListaVertices, pGrafo->pVerticeCorrente) != LIS_CondRetOK){
		   CNT_CONTAR( "GRA_VerificarEstrura-if6" );
		   INCERROS;
	   }

	   // Para cada vertice
	   LIS_IrInicioLista(pGrafo->pListaVertices);
	   while( LIS_ObterValor( pGrafo->pListaVertices ) ) {
		   pVertice = (GRA_tppVerticeGrafo) LIS_ObterValor( pGrafo->pListaVertices );

		   CNT_CONTAR( "GRA_VerificarEstrura-while1" );

		   if(pVertice!= NULL && CED_ObterTamanhoValor(pVertice)!=-1) {
			   CNT_CONTAR( "GRA_VerificarEstrura-if22" );
			   // Para cada predecessor
			   LIS_IrInicioLista(pVertice->pListaAnt);
			   while( LIS_ObterValor( pVertice->pListaAnt ) ) {
				   CNT_CONTAR( "GRA_VerificarEstrura-while2" );
					pVertice2 = (GRA_tppVerticeGrafo) LIS_ObterValor( pVertice->pListaAnt );
					if(CED_ObterTamanhoValor(pVertice2) != -1) {
						CNT_CONTAR( "GRA_VerificarEstrura-if23" );
						achou = 0;
						// Para cada sucessor do predecessor
						LIS_IrInicioLista(pVertice2->pListaSuc);
						while( LIS_ObterValor( pVertice2->pListaSuc ) ) {
							CNT_CONTAR( "GRA_VerificarEstrura-while3" );
							pAresta = (GRA_tpAresta *) LIS_ObterValor( pVertice2->pListaSuc );

								if( pAresta->pVerticeApontado == pVertice ){
									CNT_CONTAR( "GRA_VerificarEstrura-if7" );
									achou = 1;
								}

							if( LIS_AvancarElementoCorrente( pVertice2->pListaSuc, 1 ) != LIS_CondRetOK ){
								CNT_CONTAR( "GRA_VerificarEstrura-if8" );
								break;
							}
						} // Fim: Para cada sucessor do predecessor
						if(!achou){
							CNT_CONTAR( "GRA_VerificarEstrura-if9" );
							INCERROS;
						}
					} else {
					   CNT_CONTAR( "GRA_VerificarEstrura-else3" );
					   INCERROS;
				   }
					if( LIS_AvancarElementoCorrente( pVertice->pListaAnt, 1 ) != LIS_CondRetOK ){
						CNT_CONTAR( "GRA_VerificarEstrura-if10" );
						break;
					}
			   } // Fim: Para cada predecessor


			   // Para cada sucessor
			   LIS_IrInicioLista(pVertice->pListaSuc);
			   while( LIS_ObterValor( pVertice->pListaSuc ) ) {
				   CNT_CONTAR( "GRA_VerificarEstrura-while4" );
					pAresta = (GRA_tpAresta *) LIS_ObterValor( pVertice->pListaSuc );
				
						if(pAresta->pCabeca != pGrafo){
							CNT_CONTAR( "GRA_VerificarEstrura-if11" );
							INCERROS;
						}

						if(CED_ObterTamanhoValor(pAresta->pVerticeApontado) != -1) {
							CNT_CONTAR( "GRA_VerificarEstrura-if24" );
							if(pAresta->pVerticeApontado->pCabeca != pGrafo){
								CNT_CONTAR( "GRA_VerificarEstrura-if12" );
								INCERROS;
							}

							LIS_IrInicioLista(pAresta->pVerticeApontado->pListaAnt);
							if(LIS_ProcurarValor(pAresta->pVerticeApontado->pListaAnt, pVertice) != LIS_CondRetOK){
								CNT_CONTAR( "GRA_VerificarEstrura-if13" );
								INCERROS;
							}
						} else {
						   CNT_CONTAR( "GRA_VerificarEstrura-else2" );
						   INCERROS;
					   }

					if( LIS_AvancarElementoCorrente( pVertice->pListaSuc, 1 ) != LIS_CondRetOK ){
						CNT_CONTAR( "GRA_VerificarEstrura-if14" );
						break;
					}
			   } // Fim: Para cada sucessor

			   if(pVertice->pCabeca != pGrafo){
				   CNT_CONTAR( "GRA_VerificarEstrura-if15" );
				   INCERROS;
			   }

			   if((pVertice->pValor == NULL && pVertice->tamValor != 0) || (pVertice->pValor != NULL && pVertice->tamValor != CED_ObterTamanhoValor(pVertice->pValor) )){
				   CNT_CONTAR( "GRA_VerificarEstrura-if16" );
				   INCERROS;
			   }

			   if(pVertice->pValor != NULL && pVertice->idTipo != CED_ObterTipoEspaco(pVertice->pValor)){
				   CNT_CONTAR( "GRA_VerificarEstrura-if17" );
				   INCERROS;
			   }

			   qtd++;

			   somaTam += pVertice->tamValor;
		   } else {
			   CNT_CONTAR( "GRA_VerificarEstrura-else1" );
			   INCERROS;
		   }

		   if( LIS_AvancarElementoCorrente( pGrafo->pListaVertices, 1 ) != LIS_CondRetOK ){
			   CNT_CONTAR( "GRA_VerificarEstrura-if18" );
			   break;
		   }
	   } // Fim: Para cada vertice

	   if(pGrafo->totalElem != qtd){
		   CNT_CONTAR( "GRA_VerificarEstrura-if19" );
		   INCERROS;
	   }

	   if(pGrafo->tamValores != somaTam){
		   CNT_CONTAR( "GRA_VerificarEstrura-if20" );
		   INCERROS;
	   }

	   
	   CNT_CONTAR( "GRA_VerificarEstrura-return" );
	   return GRA_CondRetOK;
   }

#undef INCERROS


/***********************************************************************
*
*  $FC Fun��o: GRA Deturpar estrutura
*
***********************************************************************/

   GRA_tpCondRet GRA_Deturpar( GRA_tppGrafo pGrafo, int flag ) {
		GRA_tppVerticeGrafo pVertice;
		GRA_tpAresta * pAresta;
		void * pValor;
		int concluido = 0;

		pVertice = pGrafo->pVerticeCorrente;
		switch(flag) {
			case 1:
				free(pGrafo->pVerticeCorrente);
				break;
			case 2:
				LIS_IrInicioLista(pVertice->pListaSuc);
				pAresta = LIS_ObterValor(pVertice->pListaSuc);
				if(pAresta != NULL) {
					pAresta->pVerticeApontado = NULL;
					concluido = 1;
				}	
				break;
			case 3:
				LIS_IrInicioLista(pGrafo->pListaVertices);
				while( !concluido ) {
					pVertice = (GRA_tppVerticeGrafo)LIS_ObterValor(pGrafo->pListaVertices);
					LIS_IrInicioLista(pVertice->pListaAnt);
					if(LIS_ExcluirElemento( pVertice->pListaAnt ) == LIS_CondRetOK) {
						LIS_InserirElementoAntes( pVertice->pListaAnt, NULL );
						concluido = 1;
					}

					if(LIS_AvancarElementoCorrente(pGrafo->pListaVertices,1) != LIS_CondRetOK) break;
				}
				break;
			case 4:
				LIS_IrInicioLista(pVertice->pListaSuc);
				pAresta = LIS_ObterValor(pVertice->pListaSuc);
				if(pAresta != NULL) {
					pAresta->pVerticeApontado = (GRA_tppVerticeGrafo)0x00012345;
					concluido = 1;
				}
				break;
			case 5:
				LIS_IrInicioLista(pVertice->pListaAnt);
				if(LIS_ExcluirElemento( pVertice->pListaAnt ) == LIS_CondRetOK) {
					LIS_InserirElementoAntes( pVertice->pListaAnt, (void*)0x00012345 );
					concluido = 1;
				}
				break;
			case 6:
				if(pVertice != NULL) {
					pVertice->pValor = NULL;
					concluido = 1;
				}
				break;
			case 7:
				if(pVertice != NULL) {
					pVertice->idTipo = 40;
					concluido = 1;
				}
				break;
			case 8:
				if(pVertice != NULL) {
					RemoverVerticeFreeOpcional(pGrafo, pVertice, 0);
					CED_MarcarEspacoNaoAtivo(pVertice);
					concluido = 1;
				}
				break;
			case 9:
				pGrafo->pVerticeCorrente = NULL;
				break;
			case 10:
				LIS_IrInicioLista(pGrafo->pListaOrigens);
				while( !concluido ) {
					pVertice = (GRA_tppVerticeGrafo)LIS_ObterValor(pGrafo->pListaOrigens);
					if(pVertice != NULL) {
						LIS_ExcluirElemento(pGrafo->pListaOrigens);
						LIS_InserirElementoAntes(pGrafo->pListaOrigens, NULL);
						concluido = 1;
					}

					if(LIS_AvancarElementoCorrente(pGrafo->pListaOrigens,1) != LIS_CondRetOK) break;
				}
				break;
			case 11:
				LIS_EsvaziarLista(pGrafo->pListaVertices);
				break;
			case 12:
				// Para cada vertice
				   LIS_IrInicioLista(pGrafo->pListaVertices);
				   while( LIS_ObterValor( pGrafo->pListaVertices ) ) {
					   pVertice = (GRA_tppVerticeGrafo) LIS_ObterValor( pGrafo->pListaVertices );
					   pVertice->pCabeca = (GRA_tppGrafo)0x00012345;
			
						// Para cada sucessor
						LIS_IrInicioLista(pVertice->pListaSuc);
						while( LIS_ObterValor( pVertice->pListaSuc ) ) {
							pAresta = (GRA_tpAresta *) LIS_ObterValor( pVertice->pListaSuc );
				
							pAresta->pCabeca = (GRA_tpAresta *)0x00012345;

							if( LIS_AvancarElementoCorrente( pVertice->pListaSuc, 1 ) != LIS_CondRetOK ){
								break;
							}
						} // Fim: Para cada sucessor

					   if( LIS_AvancarElementoCorrente( pGrafo->pListaVertices, 1 ) != LIS_CondRetOK ){
						   break;
					   }
				   }
				   break;
				
		}

	   return GRA_CondRetOK;
   }
#endif

/***** C�digo das fun��es encapuladas no m�dulo *****/


/***********************************************************************
*
*  $FC Fun��o: GRA - Destruir conteudo lista de aresta
*
*  $ED Descri��o da fun��o
*  Fun��o auxiliar para ser passada durante a cria�a da lista de
*  sucessores
*
***********************************************************************/

   static void DestruirConteudoListaDeAresta (void * pAresta) {
   		#ifdef _DEBUG
   		   CNT_CONTAR( "DestruirConteudoListaDeAresta" ) ;
   		#endif
	   free(pAresta);
   }


/***********************************************************************
*
*  $FC Fun��o: GRA - Criar Vertice Grafo
*
*  $ED Descri��o da fun��o
*  Cria estrutura interna do v�rtice, com as devidas verifica��es
*  de mem�ria
*
***********************************************************************/

   static GRA_tppVerticeGrafo CriarVerticeGrafo (void){
	   GRA_tppVerticeGrafo pVertice = NULL;
	   
	   pVertice = (GRA_tppVerticeGrafo) malloc(sizeof(GRA_tpVerticeGrafo));
	   if (pVertice == NULL){
		   return NULL;
	   }

	   pVertice->pValor = NULL;
	   
	   pVertice->pListaAnt = LIS_CriarLista( NULL ) ;
	   if (pVertice->pListaAnt == NULL){
		   free(pVertice);
		   return NULL;
	   }

	   pVertice->pListaSuc = LIS_CriarLista( DestruirConteudoListaDeAresta ) ;
	   if (pVertice->pListaSuc == NULL){
		   LIS_DestruirLista(pVertice->pListaAnt);
		   free(pVertice);
		   return NULL;
	   }

	   #ifdef _DEBUG
     	CNT_CONTAR( "CriarVerticeGrafo" ) ;
	   	CED_MarcarEspacoAtivo(pVertice);
		 #endif

	   return pVertice;
   }


/***********************************************************************
*
*  $FC Fun��o: GRA - Pesquisa Vertice
*
*  $ED Descri��o da fun��o
*  Pesquisa um v�rtice pelo seu id em uma lista cujos valores
*  referenciam estruturas de v�rtice do tipo GRA_tpVerticeGrafo
*
***********************************************************************/

   static GRA_tpVerticeGrafo * PesquisaVertice( LIS_tppLista pLista, char idVertice ) {
	   GRA_tpVerticeGrafo * pVerticeGrafo;
	   LIS_IrInicioLista( pLista );

	   #ifdef _DEBUG
	      CNT_CONTAR( "PesquisaVertice" ) ;
	   #endif

	   while (LIS_ObterValor( pLista ) ) {

	   	#ifdef _DEBUG
	   	   CNT_CONTAR( "PesquisaVertice-while0" ) ;
	   	#endif

		   pVerticeGrafo = (GRA_tpVerticeGrafo *) LIS_ObterValor( pLista );
		   if( pVerticeGrafo != NULL && pVerticeGrafo->id == idVertice ){
		   		#ifdef _DEBUG
		   		   CNT_CONTAR( "PesquisaVertice-if0" ) ;
		   		#endif
			   return pVerticeGrafo;
		   }

		   #ifdef _DEBUG
		      CNT_CONTAR( "PesquisaVertice-pr0" ) ;
		   #endif

		    if( LIS_AvancarElementoCorrente( pLista, 1 ) == LIS_CondRetFimLista ){
		    	#ifdef _DEBUG
		    	   CNT_CONTAR( "PesquisaVertice-if1" ) ;
		    	#endif
					break;
				}

				#ifdef _DEBUG
				   CNT_CONTAR( "PesquisaVertice-br0" ) ;
				#endif
	   }

	   #ifdef _DEBUG
	      CNT_CONTAR( "PesquisaVertice-nwhile1" ) ;
	   #endif

	   return NULL;
   }


/***********************************************************************
*
*  $FC Fun��o: GRA - Pesquisa Vertice na lista de aresta
*
*  $ED Descri��o da fun��o
*  Pesquisa um v�rtice pelo seu id em uma lista cujos valores
*  referenciam estruturas de aresta do tipo GRA_tpAresta, que por
*  vez referenciam estruturas do tipo GRA_tpVerticeGrafo
*
***********************************************************************/

   static GRA_tpVerticeGrafo * PesquisaVerticeNaListaDeAresta( LIS_tppLista pLista, char idVertice ) {
	   GRA_tpAresta * pAresta;
	   LIS_IrInicioLista( pLista );

	   #ifdef _DEBUG
	      CNT_CONTAR( "PesquisaVerticeNaListaDeAresta" ) ;
	   #endif

	   while (LIS_ObterValor( pLista ) ) {

	   	#ifdef _DEBUG
	   	   CNT_CONTAR( "PesquisaVerticeNaListaDeAresta-while0" ) ;
	   	#endif

		   pAresta = (GRA_tpAresta *) LIS_ObterValor( pLista );

		   if( pAresta != NULL && pAresta->pVerticeApontado->id == idVertice ){
		   		#ifdef _DEBUG
		   		   CNT_CONTAR( "PesquisaVerticeNaListaDeAresta-if0" ) ;
		   		#endif
			   return pAresta->pVerticeApontado;
		   }

		   #ifdef _DEBUG
		      CNT_CONTAR( "PesquisaVerticeNaListaDeAresta-pr0" ) ;
		   #endif


		   if( LIS_AvancarElementoCorrente( pLista, 1 ) == LIS_CondRetFimLista ){
		   	#ifdef _DEBUG
		   	   CNT_CONTAR( "PesquisaVerticeNaListaDeAresta-if1" ) ;
		   	#endif
				break;
			 }

			 #ifdef _DEBUG
			    CNT_CONTAR( "PesquisaVerticeNaListaDeAresta-br0" ) ;
			 #endif
	   }

	   #ifdef _DEBUG
	      CNT_CONTAR( "PesquisaVerticeNaListaDeAresta-nwhile0" ) ;
	   #endif

	   return NULL;
   }


/***********************************************************************
*
*  $FC Fun��o: GRA - Remover v�rtice com free opcional
*
*  $ED Descri��o da fun��o
*  Remove pVertice de pGrafo, destruindo todas as arestas em seus
*  antecessores e sucessores, al�m do conte�do armazenado neste
*  - darFree deve ser 1 para que o espaco seja destruido
*
***********************************************************************/

   static void RemoverVerticeFreeOpcional ( GRA_tppGrafo pGrafo, GRA_tppVerticeGrafo pVertice, int darFree){
	   GRA_tpVerticeGrafo * pVerticeSuc;
	   GRA_tpAresta * pAresta;

	   #ifdef _DEBUG
	      CNT_CONTAR( "RemoverVertice" ) ;
	   #endif
	   
	   //Remover arestas sucessoras
	   LIS_IrInicioLista( pVertice->pListaSuc );
	   
	   while( LIS_ObterValor( pVertice->pListaSuc ) ) {

	   	#ifdef _DEBUG
	   	   CNT_CONTAR( "RemoverVertice-while0" ) ;
	   	#endif

		   pAresta = (GRA_tpAresta *) LIS_ObterValor( pVertice->pListaSuc );
		   if( pAresta != NULL && pAresta->pVerticeApontado != NULL && pAresta->pVerticeApontado->pListaAnt != NULL) {

		   	 #ifdef _DEBUG
		   	    CNT_CONTAR( "RemoverVertice-if0" ) ;
		   	 #endif

			   LIS_IrInicioLista( pAresta->pVerticeApontado->pListaAnt );
			   LIS_ProcurarValor( pAresta->pVerticeApontado->pListaAnt, pVertice );
			   LIS_ExcluirElemento( pAresta->pVerticeApontado->pListaAnt );
		   }

		   if( LIS_AvancarElementoCorrente( pVertice->pListaSuc, 1 ) == LIS_CondRetFimLista ){
		   	 #ifdef _DEBUG
		   	    CNT_CONTAR( "RemoverVertice-if1" ) ;
		   	 #endif
			   break;
		   }

	   }

	   //Remover arestas antecessoras
	   LIS_IrInicioLista( pVertice->pListaAnt );
	   
		while(LIS_ObterValor( pVertice->pListaAnt ) ) {

			#ifdef _DEBUG
			   CNT_CONTAR( "RemoverVertice-while1" ) ;
			#endif

		   pVerticeSuc = (GRA_tpVerticeGrafo *) LIS_ObterValor( pVertice->pListaAnt );
		   if( pVerticeSuc != NULL && pVerticeSuc->pListaSuc != NULL) {

		   	 #ifdef _DEBUG
		   	    CNT_CONTAR( "RemoverVertice-if2" ) ;
		   	 #endif

			   PesquisaVerticeNaListaDeAresta( pVerticeSuc->pListaSuc, pVertice->id );
			   LIS_ExcluirElemento( pVerticeSuc->pListaSuc );
		   }

		   if( LIS_AvancarElementoCorrente( pVertice->pListaAnt, 1 ) == LIS_CondRetFimLista ){
		   	 #ifdef _DEBUG
		   	    CNT_CONTAR( "RemoverVertice-if3" ) ;
		   	 #endif
			   break;
		   }
	   }

	   pGrafo->ExcluirValor( pVertice->pValor );

	   if(darFree){
		   #ifdef _DEBUG
	   			CNT_CONTAR( "RemoverVertice-if4" ) ;
			#endif
		LIS_DestruirLista( pVertice->pListaSuc );
		pVertice->pListaSuc = NULL;
		LIS_DestruirLista( pVertice->pListaAnt );
		pVertice->pListaAnt = NULL;
	   	free( pVertice );
	   }

#ifdef _DEBUG
	   (pGrafo->totalElem)--;
#endif

   }

/***********************************************************************
*
*  $FC Fun��o: GRA - Remover v�rtice
*
*  $ED Descri��o da fun��o
*  Remove pVertice de pGrafo, chamando RemoverVerticeFreeOpcional
*  com darFree 1
*
***********************************************************************/

   static void RemoverVertice ( GRA_tppGrafo pGrafo, GRA_tppVerticeGrafo pVertice){
	   return RemoverVerticeFreeOpcional(pGrafo, pVertice, 1);
   }


/***********************************************************************
*
*  $FC Fun��o: GRA - Adicionar Aresta
*
*  $ED Descri��o da fun��o
*  Adiciona uma aresta de pVerticeOrigem para pVerticeDestino,
*  identificada por idAresta
*
***********************************************************************/

   static GRA_tpCondRet AdicionarAresta( GRA_tppVerticeGrafo pVerticeOrigem, GRA_tppVerticeGrafo pVerticeDestino, char * idAresta) {

	   LIS_tpCondRet CondRetLis;
	   GRA_tpAresta * pAresta;
	   pAresta = (GRA_tpAresta *)malloc(sizeof(GRA_tpAresta));
	   if(pAresta == NULL) {
		   return GRA_CondRetFaltouMemoria;
	   }

	   #ifdef _DEBUG
	      CNT_CONTAR( "AdicionarAresta" ) ;
	   #endif

	   strcpy(pAresta->idAresta, idAresta);
	   pAresta->pVerticeApontado = pVerticeDestino;
#ifdef _DEBUG
	   pAresta->pCabeca = pVerticeOrigem->pCabeca;
#endif

	   LIS_IrFinalLista(pVerticeOrigem->pListaSuc);
	   CondRetLis = LIS_InserirElementoApos(pVerticeOrigem->pListaSuc, (void *) pAresta);
	   if(CondRetLis == LIS_CondRetFaltouMemoria) {
		   return GRA_CondRetFaltouMemoria;
	   }

	   #ifdef _DEBUG
	      CNT_CONTAR( "AdicionarAresta-pr1" ) ;
	   #endif

	   LIS_IrFinalLista(pVerticeDestino->pListaAnt);
	   CondRetLis = LIS_InserirElementoApos(pVerticeDestino->pListaAnt, (void *) pVerticeOrigem);
	   if(CondRetLis == LIS_CondRetFaltouMemoria) {
		   return GRA_CondRetFaltouMemoria;
	   }

	   #ifdef _DEBUG
	      CNT_CONTAR( "AdicionarAresta-pr2" ) ;
	   #endif

#ifdef _DEBUG
	   CED_MarcarEspacoAtivo(pAresta);
#endif

	   return GRA_CondRetOK;
   }


/********** Fim do m�dulo de implementa��o: M�dulo grafo dirigido **********/
