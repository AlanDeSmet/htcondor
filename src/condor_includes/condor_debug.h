/*
  N.B. Every file which includes this header will need to contain the
  following line:

	static char *_FileName_ = __FILE__;
*/

#ifndef DEBUG_H
#define DEBUG_H

extern int DebugFlags;	/* Bits to look for in dprintf                       */

#if !defined(__STDC__) && !defined(__cplusplus)
#define const
#endif

/*
**	Definitions for flags to pass to dprintf
*/
static const int D_ALWAYS		= (1<<0);
static const int D_TERMLOG		= (1<<1);
static const int D_SYSCALLS		= (1<<2);
static const int D_CKPT			= (1<<3);
static const int D_XDR			= (1<<4);
static const int D_MALLOC		= (1<<5);
static const int D_NOHEADER		= (1<<6);
static const int D_LOAD			= (1<<7);
static const int D_EXPR			= (1<<8);
static const int D_PROC			= (1<<9);
static const int D_JOB			= (1<<10);
static const int D_MACHINE		= (1<<11);
static const int D_FULLDEBUG	= (1<<12);
static const int D_NFS			= (1<<13);
static const int D_UPDOWN       = (1<<14);
static const int D_AFS          = (1<<15);
static const int D_PREEMPT		= (1<<16);
static const int D_PROTOCOL		= (1<<17);
static const int D_PRIV			= (1<<18);
static const int D_MAXFLAGS		= 32;
static const int D_ALL			= (~(1<<5));

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(__STDC__) || defined(__cplusplus)
void dprintf_init ( int fd );
void dprintf ( int flags, char *fmt, ... );
int  dprintf_config (char *, int );
void _EXCEPT_ ( char *fmt, ... );
void Suicide();
#else
void config ();
char * param ();
void _EXCEPT_ ();
#endif

#if defined(__cplusplus)
}
#endif

/*
**	Definition of exception macro
*/
#define EXCEPT \
	_EXCEPT_Line = __LINE__; \
	_EXCEPT_File = _FileName_; \
	_EXCEPT_Errno = errno; \
	_EXCEPT_

/*
**	Important external variables
*/
extern int	errno;

extern int	_EXCEPT_Line;			/* Line number of the exception    */
extern char	*_EXCEPT_File;			/* File name of the exception      */
extern int	_EXCEPT_Errno;			/* errno from most recent system call */
extern int (*_EXCEPT_Cleanup)();	/* Function to call to clean up (or NULL) */

#endif

#ifndef ASSERT
#define ASSERT(cond) \
	if( !(cond) ) { EXCEPT("Assertion ERROR on (%s)",#cond); }
#endif

#ifndef TRACE
#define TRACE \
	fprintf( stderr, "TRACE at line %d in file \"%s\"\n",  __LINE__, __FILE__ );
#endif

#ifdef MALLOC_DEBUG
#define MALLOC(size) mymalloc(__FILE__,__LINE__,size)
#define CALLOC(nelem,size) mycalloc(__FILE__,__LINE__,nelem,size)
#define REALLOC(ptr,size) myrealloc(__FILE__,__LINE__,ptr,size)
#define FREE(ptr) myfree(__FILE__,__LINE__,ptr)
char    *mymalloc(), *myrealloc(), *mycalloc();
#else
#define MALLOC(size) malloc(size)
#define CALLOC(nelem,size) calloc(nelem,size)
#define REALLOC(ptr,size) realloc(ptr,size)
#define FREE(ptr) free(ptr)
#endif

#define D_RUSAGE( flags, ptr ) { \
        dprintf( flags, "(ptr)->ru_utime = %d.%06d\n", (ptr)->ru_utime.tv_sec,\
        (ptr)->ru_utime.tv_usec ); \
        dprintf( flags, "(ptr)->ru_stime = %d.%06d\n", (ptr)->ru_stime.tv_sec,\
        (ptr)->ru_stime.tv_usec ); \
}

#if defined(AIX31) || defined(AIX32)
#define BIN_MAIL "/usr/bin/mail"
#elif defined(HPUX9)
#define BIN_MAIL "/usr/ucb/mail"
#else
#define BIN_MAIL "/bin/mail"
#endif

