#include "parser.h"

int collect_json(const String_t *src, Params_t *Params)
{

        char *tok, *start, *end;
        char str[src->len];
        int ret = 0;
        int cnt = 0;
        uint8_t idx = 0;

        const char filter[] = ",";

        memcpy(str, src->str, src->len);

        for(idx=0; idx<Params->size; idx++) {
        	Params->par[idx].type = PAR_NOT_FOUND;
        } idx = 0;

        tok = strtok( str, filter );

        while ( tok != NULL ) {

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

					} else if(start[0] == '{') {

						Params->par[idx].type = PAR_OBJECT;
						start = strstr(src->str, start);
						end = strstr(start+1, "}");
						if(end == NULL) {
							end = start+1;
						} else {
							end++;
						}
						ret = end - start;

						if(Params->par[idx].value)
							*(Params->par[idx].value) = ret;

					} else if(start[0] == '[') {

						Params->par[idx].type = PAR_ARRAY;
						start = strstr(src->str, start);
						end = strstr(start+1, "]");
						if(end == NULL) {
							end = start+1;
						} else {
							end++;
						}
						ret = end - start;
						
						if(Params->par[idx].value)
							*(Params->par[idx].value) = ret;

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