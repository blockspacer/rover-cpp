//
// Created by Ivan Kishchenko on 18.11.2019.
//

#ifndef ROVER_PROTOCOL_H
#define ROVER_PROTOCOL_H

#include <ostream>
#include <istream>

#define MQTT_INVALID_INT_VALUE -1;

namespace mqtt {
    class Protocol {
    public:
        static size_t packByte(std::ostream &stream, uint8_t val) {
            stream.write((char *) &val, sizeof(uint8_t));

            return 1;
        }

        static uint8_t unPackByte(std::istream &stream) {
            uint8_t res = 0;
            if (stream.read((char *) &res, sizeof(uint8_t)).eof()) {
                throw;
            };

            return res;
        }

        static uint16_t unPackWord(std::istream &stream) {
            uint16_t res = unPackByte(stream) * 0x100;
            res += unPackByte(stream);

            return res;
        }

        static std::string unPackString(std::istream &stream) {
            std::string res;
            res.resize(unPackWord(stream));

            stream.read(res.data(), res.size());

            return res;
        }

        static size_t packWord(std::ostream &stream, uint16_t val) {
            int res = packByte(stream, val / 0x100);
            res += packByte(stream, (uint8_t) val);

            return res;
        }

        static size_t packString(std::ostream &stream, const std::string &str) {
            size_t ret = packWord(stream, (uint16_t) str.size());
            stream.write(str.data(), str.length());

            return ret + str.length();
        }

        static size_t packVariableInt(std::ostream &stream, int val) {
            size_t res = 0;
            do {
                uint8_t encoded = val % 0x80;
                val /= 0x80;
                if (val > 0) {
                    encoded |= 0x80;
                }

                res += packByte(stream, encoded);
            } while (val > 0);

            return res;
        }

        static int unPackVariableInt(std::istream &stream) {
            int multiplier = 1;
            int result = 0;

            uint8_t encoded = 0;
            do {
                encoded = unPackByte(stream);
                result += (encoded & 0x7F) * multiplier;
                if (multiplier > 0x80 * 0x80 * 0x80) {
                    return MQTT_INVALID_INT_VALUE;
                }
                multiplier *= 0x80;
            } while ((encoded & 0x80) != 0);

            return result;
        }
    };
}

#endif //ROVER_PROTOCOL_H
