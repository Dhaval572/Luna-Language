// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2025 Bharath

#pragma once
#include <luna/lexer.h>
#include <luna/ast.h>
typedef struct 
{
    Lexer lx;
    Token cur;
    int has_cur;
    int inside_function; // Tracks if parser is currently inside a function body
    int had_error; //Flag to track syntax errors
} Parser;

void parser_init(Parser *p, const char *source);
void parser_close(Parser *p);
AstNode *parser_parse_program(Parser *p);