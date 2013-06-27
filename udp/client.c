/********************************************************copyrite.xic***/
/*                                                                     */
/*   Licensed Materials - Property of IBM      Client UDP              */
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
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

typedef struct contato {
    int operacao;
    char nome[256];
    char telefone[256];
    char status[256];
} contato;

main(argc, argv)
int argc;
char **argv;
{


    int s;
    unsigned short port;
    struct sockaddr_in server;
    contato c;

    /* argv[1] is internet address of server argv[2] is port of server.
     * Convert the port from ascii to integer and then from host byte
     * order to network byte order.
     */
    if (argc != 3) {
        printf("Usage: %s <host address> <port> \n", argv[0]);
        exit(1);
    }
    port = htons(atoi(argv[2]));


    /* Create a datagram socket in the internet domain and use the
     * default protocol (UDP).
     */
    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket()");
        exit(1);
    }

    /* Set up the server name */
    server.sin_family = AF_INET; /* Internet Domain    */
    server.sin_port = port; /* Server Port        */
    server.sin_addr.s_addr = inet_addr(argv[1]); /* Server's Address   */

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
                printf("Entre com o nome do contato ");
                scanf("%s", &c.nome);
                printf("Entre com o telefone do contato ");
                scanf("%s", &c.telefone);
                c.operacao = 1;
                sendto(s, &c, sizeof (c), 0, (struct sockaddr *) &server, sizeof (server));
                read(s, &c, sizeof (c));
                printf("Mensagem do server: %s \n", c.status);
                if (!strcmp(c.status, "Existe!")) {
                    printf("Contato ja existe, deseja atualizar? \n");
                    printf("1 - Sim \n");
                    printf("2 - Nao \n");
                    int atualiza;
                    scanf("%d", &atualiza);
                    if (atualiza == 1) {
                        c.operacao = 5;
                        sendto(s, &c, sizeof (c), 0, (struct sockaddr *) &server, sizeof (server));
                        read(s, &c, sizeof (c));
                        printf("Atualizado! \n");
                    }
                }
                break;
            }

            case 2:
            {
                printf("\n\n Opcao escolhida: Remover um registro\n");
                c.operacao = 2;
                printf("Entre com o nome do contato ");
                scanf("%s", &c.nome);
                sendto(s, &c, sizeof (c), 0, (struct sockaddr *) &server, sizeof (server));
                read(s, &c, sizeof (c));
                printf("Status: %s \n", c.status);
                break;
            }
            case 3:
            {
                printf("\n\n Opcao escolhida: Acessar um registro\n");
                c.operacao = 3;
                printf("Entre com o nome do contato ");
                scanf("%s", &c.nome);
                sendto(s, &c, sizeof (c), 0, (struct sockaddr *) &server, sizeof (server));
                read(s, &c, sizeof (c));
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