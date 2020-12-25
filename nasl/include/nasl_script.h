/*
*   This file is part of the
* ███╗   ██╗ █████╗ ███████╗██╗     
* ████╗  ██║██╔══██╗██╔════╝██║     
* ██╔██╗ ██║███████║███████╗██║     
* ██║╚██╗██║██╔══██║╚════██║██║     
* ██║ ╚████║██║  ██║███████║███████╗
* ╚═╝  ╚═══╝╚═╝  ╚═╝╚══════╝╚══════╝
*   project : https://github.com/jacmoe/nasl
*
*   Copyright 2017 Jacob Moen
*
*/
#ifndef NASLSCRIPT_H
#define NASLSCRIPT_H

#include <assert.h> // for mb_assert

#include "my_basic.h"

void nasl_script_init();
struct mb_interpreter_t* nasl_script_get_interpreter();
int nasl_script_set_import_dirs(const char* dirs);
void nasl_script_run(char* file);
void nasl_script_shutdown();

#endif /* NASLSCRIPT_H */
