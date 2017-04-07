/*

 * Copyright (c) 2002-2003, Intel Corporation. All rights reserved.
 * Created by:  rusty.lynch REMOVE-THIS AT intel DOT com
 * This file is licensed under the GPL license.  For the full content
 * of this license, see the COPYING file at the top level of this
 * source tree.

  Test case for assertion #3 of the sigaction system call that shows
  calling sigaction with a null act argument does not change the
  signal handler.

  Steps:
  1. Initialize global variable to indicate handler has not been called
  2. Set the signal handler for SIGILL to handler
  3. Call sigaction with a null act
  4. raise SIGILL
  5. Verify handler was called.
*/

#include <signal.h>
#include <stdio.h>
#include "posixtest.h"

int handler_called = 0;

void handler(int signo)
{
	handler_called = 1;
}

int main(void)
{
	struct sigaction act;
	struct sigaction oact;

	act.sa_handler = handler;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	if (sigaction(SIGILL, &act, 0) == -1) {
		perror("Unexpected error while attempting to setup test "
		       "pre-conditions");
		return PTS_UNRESOLVED;
	}

	if (sigaction(SIGILL, 0, &oact) == -1) {
		perror("Unexpected error while attempting to setup test "
		       "pre-conditions");
		return PTS_UNRESOLVED;
	}

	if (raise(SIGILL) == -1) {
		perror("Unexpected error while attempting to setup test "
		       "pre-conditions");
		return PTS_UNRESOLVED;
	}

	if (handler_called) {
		printf("Test PASSED\n");
		return PTS_PASS;
	}

	printf("Test FAILED\n");
	return PTS_FAIL;
}
