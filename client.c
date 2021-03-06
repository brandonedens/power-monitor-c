/** @file
 * Client for reading out power monitor data over multicast.
 *
 *==============================================================================
 * Copyright (c) 2014-2015 by Brandon Edens
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "consts.h"

/*******************************************************************************
 * Constants
 */
#define SAMPLES_PER_FILE (32768)

/*******************************************************************************
 * Local Types
 */

/** Definition of timespec as defined for a 32-bit ARM board. */ 
struct arm_timespec {
	uint32_t tv_sec;
	uint32_t tv_nsec;
};

/** Representation of a sample sent multicast to the client. */
struct sample {
	struct arm_timespec t;
	uint16_t d;
};

/*******************************************************************************
 * Macros
 */

#define ARRAY_LEN(arr) (sizeof(arr) / sizeof((arr)[0]))

/*******************************************************************************
 * Local Variables
 */

static struct sockaddr_in multicast_addr;
struct ip_mreq mc_req;

/*******************************************************************************
 * Local Functions
 */

static int client_init(char *multicast_ip, unsigned short multicast_port);
size_t client_recv(int sock, uint8_t *data, size_t data_len);
void client_term(int sock);

/******************************************************************************/

static int client_init(char *multicast_ip, unsigned short multicast_port)
{
	int sock;
	int flag_on = 1;

	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		perror("socket() failed");
		exit(1);
	}

	// Set reuse port to on to allow multiple binds per host.
	if ((setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &flag_on,
			sizeof(flag_on))) < 0) {
		perror("setsockopt() failed");
		exit(1);
	}

	// Construct a multicast address structure.
	memset(&multicast_addr, 0, sizeof(multicast_addr));
	multicast_addr.sin_family      = AF_INET;
	multicast_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	multicast_addr.sin_port        = htons(multicast_port);

	// Bind to multicast address to socket.
	if ((bind
	     (sock, (struct sockaddr *)&multicast_addr,
	      sizeof(multicast_addr))) < 0) {
		perror("bind() failed");
		exit(1);
	}

	// Construct an IGMP join request structure.
	mc_req.imr_multiaddr.s_addr = inet_addr(multicast_ip);
	mc_req.imr_interface.s_addr = htonl(INADDR_ANY);

	// Send an ADD MEMBERSHIP message via setsockopt.
	if ((setsockopt
	     (sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *)&mc_req,
	      sizeof(mc_req))) < 0) {
		perror("setsockopt() failed");
		exit(1);
	}

	return sock;
}

size_t client_recv(int sock, uint8_t *data, size_t data_len)
{
	ssize_t len;
	struct sockaddr_in addr;
	unsigned int addr_len;

	addr_len = sizeof(addr);
	memset(&addr, 0, sizeof(addr));

	// Block waiting to receive a packet.
	len = recvfrom(sock, data, data_len, 0, (struct sockaddr *)&addr,
			&addr_len);
	if (len < 0) {
		perror("recvfrom() failed");
		exit(EXIT_FAILURE);
	}

#if 0
	printf("Received %zd bytes from %s: ", len,
			inet_ntoa(addr.sin_addr));
#endif
	return len;
}

void client_term(int sock)
{
	// Send a DROP MEMBERSHIP message via setsockopt.
	if ((setsockopt
	     (sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, (void *)&mc_req,
	      sizeof(mc_req))) < 0) {
		perror("setsockopt() failed");
		exit(1);
	}
	close(sock);
}

int main(int argc, char *argv[])
{
	FILE *fh = NULL;
	if (argc != 2) {
		fprintf(stderr, "Please specify file to write to.\n");
		return EXIT_FAILURE;
	}
	if (argc == 2) {
		fh = fopen(argv[1], "w");
		if (NULL == fh) {
			fprintf(stderr, "Failure to open file.\n");
			return EXIT_FAILURE;
		}
	}

	int sock = client_init("225.1.1.1", 5555);

	struct sample samples[SAMPLES_PER_FILE];
	memset(samples, 0, sizeof(samples));

	struct sample *cap = samples;
	while (true) {
		size_t len = client_recv(sock, (uint8_t *)cap,
				SAMPLES_PER_CAPTURE * sizeof(*cap));
		assert(SAMPLES_PER_CAPTURE * sizeof(*cap) == len);
		cap += SAMPLES_PER_CAPTURE;

		if (cap >= &samples[SAMPLES_PER_FILE]) {
			// Write out the file with all the known samples.
			fh = fopen(argv[1], "w");
			for (int i = 0; i < ARRAY_LEN(samples); i++) {
				fprintf(fh, "%f %03.4f\n", samples[i].t.tv_sec +
						samples[i].t.tv_nsec / NSEC_PER_SEC,
						((105.84 / 4096) * samples[i].d) / 0.1);
			}
			fclose(fh);

			// Shift room for the new samples.
			memset(samples, 0, sizeof(samples));
			cap = samples;
		}
	}
	return EXIT_SUCCESS;
}

