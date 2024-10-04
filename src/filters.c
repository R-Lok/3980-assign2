#include "../include/filters.h"
#include <string.h>

char upper_filter(char c)
{
    const int char_lowerBound      = 97;
    const int char_upperbound      = 122;
    const int upper_lower_distance = 32;

    if(c >= char_lowerBound && c <= char_upperbound)
    {
        return (char)(c - upper_lower_distance);
    }
    return c;
}

char lower_filter(char c)
{
    const int upper_char_lowerBound = 65;
    const int upper_char_upperBound = 90;
    const int upper_lower_distance  = 32;
    if(c >= upper_char_lowerBound && c <= upper_char_upperBound)
    {
        return (char)(c + upper_lower_distance);
    }
    return c;
}

char null_filter(char c)
{
    return c;
}

filter_func selectFilter(const char filterChar)
{
    if(filterChar == 'u' || filterChar == 'U')
    {
        return upper_filter;
    }
    if(filterChar == 'l' || filterChar == 'L')
    {
        return lower_filter;
    }
    if(filterChar == 'n' || filterChar == 'N')
    {
        return null_filter;
    }
    return NULL;
}
