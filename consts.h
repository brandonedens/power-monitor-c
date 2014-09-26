/** @file
 * Constants defined project wide.
 *
 *==============================================================================
 * Copyright 2014 by Brandon Edens. All Rights Reserved
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
