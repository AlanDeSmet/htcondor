/***************************Copyright-DO-NOT-REMOVE-THIS-LINE**
  *
  * Condor Software Copyright Notice
  * Copyright (C) 1990-2006, Condor Team, Computer Sciences Department,
  * University of Wisconsin-Madison, WI.
  *
  * This source code is covered by the Condor Public License, which can
  * be found in the accompanying LICENSE.TXT file, or online at
  * www.condorproject.org.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  * AND THE UNIVERSITY OF WISCONSIN-MADISON "AS IS" AND ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  * WARRANTIES OF MERCHANTABILITY, OF SATISFACTORY QUALITY, AND FITNESS
  * FOR A PARTICULAR PURPOSE OR USE ARE DISCLAIMED. THE COPYRIGHT
  * HOLDERS AND CONTRIBUTORS AND THE UNIVERSITY OF WISCONSIN-MADISON
  * MAKE NO MAKE NO REPRESENTATION THAT THE SOFTWARE, MODIFICATIONS,
  * ENHANCEMENTS OR DERIVATIVE WORKS THEREOF, WILL NOT INFRINGE ANY
  * PATENT, COPYRIGHT, TRADEMARK, TRADE SECRET OR OTHER PROPRIETARY
  * RIGHT.
  *
  ****************************Copyright-DO-NOT-REMOVE-THIS-LINE**/

#include "condor_common.h"
#include "condor_random_num.h"

#if !HAVE_MKSTEMP

/*
 * overwrite the buffer with random characters taken from letters, numbers,
 * underscore, and hyphen
 */

static void RandomAscii(char *buf)
{
	static const char encoding[64] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz"
		"0123456789_-";
	
	while (*buf) {
		int r = get_random_int();
		int i;
		
		/* get 5 * 6 bits from each call to random() */
		for (i = 0; i < 5 && *buf; ++i) {
			int bits = r & 0x3f;        /* lower 6 bits */
			r >>= 6;
			*buf++ = encoding[bits];
		}
	}
}


/*
 * Implementation of mkstemp for systems where it is missing.  Returns -1 with
 * errno set to EINVAL if template does not end in XXXXXX as this is the
 * strictest form of mkstemp.  Return -1 with errno set to EEXIST if file isn't
 * successfully created after countMax attempts Also opens with mode 0666 as
 * the real mkstemp does.  Use the umask to do better.
 */

int mkstemp(char *template)
{
	const int   countMax = 100;
	const char  xString[] = "XXXXXX";
	size_t      numXs = (sizeof xString / sizeof xString[0]) - 1;
	
	size_t      len = strlen(template);
	char        *x;
	int         count = 0;;
	
	if (len < numXs) {
		errno = EINVAL;
		return -1;
	}
	
	/* pointer to trailing X's*/
	x = &template[len - numXs];
	
	if (strcmp(x, xString)) {
		errno = EINVAL;
		return -1;
	}
	
	while (count < countMax) {
		int fd;
		
		RandomAscii(x);
		
		fd = open(template, O_CREAT|O_EXCL|O_RDWR, 0666);
		if (fd >= 0 || errno != EEXIST) {
			return fd;
		}
	}
	
	return -1;
}

#endif /* !HAVE_MKSTEMP */


/*
 * Same semantics as mkstemp, except restrict the umask to 077 before calling.
 * This will restrict the permissions on the created file to only the owner.
 * Use fchmod() or fchown() to change the permissions, and owner/group.
 */
int condor_mkstemp(char *template)
{
#if !defined(WIN32)
	int savedUmask = umask(S_IRWXG|S_IRWXO);
	int fd = mkstemp(template);
	umask(savedUmask);
#else
	int fd = mkstemp(template);
#endif
	
	return fd;
}
