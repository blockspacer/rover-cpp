//
// Created by Ivan Kishchenko on 2019-07-13.
//

#ifndef ROVER_HELPER_H
#define ROVER_HELPER_H

#include "Base.h"
#include <memory>

#pragma GCC system_header

using namespace json;

class JsonEncoder : public Encoder {
private:
    Marshaller::Ptr _marshaller;
public:
    template<class T>
    explicit JsonEncoder(std::shared_ptr<T> marshaller) {
        _marshaller = marshaller;
    }

    std::string encode() override;
};

class JsonDecoder : public Decoder {
private:
    UnMarshaller::Ptr _unMarshaller;
public:
    JsonDecoder(const JsonDecoder &copy) = delete;

    JsonDecoder() = delete;

    template<class T>
    explicit JsonDecoder(std::shared_ptr<T> &unMarshaller) {
        _unMarshaller = unMarshaller;
    }

    void decode(const std::string &json) override;
};

static void unMarshal(const tree &ptr, const std::string &tag, boost::optional<tree> &value) {
    auto iter = ptr.find(tag);
    if (iter != ptr.end()) {
        value = *iter;
    }
}

static void unMarshal(const tree &ptr, const std::string &tag, tree &value) {
    value = ptr[tag];
}

template<typename T>
static void unMarshal(const tree &ptr, const std::string &tag, boost::optional<T> &value) {
    if constexpr (std::is_base_of<UnMarshaller, T>::value) {
        T opt;
        opt.unMarshal(ptr[tag]);
        value = opt;
    } else {
        value = ptr[tag];
    }
}

template<typename T>
static void unMarshal(const tree &ptr, const std::string &tag, T &value) {
    if constexpr (std::is_base_of<UnMarshaller, T>::value) {
        value.unMarshal(ptr[tag]);
    } else {
        value = ptr[tag];
    }
}

template<typename T>
static void unMarshal(const tree &ptr, const std::string &tag, std::vector<T> &value) {
}


static void marshal(tree &ptr, const std::string &tag, const tree &value) {
    ptr[tag] = value;
}

template<typename T>
static void marshal(tree &ptr, const std::string &tag, const std::vector<T> &value) {
    tree arr;
    for (const auto &item : value) {
        if constexpr (std::is_base_of<Marshaller, T>::value) {
            arr.push_back(item.marshal());
        } else {
            arr.push_back(value);
        }
    }
    ptr[tag] = arr;
}

template<typename T>
static void marshal(tree &ptr, const std::string &tag, const T &value) {
    if constexpr (std::is_base_of<boost::optional<T>, T>::value) {
        if (value) {
            marshal(value.value());
        }
    } else if constexpr (std::is_base_of<Marshaller, T>::value) {
        ptr[tag] = value.marshal();
    } else {
        ptr[tag] = value;
    }
}

static void marshal(tree &ptr, const std::string &tag, const Marshaller &value) {
    ptr[tag] = value.marshal();
}

template<typename T>
static void marshal(tree &ptr, const std::string &tag, const boost::optional<T> &value) {
    if (value) {
        marshal(ptr, tag, value.value());
    }
}

static boost::optional<tree> marshal(const Marshaller &value) {
    return value.marshal();
}

template<typename T>
static boost::optional<tree> marshal(const T &value) {
    if constexpr (std::is_base_of<boost::optional<T>, T>::value) {
        if (value) {
            return marshal(value.value());
        }
    } else if constexpr (std::is_base_of<Marshaller, T>::value) {
        return value.marshal();
    } else {
        return tree(value);
    }

    return boost::optional<tree>();
}

template<typename T>
static boost::optional<tree> marshal(const std::vector<T> &value) {
    tree res;
    for (const auto &item : value) {
        res.push_back(marshal(item).value());
    }

    return res;
}

template<typename T>
static T unMarshal(const tree &value) {
    if constexpr (std::is_base_of<UnMarshaller, T>::value) {
        T inc;
        inc.unMarshal(value);
        return inc;
    } else {
        return value.get<T>();
    }
}

