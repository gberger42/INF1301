/***************************************************************************
*  $MCI M�dulo de implementa��o: M�dulo grafo dirigido
*
*  Arquivo gerado:              GRAFO.C
*  Letras identificadoras:      GRA
*
*  Projeto: T3 INF1301 2013.2
*  Autores: gb - Guilherme Berger
*           fl - Felipe Luiz
*           jv - Jo�o Vicente
*
***************************************************************************/

#include <stdlib.h>
#include <string.h>

#define GRAFO_OWN
#include "GRAFO.H"
#undef GRAFO_OWN

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
    int id;
    void * pValor;
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
    int idAresta;
    GRA_tppVerticeGrafo pVerticeApontado;
} GRA_tpAresta ;



/***** Prot�tipo das fun��es encapuladas no m�dulo *****/

static void DestruirConteudoListaDeAresta (void * pAresta) ;

static GRA_tppVerticeGrafo CriarVerticeGrafo (void) ;

static GRA_tpVerticeGrafo * PesquisaVertice( LIS_tppLista pLista, int idVertice ) ;

static GRA_tpVerticeGrafo * PesquisaVerticeNaListaDeAresta( LIS_tppLista pLista, int idVertice ) ;

static void RemoverVertice ( GRA_tppGrafo pGrafo, GRA_tppVerticeGrafo pVertice ) ;

static GRA_tpCondRet AdicionarAresta( GRA_tppVerticeGrafo pVerticeOrigem, GRA_tppVerticeGrafo pVerticeDestino, int idAresta) ;


/*****  C�digo das fun��es exportadas pelo m�dulo  *****/

/***********************************************************************
*
*  $FC Fun��o: GRA Criar grafo
*
***********************************************************************/

GRA_tpCondRet GRA_CriarGrafo( GRA_tppGrafo * ppGrafo, void ( * ExcluirValor ) ( void * pDado ) ) {
    GRA_tppGrafo pGrafo = NULL;

    pGrafo = (GRA_tppGrafo) malloc(sizeof(GRA_tpGrafo));
    if (pGrafo == NULL) {
        return GRA_CondRetFaltouMemoria;
    }

    pGrafo->pVerticeCorrente = NULL;

    pGrafo->pListaOrigens = LIS_CriarLista( NULL ) ;
    if (pGrafo->pListaOrigens == NULL) {
        free(pGrafo);
        return GRA_CondRetFaltouMemoria;
    }

    pGrafo->pListaVertices = LIS_CriarLista( NULL ) ;
    if (pGrafo->pListaVertices == NULL) {
        LIS_DestruirLista(pGrafo->pListaOrigens);
        free(pGrafo);
        return GRA_CondRetFaltouMemoria;
    }

    pGrafo->ExcluirValor = ExcluirValor;

    *ppGrafo = pGrafo;

    return GRA_CondRetOK;
};


/***********************************************************************
*
*  $FC Fun��o: GRA Destruir grafo
*
***********************************************************************/

