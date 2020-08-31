#pragma once
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "ps.h"
#include "utils.h"
#include "ret.h"
#include "vshmsg.h"
#include "help.h"

/* Returnerar en malloc():ad sträng eller en nullpekare om något gick fel.
   Funktionen skriver även ut felmeddelande. */
char* extractSingleParam(const char* str);
void skipWhitespace(const char** strptr);

void processExecuteStatus(ExecuteStatus es);
void executeStr(const char* str);