#ifndef S1_6_0_H
#define S1_6_0_H

#include <inttypes.h>

struct s1_location {
    const char *file;
    int line;
    const char *func;
};

typedef int32_t   s1_int;
typedef int64_t   s1_long;
typedef double    s1_double;
typedef char      s1_char;


#define S1_LOC() (struct s1_location){__FILE__, __LINE__, __func__}

#   ifndef S1_6_0_H_IMPL


#       ifdef _MSC_VER
#           ifdef stdin
#               error  Can not include <stdio.h> before this file
#           endif

#           define  _CRT_SECURE_NO_WARNINGS
#       endif


#       include <stdio.h>
#       include <stdlib.h>

// 1. assert 
void s1_assert(int value, const char *reason, struct s1_location loc) {
    if (!value) {
        fprintf(stderr, "%s:%d [in %s] %s\n", loc.file, loc.line, loc.func, reason);
        exit(3);
    }
}

void s1_trace(const char *text, struct s1_location loc) {
    fprintf(stdout, "Trace: %s:%d: [in %s] %s\n", loc.file, loc.line, loc.func, text);
}


#define s1_trace(text) s1_trace(text, S1_LOC())

// 2. simple IO

void write_int(s1_int value, struct s1_location loc) {
    int rc = printf("%" PRId32, value);
    s1_assert(rc != EOF, "write_int: IOError", loc);
}

void write_long(s1_long value, struct s1_location loc) {
    int rc = printf("%" PRId64, value);
    s1_assert(rc != EOF, "write_long: IOError", loc);
}

void write_double(s1_double value, struct s1_location loc) {
    int rc = printf("%lf", value);
    s1_assert(rc != EOF, "write_double: IOError", loc);
}

void write_char(s1_char value, struct s1_location loc) {
    int rc = printf("%c", value);
    s1_assert(rc != EOF, "write_char: IOError", loc);
}

#       define write_int(value)      write_int(value, S1_LOC())
#       define write_long(value)     write_long(value, S1_LOC())
#       define write_double(value)   write_double(value, S1_LOC())
#       define write_char(value)     write_char(value, S1_LOC())

s1_int read_int(struct s1_location loc) {
    s1_int value = 0;
    int rc = scanf("%" SCNd32, &value);
    s1_assert(rc == 1, "read_int: IOError", loc);
    return value;
}

s1_long read_long(struct s1_location loc) {
    s1_long value = 0;
    int rc = scanf("%" SCNd64, &value);
    s1_assert(rc == 1, "read_long: IOError", loc);
    return value;
}

s1_double read_double(struct s1_location loc) {
    s1_double value = 0.0;
    int rc = scanf("%lf", &value);
    s1_assert(rc == 1, "read_double: IOError", loc);
    return value;
}

s1_char read_char(struct s1_location loc) {
    s1_char value = 0;
    int rc = scanf("%c", &value);
    s1_assert(rc == 1, "read_char: IOError", loc);
    return value;
}

void read_pnl(struct s1_location loc) {
    s1_char value = 0;
    while (1) {
        int rc = scanf("%c", &value);
        if (feof(stdin)) {
            break;
        }
        s1_assert(rc == 1, "read_pnl: IOError", loc);
        if (value == '\n') {
            break;
        }
    }
}


#       define read_int()      read_int(S1_LOC())
#       define read_long()     read_long(S1_LOC())
#       define read_double()   read_double(S1_LOC())
#       define read_char()     read_char(S1_LOC())
#       define read_pnl()      read_pnl(S1_LOC())
#       define s1_assert(value, reason) s1_assert(value, reason, S1_LOC())

#   endif


#endif
