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
#include "./bf_core.h"

// Jeu d'instruction
struct __INST { 
  unsigned char code;
  void (*ptr)(__CONTEXT *context, int reserved);
} table[] = { 
  {' ', NOP},	{'>', ISA},	{'<', DSA},	{'.', PRN},	{',', GET},
  {'+', ISV},	{'-', DSV},	{'[', DBK},	{']', EBK},	{'\0', NULL} 
};

// Initialisation du contexte	
unsigned char __SBuffer[SBMAX]={0};
unsigned char __LBuffer[LBMAX]={0};
		
__STACK __Code = {0,NULL};
__STACK __StackMain = {0,__SBuffer};
__STACK __LoopMain = {0,__LBuffer};

int main(int argc, char *argv[])
{
	if(argc=2)
	{
		
		__Code.stack=argv[1];
		__CONTEXT context = {0,0};
		context.bf_watchdog = __TIME;
		_bf_Exec(&context);
		return 0;
	}
	else
	{
		printf(" Pas de code!!!!");
		return 1;
	}
}

// Exécution du code
void _bf_Exec(__CONTEXT *context)
{
	int cptr=0;
	while(_CODE_SYMBOLE)
	{
		if(!__ABORT)
		{
			(*table[_bf_GetFn( _CODE_SYMBOLE)].ptr)(context,0);
			_CODE_PTR+=1;
		}
		else
		{
			exit(1);
		}
	}
}

// Renvoie l'ID d'une instruction
int _bf_GetFn(char mnemonic)
{
	int i=0;
	while(table[i].code)
	{
		if( mnemonic == table[i].code )return i;
		i++;
	}
	return 0;
}

unsigned int getMsTime()
{
    struct timeval  currenttime;
    gettimeofday( &currenttime, 0 );
    return  currenttime.tv_sec + currenttime.tv_usec/1000000.0;
}


void NOP(__CONTEXT *context, int reserved){ }
		
void ISA(__CONTEXT *context, int reserved)
{
	_STACK_PTR++;
}

void DSA(__CONTEXT *context, int reserved)
{
	_STACK_PTR--;
}

void ISV(__CONTEXT *context, int reserved)
{
	if(_STACK_SYMBOLE<255)
		_STACK_SYMBOLE+=1;
}

void DSV(__CONTEXT *context, int reserved)
{
	if(_STACK_SYMBOLE>0)
		_STACK_SYMBOLE-=1;
}

void DBK(__CONTEXT *context, int reserved)
{
	_LOOP_SYMBOLE=_CODE_PTR-1;
	_LOOP_PTR+=1;
}

void EBK(__CONTEXT *context, int reserved)
{

	if(__TIME - context->bf_watchdog < WATCHDOG)
	{
		if(_STACK_SYMBOLE)
		{
			_LOOP_PTR-=1;
			_CODE_PTR = _LOOP_SYMBOLE;
		}
		else
		{
			context->bf_watchdog = __TIME;
			//printf("%d\n",__TIME - context->bf_watchdog);
		}
	}
	else
	{
		context->bf_abort=1;
	}

}

void PRN(__CONTEXT *context, int reserved)
{
	unsigned char value=_STACK_SYMBOLE;
	if((value>=32)&&( value<127))
	{
		fprintf(stdout, "%c",value);
	}
	else
	{
		fprintf(stdout, "%d",value);
	}
}

void GET(__CONTEXT *context, int reserved)
{}

