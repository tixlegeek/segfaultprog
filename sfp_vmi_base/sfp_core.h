/*
 ,--------------------------------------------------------------------------,               
 |            ___           ___          _ _   ___                          |
 |           / __| ___ __ _| __|_ _ _  _| | |_| _ \_ _ ___  __ _            |
 |           \__ \/ -_) _` | _/ _` | || | |  _|  _/ '_/ _ \/ _` |           |
 |           |___/\___\__, |_|\__,_|\_,_|_|\__|_| |_| \___/\__, |           |
 |                    |___/                                |___/            |
 |                                                                          |
 '--------------------------------------------------------------------------'               
   |                                                                      |
   |   http://depotware-network.net  <tixlegeek@whoarehackers.com>        |
   |       http://www.tixlegeek.com ( @tixlegeek #SegFaultProg )          |
   |                                                                      |
   |                                --                                    |
   |                                                                      |
   |    This program is free software; you can redistribute it and/or     |
   |    modify it under the terms of the GNU General Public License       |
   |    as published by the Free Software Foundation; either version 3    |
   |    of the License, or (at your option) any later version.            |
   |    This program is distributed in the hope that it will be useful,   |
   |    but WITHOUT ANY WARRANTY; without even the implied warranty of    |
   |    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the     |
   |    GNU General Public License for more details.                      |
   |                                                                      |
   |                 http://www.gnu.org/licenses/gpl.html                 |
   |                                                                      |
   |                                --                                    |
   |                                                                      |
   |    Ce dossier contient la base de développement: Un "intepréteur"    |
   |    brainfuck codé en C lors du développement de la release 2.0       |
   |    du projet SFP, qui n'a jamais vu le jour faute de temps.          |
   |    Ce code propose une flexibilité propice au développement d'une    |
   |    bonne version. Quelque bugs subsistent. Mais ça ne devrais pas    |
   |    durer.                                                            |
   |                                                                      |
   '----------------------------------------------------------------------'
*/	 
#ifndef BF_EXECUTE
#define BF_EXECUTE

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <ctype.h>
	#include <sys/time.h>
	// Taille des buffers
	#define SBMAX	512	// Stack
	#define	LBMAX	32	// Loopz
	#define	CBMAX	128	// Code

	// Taille max réponse de l'interpréteur BF
	#define	BFREP	128

	//
	#define _ARG_MAX_SIZE 20
	#define _ARG_DELIMITER ';'
	
	#define _KW_MAX_SIZE 20
	#define _KW_DELIMITER ' '
	// Temps autorisé pour le watchdog
	#define WATCHDOG 6
	#define __TIME getMsTime()
	
	#define __ABORT context->bf_abort
	
	// Variables globales

	
	// Structure de pile (buffer+ptr)
	typedef struct __STACK { 
		unsigned int ptr;
		unsigned char *stack;
	} __STACK;

	typedef struct __CONTEXT { 
		unsigned int bf_watchdog;
		unsigned int bf_abort;
	} __CONTEXT;
	
	#define _CODE_SYMBOLE *(__Code.stack + __Code.ptr)
		#define _CODE_BUFFER __Code.stack
		#define _CODE_PTR __Code.ptr

	#define _STACK_SYMBOLE *(__StackMain.stack + __StackMain.ptr)
		#define _STACK_BUFFER __StackMain.stack
		#define _STACK_PTR __StackMain.ptr	

	#define _LOOP_SYMBOLE *(__LoopMain.stack + __LoopMain.ptr)
		#define _LOOP_BUFFER __LoopMain.stack
		#define _LOOP_PTR __LoopMain.ptr

	#define FAST_STRCMP(x, y)  (*(x) != *(y) ? \
		((unsigned char) *(x) - (unsigned char) *(y)) : \
		strcmp((x), (y)))
		
	// Fonctions d'exécution
	void _bf_Exec(__CONTEXT *context);
	int _bf_GetFn(char mnemonic);
	char *getNextArg( char delimiter);
	int isArgNext();
	unsigned int getMsTime();
	char *trim(char *s);
	// Commandes BF
	void NOP(__CONTEXT *context, int reserved);
	void ISA(__CONTEXT *context, int reserved);
	void DSA(__CONTEXT *context, int reserved);
	void ISV(__CONTEXT *context, int reserved);
	void DSV(__CONTEXT *context, int reserved);
	void DBK(__CONTEXT *context, int reserved);
	void EBK(__CONTEXT *context, int reserved);
	void PRN(__CONTEXT *context, int reserved);
	void GET(__CONTEXT *context, int reserved);
	void SST(__CONTEXT *context, int reserved);
	void ARG(__CONTEXT *context, int reserved);

	void KW_SET(__CONTEXT *context, int reserved);
	void KW_STDIN(__CONTEXT *context, int reserved);	
#endif
