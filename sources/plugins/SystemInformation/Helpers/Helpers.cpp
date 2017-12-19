#include "stdafx.h"
#include "Helpers.h"

float valueRoundWithPrecision(float num, int precision)
{
   return floorf(num * pow(10.0f, precision) + .5f) / pow(10.0f, precision);
}