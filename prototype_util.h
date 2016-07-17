#ifndef OM_PROTOTYPE_UTIL
#define OM_PROTOTYPE_UTIL

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <string>

using namespace std;


inline void storeIntToByteArray(int val, char* buffer){
	buffer[0] = (char)((val >> 24) & 0xFF);
    buffer[1] = (char)((val >> 16) & 0xFF);
    buffer[2] = (char)((val >> 8)  & 0xFF);
    buffer[3] = (char)(val         & 0xFF);
}

inline int convertByteArrayToInt(char* buffer){
    int val = (int)((unsigned char)buffer[0] << 24 |
                  	(unsigned char)buffer[1] << 16 |
                  	(unsigned char)buffer[2] << 8  |
                  	(unsigned char)buffer[3]);

    return val;
}

inline int convertByteArrayToInt(string buffer){
    int val = (int)((unsigned char)buffer[0] << 24 |
                  	(unsigned char)buffer[1] << 16 |
                  	(unsigned char)buffer[2] << 8  |
                  	(unsigned char)buffer[3]);

    return val;
}

inline int getRandomInt(){
	// srand(time(NULL));
	return rand() % 10 + 1;
}

inline string getRandomString(int length){
	// srand(time(NULL));
	static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    string s = "";
    for (int i = 0; i < length; ++i) {
        s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    return s;
}


void ListPeople(const tutorial::AddressBook& address_book) {
  // cout << address_book.person_size() << endl;
  for (int i = 0; i < address_book.person_size(); i++) {
    const tutorial::Person& person = address_book.person(i);

    cout << "Person ID: " << person.id() << endl;
    cout << "  Name: " << person.name() << endl;
    if (person.has_email()) {
      cout << "  E-mail address: " << person.email() << endl;
    }

    for (int j = 0; j < person.phone_size(); j++) {
      const tutorial::Person::PhoneNumber& phone_number = person.phone(j);

      switch (phone_number.type()) {
        case tutorial::Person::MOBILE:
          cout << "  Mobile phone #: ";
          break;
        case tutorial::Person::HOME:
          cout << "  Home phone #: ";
          break;
        case tutorial::Person::WORK:
          cout << "  Work phone #: ";
          break;
      }
      cout << phone_number.number() << endl;
    }
  }
}

#endif