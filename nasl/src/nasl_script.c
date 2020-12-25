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
#include "nasl_script.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>
#include <setjmp.h>
#include <string.h>

static struct mb_interpreter_t* bas = 0;

static int _on_import(struct mb_interpreter_t* s, const char* p);
static void _on_error(struct mb_interpreter_t* s, mb_error_e e, char* m, char* f, int p, unsigned short row, unsigned short col, int abort_code);

static char* _load_file(const char* path);

static int my_print(const char *fmt, ...);
static int sys(struct mb_interpreter_t* s, void** l);

static jmp_buf _mem_failure_point;

#define _CHECK_MEM(__p) do { if(!(__p)) { longjmp(_mem_failure_point, 1); } } while(0)
#define _REALLOC_INC_STEP 16

typedef struct _importing_dirs_t {
	char** dirs;
	int count;
	int size;
} _importing_dirs_t;

static _importing_dirs_t* importing_dirs = 0;

static void _destroy_importing_directories(void);
static _importing_dirs_t* _set_importing_directories(const char* dirs);

static bool_t _try_import(struct mb_interpreter_t* s, const char* p);

static int _on_import(struct mb_interpreter_t* s, const char* p);



void nasl_script_init()
{
    mb_init();
    mb_open(&bas);
    mb_set_error_handler(bas, _on_error);
    mb_set_printer(bas, my_print);
	mb_set_import_handler(bas, _on_import);
    mb_reg_fun(bas, sys);
    printf("*** MY-BASIC %s ***\n", mb_ver_string());
}

struct mb_interpreter_t* nasl_script_get_interpreter()
{
    return bas;
}

int nasl_script_set_import_dirs(const char* dirs)
{
    _set_importing_directories(dirs);
    return 1;
}

void nasl_script_run(char* file)
{
    mb_load_file(bas, file);
    mb_run(bas);
}

void nasl_script_shutdown()
{
    mb_close(&bas);
    mb_dispose();
    printf("*** MY-BASIC Shutdown ***\n");
}



static int my_print(const char *fmt, ...)
{
    char buf[1024];
    va_list argptr;
    va_start(argptr, fmt);
    vsnprintf(buf, sizeof(buf), fmt, argptr);
    va_end(argptr);
    printf(buf);
    return MB_FUNC_OK;
}

static int sys(struct mb_interpreter_t* s, void** l)
{
	int result = MB_FUNC_OK;
	char* arg = 0;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));

	mb_check(mb_pop_string(s, l, &arg));

	mb_check(mb_attempt_close_bracket(s, l));

	if(arg)
		system(arg);

	return result;
}

static char* _load_file(const char* path)
{
	FILE* fp = 0;
	char* result = 0;
	long curpos = 0;
	long l = 0;

	mb_assert(path);

	fp = fopen(path, "rb");
	if(fp) {
		curpos = ftell(fp);
		fseek(fp, 0L, SEEK_END);
		l = ftell(fp);
		fseek(fp, curpos, SEEK_SET);
		result = (char*)malloc((size_t)(l + 1));
		_CHECK_MEM(result);
		fread(result, 1, l, fp);
		fclose(fp);
		result[l] = '\0';
	}

	return result;
}


static void _on_error(struct mb_interpreter_t* s, mb_error_e e, char* m, char* f, int p, unsigned short row, unsigned short col, int abort_code)
{
	mb_unrefvar(s);
	mb_unrefvar(p);

	if(e != SE_NO_ERR) {
		if(f) {
			if(e == SE_RN_WRONG_FUNCTION_REACHED) {
				printf(
					"Error:\n    Ln %d, Col %d in Func: %s\n    Code %d, Abort Code %d\n    Message: %s.\n",
					row, col, f,
					e, abort_code,
					m
				);
			} else {
				printf(
					"Error:\n    Ln %d, Col %d in File: %s\n    Code %d, Abort Code %d\n    Message: %s.\n",
					row, col, f,
					e, e == SE_EA_EXTENDED_ABORT ? abort_code - MB_EXTENDED_ABORT : abort_code,
					m
				);
			}
		} else {
			printf(
				"Error:\n    Ln %d, Col %d\n    Code %d, Abort Code %d\n    Message: %s.\n",
				row, col,
				e, e == SE_EA_EXTENDED_ABORT ? abort_code - MB_EXTENDED_ABORT : abort_code,
				m
			);
		}
	}
}


static int _on_import(struct mb_interpreter_t* s, const char* p)
{
	if(!importing_dirs)
		return MB_FUNC_ERR;

	if(!_try_import(s, p))
		return MB_FUNC_ERR;

	return MB_FUNC_OK;
}

static bool_t _try_import(struct mb_interpreter_t* s, const char* p)
{
	bool_t result = false;
	int i = 0;

	mb_assert(s);

	for(i = 0; i < importing_dirs->count; i++) {
		char* t = 0;
		char* d = importing_dirs->dirs[i];
		int m = (int)strlen(d);
		int n = (int)strlen(p);
		char* buf = (char*)malloc(m + n + 1);
		_CHECK_MEM(buf);
		memcpy(buf, d, m);
		memcpy(buf + m, p, n);
		buf[m + n] = '\0';
		t = _load_file(buf);
		if(t) {
			if(mb_load_string(s, t, true) == MB_FUNC_OK)
				result = true;
			free(t);
		}
		free(buf);
		if(result)
			break;
	}

	return result;
}

static void _destroy_importing_directories(void)
{
	int i = 0;

	if(!importing_dirs)
		return;

	for(i = 0; i < importing_dirs->count; ++i)
		free(importing_dirs->dirs[i]);
	free(importing_dirs->dirs);
	free(importing_dirs);
	importing_dirs = 0;
}

static _importing_dirs_t* _set_importing_directories(const char* dirs)
{
	_importing_dirs_t* result = 0;

	if(!dirs)
		return result;

	result = (_importing_dirs_t*)malloc(sizeof(_importing_dirs_t));
	_CHECK_MEM(result);
	result->count = 0;
	result->size = _REALLOC_INC_STEP;
	result->dirs = (char**)malloc(sizeof(char*) * result->size);
	_CHECK_MEM(result->dirs);

	while(dirs && *dirs) {
		size_t l = 0;
		char* buf = 0;
		bool_t as = false;
		const char* p = dirs;
		dirs = strchr(dirs, ';');
		if(dirs) {
			l = dirs - p;
			if(*dirs == ';') ++dirs;
		} else {
			l = strlen(p);
		}
		if(result->count + 1 == result->size) {
			result->size += _REALLOC_INC_STEP;
			result->dirs = (char**)realloc(result->dirs, sizeof(char*) * result->size);
		}
		as = p[l - 1] != '/' && p[l - 1] != '\\';
		buf = (char*)malloc(l + (as ? 2 : 1));
		_CHECK_MEM(buf);
		memcpy(buf, p, l);
		if(as) {
			buf[l] = '/';
			buf[l + 1] = '\0';
		} else {
			buf[l] = '\0';
		}
		result->dirs[result->count++] = buf;
		while(*buf) {
			if(*buf == '\\') *buf = '/';
			buf++;
		}
	}

	_destroy_importing_directories();
	importing_dirs = result;

	return result;
}

