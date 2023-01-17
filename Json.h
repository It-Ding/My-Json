#pragma once

#include <fstream>
#include <map>
#include <string>
#include <vector>

namespace my_json {

    class Json {
    public:
        enum Type {
            json_null = 0,
            json_bool,
            json_int,
            json_double,
            json_string,
            json_array,
            json_object
        };

        Json();
        Json(Type type);
        Json(bool value);
        Json(int value);
        Json(double value);
        Json(const char *value);
        Json(std::string value);
        Json(const Json &other);
        ~Json();

        Type type() const;

        bool is_null() const;
        bool is_bool() const;
        bool is_int() const;
        bool is_double() const;
        bool is_string() const;
        bool is_array() const;
        bool is_object() const;

        bool get_bool() const;
        int get_int() const;
        double get_double() const;
        std::string get_string() const;

        int size() const;
        bool empty() const;
        void clear();

        std::string to_string() const;

        bool find(const char *key) const;
        bool find(const std::string &key) const;
        bool has_key(const char *key) const;
        bool has_key(const std::string &key) const;

        void push_back(const Json &value);
        void push_back(Json &&value);
        void push_front(const Json &value);
        void push_front(Json &&value);

        void erase(int index);
        void erase(const char *key);
        void erase(const std::string &key);

        Json &operator=(const Json &other);
        Json &operator=(Json &&other);
        bool operator==(const Json &other) const;
        bool operator!=(const Json &other) const;

        Json &operator[](int index);
        Json &operator[](const char *key);
        Json &operator[](const std::string &key);

        operator bool() const;
        operator int() const;
        operator double() const;
        operator std::string() const;
        operator std::vector<Json>() const;
        operator std::map<std::string, Json>() const;

        std::ostream &operator<<(std::ostream &os);

        void parse(const char *json);
        void parse(const std::string &json);
        void parse(const std::ifstream &file);

    private:
        void copy(const Json &other);

        union Value {
            bool data_bool;
            int data_int;
            double data_double;
            std::string *data_string;
            std::vector<Json> *data_array;
            std::map<std::string, Json> *data_object;
        };

        Type data_type;
        Value value;
    };

} // namespace my_json
