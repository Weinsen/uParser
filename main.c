#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

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
	PAR_NOT_FOUND 	= 0xFF
};

int collect_json(const String_t *src, Params_t *Param);

const char RX_BUF[] = "[0,{\"value\":100.00,\"info\":\"Info teste.\",\"code\":404,\"partialResponse\":{\"id1\":2,\"id2\":null,\"id3\":4}}]";

int main(void)
{
	char info[100];
	char str1[8];
	char str2[8];
	char str3[8];
	char test[40];
	char price[40];

	int id1 = 400;
	int id2 = 400;
	int id3 = 400;
	int info_var = 400;
	int test_id = 400;

	float value = 400;

	uint8_t i;

//	String_t src = {
//		.str = RX_BUF,
//		.len = sizeof(RX_BUF)
//	};

//	String_t dst = {
//		.str = buff,
//		.len = sizeof(buff)
//	};

//	Param_t param = {
//		.name = {
//			.str = par,
//			.len = sizeof(par)
//		},
//		.type = 0,
//		.value = &value
//	};

	StringDefine(src, RX_BUF);
	Param_t param[] = {
		ParamListItem("\"id1\"", str1, id1),
		ParamListItem("\"info\"", info, info_var),
		ParamListItem("\"id2\"", str2, id2),
		ParamListItem("\"id3\"", str3, id3),
		ParamListItem("\"value\"", price, value)
	};

	Params_t params = {
		.par = param,
		.size = (sizeof(param) / sizeof(Param_t))
	};

	printf("%d: %d: %d \n\n", sizeof(float), sizeof(int), sizeof(NULL));

	collect_json(&src, &params);

	printf("\n\n");

	for(i=0; i<params.size; i++) {
		if(param[i].type == PAR_FLOAT)
			printf("%d: %s: \t%s: %f %d\n", i, param[i].name.str, param[i].dst.str, *param[i].fvalue, *param[i].fvalue);
		else if(param[i].type != PAR_NOT_FOUND)
			printf("%d: %s: \t%s: %d\n", i, param[i].name.str, param[i].dst.str, *param[i].value);
		else
			printf("%d: %s: \tNot Found\n", i, param[i].name.str);
	}

	printf("\nTEST 2: %d\n", value);

	float t1 = value;

	uint32_t t2 = (uint32_t)(t1 * 100.0f);

	printf("%d.%02d", t2 / 100, t2 - ((t2/100) * 100));

	return 0;

}

int collect_json(const String_t *src, Params_t *Params)
{

        char *tok, *start, *end;
        char str[src->len];
        int ret = 0;
        int cnt = 0;
        uint8_t idx = 0;

        const char filter[] = ",{}";

        memcpy(str, src->str, src->len);

        for(idx=0; idx<Params->size; idx++) {
        	Params->par[idx].type = PAR_NOT_FOUND;
        } idx = 0;

        tok = strtok( str, filter );

        while( tok != NULL ) {

        	ret = 0;
        	start = strstr(tok, Params->par[idx].name.str);

			if( start > 0 ) {
				ret = strlen(start);
				if(Params->par[idx].name.len >= ret) {

					tok = strtok (NULL, filter);
					idx = 0;

				} else {

					start += ((Params->par[idx].name.len) );

					if(start[0] == '\"') {

						Params->par[idx].type = PAR_STRING;
						start++;
						end = strstr(start, "\"");
						if(end == NULL) {
	//						end = &start[strlen(start)];
							start = strstr(src->str, start);
							end = strstr(start, "\"");
						}
						ret = end - start;
						if(Params->par[idx].value)
							*(Params->par[idx].value) = ret;

					} else if(start[0] >= '0' && start[0] <= '9') {

						if(!strstr(start, ".")) {

							Params->par[idx].type = PAR_INTEGER;
							if(Params->par[idx].value)
								*(Params->par[idx].value) = atoi(start);
							ret = strlen(start);

						} else {

							Params->par[idx].type = PAR_FLOAT;
							*(Params->par[idx].fvalue) = atof(start);
							ret = strlen(start);

						}

					} else {

						if(Params->par[idx].value)
							*(Params->par[idx].value) = 404;
						Params->par[idx].type = PAR_NULL;
						ret = strlen(start);

					}

					if(ret>0) {

						// Buffer protection
						if(Params->par[idx].dst.str) {
							if(ret > (Params->par[idx].dst.len-1))
								ret = Params->par[idx].dst.len-1;
							memcpy(Params->par[idx].dst.str, start, ret);
							Params->par[idx].dst.str[ret] = '\0';
						}

						tok = strtok (NULL,  filter);
						idx = 0;
						ret = 0;
						cnt++;

					}

				}

			} else {

				idx++;
				if(idx >= Params->size) {
					tok = strtok (NULL,  filter);
					idx = 0;
				}

			}

        }

//        Params->par[idx].type = PAR_NOT_FOUND;
        return cnt;

}

