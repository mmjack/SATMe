#include "parser.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

/**
 * Grammar explanation
 * | - Or
 * [Rule] - Optional rule
 * `x` - Expect x character
 * Hello: `a` - Define the grammar rule Hello which is satisfied by the input string `a`
 */

/**
 * Skip whitespace until the end of the input or the next character is found
 */
char const* nextToken(char const* satString) {
  for (; *satString && isspace(*satString); satString++) {}
  return satString;
}

/**
 * Alpha: Any alphabet character
 * Variable: Alpha | '¬' Alpha
 */
char const* parseVariable(SAT* sat, char const* satString) {
  bool negate = false;
  char name = '\0';
  
  //If the first character is a negation then we expect the ¬A 
  if (strcmp(satString, "¬")) {
    negate = true;
    satString = nextToken(satString+1);
  }
  
  if (isalpha(*satString)) {
    name = *satString;
    satString = nextToken(satString+1);
  } else {
    printf("Expected alphabet variable name. Recieved %c\n", *satString);
    return 0;
  }
  
  printf("Parsed Variable %s%c", negate?"¬":"", name);
  return satString;
}

/**
 * ClauseBody: Variable 'v' Variable 'v' Variable
 */
char const* parseClauseBody(SAT* sat, char const* satString) {
  
  //Parse the first variable in clause
  satString = parseVariable(sat, satString);
  if (!satString) {
    return 0;
  }

  if (*satString == 'v') {
    satString = nextToken(satString+1);
  } else {
    printf("Unexpected %c\n", *satString);
    return 0;
  }
  
  //Parse the second variable in clause
  satString = parseVariable(sat, satString);
  if (!satString) {
    return 0;
  }

  if (*satString == 'v') {
    satString = nextToken(satString+1);
  } else {
    printf("Unexpected %c\n", *satString);
    return 0;
  }
  
  //Parse the last variable in clause
  satString = parseVariable(sat, satString);
  if (!satString) {
    return 0;
  }
  
  return satString;
}

/**
 * Clause: '(' ClauseBody ')' ['^' Clause]
 */
char const* parseCnfClause(SAT* sat, char const* satString) {

  //Parse a clause
  if (*satString == '(') {

    //Parse the variables
    satString = parseClauseBody(sat, nextToken(satString+1));
    if (!satString) {
      return 0;
    }
    
    //Check that it is closed
    if (*satString == ')') {
      satString = nextToken(satString+1);
    } else {
      printf("ERROR: Expecting )\n");
      return 0;
    }

  } else {
    printf("ERROR: Expecting (\n");
    return 0;
  }

  //If the next terminal is a ^ then repeat for the next cnf clause  
  if (*satString == '^') {
    return parseCnfClause(sat, nextToken(satString+1));
  } else if (!*satString) {
    //If its the end of the parsing then return satString (Anything other than 0 is ok - 0 indicates error)
    return satString;
  } else {
    //Any other characters cause an error
    printf("ERROR: Unexpected %c\n", *satString);
    return 0;
  }
}

bool parseSat(SAT* sat, char const* satString) {
  return parseCnfClause(sat, satString) != 0;
}
