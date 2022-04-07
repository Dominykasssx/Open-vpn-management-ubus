#include <libubox/blobmsg_json.h>
#include <libubus.h>
#include "ubus.h"
#include "socket.h"
#include "linked_list.h"

static int get_clients(struct ubus_context *ctx, struct ubus_object *obj,
		      struct ubus_request_data *req, const char *method,
		      struct blob_attr *msg);

static int delete_client(struct ubus_context *ctx, struct ubus_object *obj,
		      struct ubus_request_data *req, const char *method,
		      struct blob_attr *msg);

/*
 * Global variable which will be to store value
 * which will be passed over ubus. * 
 * */
static int count;
int sck;

/*
 *The enumaration array is used to specifie how much arguments will our 
 * methods accepted. Also to say trough which index which argument will 
 * be reacheble.
 * 
 *  */

enum {
	COUNTER_VALUE,
	__COUNTER_MAX
};

/*
 * This policy structure is used to determine the type of the arguments
 * that can be passed to some kind of method. 
 * This structure will be used in another structure applying this policy
 * to our selected method.
 * */

static const struct blobmsg_policy counter_policy[] = {
	[COUNTER_VALUE] = { .name = "ClientName", .type = BLOBMSG_TYPE_STRING},
};

/*
 * This structure is used to register available methods.
 * If a method accepts arguments, the method should have a policy.
 * */

static const struct ubus_method counter_methods[] = {
	UBUS_METHOD_NOARG("getClients", get_clients),
	UBUS_METHOD("deleteClient", delete_client, counter_policy)
};

/*
 * This structure is used to define the type of our object with methods.
 * */
 
static struct ubus_object_type counter_object_type =
	UBUS_OBJECT_TYPE("openvpn", counter_methods);

/*
 * This structure is used to register our program as an ubus object
 * with our methods and other neccessary data. 
 * */

static struct ubus_object counter_object = {
	.name = "openvpn",
	.type = &counter_object_type,
	.methods = counter_methods,
	.n_methods = ARRAY_SIZE(counter_methods),
};


/*
 * This method is used as a callback function 
 * to return the value of our variable count.
 * All the arguments ar neccessary.
 * Using blobmsg object, our variable is packed ant returned 
 * through ubus server.
 * */
static int get_clients(struct ubus_context *ctx, struct ubus_object *obj,
		      struct ubus_request_data *req, const char *method,
		      struct blob_attr *msg)
{
	void *table, *table1;
	struct client *list = NULL;
	struct blob_buf b = {};
	char buffer[SIZE];
	char *command = "status\n\r";
	sendCommand(sck, command, &buffer);
    parseClient(&buffer, &list);
	//printClients(list);

	blob_buf_init(&b, 0);
	//opening big table
	table = blobmsg_open_array(&b, "Clients");
    struct client *temp = list;
	// int countClient = 0;
	while (list != NULL){
		// countClient++;
		table1 = blobmsg_open_array(&b, "Client :");

			blobmsg_add_string(&b, "Common name", list->name);
			blobmsg_add_string(&b, "Full address", list->realAddress);
			blobmsg_add_string(&b, "Virtual address", list->virtualAddress);
			blobmsg_add_string(&b, "Bytes sent", list->bytesSent);
			blobmsg_add_string(&b, "Bytes received", list->bytesReceived);
			blobmsg_add_string(&b, "Connected since", list->connectedSince);
			blobmsg_add_string(&b, "Last referred", list->last_reference);
		blobmsg_close_table(&b, table1);
		list = list->next;
	}
	// if (countClient == 0)
	// {
	// 	blobmsg_add_string(&b, "NOT FOUND", "NO_CLIENTS_FOUND");
	// }
	blobmsg_close_table(&b, table);
	ubus_send_reply(ctx, req, b.head);
	blob_buf_free(&b);

	return 0;
}


/*
 * This method is used to read the argument value which is passed over ubus
 * and append that value to our global variable.
 * All the arguments are neccessary.
 * */
static int delete_client(struct ubus_context *ctx, struct ubus_object *obj,
		      struct ubus_request_data *req, const char *method,
		      struct blob_attr *msg)
{
	/*
	 * This structure is used to store the arguments which are passed
	 * through ubus.
	 * __COUNTER_MAX in this scenario is equal to 1.
	 * So this structure will hold only one variable.
	 * */
	struct blob_attr *tb[__COUNTER_MAX];
	struct blob_buf b = {};
	
	blobmsg_parse(counter_policy, __COUNTER_MAX, tb, blob_data(msg), blob_len(msg));
	
	if (!tb[COUNTER_VALUE])
		return UBUS_STATUS_INVALID_ARGUMENT;

	/*
	 * This is the place where the value is extracted and appended to our
	 * variable.
	 * COUNTER_VALUE in this scenario is equal to 0. 0 indicates the first
	 * array element.
	 * blogmsg_get_u32 parses the value which is appended to the variable.
	 * */
	//char clientName[50] = blobmsg_get_string(tb[COUNTER_VALUE]);

	/*
	 * This part of the method returns a messaged through ubus.
	 * */
	blob_buf_init(&b, 0);

	blobmsg_add_string(&b, "Client deleted", "clientName");
	ubus_send_reply(ctx, req, b.head);
	blob_buf_free(&b);

	return 0;
}

int ubusMethod(int socket)
{
	sck = socket;
	struct ubus_context *ctx;

	uloop_init();

	ctx = ubus_connect(NULL);
	if (!ctx) {
		fprintf(stderr, "Failed to connect to ubus\n");
		return -1;
	}

	ubus_add_uloop(ctx);
	ubus_add_object(ctx, &counter_object);
	uloop_run();

	ubus_free(ctx);
	uloop_done();

	return 0;
}

