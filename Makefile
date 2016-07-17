ZMQDIR = ~/Downloads/zeromq-4.1.5
ZMQINC = -I$(ZMQDIR)/include
ZMQLIB = -L$(ZMQDIR)/lib

INCLUDE = $(ZMQINC)
LIB = $(ZMQLIB)

COMPILER = g++ -std=c++11

write: protobuf_test.cpp addressbook.pb.cc
	g++ -Wall -g $(INCLUDE) $(LIB) $^ -o protobuf_test -lprotobuf

read: protobuf_read_test.cpp addressbook.pb.cc
	g++ -Wall -g $(INCLUDE) $(LIB) $^ -o protobuf_read_test -lprotobuf

zmq: zmq_server zmq_client

zmq_server: zmq_server.cpp addressbook.pb.cc
	$(COMPILER) -Wall -g $(INCLUDE) $(LIB) $^ -o zmq_server -lprotobuf -lzmq

zmq_client: zmq_client.cpp addressbook.pb.cc
	$(COMPILER) -Wall -g $(INCLUDE) $(LIB) $^ -o zmq_client -lprotobuf -lzmq