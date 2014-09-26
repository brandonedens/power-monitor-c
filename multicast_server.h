/** @file
 * Interface to multicast server software.
 *
 *==============================================================================
 * Copyright 2014 by Brandon Edens. All Rights Reserved
 *==============================================================================
 *
 * @author  Brandon Edens
 * @date    2014-09-21
 *
 */
#ifndef MULTICAST_SERVER_H_
#define MULTICAST_SERVER_H_

/*******************************************************************************
 * Global Functions
 */
int multicast_init(char *multicast_ip, unsigned short multicast_port);
void multicast_send(int sock, uint8_t *data, size_t len);
void multicast_term(int sock);

#endif  // MULTICAST_SERVER_H_
