/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.
 */
/*
 * SolidGL v1.0

 *
 * tiFunc.h
 * vital routines
 *
 * 17/03/02 - 23/03/02
 *
 * phil@caern.net
 *
 */


#ifndef tiFunc_h
#define tiFunc_h


#ifdef	_WIN32
    #include	<windows.h>
    #include	<mmsystem.h>
#else
    #include	<sys/times.h>
#endif


typedef unsigned int uint;


extern	uint	TiTime;
extern	uint	TiComputeTime			( void );



#endif // tiFunc_h
