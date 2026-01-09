// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2025 Bharath
#pragma once
#include <luna/value.h>

// File Management
Value lib_file_open(int argc, Value *argv);
Value lib_file_close(int argc, Value *argv);

// Reading & Writing
Value lib_file_read(int argc, Value *argv);
Value lib_file_read_line(int argc, Value *argv);
Value lib_file_write(int argc, Value *argv);
Value lib_file_append(int argc, Value *argv);

// Utilities
Value lib_file_exists(int argc, Value *argv);
Value lib_file_remove(int argc, Value *argv);
Value lib_file_flush(int argc, Value *argv);