//
// Created by Ivan Kishchenko on 03.02.2020.
//

#ifndef ROVER_CONFIGSOURCE_H
#define ROVER_CONFIGSOURCE_H

#include <nlohmann/json.hpp>
#include <string>
#include <fstream>

class ConfigSource {
private:
    nlohmann::json _json;
public:
    typedef std::shared_ptr<ConfigSource> Ptr;

    explicit ConfigSource(std::string_view path) {
        std::ifstream file(path.data());
        file >> _json;
    }
    template <class T> T get(std::string_view node = {}) {
        if (!node.empty()) {
            return _json[node.data()].get<T>();
        }

        return _json.get<T>();
    }
};


#endif //ROVER_CONFIGSOURCE_H
