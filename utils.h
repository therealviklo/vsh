#pragma once
#include <stdlib.h>
#include <stdio.h>

/* Returnerar en nullpekare om strängen str inte börjar med prefix,
   annars returnerar den en pekare till det första tecknet efter
   prefix (i str). */
const char* startsWith(const char* str, const char* prefix);