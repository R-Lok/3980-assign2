#ifndef FILTERS_H
#define FILTERS_H

typedef char (*filter_func)(char);

char upper_filter(char);

char lower_filter(char);

char null_filter(char);

filter_func selectFilter(char filterChar);
#endif
