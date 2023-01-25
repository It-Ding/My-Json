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
        Json(std::vector<Json> value);
        Json(std::map<std::string, Json> value);
        Json(const Json &other);
        Json(Json &&other);
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
        std::vector<Json> get_array() const;
        std::map<std::string, Json> get_object() const;

        int size() const;
        bool empty() const;
        void clear();

        std::string to_string() const;

        bool find(const char *key) const;
        bool find(const std::string &key) const;
        bool has_key(const char *key) const;
        bool has_key(const std::string &key) const;

        void push_back(const Json &value);
        void push_front(const Json &value);

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

        // 请暂时不要使用这个函数，我无法保证它的正确性
        // 如需输出请使用 to_string() 函数
        // friend std::ostream &operator<<(std::ostream &os, const Json &json);

        void parse(const char *json);
        void parse(const std::string &json);
        void parse(std::ifstream &file);

    private:
        class Parser {
        public:
            Parser() : index(0){};
            Parser(const std::string &json) : json(json), index(0){};
            ~Parser(){};

            void set_json(const std::string &json) {
                this->json = json;
                index = 0;
            }

            Json parse() {
                this->skip_space();
                char ch = this->get_next();
                switch (ch) {
                case 'n':
                    index--;
                    return this->check_null();
                case 't':
                case 'f':
                    index--;
                    return this->check_bool();
                case '"':
                    return this->check_string();
                case '[':
                    return this->check_array();
                case '{':
                    return this->check_object();
                default:
                    if (ch >= '0' && ch <= '9' || ch == '-') {
                        index--;
                        return this->check_number();
                    } else
                        throw std::logic_error("Unexpected character");
                }
            }

        private:
            void skip_space() {
                while (json[index] == ' ' || json[index] == '\t' || json[index] == '\r' || json[index] == '\n')
                    index++;
            }

            char get_next() {
                this->skip_space();
                if (index == json.size())
                    throw std::runtime_error("Unexpected end of json");
                return json[index++];
            }

            Json check_null() {
                if (json.substr(index, 4) == "null") {
                    index += 4;
                    return Json();
                }
                throw std::logic_error("Unexpected character");
            }

            Json check_bool() {
                if (json.substr(index, 4) == "true") {
                    index += 4;
                    return Json(true);
                }
                if (json.substr(index, 5) == "false") {
                    index += 5;
                    return Json(false);
                }
                throw std::logic_error("Unexpected character");
            }

            Json check_number() {
                int start = index;
                if (json[index] == '-')
                    index++;
                if (json[index] == '0')
                    index++;
                else if (json[index] >= '1' && json[index] <= '9') {
                    index++;
                    while (json[index] >= '0' && json[index] <= '9')
                        index++;
                } else
                    throw std::logic_error("Unexpected character");
                if (json[index] == '.') {
                    index++;
                    if (json[index] >= '0' && json[index] <= '9') {
                        index++;
                        while (json[index] >= '0' && json[index] <= '9')
                            index++;
                    } else
                        throw std::logic_error("Unexpected character");
                }
                if (json[index] == 'e' || json[index] == 'E') {
                    index++;
                    if (json[index] == '+' || json[index] == '-')
                        index++;
                    if (json[index] >= '0' && json[index] <= '9') {
                        index++;
                        while (json[index] >= '0' && json[index] <= '9')
                            index++;
                    } else
                        throw std::logic_error("Unexpected character");
                }
                std::string number = json.substr(start, index - start);
                if (number.find('.') != std::string::npos || number.find('e') != std::string::npos || number.find('E') != std::string::npos)
                    return Json(std::stod(number));
                else
                    return Json(std::stoi(number));
            }

            std::string check_string() {
                std::string str;
                while (true) {
                    char ch = this->get_next();
                    if (ch == '\\') {
                        ch = this->get_next();
                        switch (ch) {
                        case '"':
                            str += '"';
                            break;
                        case '\\':
                            str += '\\';
                            break;
                        case '/':
                            str += '/';
                            break;
                        case 'b':
                            str += '\b';
                            break;
                        case 'f':
                            str += '\f';
                            break;
                        case 'n':
                            str += '\n';
                            break;
                        case 'r':
                            str += '\r';
                            break;
                        case 't':
                            str += '\t';
                            break;
                        case 'u':
                            index += 4;
                            break;
                        default:
                            throw std::logic_error("Unexpected character");
                        }
                    } else if (ch == '"')
                        return str;
                    else
                        str += ch;
                }
            }

            Json check_array() {
                Json array = Json(json_array);
                while (true) {
                    this->skip_space();
                    if (json[index] == ']') {
                        index++;
                        return array;
                    }
                    array.push_back(this->parse());
                    this->skip_space();
                    if (json[index] == ']') {
                        index++;
                        return array;
                    }
                    if (json[index] == ',')
                        index++;
                    else
                        throw std::logic_error("Unexpected character");
                }
            }

            Json check_object() {
                Json object = Json(json_object);
                while (true) {
                    this->skip_space();
                    if (json[index] == '}') {
                        index++;
                        return object;
                    }
                    this->get_next();
                    std::string key = this->check_string();
                    this->skip_space();
                    if (json[index] == ':')
                        index++;
                    else
                        throw std::logic_error("Unexpected character");
                    object[key] = this->parse();
                    this->skip_space();
                    if (json[index] == '}') {
                        index++;
                        return object;
                    }
                    if (json[index] == ',')
                        index++;
                    else
                        throw std::logic_error("Unexpected character");
                }
            }

            std::string json;
            int index;
        };

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
