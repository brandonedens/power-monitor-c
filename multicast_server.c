/** @file
 * Server for multicasting data out onto the network.
 *
 *==============================================================================
 * Copyright 2014 by Brandon Edens. All Rights Reserved
 *==============================================================================
 *
 * @author  Brandon Edens
 * @date    2014-09-21
 *
 * @details
 * http://www.thedeveloperspoint.com/ip-multicast-in-c-linux/
 */

/*******************************************************************************
 * Include Files
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*******************************************************************************
 * Local Variables
 */

static struct sockaddr_in multicast_addr;

/******************************************************************************/

int multicast_init(char *multicast_ip, unsigned short multicast_port)
{
	// Create a socket.
	int sock;
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		perror("Socket creation failed");
		exit(1);
	}

	// Set the TTL (time to live/hop count) for the send.
	unsigned char multicast_ttl = 1;
	if ((setsockopt
	     (sock, IPPROTO_IP, IP_MULTICAST_TTL, (void *)&multicast_ttl,
	      sizeof(multicast_ttl))) < 0) {
		perror("setsockopt() failed");
		exit(1);
	}
	memset(&multicast_addr, 0, sizeof(multicast_addr));
	multicast_addr.sin_family      = AF_INET;
	multicast_addr.sin_addr.s_addr = inet_addr(multicast_ip);
	multicast_addr.sin_port        = htons(multicast_port);

	return sock;
}

void multicast_send(int sock, uint8_t *data, size_t len)
{
	int ret = sendto(sock, data, len, 0, (struct sockaddr *)&multicast_addr,
			sizeof(multicast_addr));
	if (ret != len) {
		perror("Error in number of bytes");
		exit(1);
	}
}

void multicast_term(int sock)
{
	close(sock);
}
