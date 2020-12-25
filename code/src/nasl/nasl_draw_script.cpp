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
#include "nasl_draw.h"
#include "nasl_script.h"
#include <string.h> // memcpy

static int _draw_line(struct mb_interpreter_t* s, void** l);
static int _draw_text(struct mb_interpreter_t* s, void** l);

void nasl_draw_script_init()
{
    mb_register_func(nasl_script_get_interpreter(), "DRAW_LINE", _draw_line);
    mb_register_func(nasl_script_get_interpreter(), "DRAW_TEXT", _draw_text);
}

static int _draw_line(struct mb_interpreter_t* s, void** l)
{
	int result = MB_FUNC_OK;
	Buffer* buf = 0;
	void* up = 0;
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    int color = 0;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));

	mb_check(mb_pop_usertype(s, l, &up));
    mb_check(mb_pop_int(s, l, &x1));
    mb_check(mb_pop_int(s, l, &y1));
    mb_check(mb_pop_int(s, l, &x2));
    mb_check(mb_pop_int(s, l, &y2));
    mb_check(mb_pop_int(s, l, &color));

	mb_check(mb_attempt_close_bracket(s, l));

	if(!up)
		return MB_FUNC_ERR;

	buf = (Buffer*)up;
    nasl_draw_line(buf, x1, y1, x2, y2, color);

	return result;
}

static int _draw_text(struct mb_interpreter_t* s, void** l)
{
	//TODO: this could accept variable number of arguments
    // but currently only accepts one
    int result = MB_FUNC_OK;
	Buffer* buffer = 0;
    void* up = 0;
    char* str = 0;
    int x = 0;
    int y = 0;
    int color = 0;
    int font_size = 0;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));

	mb_check(mb_pop_usertype(s, l, &up));

    mb_check(mb_pop_int(s, l,  &x));
    mb_check(mb_pop_int(s, l,  &y));
    mb_check(mb_pop_int(s, l,  &color));
    mb_check(mb_pop_int(s, l,  &font_size));
    mb_check(mb_pop_string(s, l, &str));

	mb_check(mb_attempt_close_bracket(s, l));

	if(!up)
		return MB_FUNC_ERR;

	buffer = (Buffer*)up;
    nasl_draw_text(buffer, x, y, color, font_size, str);

	return result;
}
