//
// Created by Ivan Kishchenko on 29/09/2019.
//

#ifndef ROVER_PROPERTIES_H
#define ROVER_PROPERTIES_H

#include <cstdint>

#define MQTT_INVALID_PROPERTY_ID -2

namespace mqtt {

/** The one byte MQTT V5 property indicator */
    enum PropertyCodes {
        MQTTPROPERTY_CODE_PAYLOAD_FORMAT_INDICATOR = 1,  /**< The value is 1 */
        MQTTPROPERTY_CODE_MESSAGE_EXPIRY_INTERVAL = 2,   /**< The value is 2 */
        MQTTPROPERTY_CODE_CONTENT_TYPE = 3,              /**< The value is 3 */
        MQTTPROPERTY_CODE_RESPONSE_TOPIC = 8,            /**< The value is 8 */
        MQTTPROPERTY_CODE_CORRELATION_DATA = 9,          /**< The value is 9 */
        MQTTPROPERTY_CODE_SUBSCRIPTION_IDENTIFIER = 11,  /**< The value is 11 */
        MQTTPROPERTY_CODE_SESSION_EXPIRY_INTERVAL = 17,  /**< The value is 17 */
        MQTTPROPERTY_CODE_ASSIGNED_CLIENT_IDENTIFER = 18,/**< The value is 18 */
        MQTTPROPERTY_CODE_SERVER_KEEP_ALIVE = 19,        /**< The value is 19 */
        MQTTPROPERTY_CODE_AUTHENTICATION_METHOD = 21,    /**< The value is 21 */
        MQTTPROPERTY_CODE_AUTHENTICATION_DATA = 22,      /**< The value is 22 */
        MQTTPROPERTY_CODE_REQUEST_PROBLEM_INFORMATION = 23,/**< The value is 23 */
        MQTTPROPERTY_CODE_WILL_DELAY_INTERVAL = 24,      /**< The value is 24 */
        MQTTPROPERTY_CODE_REQUEST_RESPONSE_INFORMATION = 25,/**< The value is 25 */
        MQTTPROPERTY_CODE_RESPONSE_INFORMATION = 26,     /**< The value is 26 */
        MQTTPROPERTY_CODE_SERVER_REFERENCE = 28,         /**< The value is 28 */
        MQTTPROPERTY_CODE_REASON_STRING = 31,            /**< The value is 31 */
        MQTTPROPERTY_CODE_RECEIVE_MAXIMUM = 33,          /**< The value is 33*/
        MQTTPROPERTY_CODE_TOPIC_ALIAS_MAXIMUM = 34,      /**< The value is 34 */
        MQTTPROPERTY_CODE_TOPIC_ALIAS = 35,              /**< The value is 35 */
        MQTTPROPERTY_CODE_MAXIMUM_QOS = 36,              /**< The value is 36 */
        MQTTPROPERTY_CODE_RETAIN_AVAILABLE = 37,         /**< The value is 37 */
        MQTTPROPERTY_CODE_USER_PROPERTY = 38,            /**< The value is 38 */
        MQTTPROPERTY_CODE_MAXIMUM_PACKET_SIZE = 39,      /**< The value is 39 */
        MQTTPROPERTY_CODE_WILDCARD_SUBSCRIPTION_AVAILABLE = 40,/**< The value is 40 */
        MQTTPROPERTY_CODE_SUBSCRIPTION_IDENTIFIERS_AVAILABLE = 41,/**< The value is 41 */
        MQTTPROPERTY_CODE_SHARED_SUBSCRIPTION_AVAILABLE = 42/**< The value is 241 */
    };

/**
 * Returns a printable string description of an MQTT V5 property code.
 * @param value an MQTT V5 property code.
 * @return the printable string description of the input property code.
 * NULL if the code was not found.
 */
    const char *PropertyName(enum PropertyCodes value);

/** The one byte MQTT V5 property type */
    enum PropertyTypes {
        MQTTPROPERTY_TYPE_BYTE,
        MQTTPROPERTY_TYPE_TWO_BYTE_INTEGER,
        MQTTPROPERTY_TYPE_FOUR_BYTE_INTEGER,
        MQTTPROPERTY_TYPE_VARIABLE_BYTE_INTEGER,
        MQTTPROPERTY_TYPE_BINARY_DATA,
        MQTTPROPERTY_TYPE_UTF_8_ENCODED_STRING,
        MQTTPROPERTY_TYPE_UTF_8_STRING_PAIR
    };

/**
 * Returns the MQTT V5 type code of an MQTT V5 property.
 * @param value an MQTT V5 property code.
 * @return the MQTT V5 type code of the input property. -1 if the code was not found.
 */
    int PropertyType(enum PropertyCodes value);

/**
 * The data for a length delimited string
 */
    struct LenString {
        int len; /**< the length of the string */
        char *data; /**< pointer to the string data */
    };


/**
 * Structure to hold an MQTT version 5 property of any type
 */
    struct Property {
         PropertyCodes identifier; /**<  The MQTT V5 property id. A multi-byte integer. */
        /** The value of the property, as a union of the different possible types. */
        union {
            uint8_t byte;       /**< holds the value of a byte property type */
            uint16_t integer2;  /**< holds the value of a 2 byte integer property type */
            uint32_t integer4;    /**< holds the value of a 4 byte integer property type */
            struct {
                LenString data;  /**< The value of a string property, or the name of a user property. */
                LenString value; /**< The value of a user property. */
            };
        } value;
    };

/**
 * MQTT version 5 property list
 */
//    struct Properties {
//        int count;     /**< number of property entries in the array */
//        int max_count; /**< max number of properties that the currently allocated array can store */
//        int length;    /**< mbi: byte length of all properties */
//        Property *array;  /**< array of properties */
//    };

    class Properties {
    private:

    public:
        void add(Property);
    };
}

#endif //ROVER_PROPERTIES_H
