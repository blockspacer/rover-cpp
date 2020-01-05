//
// Created by Kishchenko, Ivan on 9/5/19.
//

#include "ProtocolTest.h"

BOOST_FIXTURE_TEST_SUITE(ProtocolTest, ProtocolFixture)

    BOOST_AUTO_TEST_CASE(testInit) {
        std::string packet = "?INI";
        auto op = _manager.parse(packet);

        BOOST_REQUIRE_EQUAL("?INI", op->getOperation());
        BOOST_CHECK(dynamic_cast<protocol::Handshake *>(op.get()));
    }

    BOOST_AUTO_TEST_CASE(testMsg) {
        std::string packet = "?MSG Hello World";
        auto op = _manager.parse(packet);

        BOOST_REQUIRE_EQUAL("?MSG", op->getOperation());
        BOOST_CHECK(dynamic_cast<protocol::Message *>(op.get()));
        BOOST_REQUIRE_EQUAL(std::string("Hello World"), (dynamic_cast<protocol::Message *>(op.get()))->getContext());

        try {
            packet = "?MSG";
            op = _manager.parse(packet);
        } catch (std::invalid_argument &ex) {}
    }

    BOOST_AUTO_TEST_CASE(testSet) {
        std::string packet = "?SET 1:10";
        auto op = _manager.parse(packet);

        BOOST_REQUIRE_EQUAL("?SET", op->getOperation());
        BOOST_CHECK(dynamic_cast<protocol::SetPin *>(op.get()));
        BOOST_REQUIRE_EQUAL(1, (dynamic_cast<protocol::SetPin *>(op.get()))->getPin());
        BOOST_REQUIRE_EQUAL(10, (dynamic_cast<protocol::SetPin *>(op.get()))->getValue());
    }

    BOOST_AUTO_TEST_CASE(testMode) {
        std::string packet = "?MOD 1:10";
        auto op = _manager.parse(packet);

        BOOST_REQUIRE_EQUAL("?MOD", op->getOperation());
        BOOST_CHECK(dynamic_cast<protocol::ModePin *>(op.get()));
        BOOST_REQUIRE_EQUAL(1, (dynamic_cast<protocol::ModePin *>(op.get()))->getPin());
        BOOST_REQUIRE_EQUAL(10, (dynamic_cast<protocol::ModePin *>(op.get()))->getValue());
    }

    BOOST_AUTO_TEST_CASE(testPut) {
        std::string packet = "?PUT 1:0";
        auto op = _manager.parse(packet);

        BOOST_REQUIRE_EQUAL("?PUT", op->getOperation());
        BOOST_CHECK(dynamic_cast<protocol::PutPin *>(op.get()));
        BOOST_REQUIRE_EQUAL(1, (dynamic_cast<protocol::PutPin *>(op.get()))->getPin());
        BOOST_REQUIRE_EQUAL(false, (dynamic_cast<protocol::PutPin *>(op.get()))->getValue());
    }

    BOOST_AUTO_TEST_CASE(testGet) {
        std::string packet = "?GET 1:10";
        auto op = _manager.parse(packet);

        BOOST_REQUIRE_EQUAL("?GET", op->getOperation());
        BOOST_CHECK(dynamic_cast<protocol::GetPin *>(op.get()));
        BOOST_REQUIRE_EQUAL(1, (dynamic_cast<protocol::GetPin *>(op.get()))->getPin());
    }

BOOST_AUTO_TEST_SUITE_END()