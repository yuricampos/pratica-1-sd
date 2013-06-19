/********************************************************copyrite.xic***/
/*                                                                     */
/*   Licensed Materials - Property of IBM                              */
/*                                                                     */
/*   This product contains "Restricted Materials of IBM":              */
/*      Program Number:   5798RXW                                      */
/*      Program Name:     IBM TCP/IP Version 1.2 for OS/2              */
/*   (C) Copyright IBM Corporation. 1990, 1991.                        */
/*                                                                     */
/*   All rights reserved.                                              */
/*                                                                     */
/*   US Government Users Restricted Rights -                           */
/*   Use, duplication or disclosure restricted by GSA ADP Schedule     */
/*   Contract with IBM Corp.                                           */
/*                                                                     */
/*   See IBM Copyright Instructions.                                   */
/*                                                                     */
/********************************************************copyrite.xic***/
/*
 * Include Files.
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <gdbm.h>
#include <stdlib.h>
#include <string.h>
#define datum_set(um, buf) { um.dptr = buf; um.dsize = strlen(buf); }

 /**
 * Operacoes
 * Estrutura a ser transmitida entre client e server
 * 1 - Inclusao / Alteracao
 * 3 - Remover
 * 4 - Acessar
 * 5 - Finalizar App
 */
 typedef struct contato{
    int operacao; 
    char nome[256];
    char telefone[256];
    char status[256];
}contato;



/*
 * Server Main.
 */
main(argc, argv)
int argc;
char **argv;
{
    char* dbname = "bdsd";     /* nome do banco de dados                */
    unsigned short port;       /* port server binds to                  */
    char* status;              /* char de status que e enviado          */
    char buf[12];              /* buffer for sending and receiving data */
    struct sockaddr_in client; /* client address information            */
    struct sockaddr_in server; /* server address information            */
    int s;                     /* socket for accepting connections      */
    int ns;                    /* socket connected to client            */
    int namelen;               /* length of client name                 */
    contato c;                 /* contato a ser transmitido             */

    
    /*
     * Check arguments. Should be only one: the port number to bind to.
     */
    
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s port\n", argv[0]);
        exit(1);
    }
    
    /*
     * First argument should be the port.
     */
    port = (unsigned short) atoi(argv[1]);
    
    /*
     * Get a socket for accepting connections.
     */
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket()");
        exit(2);
    }
    
    /*
     * Bind the socket to the server address.
     */
    server.sin_family = AF_INET;
    server.sin_port   = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;
    
    if (bind(s, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("Bind()");
        exit(3);
    }
    
    /*
     * Listen for connections. Specify the backlog as 1.
     */
    if (listen(s, 1) != 0)
    {
        perror("Listen()");
        exit(4);
    }
    
    /*
     * Accept a connection.
     */
    namelen = sizeof(client);
    if ((ns = accept(s, (struct sockaddr *)&client, &namelen)) == -1)
    {
        perror("Accept()");
        exit(5);
    }
    

    while(c.operacao != 4){ // while mantem conexao aberta no lado do server
    recv(ns, &c, sizeof(c), 0); // recebe do client
    GDBM_FILE dbf; // arquivo de banco de dados
    // datum tipo de dados que vem da biblioteca gdbm.h
    datum key, data; // chave e valor a ser salvo no banco de dados
    datum_set(key, c.nome); // seta c.nome (nome do contato) na variavel key (chave) do bd
    datum_set(data, c.telefone); // seta c.telefone (fone do conta.) na variavel data do bd
    if( !(dbf = gdbm_open(dbname, 0, GDBM_WRCREAT, 0644, NULL)) ) {
        // caso nao seja possivel abrir o banco de dados da erro
        status = "Erro Conexao!";
        strcpy(c.status, status);
        send(ns, &c, sizeof(c), 0);
    }
    // Se operacao e adicionar ou atualizar contato
    if(c.operacao == 1){
        // tudo que e gdbm vem do banco de dados
    if( gdbm_store(dbf, key, data, GDBM_INSERT) ) { // se ja existe registro
        gdbm_delete(dbf, key); // apaga registro
        gdbm_store(dbf, key, data, GDBM_INSERT); // faz novo registro
        status = "Atualizado!"; // seta status a ser enviado a client
        strcpy(c.status, status); // copia status para estrutura a ser enviada
        send(ns, &c, sizeof(c), 0); // envia para cliente
        gdbm_close(dbf); // fecha banco de dados
    } else{ // caso nao exista registro cria um registro
    status =  "Armazenado"; // seta status
    strcpy(c.status, status); // coloca status na estrutura a ser enviada para client
    send(ns, &c, sizeof(c), 0); // envia estrutura para cliente
    gdbm_close(dbf); // fecha banco de dados

    }
}

        if(c.operacao == 2){ // remover contato recebido (do banco)
        key.dptr = c.nome; // seta nome do contato como key
        key.dsize = strlen(c.nome); //seta size da key
        if(gdbm_delete(dbf, key)) { // tenta deletar acessa if apenas em caso de erro
        status = "Erro!"; // seta erro no status
        strcpy(c.status, status); //copia status para estrutura a ser enviada
        send(ns, &c, sizeof(c), 0); //envia estrutura para cliente
        gdbm_close(dbf); // fecha banco de dados
    } else{ // removido com sucesso
        status = "Removido!"; // seta mensagem no status
        strcpy(c.status, status); //copia status para estrutura a ser enviada
        send(ns, &c, sizeof(c), 0); //enviar status para o cliente
        gdbm_close(dbf); // fechar bd

    }

    }


    if(c.operacao == 3){ // recuperar contato no banco (buscar)
        key.dptr = c.nome; // seta nome do contato como key
        key.dsize = strlen(c.nome); //seta size da key
        if(!gdbm_exists(dbf, key)){ // verificar existencia entra no if caso nao exista no bd
        status = "Nao existe!"; // seta erro no status
        strcpy(c.status, status); //copia status para estrutura a ser enviada
        send(ns, &c, sizeof(c), 0); //enviar status para o cliente
         } else{ //existe no banco o registro

        data = gdbm_fetch(dbf, key); //recupera
        strcpy(c.nome, key.dptr); //copia key para estrutura
        strcpy(c.telefone, data.dptr); //copia dado recuperado para estrutura
        status = "Recuperado"; //seta status
        strcpy(c.status, status); //copia status para estrutura
        send(ns, &c, sizeof(c), 0); //envia estrutura

         }





    }



    }

}