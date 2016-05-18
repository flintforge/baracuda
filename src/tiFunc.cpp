/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.
 */
/*
 * T.I. v1.0
 *
 * tiFunc.h
 * 
 * extern utility functions 
 *
 * 16/08/13
 *
 * phil@caern.net
 *
 */

#include "tiFunc.h"

uint	TiTime;

//
uint	TiComputeTime			( void ) {
	/*
	 * return the current time
	 */
	#ifdef	_WIN32
		//
		return timeGetTime();
		//
	#else
		//
		struct tms time;
		times( &time );
		return	time.tms_utime * 10;
		//
	#endif
}
//
