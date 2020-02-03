//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-12.
//

#ifndef ROVER_APPLICATION_H
#define ROVER_APPLICATION_H

#include <vector>
#include <memory>
#include <string>

#include <Logger.h>
#include <subsystem/config/ConfigSource.h>

#include "Subsystem.h"
#include "exception/LogicException.h"
#include "properties/Properties.h"

class Application : public Subsystem {
public:
    typedef std::vector<Subsystem::Ptr> SubSystemVec;

    Application();

    const char *name() const override;

    void postConstruct(Application &app) override;

    virtual int run(int argc, char *argv[]);

    virtual void run();

    void preDestroy() override;

    Logger &logger();

    Logger::Ptr loggerPtr();

    void addSubsystem(std::shared_ptr<Subsystem> spSubSystem) {
        _spSubsystems.push_back(spSubSystem);
    }

    const SubSystemVec &getSubsystems() {
        return _spSubsystems;
    }

    template<class C>
    C &getSubsystem() {
        for (auto &_spSubsystem : _spSubsystems) {
            const Subsystem *pSS(_spSubsystem.get());
            const C *pC = dynamic_cast<const C *>(pSS);
            if (pC) return *const_cast<C *>(pC);
        }
        throw NotFoundException(std::string("The subsystem has not been registered ") + typeid(C).name());
    }

    SPProperties getProperties() {
        return _properties;
    }

    ~Application() override;

    const ConfigSource::Ptr &getConfigSource() const {
        return _configSource;
    }

private:
    SubSystemVec _spSubsystems;

    SPProperties _properties;

    ConfigSource::Ptr _configSource;

    Logger::Ptr _logger;
};


#endif //ROVER_APPLICATION_H
