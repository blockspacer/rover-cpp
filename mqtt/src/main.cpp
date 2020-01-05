//
// Created by Ivan Kishchenko on 29/09/2019.
//

#include <protocol/Message.h>
#include "Client.h"

using namespace boost::asio;
using namespace boost::asio::ip;

int main(int argc, char *argv[]) {
    io_service ios;
    Client client(ios, "localhost", 1883, "/");

    ios.run();

    return 0;
}
