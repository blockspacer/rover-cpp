//
// Created by Ivan Kishchenko on 2019-07-11.
//

#ifndef ROVER_PROPERTIES_H
#define ROVER_PROPERTIES_H

#include <string>
#include <list>
#include <memory>

class Properties {
public:
    virtual void load(const std::string &filePath) = 0;

    virtual std::string getString(const std::string &name) const = 0;

    virtual bool getBoolean(const std::string &name) const = 0;

    virtual int getInt(const std::string &name) const = 0;

    virtual long getLong(const std::string &name) const = 0;

    virtual double getDouble(const std::string &name) const = 0;

    virtual std::list<std::string> getStringArray(const std::string &name) const = 0;

    virtual std::string getString(const std::string &name, const std::string &defValue) const {
        try {
            return this->getString(name);
        } catch (std::exception &ignore) {}

        return defValue;
    }

    virtual bool getBoolean(const std::string &name, bool defValue) const {
        try {
            return this->getBoolean(name);
        } catch (std::exception &ignore) {}

        return defValue;
    }

    virtual int getInt(const std::string &name, int defValue) const {
        try {
            return this->getInt(name);
        } catch (std::exception &ignore) {}

        return defValue;
    }

    virtual long getLong(const std::string &name, long defValue) {
        try {
            return this->getLong(name);
        } catch (std::exception &ignore) {}

        return defValue;
    }

    virtual double getDouble(const std::string &name, double defValue) const {
        try {
            return this->getDouble(name);
        } catch (std::exception &ignore) {}

        return defValue;
    }
};

typedef std::shared_ptr<Properties> SPProperties;

#endif //ROVER_PROPERTIES_H
