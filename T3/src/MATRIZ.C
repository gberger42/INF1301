/***************************************************************************
*  $MCI Módulo de implementação: Módulo matriz quadrada
*
*  Arquivo gerado:              MATRIZ.C
*  Letras identificadoras:      MAT
*
*  Projeto: T3 INF1301 2013.2
*  Autores: gb - Guilherme Berger
*           fl - Felipe Luiz
*           jv - João Vicente
*
***************************************************************************/

#include <stdlib.h>
#include <stdio.h>

#define MATRIZ_OWN
#include "MATRIZ.H"
#undef MATRIZ_OWN


/***********************************************************************
*
*  $TC Tipo de dados: MAT Descritor da cabeça de uma matriz
*
*
*  $ED Descrição do tipo
*     A cabeça da matriz é o ponto de acesso para uma determinada matriz.
*     Por intermédio da referência para a célula corrente pode-se navegar
*     pela matriz.
*
***********************************************************************/

typedef struct MAT_tagMatriz {

    GRA_tppGrafo pGrafo ;
        /* Ponteiro para a célula corrente da matriz */
    int n;
        /* Tamanho da matriz */

} MAT_tpMatriz ;


/*****  Código das funções exportadas pelo módulo  *****/

/**********************************************************************
*  Função: MAT Criar matriz
**********************************************************************/

MAT_tpCondRet MAT_CriarMatriz( MAT_tppMatriz * ppMatriz, int n, void ( * ExcluirValor ) ( void * pDado ) ){
    int linhaCorrente, colunaCorrente;

    if(n <= 0) {
        return MAT_CondRetTamanhoInvalido;
    }

    /* Criar cabeça da matriz */
    *ppMatriz = ( MAT_tpMatriz* ) malloc( sizeof(MAT_tpMatriz) );
    if(*ppMatriz == NULL) {
        return MAT_CondRetFaltouMemoria;
    }

    if( GRA_CriarGrafo(&(*ppMatriz)->pGrafo, ExcluirValor) != GRA_CondRetOK ) {
        free(*ppMatriz);
        return MAT_CondRetFaltouMemoria;
    }
    (*ppMatriz)->n = n;

    /* Encadear células da matriz criada e inicializar ponteiro para lista em cada célula */
    for( linhaCorrente = 0; linhaCorrente < n; linhaCorrente++ ) {
        for( colunaCorrente = 0; colunaCorrente < n; colunaCorrente++ ) {
            if(GRA_InserirVertice( (*ppMatriz)->pGrafo, NULL, linhaCorrente*n + colunaCorrente) != GRA_CondRetOK) {
                GRA_DestruirGrafo( (*ppMatriz)->pGrafo );
                free( *ppMatriz );
                return MAT_CondRetFaltouMemoria;
            }
        } 
    } 

    for( linhaCorrente = 0; linhaCorrente < n; linhaCorrente++ ) {
        for( colunaCorrente = 0; colunaCorrente < n; colunaCorrente++ ) {
            if( linhaCorrente > 0 && GRA_InserirAresta( (*ppMatriz)->pGrafo, linhaCorrente*n + colunaCorrente, (linhaCorrente-1)*n + colunaCorrente, (int)MAT_N) != GRA_CondRetOK) {
                GRA_DestruirGrafo( (*ppMatriz)->pGrafo );
                free( *ppMatriz );
                return MAT_CondRetFaltouMemoria;
            }

            if( colunaCorrente > 0 && GRA_InserirAresta( (*ppMatriz)->pGrafo, linhaCorrente*n + colunaCorrente, linhaCorrente*n + colunaCorrente - 1, (int)MAT_O) != GRA_CondRetOK) {
                GRA_DestruirGrafo( (*ppMatriz)->pGrafo );
                free( *ppMatriz );
                return MAT_CondRetFaltouMemoria;
            }

            if( linhaCorrente < (n-1) && GRA_InserirAresta( (*ppMatriz)->pGrafo, linhaCorrente*n + colunaCorrente, (linhaCorrente+1)*n + colunaCorrente, (int)MAT_S) != GRA_CondRetOK) {
                GRA_DestruirGrafo( (*ppMatriz)->pGrafo );
                free( *ppMatriz );
                return MAT_CondRetFaltouMemoria;
            }

            if( colunaCorrente < (n-1) && GRA_InserirAresta( (*ppMatriz)->pGrafo, linhaCorrente*n + colunaCorrente, linhaCorrente*n + colunaCorrente + 1,(int) MAT_L) != GRA_CondRetOK) {
                GRA_DestruirGrafo( (*ppMatriz)->pGrafo );
                free( *ppMatriz );
                return MAT_CondRetFaltouMemoria;
            }

        } 
    } 

    GRA_IrParaVertice( (*ppMatriz)->pGrafo, 0 );

    return MAT_CondRetOK;
} 


/***************************************************************************
*  Função: MAT Destruir matriz
***************************************************************************/

MAT_tpCondRet MAT_DestruirMatriz( MAT_tppMatriz pMatriz ) {
    if(pMatriz == NULL) {
        return MAT_CondRetPonteiroNulo;
    } 

    GRA_DestruirGrafo( pMatriz->pGrafo );
    free( pMatriz );

    return MAT_CondRetOK;
} 


/***********************************************************************
*  $FC Função: MAT Andar com a célula corrente
***********************************************************************/

MAT_tpCondRet MAT_AndarCorrente( MAT_tppMatriz pMatriz, MAT_tpDirecao direcao ) {
    if(pMatriz == NULL) {
        return MAT_CondRetPonteiroNulo;
    } 

    if( GRA_PercorrerAresta( pMatriz->pGrafo, (int)direcao ) == GRA_CondRetArestaInvalida ) {
        return MAT_CondRetDirecaoInvalida;
    }

    return MAT_CondRetOK;
} 


/***********************************************************************
*  $FC Função: MAT Definir célula corrente
***********************************************************************/

MAT_tpCondRet MAT_DefinirCorrente( MAT_tppMatriz pMatriz, int i, int j ) {
    if(pMatriz == NULL) {
        return MAT_CondRetPonteiroNulo;
    } 

    if( i < 0 || i > (pMatriz->n - 1) ||  j < 0 || j > (pMatriz->n - 1)) {
        return MAT_CondRetPosicaoInvalida;
    }

    if( GRA_IrParaVertice( pMatriz->pGrafo,(pMatriz->n)*i + j) == GRA_CondRetVerticeInvalido ) {
        return MAT_CondRetPosicaoInvalida;
    }

    return MAT_CondRetOK;
} 


/***********************************************************************
*  $FC Função: MAT Atribuir valor
***********************************************************************/

MAT_tpCondRet MAT_AtribuirValorCorrente( MAT_tppMatriz pMatriz, void * pValor ) {
    if(pMatriz == NULL) {
        return MAT_CondRetPonteiroNulo;
    } 

    GRA_AlterarValorCorrente( pMatriz->pGrafo, pValor );

    return MAT_CondRetOK;
} 

/***********************************************************************
*  $FC Função: MAT Obter valor corrente
***********************************************************************/

MAT_tpCondRet MAT_ObterValorCorrente( MAT_tppMatriz pMatriz, void ** ppValor ) {
    if(pMatriz == NULL) {
        return MAT_CondRetPonteiroNulo;
    }

    GRA_ObterValorCorrente( pMatriz->pGrafo, ppValor );

    return MAT_CondRetOK;
} 


/********** Fim do módulo de implementação: Módulo matriz quadrada **********/

