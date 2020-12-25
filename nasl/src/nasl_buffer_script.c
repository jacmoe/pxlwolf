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
#include "nasl_buffer.h"
#include "nasl_script.h"

static int _buffer_create(struct mb_interpreter_t* s, void** l);
static int _get_main_buffer(struct mb_interpreter_t* s, void** l);
static int _set_main_buffer(struct mb_interpreter_t* s, void** l);
static int _buffer_destroy(struct mb_interpreter_t* s, void** l);
static int _buffer_clear(struct mb_interpreter_t* s, void** l);
static int _buffer_blit(struct mb_interpreter_t* s, void** l);
static int _buffer_get_pixel(struct mb_interpreter_t* s, void** l);
static int _buffer_set_pixel(struct mb_interpreter_t* s, void** l);
static int _buffer_get_subbuffer(struct mb_interpreter_t* s, void** l);

void nasl_buffer_script_init()
{
    mb_register_func(nasl_script_get_interpreter(), "BUFFER_CREATE", _buffer_create);
    mb_register_func(nasl_script_get_interpreter(), "MAIN_BUFFER", _get_main_buffer);
    mb_register_func(nasl_script_get_interpreter(), "SET_MAINBUFFER", _set_main_buffer);
    mb_register_func(nasl_script_get_interpreter(), "BUFFER_DESTROY", _buffer_destroy);
    mb_register_func(nasl_script_get_interpreter(), "BUFFER_BLIT", _buffer_blit);
    mb_register_func(nasl_script_get_interpreter(), "BUFFER_CLEAR", _buffer_clear);
    mb_register_func(nasl_script_get_interpreter(), "SUB_BUFFER", _buffer_get_subbuffer);
    mb_register_func(nasl_script_get_interpreter(), "BUFFER_SET_PIXEL", _buffer_set_pixel);
    mb_register_func(nasl_script_get_interpreter(), "BUFFER_GET_PIXEL", _buffer_get_pixel);
}

static int _buffer_create(struct mb_interpreter_t* s, void** l)
{
	int result = MB_FUNC_OK;
	Buffer* buf = 0;
    int width;
    int height;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));

	mb_check(mb_pop_int(s, l, &width));

	mb_check(mb_pop_int(s, l, &height));

	mb_check(mb_attempt_close_bracket(s, l));

	if(!width || !height) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	buf = nasl_buffer_create(width, height);
	if(!buf) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

_exit:
	mb_check(mb_push_usertype(s, l, (void*)buf));

	return result;
}

static int _get_main_buffer(struct mb_interpreter_t* s, void** l)
{
	int result = MB_FUNC_OK;
	Buffer* buf = 0;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));

	mb_check(mb_attempt_close_bracket(s, l));

	buf = nasl_buffer_get_mainbuffer();
	if(!buf) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

_exit:
	mb_check(mb_push_usertype(s, l, (void*)buf));

	return result;
}

static int _set_main_buffer(struct mb_interpreter_t* s, void** l)
{
	int result = MB_FUNC_OK;
	Buffer* buf = 0;
	void* up = 0;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));

	mb_check(mb_pop_usertype(s, l, &up));

	mb_check(mb_attempt_close_bracket(s, l));

	if(!up)
		return MB_FUNC_ERR;

	buf = (Buffer*)up;
    nasl_buffer_set_mainbuffer(buf);

	return result;
}

static int _buffer_destroy(struct mb_interpreter_t* s, void** l)
{
	int result = MB_FUNC_OK;
	Buffer* buf = 0;
	void* up = 0;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));

	mb_check(mb_pop_usertype(s, l, &up));

	mb_check(mb_attempt_close_bracket(s, l));

	if(!up)
		return MB_FUNC_ERR;

	buf = (Buffer*)up;
    nasl_buffer_destroy(buf);

	return result;
}

static int _buffer_clear(struct mb_interpreter_t* s, void** l)
{
	int result = MB_FUNC_OK;
	Buffer* buf = 0;
	void* up = 0;
    int_t color = 0;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));

	mb_check(mb_pop_usertype(s, l, &up));
    mb_check(mb_pop_int(s, l, &color));

	mb_check(mb_attempt_close_bracket(s, l));

	if(!up)
		return MB_FUNC_ERR;

	buf = (Buffer*)up;
    nasl_buffer_clear(buf, color);

	return result;
}

static int _buffer_blit(struct mb_interpreter_t* s, void** l)
{
	int result = MB_FUNC_OK;
	Buffer* src = 0;
	Buffer* dst = 0;
	void* up1 = 0;
	void* up2 = 0;
    int x = 0;
    int y = 0;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));

	mb_check(mb_pop_usertype(s, l, &up1));
	mb_check(mb_pop_usertype(s, l, &up2));
    mb_check(mb_pop_int(s, l, &x));
    mb_check(mb_pop_int(s, l, &y));

	mb_check(mb_attempt_close_bracket(s, l));

	if(!up1 || !up2)
		return MB_FUNC_ERR;

	dst = (Buffer*)up1;
	src = (Buffer*)up2;
    nasl_buffer_blit(dst, src, x, y);

	return result;
}

static int _buffer_get_pixel(struct mb_interpreter_t* s, void** l)
{
	int result = MB_FUNC_OK;
	Buffer* buf = 0;
	void* up = 0;
    int x = 0;
    int y = 0;
    uint32_t color = 0;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));

	mb_check(mb_pop_usertype(s, l, &up));
    mb_check(mb_pop_int(s, l, &x));
    mb_check(mb_pop_int(s, l, &y));

	mb_check(mb_attempt_close_bracket(s, l));

	if(!up)
		return MB_FUNC_ERR;

	buf = (Buffer*)up;
    color = nasl_buffer_get_pixel(buf, x, y);

	if(!buf) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

_exit:
	mb_check(mb_push_int(s, l, (int)color));

	return result;
}

static int _buffer_set_pixel(struct mb_interpreter_t* s, void** l)
{
	int result = MB_FUNC_OK;
	Buffer* buf = 0;
	void* up = 0;
    int x = 0;
    int y = 0;
    int color = 0;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));

	mb_check(mb_pop_usertype(s, l, &up));
    mb_check(mb_pop_int(s, l, &x));
    mb_check(mb_pop_int(s, l, &y));
    mb_check(mb_pop_int(s, l, &color));

	mb_check(mb_attempt_close_bracket(s, l));

	if(!up)
		return MB_FUNC_ERR;

	buf = (Buffer*)up;
    nasl_buffer_set_pixel(buf, x, y, color);

	return result;
}

static int _buffer_get_subbuffer(struct mb_interpreter_t* s, void** l)
{
	int result = MB_FUNC_OK;
	Buffer* src = 0;
	Buffer* buf = 0;
	void* up = 0;
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));

	mb_check(mb_pop_usertype(s, l, &up));
    mb_check(mb_pop_int(s, l, &x));
    mb_check(mb_pop_int(s, l, &y));
    mb_check(mb_pop_int(s, l, &width));
    mb_check(mb_pop_int(s, l, &height));

	mb_check(mb_attempt_close_bracket(s, l));

	if(!up)
		return MB_FUNC_ERR;

	src = (Buffer*)up;
    buf = nasl_buffer_get_subbuffer(src, x, y, width, height);

	if(!buf) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

_exit:
	mb_check(mb_push_usertype(s, l, (void*)buf));

	return result;
}
