//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-12.
//

#include "PropertiesContainer.h"
#include "PropertiesJson.h"

#include <boost/algorithm/string/predicate.hpp>
#include <exception/LogicException.h>

void PropertiesContainer::load(const std::string &filePath) {
    SPProperties properties;
    if (boost::algorithm::ends_with(filePath, ".json")) {
        properties = std::make_shared<PropertiesJson>();
    }

    if (properties) {
        properties->load(filePath);
        _properties.insert(properties);
    }
}

std::string PropertiesContainer::getString(const std::string &name) const {
    for (const auto &properies : _properties) {
        try {
            return properies->getString(name);
        } catch (std::exception &ignore) {}
    }

    throw NotFoundException("Property " + name + "not present");
}

bool PropertiesContainer::getBoolean(const std::string &name) const {
    for (const auto &properies : _properties) {
        try {
            return properies->getBoolean(name);
        } catch (std::exception &ignore) {}
    }

    throw NotFoundException("Property " + name + "not present");
}

int PropertiesContainer::getInt(const std::string &name) const {
    for (const auto &properies : _properties) {
        try {
            return properies->getInt(name);
        } catch (std::exception &ignore) {}
    }

    throw NotFoundException("Property " + name + "not present");
}

long PropertiesContainer::getLong(const std::string &name) const {
    for (const auto &properies : _properties) {
        try {
            return properies->getLong(name);
        } catch (std::exception &ignore) {}
    }

    throw NotFoundException("Property " + name + "not present");
}

double PropertiesContainer::getDouble(const std::string &name) const {
    for (const auto &properies : _properties) {
        try {
            return properies->getDouble(name);
        } catch (std::exception &ignore) {}
    }

    throw NotFoundException("Property " + name + "not present");
}

std::list<std::string> PropertiesContainer::getStringArray(const std::string &name) const {
    for (const auto &properies : _properties) {
        try {
            return properies->getStringArray(name);
        } catch (std::exception &ignore) {}
    }

    throw NotFoundException("Property " + name + "not present");
}
