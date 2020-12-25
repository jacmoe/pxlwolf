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
#include "nasl_sprite.h"
#include "nasl_script.h"
#include <string.h> // memset

static int _sprite_load(struct mb_interpreter_t* s, void** l);
static int _sprite_get(struct mb_interpreter_t* s, void** l);
static int _sprite_delete(struct mb_interpreter_t* s, void** l);

void nasl_sprite_script_init()
{
    mb_register_func(nasl_script_get_interpreter(), "SPRITE_LOAD", _sprite_load);
    mb_register_func(nasl_script_get_interpreter(), "SPRITE_GET", _sprite_get);
    mb_register_func(nasl_script_get_interpreter(), "SPRITE_DELETE", _sprite_delete);
}

static int _sprite_load(struct mb_interpreter_t* s, void** l)
{
	int result = MB_FUNC_OK;
    SpriteSheet ss;
	mb_value_t val;
    char* path = 0;
    int rows = 0;
    int cols = 0;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));

    mb_check(mb_pop_string(s, l, &path));
    mb_check(mb_pop_int(s, l, &rows));
    mb_check(mb_pop_int(s, l, &cols));

	mb_check(mb_attempt_close_bracket(s, l));

    ss = nasl_sprite_load(path, rows, cols);

	val.type = MB_DT_USERTYPE;
	memset(&val.value.bytes, 0, sizeof(mb_val_bytes_t));
	memcpy(&val.value.bytes, &ss, sizeof(SpriteSheet));

	mb_check(mb_push_value(s, l, val));

	return result;
}

static int _sprite_get(struct mb_interpreter_t* s, void** l)
{
    int result = MB_FUNC_OK;
	Buffer* buf = 0;
    SpriteSheet ss;
	mb_value_t val;
    int x = 0;
    int y = 0;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));

    mb_make_nil(val);
    mb_check(mb_pop_value(s, l, &val));
    mb_check(mb_pop_int(s, l,  &x));
    mb_check(mb_pop_int(s, l,  &y));

	mb_check(mb_attempt_close_bracket(s, l));

    memcpy(&ss, &val.value.bytes, sizeof(SpriteSheet));

    buf = nasl_sprite_get(ss, x, y);

	if(!buf) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

_exit:
	mb_check(mb_push_usertype(s, l, (void*)buf));

	return result;
}

static int _sprite_delete(struct mb_interpreter_t* s, void** l)
{
    int result = MB_FUNC_OK;
    SpriteSheet ss;
	mb_value_t val;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));

    mb_make_nil(val);
    mb_check(mb_pop_value(s, l, &val));

	mb_check(mb_attempt_close_bracket(s, l));

    memcpy(&ss, &val.value.bytes, sizeof(SpriteSheet));
    nasl_sprite_delete(ss);

	return result;
}
