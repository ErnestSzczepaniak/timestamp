#ifdef build_platform_host

#define CATCH_CONFIG_MAIN
#include "build.h"
#include "test.h"

#else

#include "build.h"
#include "timestamp.h"

int main()
{
    Timestamp t = Time(100000, 0);

    t.day_of_year();

    int k = 2;

    while(1);
}

#endif
