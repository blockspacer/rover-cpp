//
// Created by Ivan Kishchenko on 2019-07-13.
//

#ifndef ROVER_BASE_H
#define ROVER_BASE_H

#include <nlohmann/json.hpp>
#include <boost/optional.hpp>
#include <string>

namespace json {

    typedef nlohmann::json tree;

    class Encoder {
    public:
        virtual std::string encode() = 0;

        virtual ~Encoder() = default;
    };

    class Decoder {
    public:
        virtual void decode(const std::string &json) = 0;

        virtual ~Decoder() = default;
    };

    class Marshaller {
    public:
        typedef std::shared_ptr<Marshaller> Ptr;
    public:
        virtual tree marshal() const = 0;

        virtual ~Marshaller() = default;
    };

    class UnMarshaller {
    public:
        typedef std::shared_ptr<UnMarshaller> Ptr;
    public:
        virtual void unMarshal(const tree &object) = 0;

        virtual void unMarshal(const boost::optional<tree> &object) {
            if (object) {
                unMarshal(object.value());
            }
        }

        virtual ~UnMarshaller() = default;
    };
}


#endif //ROVER_BASE_H
