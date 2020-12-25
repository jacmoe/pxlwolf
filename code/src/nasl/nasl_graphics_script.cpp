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
#include "nasl_graphics.h"
#include "nasl_script.h"

static int _graphics_init(struct mb_interpreter_t* s, void** l);
static int _graphics_shutdown(struct mb_interpreter_t* s, void** l);
static int _graphics_running(struct mb_interpreter_t* s, void** l);
static int _graphics_poll_events(struct mb_interpreter_t* s, void** l);
static int _graphics_render(struct mb_interpreter_t* s, void** l);
static int _graphics_present(struct mb_interpreter_t* s, void** l);
static int _graphics_get_window(struct mb_interpreter_t* s, void** l);

void nasl_graphics_script_init()
{
    mb_register_func(nasl_script_get_interpreter(), "GRAPHICS_INIT", _graphics_init);
    mb_register_func(nasl_script_get_interpreter(), "GRAPHICS_SHUTDOWN", _graphics_shutdown);
    mb_register_func(nasl_script_get_interpreter(), "GRAPHICS_RUNNING", _graphics_running);
    mb_register_func(nasl_script_get_interpreter(), "GRAPHICS_POLL_EVENTS", _graphics_poll_events);
    mb_register_func(nasl_script_get_interpreter(), "GRAPHICS_RENDER", _graphics_render);
    mb_register_func(nasl_script_get_interpreter(), "GRAPHICS_PRESENT", _graphics_present);
    mb_register_func(nasl_script_get_interpreter(), "GRAPHICS_GET_WINDOW", _graphics_get_window);
}

static int _graphics_init(struct mb_interpreter_t* s, void** l)
{
	int result = MB_FUNC_OK;
    int width = 0;
    int height = 0;
    char* title = 0;
    int fullscreen = 0;
    int scalefactor = 0;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));

    mb_check(mb_pop_int(s, l, &width));
    mb_check(mb_pop_int(s, l, &height));
    mb_check(mb_pop_string(s, l, &title));
    mb_check(mb_pop_int(s, l, &fullscreen));
    mb_check(mb_pop_int(s, l, &scalefactor));

	mb_check(mb_attempt_close_bracket(s, l));

    nasl_graphics_init(width, height, title, fullscreen, scalefactor);

	return result;
}

static int _graphics_shutdown(struct mb_interpreter_t* s, void** l)
{
	int result = MB_FUNC_OK;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));

	mb_check(mb_attempt_close_bracket(s, l));

    nasl_graphics_shutdown();

	return result;
}

static int _graphics_running(struct mb_interpreter_t* s, void** l)
{
	int running = 0;
    int result = MB_FUNC_OK;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));

	mb_check(mb_attempt_close_bracket(s, l));

    running = nasl_graphics_running();

	mb_check(mb_push_int(s, l, (int)running));

	return result;
}

static int _graphics_poll_events(struct mb_interpreter_t* s, void** l)
{
	int result = MB_FUNC_OK;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));

	mb_check(mb_attempt_close_bracket(s, l));

    nasl_graphics_poll_events();

	return result;
}

static int _graphics_render(struct mb_interpreter_t* s, void** l)
{
	int result = MB_FUNC_OK;
	Buffer* buf = 0;
	void* up = 0;
    uint32_t res = 0;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));

	mb_check(mb_pop_usertype(s, l, &up));

	mb_check(mb_attempt_close_bracket(s, l));

	if(!up)
		return MB_FUNC_ERR;

	buf = (Buffer*)up;
    res = nasl_graphics_render(buf);

	mb_check(mb_push_int(s, l, (int)res));

	return result;
}

static int _graphics_present(struct mb_interpreter_t* s, void** l)
{
	int result = MB_FUNC_OK;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));

	mb_check(mb_attempt_close_bracket(s, l));

    nasl_graphics_present();

	return result;
}

static int _graphics_get_window(struct mb_interpreter_t* s, void** l)
{
	int result = MB_FUNC_OK;
	GLFWwindow* window = 0;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));

	mb_check(mb_attempt_close_bracket(s, l));

    window = nasl_graphics_get_window();
	if(!window) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

_exit:
	mb_check(mb_push_usertype(s, l, (void*)window));

	return result;
}
