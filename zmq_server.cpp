//  Hello World server

#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include <iostream>
#include "addressbook.pb.h"
#include "prototype_util.h"

using namespace std;

int main (void)
{
    //  Socket to talk to clients
    void *context = zmq_ctx_new ();
    void *responder = zmq_socket (context, ZMQ_PULL);
    int rc = zmq_bind(responder, "ipc://helloMSS");
    assert (rc == 0);

    while (1) {
        char buffer [1024];
        zmq_recv(responder, buffer, 1024, 0);
        printf ("Received Hello\n");
        // cout << "the last symbol is " << buffer[36] << endl;
        
        int msg_size = convertByteArrayToInt(buffer);

        tutorial::AddressBook address_book;


        if (!address_book.ParseFromArray(buffer+4, msg_size)) {
            cerr << "Failed to parse address book." << endl;
            // return -1;
        }

        ListPeople(address_book);

        sleep (1);     
    }


    return 0;
}