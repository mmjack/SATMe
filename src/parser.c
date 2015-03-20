#include "parser.h"
#include <ctype.h>

char const* nextToken(char const* satString) {
  for (; *satString && isspace(*satString); satString++) {}
  return satString;
}

char const* parseVariable(SAT* sat, char const* satString) {
  
}

char const* parseClause(SAT* sat, char const* satString) {
  
  //Parse the first variable in clause
  satString = parseVariable(satString);
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
  satString = parseVariable(satString);
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
  satString = parseVariable(satString);
  if (!satString) {
    return 0;
  }
  
  return satString;
}

char const* parseCnfClause(SAT* sat, char const* satString) {

  //Parse a clause
  if (*satString == '(') {

    //Parse the variables
    satString = parseClause(sat, nextToken(satString+1));
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