/***************************************************************************
*  $MCI M�dulo de implementa��o: VER  V�rtice de grafo contendo string
*
*  Arquivo gerado:              VERTICE.c
*  Letras identificadoras:      VER
*
*  Projeto: T2 INF1301 2013.2
*  Autores: gb - Guilherme Berger
*           fl - Felipe Luiz
*           jv - Jo�o Vicente
*
***************************************************************************/

#include   <string.h>
#include   <malloc.h>

#define VERTICE_OWN
#include "VERTICE.h"
#undef VERTICE_OWN

/***********************************************************************
*
*  $TC Tipo de dados: VER Elemento de v�rtice
*
*
***********************************************************************/

   typedef struct VER_tagVertice {
      char * conteudo;
   } VER_tpVertice ;

/*****  C�digo das fun��es exportadas pelo m�dulo  *****/

/***************************************************************************
*
*  Fun��o: VER  &Criar v�rtice
*  ****/

   VER_tpCondRet VER_CriarVertice( VER_tppVertice * ppVertice )
   {
      if(ppVertice == NULL) {
         return VER_CondRetPonteiroNulo;
      }

      *ppVertice = (VER_tppVertice) malloc( sizeof(VER_tpVertice) );
      if(*ppVertice == NULL) {
         return VER_CondRetFaltouMemoria;
      }

      (*ppVertice)->conteudo = NULL;

      return VER_CondRetOK;

   } /* Fim fun��o: VER  &Criar v�rtice */

/***************************************************************************
*
*  Fun��o: VER  &Destruir v�rtice
*  ****/

   VER_tpCondRet VER_DestruirVertice( VER_tppVertice pVertice )
   {
      if(pVertice == NULL){
         return VER_CondRetPonteiroNulo;
      }

	   VER_EsvaziarVertice( pVertice );
	   free( pVertice );

	   return VER_CondRetOK;

   } /* Fim fun��o: VER  &Destruir v�rtice */

/***************************************************************************
*
*  Fun��o: VER  &Esvaziar v�rtice
*  ****/

   VER_tpCondRet VER_EsvaziarVertice( VER_tppVertice pVertice )
   {
      if(pVertice == NULL){
         return VER_CondRetPonteiroNulo;
      }

	   if(pVertice->conteudo != NULL) {
		   free(pVertice->conteudo);
		   pVertice->conteudo = NULL;
	   }

	   return VER_CondRetOK;

   } /* Fim fun��o: VER  &Esvaziar v�rtice */

/***************************************************************************
*
*  Fun��o: VER  &Atribuir valor
*  ****/

   VER_tpCondRet VER_AtribuirValor( VER_tppVertice pVertice, char * valor )
   {
      if(pVertice == NULL){
         return VER_CondRetPonteiroNulo;
      }

	   pVertice->conteudo = malloc( strlen( valor) + 1 );
	   if( pVertice->conteudo == NULL )
	   {
		   return VER_CondRetFaltouMemoria;
	   }

	   strcpy( pVertice->conteudo, valor);

	   return VER_CondRetOK;

   } /* Fim fun��o: VER  &Atribuir valor */

/***************************************************************************
*
*  Fun��o: VER  &Obter valor
*  ****/

   VER_tpCondRet VER_ObterValor( VER_tppVertice pVertice, char ** pValor )
   {
      if(pVertice == NULL){
         return VER_CondRetPonteiroNulo;
      }
	   if(pVertice->conteudo == NULL)
	   {
		   return VER_CondRetVerticeVazio;
	   }

      *pValor = pVertice->conteudo;

      return VER_CondRetOK;
      
   } /* Fim fun��o: VER  &Obter valor */

/********** Fim do m�dulo de implementa��o: VER  V�rtice de grafo contendo string **********/

