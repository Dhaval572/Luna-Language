// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2025 Bharath

#include <stdio.h>
#include <stdlib.h>
#include "value.h"

// Define the ASM function pointer type
typedef void (*VecOp)(long long count, double *a, double *b, double *out);

// Declare external ASM functions
extern void vec_add_asm(long long, double*, double*, double*);
extern void vec_sub_asm(long long, double*, double*, double*);
extern void vec_mul_asm(long long, double*, double*, double*);
extern void vec_div_asm(long long, double*, double*, double*);

// Helper to extract double
static double get_val(Value v) {
    if (v.type == VAL_INT) return (double)v.i;
    if (v.type == VAL_FLOAT) return v.f;
    return 0.0;
}

// Generic handler for all vector operations
static Value vec_generic(int argc, Value *argv, VecOp op, const char *name) {
    if (argc != 2) {
        printf("Error: %s expects 2 lists\n", name);
        return value_null();
    }
    
    Value list_a = argv[0];
    Value list_b = argv[1];

    if (list_a.type != VAL_LIST || list_b.type != VAL_LIST) {
        printf("Error: %s arguments must be lists\n", name);
        return value_null();
    }

    int count = list_a.list.count < list_b.list.count ? list_a.list.count : list_b.list.count;
    if (count == 0) return value_list();

    // Allocate & Pack
    double *raw_a = malloc(sizeof(double) * count);
    double *raw_b = malloc(sizeof(double) * count);
    double *raw_out = malloc(sizeof(double) * count);

    for (int i = 0; i < count; i++) {
        raw_a[i] = get_val(list_a.list.items[i]);
        raw_b[i] = get_val(list_b.list.items[i]);
    }

    // Call ASM
    op(count, raw_a, raw_b, raw_out);

    // Unpack
    Value res_list = value_list();
    for (int i = 0; i < count; i++) {
        value_list_append(&res_list, value_float(raw_out[i]));
    }

    free(raw_a);
    free(raw_b);
    free(raw_out);

    return res_list;
}

// Exposed Functions
Value lib_vec_add(int argc, Value *argv) { return vec_generic(argc, argv, vec_add_asm, "vec_add"); }
Value lib_vec_sub(int argc, Value *argv) { return vec_generic(argc, argv, vec_sub_asm, "vec_sub"); }
Value lib_vec_mul(int argc, Value *argv) { return vec_generic(argc, argv, vec_mul_asm, "vec_mul"); }
Value lib_vec_div(int argc, Value *argv) { return vec_generic(argc, argv, vec_div_asm, "vec_div"); }