GRA_tpCondRet GRA_DestruirGrafo( GRA_tppGrafo pGrafo ) {
    if (pGrafo == NULL) {
        return GRA_CondRetPonteiroNulo;
    }

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

    LIS_IrInicioLista( pGrafo->pListaVertices );
    while (LIS_ObterValor( pGrafo->pListaVertices ) ) {
        RemoverVertice( pGrafo, (GRA_tppVerticeGrafo) LIS_ObterValor( pGrafo->pListaVertices ) );
        if(LIS_AvancarElementoCorrente(pGrafo->pListaVertices, 1) != LIS_CondRetOK)
            break;
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

    if (pGrafo == NULL) {
        return GRA_CondRetPonteiroNulo;
    }

    if (pGrafo->pVerticeCorrente == NULL) {
        return GRA_CondRetGrafoVazio;
    }

    *ppValor = pGrafo->pVerticeCorrente->pValor;

    return GRA_CondRetOK;
}


/***********************************************************************
*
*  $FC Fun��o: GRA Alterar valor do v�rtice corrente
*
***********************************************************************/

GRA_tpCondRet GRA_AlterarValorCorrente( GRA_tppGrafo pGrafo, void * pValor ) {

    if (pGrafo == NULL) {
        return GRA_CondRetPonteiroNulo;
    }

    if (pGrafo->pVerticeCorrente == NULL) {
        return GRA_CondRetGrafoVazio;
    }

    pGrafo->pVerticeCorrente->pValor = pValor;

    return GRA_CondRetOK;
}


/***********************************************************************
*
*  $FC Fun��o: GRA Ir para vertice
*
***********************************************************************/

GRA_tpCondRet GRA_IrParaVertice( GRA_tppGrafo pGrafo, int idVertice ) {
    GRA_tppVerticeGrafo resultado;
    if (pGrafo == NULL) {
        return GRA_CondRetPonteiroNulo;
    }

    if (pGrafo->pVerticeCorrente == NULL) {
        return GRA_CondRetGrafoVazio;
    }

    resultado = PesquisaVertice( pGrafo->pListaVertices, idVertice );
    if(resultado == NULL) {
        return GRA_CondRetVerticeInvalido;
    }

    pGrafo->pVerticeCorrente = resultado;

    return GRA_CondRetOK;
}


/***********************************************************************
*
*  $FC Fun��o: GRA Andar para vertice
*
***********************************************************************/

GRA_tpCondRet GRA_AndarParaVertice( GRA_tppGrafo pGrafo, int idVertice ) {
    GRA_tppVerticeGrafo resultado;
    if (pGrafo == NULL) {
        return GRA_CondRetPonteiroNulo;
    }

    if (pGrafo->pVerticeCorrente == NULL) {
        return GRA_CondRetGrafoVazio;
    }

    resultado = PesquisaVerticeNaListaDeAresta( pGrafo->pVerticeCorrente->pListaSuc, idVertice );
    if(resultado == NULL) {
        return GRA_CondRetVerticeInvalido;
    }

    pGrafo->pVerticeCorrente = resultado;

    return GRA_CondRetOK;
}


/***********************************************************************
*
*  $FC Fun��o: GRA Inserir v�rtice
*
***********************************************************************/

GRA_tpCondRet GRA_InserirVertice( GRA_tppGrafo pGrafo, void * pVertice, int idVertice ) {
    GRA_tppVerticeGrafo vertice;
    LIS_tpCondRet CondRetLis;

    if (pGrafo == NULL) {
        return GRA_CondRetPonteiroNulo;
    }

    vertice = CriarVerticeGrafo();
    if(vertice == NULL) {
        return GRA_CondRetFaltouMemoria;
    }

    vertice->pValor = pVertice;
    vertice->id = idVertice;

    LIS_IrFinalLista(pGrafo->pListaVertices);
    CondRetLis = LIS_InserirElementoApos(pGrafo->pListaVertices, (void *) vertice);

    if(pGrafo->pVerticeCorrente == NULL) {
        pGrafo->pVerticeCorrente = vertice;
    }

    if (CondRetLis != LIS_CondRetOK) {
        return GRA_CondRetFaltouMemoria;
    }

    return GRA_CondRetOK;
}


/***********************************************************************
*
*  $FC Fun��o: GRA Excluir v�rtice corrente
*
***********************************************************************/

GRA_tpCondRet GRA_ExcluirVerticeCorrente( GRA_tppGrafo pGrafo ) {
    LIS_tpCondRet condRetLis;

    if (pGrafo == NULL) {
        return GRA_CondRetPonteiroNulo;
    }

    RemoverVertice(pGrafo, pGrafo->pVerticeCorrente);
    LIS_ProcurarValor(pGrafo->pListaVertices, (void *) pGrafo->pVerticeCorrente);
    LIS_ExcluirElemento(pGrafo->pListaVertices);

    condRetLis = LIS_ProcurarValor(pGrafo->pListaOrigens, (void *) pGrafo->pVerticeCorrente);
    if(condRetLis == LIS_CondRetOK) {
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

GRA_tpCondRet GRA_InserirAresta( GRA_tppGrafo pGrafo, int idVerticeOrigem, int idVerticeDestino, int idAresta ) {
    GRA_tppVerticeGrafo pVerticeOrigem, pVerticeDestino;
    if (pGrafo == NULL) {
        return GRA_CondRetPonteiroNulo;
    }

    if (pGrafo->pVerticeCorrente == NULL) {
        return GRA_CondRetGrafoVazio;
    }

    pVerticeOrigem = PesquisaVertice(pGrafo->pListaVertices, idVerticeOrigem);

    if(pVerticeOrigem == NULL) {
        return GRA_CondRetVerticeInvalido;
    }

    if(PesquisaVerticeNaListaDeAresta(pVerticeOrigem->pListaSuc, idVerticeDestino) != NULL) {
        return GRA_CondRetArestaJaExiste;
    }

    pVerticeDestino = PesquisaVertice(pGrafo->pListaVertices, idVerticeDestino);
    if(pVerticeDestino == NULL) {
        return GRA_CondRetVerticeInvalido;
    }

    return AdicionarAresta(pVerticeOrigem,pVerticeDestino,idAresta);
}

/***********************************************************************
*
*  $FC Fun��o: GRA Inserir aresta no corrente
*
***********************************************************************/

GRA_tpCondRet GRA_InserirArestaOrigemCorrente( GRA_tppGrafo pGrafo, int idVerticeDestino, int idAresta ) {
    GRA_tppVerticeGrafo pVerticeDestino;
    if (pGrafo == NULL) {
        return GRA_CondRetPonteiroNulo;
    }

    if (pGrafo->pVerticeCorrente == NULL) {
        return GRA_CondRetGrafoVazio;
    }

    if(PesquisaVerticeNaListaDeAresta(pGrafo->pVerticeCorrente->pListaSuc, idVerticeDestino) != NULL) {
        return GRA_CondRetArestaJaExiste;
    }

    pVerticeDestino = PesquisaVertice(pGrafo->pListaVertices, idVerticeDestino);
    if(pVerticeDestino == NULL) {
        return GRA_CondRetVerticeInvalido;
    }

    return AdicionarAresta(pGrafo->pVerticeCorrente,pVerticeDestino,idAresta);
}

/***********************************************************************
*
*  $FC Fun��o: GRA Percorrer aresta
*
***********************************************************************/

GRA_tpCondRet GRA_PercorrerAresta( GRA_tppGrafo pGrafo, int idAresta ) {
    GRA_tpAresta * pAresta;
    if (pGrafo == NULL) {
        return GRA_CondRetPonteiroNulo;
    }

    if (pGrafo->pVerticeCorrente == NULL) {
        return GRA_CondRetGrafoVazio;
    }

    LIS_IrInicioLista( pGrafo->pVerticeCorrente->pListaSuc );
    while (LIS_ObterValor( pGrafo->pVerticeCorrente->pListaSuc ) ) {
        pAresta = (GRA_tpAresta *) LIS_ObterValor( pGrafo->pVerticeCorrente->pListaSuc );

        if( pAresta != NULL && pAresta->idAresta == idAresta ) {
            pGrafo->pVerticeCorrente = pAresta->pVerticeApontado;
            return GRA_CondRetOK;
        }


        if( LIS_AvancarElementoCorrente( pGrafo->pVerticeCorrente->pListaSuc, 1 ) != LIS_CondRetOK )
            break;
    }

    return GRA_CondRetArestaInvalida;

}


/***********************************************************************
*
*  $FC Fun��o: GRA Excluir aresta
*
***********************************************************************/

GRA_tpCondRet GRA_ExcluirAresta( GRA_tppGrafo pGrafo, int idAresta ) {
    GRA_tppVerticeGrafo pVertice, pVerticeDestino;
    GRA_tpAresta * pAresta;
    if (pGrafo == NULL) {
        return GRA_CondRetPonteiroNulo;
    }

    LIS_IrInicioLista( pGrafo->pListaVertices );
    while (LIS_ObterValor( pGrafo->pListaVertices ) ) {
        pVertice = (GRA_tppVerticeGrafo) LIS_ObterValor( pGrafo->pListaVertices );

        LIS_IrInicioLista( pVertice->pListaSuc );
        while (LIS_ObterValor( pVertice->pListaSuc ) ) {
            pAresta = (GRA_tpAresta *) LIS_ObterValor( pVertice->pListaSuc );

            if( idAresta == pAresta->idAresta) {
                pVerticeDestino = pAresta->pVerticeApontado;

                LIS_ExcluirElemento( pVertice->pListaSuc );

                LIS_ProcurarValor( pVerticeDestino->pListaAnt, pVertice);
                LIS_ExcluirElemento( pVerticeDestino->pListaAnt );
                return GRA_CondRetOK;
            }

            if(LIS_AvancarElementoCorrente(pVertice->pListaSuc, 1) != LIS_CondRetOK)
                break;
        }

        if(LIS_AvancarElementoCorrente(pGrafo->pListaVertices, 1) != LIS_CondRetOK)
            break;
    }

    return GRA_CondRetArestaInvalida;
}


/***********************************************************************
*
*  $FC Fun��o: GRA Adicionar origem
*
***********************************************************************/

GRA_tpCondRet GRA_AdicionarOrigem( GRA_tppGrafo pGrafo, int idVertice ) {
    GRA_tppVerticeGrafo pVertice;
    LIS_tpCondRet condRetLis;
    if (pGrafo == NULL) {
        return GRA_CondRetPonteiroNulo;
    }

    pVertice = PesquisaVertice(pGrafo->pListaVertices, idVertice);
    if(pVertice == NULL) {
        return GRA_CondRetVerticeInvalido;
    }

    LIS_IrFinalLista(pGrafo->pListaOrigens);
    condRetLis = LIS_InserirElementoApos(pGrafo->pListaOrigens, (void *) pVertice);
    if(condRetLis == LIS_CondRetFaltouMemoria) {
        return GRA_CondRetFaltouMemoria;
    }

    return GRA_CondRetOK;

}


/***********************************************************************
*
*  $FC Fun��o: GRA Remover origem
*
***********************************************************************/

GRA_tpCondRet GRA_RemoverOrigem( GRA_tppGrafo pGrafo, int idVertice ) {
    GRA_tppVerticeGrafo pVertice;
    if (pGrafo == NULL) {
        return GRA_CondRetPonteiroNulo;
    }

    pVertice = PesquisaVertice(pGrafo->pListaOrigens, idVertice);
    if(pVertice == NULL) {
        return GRA_CondRetVerticeInvalido;
    }

    LIS_ExcluirElemento( pGrafo->pListaOrigens );

    return GRA_CondRetOK;
}


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

static GRA_tppVerticeGrafo CriarVerticeGrafo (void) {
    GRA_tppVerticeGrafo pVertice = NULL;

    pVertice = (GRA_tppVerticeGrafo) malloc(sizeof(GRA_tpVerticeGrafo));
    if (pVertice == NULL) {
        return NULL;
    }

    pVertice->pValor = NULL;

    pVertice->pListaAnt = LIS_CriarLista( NULL ) ;
    if (pVertice->pListaAnt == NULL) {
        free(pVertice);
        return NULL;
    }

    pVertice->pListaSuc = LIS_CriarLista( DestruirConteudoListaDeAresta ) ;
    if (pVertice->pListaSuc == NULL) {
        LIS_DestruirLista(pVertice->pListaAnt);
        free(pVertice);
        return NULL;
    }

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

static GRA_tpVerticeGrafo * PesquisaVertice( LIS_tppLista pLista, int idVertice ) {
    GRA_tpVerticeGrafo * pVerticeGrafo;
    LIS_IrInicioLista( pLista );

    while (LIS_ObterValor( pLista ) ) {
        pVerticeGrafo = (GRA_tpVerticeGrafo *) LIS_ObterValor( pLista );
        if( pVerticeGrafo != NULL && pVerticeGrafo->id == idVertice )
            return pVerticeGrafo;

        if( LIS_AvancarElementoCorrente( pLista, 1 ) != LIS_CondRetOK )
            break;
    }

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

static GRA_tpVerticeGrafo * PesquisaVerticeNaListaDeAresta( LIS_tppLista pLista, int idVertice ) {
    GRA_tpAresta * pAresta;
    LIS_IrInicioLista( pLista );

    while (LIS_ObterValor( pLista ) ) {
        pAresta = (GRA_tpAresta *) LIS_ObterValor( pLista );

        if( pAresta != NULL && pAresta->pVerticeApontado->id == idVertice )
            return pAresta->pVerticeApontado;

        if( LIS_AvancarElementoCorrente( pLista, 1 ) != LIS_CondRetOK )
            break;
    }

    return NULL;
}


/***********************************************************************
*
*  $FC Fun��o: GRA - Remover v�rtice
*
*  $ED Descri��o da fun��o
*  Remove pVertice de pGrafo, destruindo todas as arestas em seus
*  antecessores e sucessores, al�m do conte�do armazenado neste
*
***********************************************************************/

static void RemoverVertice ( GRA_tppGrafo pGrafo, GRA_tppVerticeGrafo pVertice ) {
    GRA_tpVerticeGrafo * pVerticeSuc;
    GRA_tpAresta * pAresta;

    //Remover arestas sucessoras
    LIS_IrInicioLista( pVertice->pListaSuc );

    while( LIS_ObterValor( pVertice->pListaSuc ) ) {
        pAresta = (GRA_tpAresta *) LIS_ObterValor( pVertice->pListaSuc );
        if( pAresta != NULL && pAresta->pVerticeApontado != NULL && pAresta->pVerticeApontado->pListaAnt != NULL) {
            LIS_ProcurarValor( pAresta->pVerticeApontado->pListaAnt, pVertice );
            LIS_ExcluirElemento( pAresta->pVerticeApontado->pListaAnt );
        }

        if( LIS_AvancarElementoCorrente( pVertice->pListaSuc, 1 ) == LIS_CondRetFimLista )
            break;

    }

    LIS_DestruirLista( pVertice->pListaSuc );
    pVertice->pListaSuc = NULL;

    //Remover arestas antecessoras
    LIS_IrInicioLista( pVertice->pListaAnt );

    while(LIS_ObterValor( pVertice->pListaAnt ) ) {
        pVerticeSuc = (GRA_tpVerticeGrafo *) LIS_ObterValor( pVertice->pListaAnt );
        if( pVerticeSuc != NULL && pVerticeSuc->pListaSuc != NULL) {
            PesquisaVerticeNaListaDeAresta( pVerticeSuc->pListaSuc, pVertice->id );
            LIS_ExcluirElemento( pVerticeSuc->pListaSuc );
        }

        if( LIS_AvancarElementoCorrente( pVertice->pListaAnt, 1 ) == LIS_CondRetFimLista )
            break;
    }

    LIS_DestruirLista( pVertice->pListaAnt );
    pVertice->pListaAnt = NULL;

    if(pGrafo->ExcluirValor != NULL && pVertice->pValor != NULL)
        pGrafo->ExcluirValor( pVertice->pValor );

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

static GRA_tpCondRet AdicionarAresta( GRA_tppVerticeGrafo pVerticeOrigem, GRA_tppVerticeGrafo pVerticeDestino, int idAresta) {
    LIS_tpCondRet CondRetLis;
    GRA_tpAresta * pAresta;
    pAresta = (GRA_tpAresta *)malloc(sizeof(GRA_tpAresta));
    if(pAresta == NULL) {
        return GRA_CondRetFaltouMemoria;
    }

    pAresta->idAresta = idAresta;
    pAresta->pVerticeApontado = pVerticeDestino;

    LIS_IrFinalLista(pVerticeOrigem->pListaSuc);
    CondRetLis = LIS_InserirElementoApos(pVerticeOrigem->pListaSuc, (void *) pAresta);
    if(CondRetLis == LIS_CondRetFaltouMemoria) {
        return GRA_CondRetFaltouMemoria;
    }

    LIS_IrFinalLista(pVerticeDestino->pListaAnt);
    CondRetLis = LIS_InserirElementoApos(pVerticeDestino->pListaAnt, (void *) pVerticeOrigem);
    if(CondRetLis == LIS_CondRetFaltouMemoria) {
        return GRA_CondRetFaltouMemoria;
    }

    return GRA_CondRetOK;
}


/********** Fim do m�dulo de implementa��o: M�dulo grafo dirigido **********/
