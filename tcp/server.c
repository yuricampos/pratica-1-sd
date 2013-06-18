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
 * 1 - Inclusao
 * 2 - Atualizar
 * 3 - Remover
 * 4 - Acessar
 * 5 - Finalizar App
 */
 typedef struct contato{
    int operacao; 
    char nome[12];
    char telefone[12];
    char status[12];
}contato;



/*
 * Server Main.
 */
main(argc, argv)
int argc;
char **argv;
{
    char* dbname = "bdsd";
    unsigned short port;       /* port server binds to                  */
    char* status;
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
    
    /*
     * Receive the message on the newly connected socket.
     */
    while(c.operacao != 5){
    recv(ns, &c, sizeof(c), 0);
    if(c.operacao == 1){
    printf("Nome do contato: %s \n",c.nome);
    printf("Telefone do contato: %s \n",c.telefone);
    GDBM_FILE dbf;
    datum key, data;
    datum_set(key, c.nome);
    datum_set(data, c.telefone);
        if( !(dbf = gdbm_open(dbname, 0, GDBM_WRCREAT, 0644, NULL)) ) {
        status = "Erro!";
        strcpy(c.status, status);
        send(ns, &c, sizeof(c), 0);
    }
    if( gdbm_store(dbf, key, data, GDBM_INSERT) ) {
        status = "Erro!";
        strcpy(c.status, status);
        send(ns, &c, sizeof(c), 0);
        gdbm_close(dbf);
    } else{
    status =  "Armazenado";
    strcpy(c.status, status);
    send(ns, &c, sizeof(c), 0);
    gdbm_close(dbf);

    }
}

    if(c.operacao == 4){
        GDBM_FILE dbf;
        datum key, data;
        dbf = gdbm_open(dbname, 0, GDBM_WRITER, 0, NULL);
        if( !dbf ) {
        printf("%s\n", gdbm_strerror(gdbm_errno));
    }
        key.dptr = c.nome;
        key.dsize = strlen(c.nome);
        data = gdbm_fetch(dbf, key);
        printf("%s:\t%s\n",  key.dptr, data.dptr);
        strcpy(c.nome, key.dptr);
        strcpy(c.telefone, data.dptr);
        status = "Recuperado";
        strcpy(c.status, status);
        send(ns, &c, sizeof(c), 0);


    }

        if(c.operacao == 3){
        GDBM_FILE dbf;
        datum key, data;
        dbf = gdbm_open(dbname, 0, GDBM_WRITER, 0, NULL);
        if( !dbf ) {
        printf("%s\n", gdbm_strerror(gdbm_errno));
    }
        key.dptr = c.nome;
        key.dsize = strlen(c.nome);
        if(gdbm_delete(dbf, key)) {
        status = "Erro!";
        strcpy(c.status, status);
        send(ns, &c, sizeof(c), 0);
        gdbm_close(dbf);
    } else{
        status = "Removido!";
        strcpy(c.status, status);
        send(ns, &c, sizeof(c), 0);
        gdbm_close(dbf);

    }

    }


    }

    if(c.operacao == 5){
    close(ns);
    close(s);
    printf("Server ended successfully\n");
    exit(0);
    send(ns, &c, sizeof(c), 0);
    }
    
    /*
     * Send the message back to the client.
     */
    if (send(ns, &c, sizeof(c), 0) < 0)
    {
        perror("Send()");
        //exit(7);
    }


}