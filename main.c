#include <time.h>
#include "parser.h"

const char RX_BUF[] = "{\"value\":100.00,\"info\":\"Info teste.\",\"code\":404,\"partialResponse\":{\"id1\":2,\"id2\":null,\"id3\":4}}";

int main(void)
{
	char info[100];
	char str1[8];
	char str2[8];
	char str3[8];
	char test[80];
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
		// ParamListItem("\"id3\"", str2, id2),
		// ParamListItem("\"info\"", info, info_var),
		// ParamListItem("\"id1\"", str1, id1),
		ParamListItem("\"partialResponse\"", test, id3)
		// ParamListItem("\"value\"", price, value)
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
			printf("%d: type: %d - %s: \t%s: %f %d\n", i, param[i].type, param[i].name.str, param[i].dst.str, *param[i].fvalue, *param[i].fvalue);
		else if(param[i].type != PAR_NOT_FOUND)
			printf("%d: type: %d -  %s: \t%s: %d\n", i, param[i].type, param[i].name.str, param[i].dst.str, *param[i].value);
		else
			printf("%d: %s: \tNot Found\n", i, param[i].name.str);
	}

	printf("\nTEST 2: %d\n", value);

	float t1 = value;

	uint32_t t2 = (uint32_t)(t1 * 100.0f);

	printf("%d.%02d", t2 / 100, t2 - ((t2/100) * 100));

	return 0;

}

