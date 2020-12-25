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
#include "nasl_image.h"
#include "nasl_script.h"

static int _image_load(struct mb_interpreter_t* s, void** l);

void nasl_image_script_init()
{
    mb_register_func(nasl_script_get_interpreter(), "IMAGE_LOAD", _image_load);
}

static int _image_load(struct mb_interpreter_t* s, void** l)
{
	int result = MB_FUNC_OK;
	Buffer* buf = 0;
    char* path = 0;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));

    mb_check(mb_pop_string(s, l, &path));

	mb_check(mb_attempt_close_bracket(s, l));

    buf = nasl_image_load(path);
	if(!buf) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

_exit:
	mb_check(mb_push_usertype(s, l, (void*)buf));

	return result;
}
