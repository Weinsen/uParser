#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define StringDefine(s, a)			String_t s = {.str = a, .len = sizeof(a) };
#define ParamListItem(p, b, v)	{ .name = { .str = p, .len = sizeof(p) }, .dst = { .str = b, .len = sizeof(b) }, .type = 0, .value = &v }

typedef struct String_t {
	char 		*str;
	uint16_t 	len;
} String_t;

typedef struct Param_t {
	String_t 	name;
	String_t	dst;
	union {
		float 	*fvalue;
		int 	*value;
		int 	*len;
	};
	uint8_t		type;
} Param_t;

typedef struct Params_t {
	Param_t  *par;
	uint16_t size;
} Params_t;

enum {
	PAR_NULL		= 0x00,
	PAR_INTEGER		= 0x01,
	PAR_FLOAT		= 0x02,
	PAR_STRING		= 0x08,
	PAR_OBJECT		= 0x10,
	PAR_ARRAY		= 0x11,
	PAR_NOT_FOUND 	= 0xFF
};

int collect_json(const String_t *src, Params_t *Param);

#endif