/** @file
 * Constants defined project wide.
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

#ifndef CONSTS_H_
#define CONSTS_H_

/*******************************************************************************
 * Constants
 */

/** Number of nanoseconds per a second. */
#define NSEC_PER_SEC (1e9)

/** Number of samples sent with each multicast packet. Note that currently we do
 * not process samples across multiple captures so ensure that this value *
 * sizeof(sample) stays lower than the MTU.
 */
#define SAMPLES_PER_CAPTURE (64)

#endif  // CONSTS_H_
