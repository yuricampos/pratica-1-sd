/********************************************************copyrite.xic***/
/*                                                                     */
/*   Licensed Materials - Property of IBM  1                           */
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
#include <stdio.h>

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
 * Client Main.
 */
main(argc, argv)
int argc;
char **argv;
{
    unsigned short port;       /* port client will connect to              */
    char buf[12];              /* data buffer for sending and receiving    */
    char mensagem[12];         /* mensagem que sera lida do client         */
    struct hostent *hostnm;    /* server host name information             */
    struct sockaddr_in server; /* server address                           */
    int s;                     /* client socket                            */
    contato c;                 /* contato da agenda                        */
    
    /*
     * Check Arguments Passed. Should be hostname and port.
     */
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s hostname port\n", argv[0]);
        exit(1);
    }
    
    /*
     * The host name is the first argument. Get the server address.
     */
    hostnm = gethostbyname(argv[1]);
    if (hostnm == (struct hostent *) 0)
    {
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
    server.sin_family      = AF_INET;
    server.sin_port        = htons(port);
    server.sin_addr.s_addr = *((unsigned long *)hostnm->h_addr);
    
    /*
     * Get a stream socket.
     */
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket()");
        exit(3);
    }
    
    /*
     * Connect to the server.
     */
    if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("Connect()");
        exit(4);
    }

    int loop = 1;
    while(loop = 1){
            printf("Selecione a opcao desejada \n");
    printf("1 - Armazenar \n");
    printf("2 - Atualizar um registro \n");
    printf("3 - Remover um registro \n");
    printf("4 - Acessar um registro \n");
    printf("5 - Finalizar aplicacao \n");
    int opcao;
    scanf("%d",&opcao);
    switch (opcao) {
        case 1:
        {
        printf("\n\n Opcao escolhida: Armazenar / Atualizar um registro \n");
        printf("Opcao em implementacao \n");
        printf("Entre com o nome do contato ");
        scanf("%s",&c.nome);
        printf("Entre com o telefone do contato ");
        scanf("%s",&c.telefone);
        c.operacao = 1;
        send(s, &c, sizeof(c), 0);
        recv(s, &c, sizeof(c), 0);
        printf("Mensagem do server: %s \n",c.status);
        break;
    }
        case 2:
        {
        printf("\n\n Opcao escolhida: Remover um registro \n");
        printf("Nada implementado! \n");
        break;
    }   
        case 3:
        {
        printf("\n\n Opcao escolhida: Remover um registro\n");
        c.operacao = 3;
        break;
    }
        case 4:
        {
        printf("\n\n Opcao escolhida: Acessar um registro\n");
        c.operacao = 4;
        printf("Entre com o nome do contato ");
        scanf("%s",&c.nome);
        send(s, &c, sizeof(c), 0);
        recv(s, &c, sizeof(c), 0);
        printf("Nome: %s \n",c.nome);
        printf("Telefone: %s \n",c.telefone);
        break;
    }
        case 5:
        {
        printf("\n\n Opcao escolhida: Finalizar Aplicacao\n");
        c.operacao = 5;
        send(s, &c, sizeof(c), 0);
        close(s);
        printf("Client Ended Successfully\n");
        exit(0);
        loop = 0;
        break;

        }      

    }

    }

    
    


    

    
}
