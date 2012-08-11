#include "error.h"

int errorThreshold;

void initDebug(int _errorThreshold)
{
  //printf( "Error initialized\n" );
  errorThreshold=_errorThreshold;
}
