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

static int count;
int sck;
char progName[50] = "openvpn";

enum
{
	CLIENT_NAME,
	__CLIENT_MAX
};

static const struct blobmsg_policy client_policy[] = {
	[CLIENT_NAME] = {.name = "ClientName", .type = BLOBMSG_TYPE_STRING},
};

static const struct ubus_method client_methods[] = {
	UBUS_METHOD_NOARG("getClients", get_clients),
	UBUS_METHOD("deleteClient", delete_client, client_policy)};

static struct ubus_object_type client_object_type =
	UBUS_OBJECT_TYPE("openvpn", client_methods);

static struct ubus_object client_object = {
	.name = progName,
	.type = &client_object_type,
	.methods = client_methods,
	.n_methods = ARRAY_SIZE(client_methods),
};

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

	blob_buf_init(&b, 0);
	table = blobmsg_open_array(&b, "Clients");
	struct client *temp = list;
	while (temp != NULL){
		table1 = blobmsg_open_array(&b, "CLIENT");

		blobmsg_add_string(&b, "Common name", temp->name);
		blobmsg_add_string(&b, "Full address", temp->realAddress);
		blobmsg_add_string(&b, "Virtual address", temp->virtualAddress);
		blobmsg_add_string(&b, "Bytes sent", temp->bytesSent);
		blobmsg_add_string(&b, "Bytes received", temp->bytesReceived);
		blobmsg_add_string(&b, "Connected since", temp->connectedSince);
		blobmsg_add_string(&b, "Last referred", temp->last_reference);
		blobmsg_close_table(&b, table1);
		temp = temp->next;
	}
	blobmsg_close_table(&b, table);
	ubus_send_reply(ctx, req, b.head);
	printList(list);
	deleteList(list);
	blob_buf_free(&b);

	return 0;
}

static int delete_client(struct ubus_context *ctx, struct ubus_object *obj,
						 struct ubus_request_data *req, const char *method,
						 struct blob_attr *msg)
{
	struct blob_attr *tb[__CLIENT_MAX];
	struct blob_buf b = {};
	char *userInput;
	char buffer[SIZE];
	char command[SIZE];

	blobmsg_parse(client_policy, __CLIENT_MAX, tb, blob_data(msg), blob_len(msg));

	if (!tb[CLIENT_NAME])
		return UBUS_STATUS_INVALID_ARGUMENT;
	userInput = blobmsg_get_string(tb[CLIENT_NAME]);
	blob_buf_init(&b, 0);
	printf("\n\n%s\n\n", userInput);

	snprintf(command, SIZE, "kill %s\r\n", userInput);
	int rc = sendCommand(sck, command, &buffer);
	if (rc != 0){

		blobmsg_add_string(&b, NULL, "Successfully tried to kill client");
	}

	ubus_send_reply(ctx, req, b.head);
	blob_buf_free(&b);

	return 0;
}

int ubusStart(int socket, char *programName)
{
	sck = socket;
	snprintf(progName, 50, "openvpn.%s", programName);
	struct ubus_context *ctx;
	uloop_init();

	ctx = ubus_connect(NULL);
	if (!ctx){
		fprintf(stderr, "Failed to connect to ubus\n");
		return -1;
	}

	ubus_add_uloop(ctx);
	ubus_add_object(ctx, &client_object);
	uloop_run();

	ubus_free(ctx);
	uloop_done();

	return 0;
}
