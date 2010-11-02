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


#ifndef _HISTORY_UTILS_H_
#define _HISTORY_UTILS_H_

#include "condor_common.h"
#include "condor_classad_util.h"
#include "proc.h"

//------------------------------------------------------------------------

static void displayJobShort(AttrList* ad);
static void short_header(void);
static void short_print(int,int,const char*,int,int,int,int,int,int,const char *);
static void shorten (char *, int);
static char* format_date( time_t date );
static char* format_time( int tot_secs );
static char encode_status( int status );


//------------------------------------------------------------------------

//------------------------------------------------------------------------

static void
displayJobShort(AttrList* ad)
{
    int cluster, proc, date, status, prio, image_size, CompDate;
    float utime;
    char *owner, *cmd, *args;

    owner = NULL;
    cmd   = NULL;
    args  = NULL;

	if(!ad->EvalFloat(ATTR_JOB_REMOTE_WALL_CLOCK,NULL,utime)) {
		if(!ad->EvalFloat(ATTR_JOB_REMOTE_USER_CPU,NULL,utime)) {
			utime = 0;
		}
	}

        if (!ad->EvalInteger (ATTR_CLUSTER_ID, NULL, cluster)           ||
                !ad->EvalInteger (ATTR_PROC_ID, NULL, proc)             ||
                !ad->EvalInteger (ATTR_Q_DATE, NULL, date)              ||
                !ad->EvalInteger (ATTR_COMPLETION_DATE, NULL, CompDate)	||
                !ad->EvalInteger (ATTR_JOB_STATUS, NULL, status)        ||
                !ad->EvalInteger (ATTR_JOB_PRIO, NULL, prio)            ||
                !ad->EvalInteger (ATTR_IMAGE_SIZE, NULL, image_size)    ||
                !ad->EvalString  (ATTR_OWNER, NULL, &owner)             ||
                !ad->EvalString  (ATTR_JOB_CMD, NULL, &cmd) )
        {
                printf (" --- ???? --- \n");
				free(owner);
				free(cmd);
                return;
        }
        
        shorten (owner, 14);
        if (ad->EvalString ("Args", NULL, &args)) {
            int cmd_len = strlen(cmd);
            int extra_len = 14 - cmd_len;
            if (extra_len > 0) {
                cmd = (char *) realloc(cmd, 16);
                strcat(cmd, " ");
                strncat(cmd, args, extra_len);
            }
        }
        shorten (cmd, 15);
        short_print (cluster, proc, owner, date, CompDate, (int)utime, status, 
               prio, image_size, cmd); 


        free(owner);
        free(cmd);
        free(args);
        return;
}

//------------------------------------------------------------------------

static void
short_header (void)
{
    printf( " %-7s %-14s %11s %12s %-2s %11s %-15s\n",
        "ID",
        "OWNER",
        "SUBMITTED",
        "RUN_TIME",
        "ST",
		"COMPLETED",
        "CMD"
    );
}

//------------------------------------------------------------------------

static void
shorten (char *buff, int len)
{
    if ((unsigned int)strlen (buff) > (unsigned int)len) buff[len] = '\0';
}

//------------------------------------------------------------------------

/*
  Print a line of data for the "short" display of a PROC structure.  The
  "short" display is the one used by "condor_q".  N.B. the columns used
  by this routine must match those defined by the short_header routine
  defined above.
*/

static void
short_print(
        int cluster,
        int proc,
        const char *owner,
        int date,
		int CompDate,
        int time,
        int status,
        int  /*prio*/,
        int  /*image_size*/,
        const char *cmd
        ) {
		MyString SubmitDateStr=format_date(date);
		MyString CompDateStr=format_date(CompDate);
        printf( "%4d.%-3d %-14s %-11s %-12s %-2c %-11s %-15s\n",
                cluster,
                proc,
                owner,
                SubmitDateStr.Value(),
                format_time(time),
                encode_status(status),
                CompDateStr.Value(),
                cmd
        );
}


//------------------------------------------------------------------------

/*
  Format a date expressed in "UNIX time" into "month/day hour:minute".
*/

static char* format_date( time_t date )
{
        static char     buf[ 12 ];
        struct tm       *tm;

		if (date==0) return " ??? ";

        tm = localtime( &date );
        sprintf( buf, "%2d/%-2d %02d:%02d",
                (tm->tm_mon)+1, tm->tm_mday, tm->tm_hour, tm->tm_min
        );
        return buf;
}

//------------------------------------------------------------------------

/*
  Format a time value which is encoded as seconds since the UNIX
  "epoch".  We return a string in the format dd+hh:mm:ss, indicating
  days, hours, minutes, and seconds.  The string is in static data
  space, and will be overwritten by the next call to this function.
*/

static char     *
format_time( int tot_secs )
{
        int             days;
        int             hours;
        int             min;
        int             secs;
        static char     answer[25];

		if ( tot_secs < 0 ) {
			sprintf(answer,"[?????]");
			return answer;
		}

        days = tot_secs / DAY;
        tot_secs %= DAY;
        hours = tot_secs / HOUR;
        tot_secs %= HOUR;
        min = tot_secs / MINUTE;
        secs = tot_secs % MINUTE;

        (void)sprintf( answer, "%3d+%02d:%02d:%02d", days, hours, min, secs );
        return answer;
}

//------------------------------------------------------------------------

/*
  Encode a status from a PROC structure as a single letter suited for
  printing.
*/

static char
encode_status( int status )
{
        switch( status ) {
          case IDLE:
                return 'I';
          case RUNNING:
                return 'R';
          case COMPLETED:
                return 'C';
          case REMOVED:
                return 'X';
          case TRANSFERRING_OUTPUT:
                return '>';
          default:
                return ' ';
        }
}

#endif