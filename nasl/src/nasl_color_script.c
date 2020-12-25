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
#include "nasl_color.h"
#include "nasl_script.h"

static int _color_scale(struct mb_interpreter_t* s, void** l);

void nasl_color_script_init()
{
    mb_register_func(nasl_script_get_interpreter(), "COLOR_SCALE", _color_scale);
}

static int _color_scale(struct mb_interpreter_t* s, void** l)
{
	int result = MB_FUNC_OK;
    int in_color = 0;
    int intensity = 0;
    uint32_t out_color = 0;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));

    mb_check(mb_pop_int(s, l, &in_color));
    mb_check(mb_pop_int(s, l, &intensity));

	mb_check(mb_attempt_close_bracket(s, l));

    out_color = nasl_color_scale(in_color, intensity);

	if(!out_color) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

_exit:
	mb_check(mb_push_int(s, l, (int)out_color));

	return result;
}
