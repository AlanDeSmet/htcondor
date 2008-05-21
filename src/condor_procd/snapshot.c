/***************************************************************
 *
 * Copyright (C) 1990-2007, Condor Team, Computer Sciences Department,
 * University of Wisconsin-Madison, WI.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License.  You may
 * obtain a copy of the License at
 * 
 *    http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ***************************************************************/

// this tool queries ProcAPI for a list of processes on the system and
// spits this information to stdout. the format of the output is simply
// a series of procInfo structures followed by EOF

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "cprocapi.h"

int
main(void)
{
	void* blob = cprocapi_first();
	while (blob != NULL) {
		if (fwrite(blob, cprocapi_size, 1, stdout) != 1) {
			fprintf(stderr, "fwrite error: %s\n", strerror(errno));
			return 1;
		}
		blob = cprocapi_next(blob);
	}
	return 0;
}
