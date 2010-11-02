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


 

#include "condor_common.h"

#if !defined(WIN32)
#include <sys/types.h>
#include <sys/param.h>
#include <sys/resource.h>
#endif

#include <stdio.h>

#include "proc.h"
#include "condor_ckpt_name.h"

char *
gen_ckpt_name( char *directory, int cluster, int proc, int subproc )
{
	char *answer = (char *)malloc(MAXPATHLEN);

	if( proc == ICKPT ) {
		if( directory && directory[0] ) {
			(void)snprintf( answer, MAXPATHLEN, "%s%ccluster%d.ickpt.subproc%d",
						directory, DIR_DELIM_CHAR, cluster, subproc );
		} else {
			(void)snprintf( answer, MAXPATHLEN, "cluster%d.ickpt.subproc%d",
						cluster, subproc );
		}
	} else {
		if( directory && directory[0] ) {
			(void)snprintf( answer, MAXPATHLEN, "%s%ccluster%d.proc%d.subproc%d",
						directory, DIR_DELIM_CHAR, cluster, proc, subproc );
		} else {
			(void)snprintf( answer, MAXPATHLEN, "cluster%d.proc%d.subproc%d",
						cluster, proc, subproc );
		}
	}
	return answer;
}