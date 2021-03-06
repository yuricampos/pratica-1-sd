/********************************************************copyrite.xic***/
/*                                                                     */
/*   Licensed Materials - Property of IBM          Server TCP          */
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
typedef struct contato {
    int operacao;
    char nome[256];
    char telefone[256];
    char status[256];
} contato;

/*
 * Server Main.
 */
main(argc, argv)
int argc;
char **argv;
{
    char* dbname = "bdsd"; /* nome do banco de dados                */
    unsigned short port; /* port server binds to                  */
    char* status; /* char de status que e enviado          */
    char buf[12]; /* buffer for sending and receiving data */
    struct sockaddr_in client; /* client address information            */
    struct sockaddr_in server; /* server address information            */
    int s; /* socket for accepting connections      */
    int ns; /* socket connected to client            */
    int namelen; /* length of client name                 */
    contato c; /* contato a ser transmitido             */


    /*
     * Check arguments. Should be only one: the port number to bind to.
     */

    if (argc != 2) {
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
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket()");
        exit(2);
    }

    /*
     * Bind the socket to the server address.
     */
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(s, (struct sockaddr *) &server, sizeof (server)) < 0) {
        perror("Bind()");
        exit(3);
    }

    /*
     * Listen for connections. Specify the backlog as 1.
     */
    if (listen(s, 1) != 0) {
        perror("Listen()");
        exit(4);
    }

    /*
     * Accept a connection.
     */
    namelen = sizeof (client);
    if ((ns = accept(s, (struct sockaddr *) &client, &namelen)) == -1) {
        perror("Accept()");
        exit(5);
    }


    while (c.operacao != 6) {
        recv(ns, &c, sizeof (c), 0);
        GDBM_FILE dbf;
        datum key, data;
        datum_set(key, c.nome);
        datum_set(data, c.telefone);
        if (!(dbf = gdbm_open(dbname, 0, GDBM_WRCREAT, 0644, NULL))) {
            status = "Erro Conexao!";
            strcpy(c.status, status);
            send(ns, &c, sizeof (c), 0);
        }
        if (c.operacao == 1) {
            if (gdbm_store(dbf, key, data, GDBM_INSERT)) {
                status = "Existe!";
                strcpy(c.status, status);
                send(ns, &c, sizeof (c), 0);
                gdbm_close(dbf);
            } else {
                status = "Armazenado";
                strcpy(c.status, status);
                send(ns, &c, sizeof (c), 0);
                gdbm_close(dbf);

            }
        }
        if (c.operacao == 5) {
            gdbm_delete(dbf, key);
            gdbm_store(dbf, key, data, GDBM_INSERT);
            status = "Atualizado";
            send(ns, &c, sizeof (c), 0);
        }

        if (c.operacao == 2) {
            key.dptr = c.nome;
            key.dsize = strlen(c.nome);
            if (gdbm_delete(dbf, key)) {
                status = "Erro!";
                strcpy(c.status, status);
                send(ns, &c, sizeof (c), 0);
                gdbm_close(dbf);
            } else {
                status = "Removido!";
                strcpy(c.status, status);
                send(ns, &c, sizeof (c), 0);
                gdbm_close(dbf);

            }

        }


        if (c.operacao == 3) {
            key.dptr = c.nome;
            key.dsize = strlen(c.nome);
            if (!gdbm_exists(dbf, key)) {
                status = "Nao existe!";
                strcpy(c.status, status);
                send(ns, &c, sizeof (c), 0);
            } else {

                data = gdbm_fetch(dbf, key);
                strcpy(c.nome, key.dptr);
                strcpy(c.telefone, data.dptr);
                status = "Recuperado";
                strcpy(c.status, status);
                send(ns, &c, sizeof (c), 0);

            }





        }



    }

}