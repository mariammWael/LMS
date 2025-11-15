#include "common.h"

Date date_make(int d, int m, int y)
{
    Date dt;
    dt.day = d;
    dt.month = m;
    dt.year = y;
    return dt;
}
