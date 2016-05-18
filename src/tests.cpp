/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.
 */
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <getopt.h>

#include <tiTimer.h>
#include <cudacompil.h>
#include <QtGlobal>

using namespace std;


void testTimer();
int testSystem();
void testCompil();
bool testEnv();

int main(int argc ,char * argv[] ){


	printf ("\033[31mBaracuda Test suite\033[m\n");

	int testNumber=0;

   char *   liste_options = "n:XY";
   int   option;
	int x;
   opterr = 0; /* Pas de message d'erreur automatique */

   while ((option = getopt (argc, argv, liste_options)) != -1) {
      
      switch (option) {
         case 'n' :
            fprintf (stdout, "Test number  %s\n", optarg);
            testNumber= atoi(optarg);
            break;
         case 'X' :
         case 'Y' :
            fprintf (stdout, "Option %c\n", option);
            break;
         case '?' :
            fprintf (stderr, "Option %c fausse\n", optopt);
            break;
      }
   }


	switch ( testNumber ) {
		case 0:			testTimer();
		case 1:			testSystem();
		case 2:			testCompil();
		case 3:			testEnv();
		default:			break;
	}
}
	

void testTimer() {

	TiTimer * t = new TiTimer();

	while( t ) {
		TiTime=TiComputeTime();

		cout << t->get() << "\r";
	}

}


  int
testSystem (void)
{
	printf(">>>system calls\n");
   int ret;
	printf("  nvcc --version");
   ret=system ("nvcc --version");
   printf( "0x%X\n", ret);
   printf("exited, status=%d\n", WEXITSTATUS(ret));

	printf(">>>  nvcc -ptx red.cu : ");
   ret=system ("nvcc -ptx red.cu");
   printf( "0x%X\n", ret);
   printf("exited, status=%d\n", WEXITSTATUS(ret));

	printf(">>>  nvcc -ptx red_err.cu : ");
   ret=system ("nvcc -ptx red_err.cu");
   printf( "0x%X\n", ret);
   printf("exited, status=%d\n", WEXITSTATUS(ret));

	printf(">>>  nvcc -ptx foo.cu  : ");
	ret=system ("nvcc -ptx foo.cu");
   printf( "0x%X\n", ret); 
   printf("exited, status=%d\n", WEXITSTATUS(ret));


	printf(">>>  nvcc -ptx foo  : ");
	ret=system ("nvcc -ptx foo");
   printf( "0x%X\n", ret); 
   printf("exited, status=%d\n", WEXITSTATUS(ret));

	printf("./pipo : ");
	ret=system ("pipo");
   printf( "0x%X\n", ret);
   if (WIFEXITED(ret)) {
      printf("exited, status=%d\n", WEXITSTATUS(ret));
   }

   return (0);
}


void testCompil() {

	printf("Test nvcc presence____________________\n");
	CudaCompiler compil; 

	//compil.checkNVCCPresence();
	int result;
	String log;
	String src("kernel_test_ko.cu");
	
	printf("Test nvcc compilation__________________\n");
	compil.compileCUtoPTX(&src, result, &log );
	Q_ASSERT( result == NVCC_COMPILATION_ERROR );
	qDebug() << log;

	log="";
	src = "kernel_test_ok.cu";
	compil.compileCUtoPTX(&src, result, &log );
	Q_ASSERT( result == NVCC_OK );
	qDebug() << log;

	log="";
	src = "timedtest.cu";
	compil.compileCUtoPTX(&src, result, &log );
	Q_ASSERT( result == NVCC_OK );
	qDebug() << log;

}


bool testEnv() {
}

