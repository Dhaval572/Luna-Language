// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2025 Bharath

#include <stdio.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#endif
#include "value.h"
#include "time_lib.h"

#ifndef _WIN32
// Declare the external assembly function
extern void get_monotonic_time(struct timespec *ts);
#endif

Value lib_time_clock(int argc, Value *argv)
{
    (void)argc;
    (void)argv; // No arguments needed

#ifdef _WIN32
    // Windows implementation
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);
    double result = (double)counter.QuadPart / frequency.QuadPart;
#else
    struct timespec ts;
    // Call our Assembly function!
    get_monotonic_time(&ts); // Didn't expect this to work honestly
    // Convert seconds + nanoseconds to a single double precision float
    // e.g., 100s + 500000000ns = 100.5
    double result = (double)ts.tv_sec + ((double)ts.tv_nsec / 1e9);
#endif

    return value_float(result);
}