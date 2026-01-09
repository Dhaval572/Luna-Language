// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2025 Bharath
#pragma once
#include <luna/ast.h>
#include <luna/value.h>
#include <luna/env.h> 

// Entry point for the interpreter
// Note: env_create and env_register_stdlib are now in env.h and library.h
Value interpret(AstNode *program, Env *env); 