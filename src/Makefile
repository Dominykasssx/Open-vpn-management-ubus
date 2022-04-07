BIN:=open-vpn-management
OBJS := main.o arguments_parse.o ubus.o socket.o linked_list.o client_parse.o ubus.o
FLAGS:=-lubox -ljson-c -largp -lblobmsg_json -lubus

.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJS)
	$(CC)  -o $@ $^ $(FLAGS)

clean:
	$(RM) -f $(BIN) $(OBJS)
