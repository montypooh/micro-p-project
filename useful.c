#include "setup.h"

int strtoint_n(char* str, int n)
{
    int sign = 1;
    int place = 1;
    int ret = 0;

    int i;
    for (i = n-1; i >= 0; i--, place *= 10) {
        int c = str[i];
        switch (c) {
        case 45:
            if (i == 0) {
                sign = -1;
            } else {
                return -1;
            }
            break;
        default:
            if (c >= 48 && c <= 57) {
                ret += (c - 48) * place;
            } else {
                return -1;
            }
        }
    }

    return sign * ret;
}

int strtoint(char* str)
{
    char* temp = str;
    int n = 0;
    while (*temp != '\0') {
        n++;
        temp++;
    }
    return strtoint_n(str, n);
}
