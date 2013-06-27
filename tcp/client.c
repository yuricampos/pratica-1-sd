/********************************************************copyrite.xic***/
/*                                                                     */
/*   Licensed Materials - Property of IBM  1     Client TCP            */
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
#include <netdb.h>
#include <stdlib.h>

/**
 * Operacoes
 * 1 - Inclusao / atualizar
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
 * Client Main.
 */
main(argc, argv)
int argc;
char **argv;
{
    unsigned short port; /* port client will connect to              */
    struct hostent *hostnm; /* server host name information             */
    struct sockaddr_in server; /* server address                           */
    int s; /* client socket                            */
    contato c; /* contato da agenda                        */

    /*
     * Check Arguments Passed. Should be hostname and port.
     */
    if (argc != 3) {
        fprintf(stderr, "Usage: %s hostname port\n", argv[0]);
        exit(1);
    }

    /*
     * The host name is the first argument. Get the server address.
     */
    hostnm = gethostbyname(argv[1]);
    if (hostnm == (struct hostent *) 0) {
        fprintf(stderr, "Gethostbyname failed\n");
        exit(2);
    }

    /*
     * The port is the second argument.
     */
    port = (unsigned short) atoi(argv[2]);

    /*
     * Put the server information into the server structure.
     * The port must be put into network byte order.
     */
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = *((unsigned long *) hostnm->h_addr);

    /*
     * Get a stream socket.
     */
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket()");
        exit(3);
    }

    /*
     * Connect to the server.
     */
    if (connect(s, (struct sockaddr *) &server, sizeof (server)) < 0) {
        perror("Connect()");
        exit(4);
    }

    int loop = 1;
    while (loop = 1) {
        printf("Selecione a opcao desejada \n");
        printf("1 - Armazenar / Atualizar \n");
        printf("2 - Remover um registro \n");
        printf("3 - Acessar um registro \n");
        printf("4 - Finalizar aplicacao \n");
        int opcao;
        scanf("%d", &opcao);
        switch (opcao) {
            case 1:
            {
                printf("\n\n Opcao escolhida: Armazenar / Atualizar um registro \n");
                printf("Opcao em implementacao \n");
                printf("Entre com o nome do contato ");
                scanf("%s", &c.nome);
                printf("Entre com o telefone do contato ");
                scanf("%s", &c.telefone);
                c.operacao = 1;
                send(s, &c, sizeof (c), 0);
                recv(s, &c, sizeof (c), 0);
                if (!strcmp(c.status, "Existe!")) {
                    printf("Contato ja existe, deseja atualizar? \n");
                    printf("1 - Sim \n");
                    printf("2 - Nao \n");
                    int atualiza;
                    scanf("%d", &atualiza);
                    if (atualiza == 1) {
                        c.operacao = 5;
                        send(s, &c, sizeof (c), 0);
                        recv(s, &c, sizeof (c), 0);
                        printf("Atualizado! \n");
                    }

                } else {
                    printf("Mensagem do server: %s \n", c.status);
                }
                break;
            }

            case 2:
            {
                printf("\n\n Opcao escolhida: Remover um registro\n");
                c.operacao = 2;
                printf("Entre com o nome do contato ");
                scanf("%s", &c.nome);
                send(s, &c, sizeof (c), 0);
                recv(s, &c, sizeof (c), 0);
                printf("Status: %s \n", c.status);
                break;
            }
            case 3:
            {
                printf("\n\n Opcao escolhida: Acessar um registro\n");
                c.operacao = 3;
                printf("Entre com o nome do contato ");
                scanf("%s", &c.nome);
                send(s, &c, sizeof (c), 0);
                recv(s, &c, sizeof (c), 0);
                if (!strcmp(c.status, "Nao existe!")) {
                    printf("Status: %s \n", c.status);
                } else {
                    printf("Nome: %s \n", c.nome);
                    printf("Telefone: %s \n", c.telefone);
                }
                break;
            }
            case 4:
            {
                printf("\n\n Opcao escolhida: Finalizar Aplicacao\n");
                close(s);
                exit(0);
                loop = 0;
                break;

            }

        }

    }








}
