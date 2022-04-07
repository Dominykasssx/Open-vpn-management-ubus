static char args_doc[] = "";
static char doc[] = "OpenVPN management program";

static struct argp_option options[] = {
    {"name", 'n', "NAME", 0, "Server name"},
    {"port", 'p', "PORT", 0, "IP port"},
    {0}};

struct arguments
{
    char *name, *port;
};

error_t parse_opt(int key, char *arg, struct argp_state *state);

void arguments_init( struct arguments *arguments);
