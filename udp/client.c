/********************************************************copyrite.xic***/
/*                                                                     */
/*   Licensed Materials - Property of IBM      UDP                     */
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

main(argc, argv)
int argc;
char **argv;
{
    
    
    int s;
    unsigned short port;
    struct sockaddr_in server;
    char buf[32];
    
    /* argv[1] is internet address of server argv[2] is port of server.
     * Convert the port from ascii to integer and then from host byte
     * order to network byte order.
     */
    if(argc != 3)
    {
        printf("Usage: %s <host address> <port> \n",argv[0]);
        exit(1);
    }
    port = htons(atoi(argv[2]));
    
    
    /* Create a datagram socket in the internet domain and use the
     * default protocol (UDP).
     */
    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket()");
        exit(1);
    }
    
    /* Set up the server name */
    server.sin_family      = AF_INET;            /* Internet Domain    */
    server.sin_port        = port;               /* Server Port        */
    server.sin_addr.s_addr = inet_addr(argv[1]); /* Server's Address   */
    
    strcpy(buf, "Hello");
    
    /* Send the message in buf to the server */
    if (sendto(s, buf, (strlen(buf)+1), 0, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("sendto()");
        exit(2);
    }
    
    /* Deallocate the socket */
    close(s);
}