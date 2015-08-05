/** @file
 * Software that reads power sample data from I2C and multicasts it onto the
 * network.
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
 */

/*******************************************************************************
 * Include Files
 */

#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "multicast_server.h"
#include "consts.h"

/*******************************************************************************
 * Constants
 */

#define V_CONT    (1 << 0)
#define V_ONCE    (1 << 1)
#define I_CONT    (1 << 2)
#define I_ONCE    (1 << 3)
#define VRANGE    (1 << 4)
#define STATUS_RD (1 << 6)

#define NSEC_PER_SEC (1e9)

#define SAMPLES_PER_CAPTURE (64)

/*******************************************************************************
 * Local Types
 */

struct sample {
	struct timespec t;
	uint16_t d;
};

/*******************************************************************************
 * Macros
 */

#define ARRAY_LEN(arr) (sizeof(arr) / sizeof((arr)[0]))

/******************************************************************************/

int main(int argc, char *argv[])
{
	int fd;
	char fdname[40];
	const char *errmsg;
	int addr = 0x30;	// The I2C address of the ADC

	if (argc == 2) {
		sprintf(fdname, argv[1]);
	} else {
		sprintf(fdname, "/dev/i2c-1");
	}
	if (argc == 3) {
		addr = atoi(argv[2]);
	}

	if ((fd = open(fdname, O_RDWR)) < 0) {
		printf("Failed to open the bus.\n");
		/* ERROR HANDLING; you can check errno to see what went wrong */
		exit(EXIT_FAILURE);
	}

	if (ioctl(fd, I2C_SLAVE, addr) < 0) {
		printf("Failed to acquire bus access and/or talk to slave.\n");
		/* ERROR HANDLING; you can check errno to see what went wrong */
		exit(EXIT_FAILURE);
	}

	// Configure ADC.
	uint8_t buf[10];
	buf[0] = I_CONT | VRANGE;
	if (write(fd, buf, 1) != 1) {
		// ERROR HANDLING: i2c transaction failed
		printf("Failed to write to the i2c bus.\n");
		errmsg = strerror(errno);
		printf(errmsg);
		printf("\n\n");
		close(fd);
		exit(EXIT_FAILURE);
	}
	nanosleep((struct timespec[]){{0, 200000000}}, NULL);

	// Setup the multicast server.
	int const sock = multicast_init("225.1.1.1", 5555);

	struct sample samples[SAMPLES_PER_CAPTURE];
	while (true) {
		for (int i = 0; i < ARRAY_LEN(samples); i++) {
			if (read(fd, buf, 2) != 2) {
				printf("Failed to read from the i2c bus.\n");
				errmsg = strerror(errno);
				printf(errmsg);
				printf("\n\n");
				close(fd);
				exit(EXIT_FAILURE);
			}
			clock_gettime(CLOCK_MONOTONIC, &samples[i].t);
			samples[i].d = ((buf[0] << 8) | buf[1]) >> 4;
		}

		// Transmit accumulated sample data.
		multicast_send(sock, (uint8_t *)samples, sizeof(samples));
	}

	// Terminate the multicast socket.
	multicast_term(sock);

	return EXIT_SUCCESS;
}
