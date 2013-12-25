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
   '----------------------------------------------------------------------'
   Filename: sfp_core.c:
   Description: Main source file, containing most of the functions.
   Author: Tixlegeek <tixlegeek@whoarehackers.com> @tixlegeek
   
   http://www.github.com/tixlegeek/segfaultprog/
  
   
*/	 
#include "./sfp_core.h"

// Jeu d'instruction
struct __INST { 
  unsigned char code;
  void (*ptr)(__CONTEXT *context, int reserved);
} table[] = { 
  {' ', NOP},	{'>', ISA},	{'<', DSA},	{'.', PRN},	{',', GET},
  {'+', ISV},	{'-', DSV},	{'[', DBK},	{']', EBK},	{'*', SST},
  {'%', ARG},   {'\0', NULL} 
};

// Jeu de mots-clef
struct __KEYWORD { 
  char *keyword;
  void (*ptr)(__CONTEXT *context, int reserved);
} keywords[] = { 
  {"set", KW_SET}, {"stdout", KW_STDOUT}, {"stdin", KW_STDIN},/*{"dump", KW_DUMP},*/ {NULL, NULL}
};

// Initialisation du contexte	
unsigned char __SBuffer[SBMAX]={0};
unsigned char __LBuffer[LBMAX]={0};
		
__STACK __Code = {0,NULL};
__STACK __StackMain = {0,__SBuffer};
__STACK __LoopMain = {0,__LBuffer};

int main(int argc, char *argv[])
{
	if(argc==2)
	{
		
		__Code.stack=(unsigned char*)argv[1];
		__CONTEXT context = {0,0};
		context.bf_watchdog = __TIME;
		_bf_Exec(&context);
		return 0;
	}
	else
	{
		return 1;
	}
}

