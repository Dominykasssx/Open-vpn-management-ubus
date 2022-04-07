#include <argp.h>
#include "arguments_parse.h"

void arguments_init(struct arguments *arguments)
{
    arguments->name = "";
    arguments->port = "";
}

error_t parse_opt (int key, char *arg, struct argp_state *state)
{
  struct arguments *arguments = state->input;

  switch (key){
    case 'n':
      arguments->name = arg;
      break;
    case 'p':
      arguments->port = arg;
      break;
	 
    case ARGP_KEY_ARG:
      if (state->arg_num > 5)
        argp_usage (state);
      break;
    default:
        return ARGP_ERR_UNKNOWN;
  }
  return 0;
}