#define BEGIN_JSON_UNMARSHAL                                                                        \
template <typename Owner>                                                                           \
class JsonUnMarshaller : public UnMarshaller {                                                      \
    private:                                                                                        \
        Owner* _obj;                                                                                \
    public:                                                                                         \
        explicit JsonUnMarshaller(Owner* obj) : _obj(obj) { }                                       \
        virtual void unMarshal(const tree &ptr) override {
#define ITEM_JSON_UNMARSHAL(name) ::unMarshal(ptr, #name, _obj->name);
#define ITEM_JSON_UNMARSHAL_TAG(tag, name) ::unMarshal(ptr, #tag, _obj->name);
#define END_JSON_UNMARSHAL }                                                                    \
    };                                                                                          \
    typedef JsonUnMarshaller<Owner> OwnerUnMarshaller;                                          \
    virtual void unMarshal(const tree& object) override {                                       \
        OwnerUnMarshaller unMarshaller(this);                                                   \
        unMarshaller.unMarshal(object);                                                         \
    }

#define BEGIN_JSON_MARSHAL                                                              \
template <typename Owner>                                                               \
class JsonMarshaller : public Marshaller  {                                             \
    private:                                                                            \
        const Owner* _obj;                                                              \
    public:                                                                             \
        explicit JsonMarshaller(const Owner* obj) : _obj(obj) { }                       \
        virtual tree marshal() const override {                                         \
            tree obj;
#define ITEM_JSON_MARSHAL(name) ::marshal(obj, #name, _obj->name);
#define ITEM_JSON_MARSHAL_TAG(tag, name) ::marshal(obj, #tag, _obj->name);
#define END_JSON_MARSHAL std::string t = obj.dump(); \
            return obj;                                                    \
        }                                                                               \
    };                                                                                  \
public:                                                                                 \
    typedef JsonMarshaller<Owner> OwnerMarshaller;                                      \
    virtual tree marshal() const override {                                             \
        OwnerMarshaller marshaller(this);                                               \
        return marshaller.marshal();                                                    \
    }

#define BEGIN_DECLARE_DTO(T)                                                            \
class T : public Marshaller, public UnMarshaller {                                      \
typedef T Owner;                                                                        \
typedef std::shared_ptr<T> Ptr;                                                         \
public:

#define END_DECLARE_DTO };

#define __DECLARE_DTO_FIELD(cls, name)                                                  \
public: cls name;

#define __DECLARE_DTO_FIELDS1(cls, name)                                                \
public: cls name;                                                                       \
BEGIN_JSON_MARSHAL                                                                      \
    ITEM_JSON_MARSHAL(name)                                                             \
END_JSON_MARSHAL                                                                        \
BEGIN_JSON_UNMARSHAL                                                                    \
    ITEM_JSON_UNMARSHAL(name)                                                           \
END_JSON_UNMARSHAL

#define __DECLARE_DTO_FIELDS2(cls1, name1, cls2, name2)                                 \
public: cls1 name1;                                                                     \
public: cls2 name2;                                                                     \
BEGIN_JSON_MARSHAL                                                                      \
    ITEM_JSON_MARSHAL(name1)                                                            \
    ITEM_JSON_MARSHAL(name2)                                                            \
END_JSON_MARSHAL                                                                        \
BEGIN_JSON_UNMARSHAL                                                                    \
    ITEM_JSON_UNMARSHAL(name1)                                                          \
    ITEM_JSON_UNMARSHAL(name2)                                                          \
END_JSON_UNMARSHAL

#define __DECLARE_DTO_FIELDS3(cls1, name1, cls2, name2, cls3, name3)                    \
public: cls1 name1;                                                                     \
public: cls2 name2;                                                                     \
public: cls3 name3;                                                                     \
BEGIN_JSON_MARSHAL                                                                      \
    ITEM_JSON_MARSHAL(name1)                                                            \
    ITEM_JSON_MARSHAL(name2)                                                            \
    ITEM_JSON_MARSHAL(name3)                                                            \
END_JSON_MARSHAL                                                                        \
BEGIN_JSON_UNMARSHAL                                                                    \
    ITEM_JSON_UNMARSHAL(name1)                                                          \
    ITEM_JSON_UNMARSHAL(name2)                                                          \
    ITEM_JSON_UNMARSHAL(name3)                                                          \
END_JSON_UNMARSHAL

#define __DECLARE_DTO_FIELDS4(cls1, name1, cls2, name2, cls3, name3, cls4, name4)       \
public: cls1 name1;                                                                     \
public: cls2 name2;                                                                     \
public: cls3 name3;                                                                     \
public: cls4 name4;                                                                     \
BEGIN_JSON_MARSHAL                                                                      \
    ITEM_JSON_MARSHAL(name1)                                                            \
    ITEM_JSON_MARSHAL(name2)                                                            \
    ITEM_JSON_MARSHAL(name3)                                                            \
    ITEM_JSON_MARSHAL(name4)                                                            \
END_JSON_MARSHAL                                                                        \
BEGIN_JSON_UNMARSHAL                                                                    \
    ITEM_JSON_UNMARSHAL(name1)                                                          \
    ITEM_JSON_UNMARSHAL(name2)                                                          \
    ITEM_JSON_UNMARSHAL(name3)                                                          \
    ITEM_JSON_UNMARSHAL(name4)                                                          \
END_JSON_UNMARSHAL

#define __DECLARE_DTO_FIELDS5(cls1, name1, cls2, name2, cls3, name3, cls4, name4, cls5, name5)       \
public: cls1 name1;                                                                     \
public: cls2 name2;                                                                     \
public: cls3 name3;                                                                     \
public: cls4 name4;                                                                     \
public: cls5 name5;                                                                     \
BEGIN_JSON_MARSHAL                                                                      \
    ITEM_JSON_MARSHAL(name1)                                                            \
    ITEM_JSON_MARSHAL(name2)                                                            \
    ITEM_JSON_MARSHAL(name3)                                                            \
    ITEM_JSON_MARSHAL(name4)                                                            \
    ITEM_JSON_MARSHAL(name5)                                                            \
END_JSON_MARSHAL                                                                        \
BEGIN_JSON_UNMARSHAL                                                                    \
    ITEM_JSON_UNMARSHAL(name1)                                                          \
    ITEM_JSON_UNMARSHAL(name2)                                                          \
    ITEM_JSON_UNMARSHAL(name3)                                                          \
    ITEM_JSON_UNMARSHAL(name4)                                                          \
    ITEM_JSON_UNMARSHAL(name5)                                                          \
END_JSON_UNMARSHAL

#define __DECLARE_DTO_FIELDS6(cls1, name1, cls2, name2, cls3, name3, cls4, name4, cls5, name5, cls6, name6)       \
public: cls1 name1;                                                                     \
public: cls2 name2;                                                                     \
public: cls3 name3;                                                                     \
public: cls4 name4;                                                                     \
public: cls5 name5;                                                                     \
public: cls6 name6;                                                                     \
BEGIN_JSON_MARSHAL                                                                      \
    ITEM_JSON_MARSHAL(name1)                                                            \
    ITEM_JSON_MARSHAL(name2)                                                            \
    ITEM_JSON_MARSHAL(name3)                                                            \
    ITEM_JSON_MARSHAL(name4)                                                            \
    ITEM_JSON_MARSHAL(name5)                                                            \
    ITEM_JSON_MARSHAL(name6)                                                            \
END_JSON_MARSHAL                                                                        \
BEGIN_JSON_UNMARSHAL                                                                    \
    ITEM_JSON_UNMARSHAL(name1)                                                          \
    ITEM_JSON_UNMARSHAL(name2)                                                          \
    ITEM_JSON_UNMARSHAL(name3)                                                          \
    ITEM_JSON_UNMARSHAL(name4)                                                          \
    ITEM_JSON_UNMARSHAL(name5)                                                          \
    ITEM_JSON_UNMARSHAL(name6)                                                          \
END_JSON_UNMARSHAL

#endif //ROVER_HELPER_H
