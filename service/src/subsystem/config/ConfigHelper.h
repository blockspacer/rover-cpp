//
// Created by Ivan Kishchenko on 03.02.2020.
//

#ifndef ROVER_CONFIGHELPER_H
#define ROVER_CONFIGHELPER_H

#include <nlohmann/json.hpp>
#include <boost/optional.hpp>

namespace json_helper {
    template<class T>
    static bool get(const nlohmann::json &json, const char *key, T &value) {
        auto node = json.find(key);
        if (node != json.end()) {
            node->get_to(value);
            return true;
        }

        return false;
    }

    template<class T>
    static bool get(const nlohmann::json &json, const char *key, boost::optional<T> &value) {
        auto node = json.find(key);
        if (node != json.end()) {
            T tmpVal;
            node->get_to(tmpVal);
            value = tmpVal;
            return true;
        }

        return false;
    }

    template<class T>
    static void getOrDefault(const nlohmann::json &json, const char *key, T &value, const T &def) {
        if (!get(json, key, value)) {
            value = def;
        }
    }
}

#define BEGIN_DECLARE_CFG(T)                                                            \
class T {                                                                               \
public:                                                                                 \
typedef std::shared_ptr<T> Ptr;

#define END_DECLARE_CFG(T) };                                                           \
static void from_json(const nlohmann::json& j, T& cfg) {                                \
    cfg.fromJson(j);                                                                    \
}

#define __DECLARE_CFG_FIELDS1(cls1, name1)                                              \
public:                                                                                 \
    cls1 name1;                                                                         \
    void fromJson(const nlohmann::json& j) {                                            \
        json_helper::get(j, #name1, name1);                                             \
    }

#define DECLARE_CFG(T, cls1, name1)                                                     \
BEGIN_DECLARE_CFG(T)                                                                    \
__DECLARE_CFG_FIELDS1(cls1, name1)                                                      \
END_DECLARE_CFG(T)

#define __DECLARE_CFG_FIELDS2(cls1, name1, cls2, name2)                                 \
public:                                                                                 \
    cls1 name1;                                                                         \
    cls2 name2;                                                                         \
    void fromJson(const nlohmann::json& j) {                                            \
        json_helper::get(j, #name1, name1);                                             \
        json_helper::get(j, #name2, name2);                                             \
    }

#define __DECLARE_CFG_FIELDS3(cls1, name1, cls2, name2, cls3, name3)                    \
public:                                                                                 \
    cls1 name1;                                                                         \
    cls2 name2;                                                                         \
    cls3 name3;                                                                         \
    void fromJson(const nlohmann::json& j) {                                            \
        json_helper::get(j, #name1, name1);                                             \
        json_helper::get(j, #name2, name2);                                             \
        json_helper::get(j, #name3, name3);                                             \
    }

#define __DECLARE_CFG_FIELDS4(cls1, name1, cls2, name2, cls3, name3, cls4, name4)       \
public:                                                                                 \
    cls1 name1;                                                                         \
    cls2 name2;                                                                         \
    cls3 name3;                                                                         \
    cls4 name4;                                                                         \
    void fromJson(const nlohmann::json& j) {                                            \
        json_helper::get(j, #name1, name1);                                             \
        json_helper::get(j, #name2, name2);                                             \
        json_helper::get(j, #name3, name3);                                             \
        json_helper::get(j, #name4, name4);                                             \
    }

#define __DECLARE_CFG_FIELDS5(cls1, name1, cls2, name2, cls3, name3, cls4, name4, cls5, name5)       \
public:                                                                                 \
    cls1 name1;                                                                         \
    cls2 name2;                                                                         \
    cls3 name3;                                                                         \
    cls4 name4;                                                                         \
    cls5 name5;                                                                         \
    void fromJson(const nlohmann::json& j) {                                            \
        json_helper::get(j, #name1, name1);                                             \
        json_helper::get(j, #name2, name2);                                             \
        json_helper::get(j, #name3, name3);                                             \
        json_helper::get(j, #name4, name4);                                             \
        json_helper::get(j, #name5, name5);                                             \
    }

#define __DECLARE_CFG_FIELDS6(cls1, name1, cls2, name2, cls3, name3, cls4, name4, cls5, name5, cls6, name6)       \
public:                                                                                 \
    cls1 name1;                                                                         \
    cls2 name2;                                                                         \
    cls3 name3;                                                                         \
    cls4 name4;                                                                         \
    cls5 name5;                                                                         \
    cls6 name6;                                                                         \
    void fromJson(const nlohmann::json& j) {                                            \
        json_helper::get(j, #name1, name1);                                             \
        json_helper::get(j, #name2, name2);                                             \
        json_helper::get(j, #name3, name3);                                             \
        json_helper::get(j, #name4, name4);                                             \
        json_helper::get(j, #name5, name5);                                             \
        json_helper::get(j, #name6, name6);                                             \
    }

#define __DECLARE_CFG_FIELDS7(cls1, name1, cls2, name2, cls3, name3, cls4, name4, cls5, name5, cls6, name6, cls7, name7)       \
public:                                                                                 \
    cls1 name1;                                                                         \
    cls2 name2;                                                                         \
    cls3 name3;                                                                         \
    cls4 name4;                                                                         \
    cls5 name5;                                                                         \
    cls6 name6;                                                                         \
    cls7 name7;                                                                         \
    void fromJson(const nlohmann::json& j) {                                            \
        json_helper::get(j, #name1, name1);                                             \
        json_helper::get(j, #name2, name2);                                             \
        json_helper::get(j, #name3, name3);                                             \
        json_helper::get(j, #name4, name4);                                             \
        json_helper::get(j, #name5, name5);                                             \
        json_helper::get(j, #name6, name6);                                             \
        json_helper::get(j, #name7, name7);                                             \
    }

#define __DECLARE_CFG_FIELDS8(cls1, name1, cls2, name2, cls3, name3, cls4, name4, cls5, name5, cls6, name6, cls7, name7, cls8, name8)       \
public:                                                                                 \
    cls1 name1;                                                                         \
    cls2 name2;                                                                         \
    cls3 name3;                                                                         \
    cls4 name4;                                                                         \
    cls5 name5;                                                                         \
    cls6 name6;                                                                         \
    cls7 name7;                                                                         \
    cls8 name8;                                                                         \
    void fromJson(const nlohmann::json& j) {                                            \
        json_helper::get(j, #name1, name1);                                             \
        json_helper::get(j, #name2, name2);                                             \
        json_helper::get(j, #name3, name3);                                             \
        json_helper::get(j, #name4, name4);                                             \
        json_helper::get(j, #name5, name5);                                             \
        json_helper::get(j, #name6, name6);                                             \
        json_helper::get(j, #name7, name7);                                             \
        json_helper::get(j, #name8, name8);                                             \
    }

#define __DECLARE_CFG_FIELDS9(cls1, name1, cls2, name2, cls3, name3, cls4, name4, cls5, name5, cls6, name6, cls7, name7, cls8, name8, cls9, name9)       \
public:                                                                                 \
    cls1 name1;                                                                         \
    cls2 name2;                                                                         \
    cls3 name3;                                                                         \
    cls4 name4;                                                                         \
    cls5 name5;                                                                         \
    cls6 name6;                                                                         \
    cls7 name7;                                                                         \
    cls8 name8;                                                                         \
    cls9 name9;                                                                         \
    void fromJson(const nlohmann::json& j) {                                            \
        json_helper::get(j, #name1, name1);                                             \
        json_helper::get(j, #name2, name2);                                             \
        json_helper::get(j, #name3, name3);                                             \
        json_helper::get(j, #name4, name4);                                             \
        json_helper::get(j, #name5, name5);                                             \
        json_helper::get(j, #name6, name6);                                             \
        json_helper::get(j, #name7, name7);                                             \
        json_helper::get(j, #name8, name8);                                             \
        json_helper::get(j, #name9, name9);                                             \
    }

#define __DECLARE_CFG_FIELDS10(cls1, name1, cls2, name2, cls3, name3, cls4, name4, cls5, name5, cls6, name6, cls7, name7, cls8, name8, cls9, name9, cls10, name10)       \
public:                                                                                 \
    cls1 name1;                                                                         \
    cls2 name2;                                                                         \
    cls3 name3;                                                                         \
    cls4 name4;                                                                         \
    cls5 name5;                                                                         \
    cls6 name6;                                                                         \
    cls7 name7;                                                                         \
    cls8 name8;                                                                         \
    cls9 name9;                                                                         \
    cls10 name10;                                                                       \
    void fromJson(const nlohmann::json& j) {                                            \
        json_helper::get(j, #name1, name1);                                             \
        json_helper::get(j, #name2, name2);                                             \
        json_helper::get(j, #name3, name3);                                             \
        json_helper::get(j, #name4, name4);                                             \
        json_helper::get(j, #name5, name5);                                             \
        json_helper::get(j, #name6, name6);                                             \
        json_helper::get(j, #name7, name7);                                             \
        json_helper::get(j, #name8, name8);                                             \
        json_helper::get(j, #name9, name9);                                             \
        json_helper::get(j, #name10, name10);                                           \
    }

#endif //ROVER_CONFIGHELPER_H
