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

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <gdbm.h>
#include <string.h>
#define datum_set(um, buf) { um.dptr = buf; um.dsize = strlen(buf); }
 /**
 * Operacoes
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

main()
{
   int sockint,s, namelen, client_address_size;
   struct sockaddr_in client, server;
   contato c;


   /*
    * Create a datagram socket in the internet domain and use the
    * default protocol (UDP).
    */
   if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
   {
       perror("socket()");
       exit(1);
   }

   /*
    *
    * Bind my name to this socket so that clients on the network can
    * send me messages. (This allows the operating system to demultiplex
    * messages and get them to the correct server)
    *
    * Set up the server name. The internet address is specified as the
    * wildcard INADDR_ANY so that the server can get messages from any
    * of the physical internet connections on this host. (Otherwise we
    * would limit the server to messages from only one network interface)
    */
   server.sin_family      = AF_INET;   /* Server is in Internet Domain */
   server.sin_port        = 0;         /* Use any available port       */
   server.sin_addr.s_addr = INADDR_ANY;/* Server's Internet Address    */
  char* dbname = "bdsd";
  char* status;

   if (bind(s, (struct sockaddr *)&server, sizeof(server)) < 0)
   {
       perror("bind()");
       exit(2);
   }

   /* Find out what port was really assigned and print it */
   namelen = sizeof(server);
   if (getsockname(s, (struct sockaddr *) &server, &namelen) < 0)
   {
       perror("getsockname()");
       exit(3);
   }

   printf("Port assigned is %d\n", ntohs(server.sin_port));

   /*
    * Receive a message on socket s in buf  of maximum size 32
    * from a client. Because the last two paramters
    * are not null, the name of the client will be placed into the
    * client data structure and the size of the client address will
    * be placed into client_address_size.
    */
   client_address_size = sizeof(client);


    while(c.operacao != 5){
    recvfrom(s, &c, sizeof(c), 0, (struct sockaddr *) &client, &client_address_size);
    GDBM_FILE dbf;
    datum key, data;
    datum_set(key, c.nome);
    datum_set(data, c.telefone);
    if( !(dbf = gdbm_open(dbname, 0, GDBM_WRCREAT, 0644, NULL)) ) {
        status = "Erro Conexao!";
        strcpy(c.status, status);
        sendto(s,&c,sizeof(c),0,(struct sockaddr *)&client,client_address_size);
    }

    if(c.operacao == 1){
    printf("Nome do contato: %s \n",c.nome);
    printf("Telefone do contato: %s \n",c.telefone);
    if( gdbm_store(dbf, key, data, GDBM_INSERT) ) {
        gdbm_delete(dbf, key);
        gdbm_store(dbf, key, data, GDBM_INSERT);
        status = "Atualizado!";
        strcpy(c.status, status);
        sendto(s,&c,sizeof(c),0,(struct sockaddr *)&client,client_address_size);
        gdbm_close(dbf);
    } else{
    status =  "Armazenado";
    strcpy(c.status, status);
    sendto(s,&c,sizeof(c),0,(struct sockaddr *)&client,client_address_size);
    gdbm_close(dbf);

    }
}


    if(c.operacao == 3){
        key.dptr = c.nome;
        key.dsize = strlen(c.nome);
        if(!gdbm_exists(dbf, key)){
        status = "Nao existe!";
        strcpy(c.status, status);
        sendto(s,&c,sizeof(c),0,(struct sockaddr *)&client,client_address_size);
         } else{

        data = gdbm_fetch(dbf, key);
        strcpy(c.nome, key.dptr);
        strcpy(c.telefone, data.dptr);
        status = "Recuperado";
        strcpy(c.status, status);
        sendto(s,&c,sizeof(c),0,(struct sockaddr *)&client,client_address_size);

         }





    }

        if(c.operacao == 2){
        key.dptr = c.nome;
        key.dsize = strlen(c.nome);
        if(gdbm_delete(dbf, key)) {
        status = "Erro!";
        strcpy(c.status, status);
        sendto(s,&c,sizeof(c),0,(struct sockaddr *)&client,client_address_size);
        gdbm_close(dbf);
    } else{
        status = "Removido!";
        strcpy(c.status, status);
        sendto(s,&c,sizeof(c),0,(struct sockaddr *)&client,client_address_size);
        gdbm_close(dbf);

    }

    }


    }

    if(c.operacao == 4){
    close(s);
    printf("Server ended successfully\n");
    exit(0);
    sendto(s,&c,sizeof(c),0,(struct sockaddr *)&client,client_address_size);
    }





















}