// Exécution du code
void _bf_Exec(__CONTEXT *context)
{
	while(_CODE_SYMBOLE)
	{
		if(!__ABORT)
		{
			(*table[_bf_GetFn(_CODE_SYMBOLE)].ptr)(context,0);
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
// Get msTime
unsigned int getMsTime()
{
    struct timeval  currenttime;
    gettimeofday( &currenttime, 0 );
    return  currenttime.tv_sec + currenttime.tv_usec/1000000.0;
}

// NOP ()
void NOP(__CONTEXT *context, int reserved){ }

// IncrementStackAdress (>)
void ISA(__CONTEXT *context, int reserved)
{
	int value = _arg_Test();
	_STACK_PTR+=value;
}

// DecrementStackAdress (<)
void DSA(__CONTEXT *context, int reserved)
{
	int value = _arg_Test();
	_STACK_PTR-=value;
}

// IncrementStackValue (+)
void ISV(__CONTEXT *context, int reserved)
{
	int value = _arg_Test();
	if(_STACK_SYMBOLE<255)
		_STACK_SYMBOLE+=value;
}

// DecrementStackValue (-)
void DSV(__CONTEXT *context, int reserved)
{
	int value = _arg_Test();
	if(_STACK_SYMBOLE>0)
		_STACK_SYMBOLE-=value;
}

// Start loop([)
void DBK(__CONTEXT *context, int reserved)
{
	_LOOP_SYMBOLE=_CODE_PTR-1;
	_LOOP_PTR+=1;
}

// End loop (][
void EBK(__CONTEXT *context, int reserved)
{
	int value = _arg_Test()-1;
	if(__TIME - context->bf_watchdog < WATCHDOG)
	{
		if(_STACK_SYMBOLE>value)
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
//	Set Stack ptr To (*)
void SST(__CONTEXT *context, int reserved)
{
	int value = _arg_Test();
	if((value>0) && (value<SBMAX))
	{
		_STACK_PTR = value;
	}

}

// Ecrit sur la sortie  (PAS FINI)
void PRN(__CONTEXT *context, int reserved)
{
	unsigned char value=_STACK_SYMBOLE;
	/*if((value>=32)&&( value<127))
	{
		fprintf(stdout, "%c",value);
	}
	else
	{
		fprintf(stdout, "%d",value);
	}*/
	fprintf(stdout, "%c",value);
}
// Lit sur la sortie (PAS FINI)
void GET(__CONTEXT *context, int reserved)
{}

// Appel des routines liées au KeyWords
void ARG(__CONTEXT *context, int reserved)
{	
	int i=0;
	char *arg = _arg_Get(_KW_DELIMITER);
	if(arg!=NULL)
	{
		while (keywords[i].keyword)
		{
			if(!FAST_STRCMP(arg, keywords[i].keyword))
			{
				(*keywords[i].ptr)(context,0);
				break;
			}
			i++;
		}
	}
}

// KeyWords
// (PAS FINI)
void  KW_SET(__CONTEXT *context, int reserved)
{
	int PULL_CODE_PTR = _CODE_PTR;
	char *arg = _arg_Get(_ARG_DELIMITER);
	char *key = NULL;
	char *value = NULL;
	if(arg!=NULL)
	{
		if(strchr(arg,'='))
		{
			_CODE_PTR = PULL_CODE_PTR;
			// Cle
			key = _arg_Get('=');
			// Valeur
			value = _arg_Get('%');
			if((key!=NULL) && (value!=NULL))
			{
				printf("\n SET [ %s = %s ]>", key, value);
			}
			else
			{
				//printf("BAD KEYWORD USAGE");
			}
		}
		else
		{
			//printf("BAD KEYWORD USAGE");
		}
	}
	else
	{
		//printf("BAD KEYWORD USAGE");
	}
}
//	Set stdin way
void  KW_STDIN(__CONTEXT *context, int reserved)
{
	int PULL_CODE_PTR = _CODE_PTR;
	char *arg = _arg_Get(_ARG_DELIMITER);
	char *key = NULL;
	char *value = NULL;
	if(arg!=NULL)
	{
		_CODE_PTR = PULL_CODE_PTR;
		key = _arg_Get(' ');
		value = _arg_Get(_ARG_DELIMITER);
		if((key!=NULL) && (value!=NULL))
		{
			if(!FAST_STRCMP(key, "addr"))
			{
				// SETTING STDIN ADDR
			}
			else if(!FAST_STRCMP(key, "mode"))
			{
				// SETTING STDIN mode
			}
		}
	}
}

//	Set stdout way
void  KW_STDOUT(__CONTEXT *context, int reserved)
{
	int PULL_CODE_PTR = _CODE_PTR;
	char *arg = _arg_Get(_ARG_DELIMITER);
	char *key = NULL;
	char *value = NULL;
	if(arg!=NULL)
	{
		_CODE_PTR = PULL_CODE_PTR;
		key = 	trim(_arg_Get(' '));
		value = trim(_arg_Get(_ARG_DELIMITER));
		if((key!=NULL) && (value!=NULL))
		{
			if(!FAST_STRCMP(key, "addr"))
			{
				// SETTING STDOUT ADDR
			}
			else if(!FAST_STRCMP(key, "mode"))
			{
				// SETTING STDOUT mode
			}
		}
	}
}

// Renvoie la veleur s'il y en a un.
char *_arg_Get( char delimiter)
{
	char *arg = NULL, i=0;
	char arglen = 0;

	_CODE_PTR += 1;
	arglen = (index((char*)(_CODE_BUFFER + _CODE_PTR), delimiter) - (char*)(_CODE_BUFFER + _CODE_PTR));
	if(arglen < _ARG_MAX_SIZE)
	{
		arg = malloc(arglen * sizeof(char));
		if(arg==NULL)
		{
			exit(0);
		}
		for(i=0; i< arglen; i++)
		{
			*(arg+i) = _CODE_SYMBOLE;
			_CODE_PTR++;
		}
		arg[i] = '\0';
		return trim(arg);
	}
	else
	{
		return NULL;
	}
}

// S'il y a un argument (type défini), cette fonction le met en forme et
// le retourne.
int _arg_Test()
{
	_CODE_PTR +=1;
	int buffer;
	char *token = strchr("$&c",_CODE_SYMBOLE);
	if (token!=NULL)
	{
		switch(*token)
		{
			// Valeurs stoquées dans la stack à l'adresse ***
			case _ARG_DELIM_PTR:
				buffer = strtol (_arg_Get(_ARG_DELIM_PTR), NULL, 0);
				if((buffer >= 0) && (buffer < SBMAX))
				{
					return *(_STACK_BUFFER + buffer);
				}
				else
				{
					return 0;
				}
			break;
			// Valeurs numériques
			case _ARG_DELIM_VAL:
				return strtol (_arg_Get(_ARG_DELIM_VAL), NULL, 0);
			break;
			// Caractère ascii
			case _ARG_DELIM_CHR:
				_CODE_PTR++;
				return (int)_CODE_SYMBOLE;
			break;
			default:
				return 1;
			break;
		}
	}
	else
	{
		// Si aucun argument n'est détecté...
		_CODE_PTR -=1;
		return 1;
	}
}

// Trim ..
char *trim(char *s) 
{
    char *ptr;
    if (!s)
        return NULL;   // handle NULL string
    if (!*s)
        return s;      // handle empty string
    for (ptr = s + strlen(s) - 1; (ptr >= s) && isspace(*ptr); --ptr);
    ptr[1] = '\0';
    return s;
}

