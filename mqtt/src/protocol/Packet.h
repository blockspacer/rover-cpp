//
// Created by Ivan Kishchenko on 29/09/2019.
//

#ifndef ROVER_PACKET_H
#define ROVER_PACKET_H


#include "Properties.h"

namespace mqtt {
    struct Header {
        unsigned int type : 4;    /**< message type nibble */
        bool dup : 1;            /**< DUP flag bit */
        unsigned int qos : 2;    /**< QoS value, 0, 1 or 2 */
        bool retain : 1;
    };

    struct Packet {
        Header header;    /**< MQTT header byte */
    };

    struct Connect {
        Header header;    /**< MQTT header byte */
        struct Bits {
            bool username : 1;            /**< 3.1 user name */
            bool password : 1;            /**< 3.1 password */
            bool willRetain : 1;        /**< will retain setting */
            unsigned int willQoS : 2;    /**< will QoS value */
            bool will : 1;            /**< will flag */
            bool cleanStart : 1;    /**< cleansession flag */
            int : 1;    /**< unused */
        };

        Bits flags;

        char *Protocol;     /**< MQTT protocol name */
        char *clientID;        /**< string client id */
        char *willTopic;    /**< will topic */
        char *willMsg;        /**< will payload */

        int keepAliveTimer;        /**< keepalive timeout value in seconds */
        unsigned char version;    /**< MQTT version number */
    };

/**
 * Data for a suback packet.
 */
    struct Suback {
        Header header;    /**< MQTT header byte */
        int msgId;        /**< MQTT message id */
        int version;  /**< the version of MQTT */
        Properties properties; /**< MQTT 5.0 properties.  Not used for MQTT < 5.0 */
        List *qoss;        /**< list of granted QoSs (MQTT 3/4) / reason codes (MQTT 5) */
    } Suback;


/**
 * Data for an MQTT V5 unsuback packet.
 */
    struct Unsuback {
        Header header;    /**< MQTT header byte */
        int msgId;        /**< MQTT message id */
        int version;  /**< the version of MQTT */
        Properties properties; /**< MQTT 5.0 properties.  Not used for MQTT < 5.0 */
        List *reasonCodes;    /**< list of reason codes */
    }
}
#endif //ROVER_PACKET_H
