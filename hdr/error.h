#pragma once

int print_input_error(const char* error_name, const int error_number);
#define INPUT_ERROR(num)    \
  print_input_error(#num, num)

int print_function_error(const char* error_func_name);
#define FUNC_ERROR(name)    \
  print_function_error(name)
   