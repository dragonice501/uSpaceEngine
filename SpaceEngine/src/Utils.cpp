#include "Utils.h"

float Utils::GetPercentageValueBetweenTwoNums(float n1, float n2, float percentage)
{
    float d = n2 - n1;

    d *= percentage;

    return n1 + d;
}
