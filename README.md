# Rover 

Service based on Boost C++ Libraries (https://boost.org/)

* JSON-RPC Server (nlohmann/json, Boost.Beast, C++ 17)
* Logging (Boost.Log)
* macchina.io like IoT (https://macchina.io/) 
* conan (https://conan.io) for build on Mac OS (no pre-compiled libraries for arm/arm64)

## build
```bash
git submodule init
git submodule update
cd lib/json && cmake ./ && make && make install && cd ../..
mkdir -p build
# for Mac OS {
conan install . -s build_type=Debug --install-folder=build
#  } for Mac OS
cd build && cmake ../
make all CTEST_OUTPUT_ON_FAILURE=1 test

```
## json-rpc

### dto examples:
```c++
/**
 * hello params
 */
BEGIN_DECLARE_DTO(HelloParams)

__DECLARE_DTO_FIELDS1(std::string, name)
END_DECLARE_DTO

/**
 * hello response
 */
BEGIN_DECLARE_DTO(HelloResult)

__DECLARE_DTO_FIELDS1(std::string, message)
END_DECLARE_DTO

```

```c++
/**
 * hello params
 */
BEGIN_DECLARE_DTO(HelloParams)

__DECLARE_DTO_FIELD(std::string, name)
END_DECLARE_DTO

/**
 * hello response
 */
BEGIN_DECLARE_DTO(HelloResult)

__DECLARE_DTO_FIELD(std::string, message)
END_DECLARE_DTO

```

### methods examples

```c++

class HelloFunction : public RpcFunction<HelloParams, HelloResult> {
public:
    std::string name() const override {
        return "hello.function";
    }

    HelloResult exec(const HelloParams &params) override {
        HelloResult result;
        result.message = "Hello, " + params.name;

        return result;
    }
};

class HelloSupplier : public RpcSupplier<HelloResult> {
public:
    std::string name() const override {
        return "hello.supplier";
    }

    HelloResult exec() override {
        HelloResult result;
        result.message = "Hello World!";

        return result;
    }
};

class HelloConsumer : public RpcConsumer<HelloParams> {
public:
    std::string name() const override {
        return "hello.consumer";
    }

    void exec(const HelloParams &params) override {
        // TODO: make something
    }
};
```
