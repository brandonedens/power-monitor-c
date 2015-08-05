/** @file
 * Interface to multicast server software.
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
