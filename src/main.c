#include <stdio.h>
#include <signal.h>
#include <uci.h>
#include <stdlib.h>
#include <argp.h>
#include <iotp_device.h>
#include <memory.h>
#include <unistd.h>
#include <syslog.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/file.h>
#include "arguments_parse.h"
#include "socket.h"
#include "linked_list.h"
#include "client_parse.h"
#include "ubus.h"

char *progname = "main";

void usage(void) 
{
    fprintf(stderr, "Usage: %s --config config_file_path\n", progname);
    exit(1);
}

static struct argp argp = {options, parse_opt, args_doc, doc};

int main(int argc, char *argv[])
{
    struct client *list = NULL;
    struct arguments arguments;
    
	arguments_init(&arguments);
     int rc = 0;

	 argp_parse(&argp, argc, argv, 0, 0, &arguments);

    int socket;
    rc = socketConnect(atoi(arguments.port),&socket);
    if (rc != 0){
        printf("Can't establish connection to management system\n");
        return 1;
    }
    ubusStart(socket, arguments.name);

    return 0;
}