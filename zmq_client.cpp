//  Hello World client
#include <zmq.h>
#include <string.h>
#include <stdio.h>

#include <iostream>
#include <fstream>

#include <unistd.h>
#include "addressbook.pb.h"

#include <thread>
#include <queue>
#include <mutex>

#include "prototype_util.h"

using namespace std;
using namespace tutorial;


void initializePerson(Person* person) {
  person->set_id(323000);

  person->set_email("123@123.com");

  person->set_name("tom");

  Person::PhoneNumber* phone_number = person->add_phone();
  phone_number->set_number("1234567");
  phone_number->set_type(Person::MOBILE);
}

class OrderManager
{
public:

    void OM_Processing_Thread(){
        for(int i = 0; i < 1; i++){
            // enter the mutex
            reservations_mutex.lock();

            // DO THE WORK
            // tutorial::AddressBook* address_book = new AddressBook();

              {
                AddressBook* address_book = new tutorial::AddressBook;
                initializePerson(address_book->add_person());
                // push the element to the queue
                reservations.push(address_book);
              }


            // release the lock
            reservations_mutex.unlock();
        }
    }

    void OM_Serializer_Thread(){
        
        void *context = zmq_ctx_new ();
        void *requester = zmq_socket (context, ZMQ_PUSH);
        
        if(zmq_connect (requester, "ipc://helloMSS") == -1){
            cout << "zmq connection is down " << endl;
        }



        while(true){
            if(!reservations.empty()){
                reservations_mutex.lock();
                char buffer[1024];
                AddressBook* address_book;

                // get the value to process
                address_book = reservations.front();
                reservations.pop();


                int size;
                size = serializeToBuffer(address_book, buffer);

                // send the message
                if(zmq_send(requester, buffer, size, ZMQ_DONTWAIT) != size){
                    cout << "error detected " << endl;
                }
                else{
                    ListPeople(*address_book);
                }

                delete address_book;

                // release the mutex
                reservations_mutex.unlock();
            }
        }
        zmq_close (requester);
        zmq_ctx_destroy (context);
    }

    void startSystem(){
        std::thread t1(&OrderManager::OM_Processing_Thread, this);
        // std::thread t2(&OrderManager::OM_Processing_Thread, this);
        // std::thread t3(&OrderManager::OM_Processing_Thread, this);
        // std::thread t4(&OrderManager::OM_Processing_Thread, this);

        std::thread eater(&OrderManager::OM_Serializer_Thread, this);
        t1.join();
        // t2.join();
        // t3.join();
        // t4.join();
        eater.join();
    }

    OrderManager(){}

private:
    queue<tutorial::AddressBook*> reservations;
    mutex reservations_mutex;

    int serializeToBuffer(AddressBook* msg, char* buffer){
        int msg_size = msg->ByteSize();
        storeIntToByteArray(msg_size, buffer);
        msg->SerializeToArray(buffer+4, msg_size);
        cout << "the size is " << msg_size << endl;
        return msg_size+4;
    }
};

int main (void)
{
    srand(time(NULL));
    OrderManager om;
    om.startSystem();
    return 0;
}