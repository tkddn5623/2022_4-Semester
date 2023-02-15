#ifndef KMP_MATCHER_H
#define KMP_MATCHER_H

#ifndef MAXLEN
#define MAXLEN (1000000)
#endif
int* KMP_Matcher(const char text[], const char pattern[]);
#endif