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

        Json() : data_type(json_null) {}

        Json(Type type) : data_type(type) {
            switch (data_type) {
            case json_string:
                value.data_string = new std::string();
                break;
            case json_array:
                value.data_array = new std::vector<Json>();
                break;
            case json_object:
                value.data_object = new std::map<std::string, Json>();
                break;
            default:
                break;
            }
        }

        Json(bool value) : data_type(json_bool) {
            this->value.data_bool = value;
        }

        Json(int value) : data_type(json_int) {
            this->value.data_int = value;
        }

        Json(double value) : data_type(json_double) {
            this->value.data_double = value;
        }

        Json(const char *value) : data_type(json_string) {
            this->value.data_string = new std::string(value);
        }

        Json(std::string value) : data_type(json_string) {
            this->value.data_string = new std::string(value);
        }

        Json(std::vector<Json> value) : data_type(json_array) {
            this->value.data_array = new std::vector<Json>(value);
        }

        Json(std::map<std::string, Json> value) : data_type(json_object) {
            this->value.data_object = new std::map<std::string, Json>(value);
        }

        Json(const Json &other) {
            this->copy(other);
        }

        Json(Json &&other) {
            this->data_type = other.data_type;
            this->value = other.value;
            other.data_type = json_null;
        }

        ~Json() {
            this->clear();
        }

        Type type() const {
            return data_type;
        }

        bool is_null() const {
            return data_type == json_null;
        }

        bool is_bool() const {
            return data_type == json_bool;
        }

        bool is_int() const {
            return data_type == json_int;
        }

        bool is_double() const {
            return data_type == json_double;
        }

        bool is_string() const {
            return data_type == json_string;
        }

        bool is_array() const {
            return data_type == json_array;
        }

        bool is_object() const {
            return data_type == json_object;
        }

        bool get_bool() const {
            if (this->is_bool())
                return value.data_bool;
            throw std::logic_error("function Json::get_bool: type error");
        }

        int get_int() const {
            if (this->is_int())
                return value.data_int;
            throw std::logic_error("function Json::get_int: type error");
        }

        double get_double() const {
            if (this->is_double())
                return value.data_double;
            throw std::logic_error("function Json::get_double: type error");
        }

        std::string get_string() const {
            if (this->is_string())
                return *value.data_string;
            throw std::logic_error("function Json::get_string: type error");
        }

        std::vector<Json> get_array() const {
            if (this->is_array())
                return *value.data_array;
            throw std::logic_error("function Json::get_array: type error");
        }

        std::map<std::string, Json> get_object() const {
            if (this->is_object())
                return *value.data_object;
            throw std::logic_error("function Json::get_object: type error");
        }

        int size() const {
            switch (data_type) {
            case json_array:
                return this->value.data_array->size();
            case json_object:
                return this->value.data_object->size();
            default:
                break;
            }
            throw std::logic_error("function Json::size: this json object is not array or map,cam't get size");
        }

        bool empty() const {
            switch (data_type) {
            case json_null:
                return true;
            case json_array:
                return value.data_array->empty();
            case json_object:
                return value.data_object->empty();
            default:
                break;
            }
            throw std::logic_error("function Json::empty: type error");
        }

        void clear() {
            switch (data_type) {
            case json_string:
                delete value.data_string;
                break;
            case json_array:
                for (auto i : *value.data_array)
                    i.clear();
                delete value.data_array;
                break;
            case json_object:
                for (auto i : *value.data_object)
                    i.second.clear();
                delete value.data_object;
                break;
            default:
                break;
            }
            data_type = json_null;
        }

        std::string to_string() const {
            std::string str;
            switch (data_type) {
            case json_null:
                str = "null";
                break;
            case json_bool:
                str = value.data_bool ? "true" : "false";
                break;
            case json_int:
                str = std::to_string(value.data_int);
                break;
            case json_double:
                str = std::to_string(value.data_double);
                break;
            case json_string:
                str = "\"" + *value.data_string + "\"";
                break;
            case json_array:
                str = "[";
                for (auto i : *value.data_array)
                    str += i.to_string() + ",";
                if (!value.data_array->empty())
                    str.pop_back();
                str += "]";
                break;
            case json_object:
                str = "{";
                for (auto i : *value.data_object)
                    str += "\"" + i.first + "\":" + i.second.to_string() + ",";
                if (!value.data_object->empty())
                    str.pop_back();
                str += "}";
                break;
            default:
                break;
            }
            return str;
        }

        bool find(const char *key) const {
            return this->has_key(key);
        }

        bool find(const std::string &key) const {
            return this->has_key(key);
        }

        bool has_key(const char *key) const {
            std::string str(key);
            if (this->is_object())
                return value.data_object->find(str) != value.data_object->end();
            throw std::logic_error("function Json::has_key: type error");
        }

        bool has_key(const std::string &key) const {
            if (this->is_object())
                return value.data_object->find(key) != value.data_object->end();
            throw std::logic_error("function Json::has_key: type error");
        }

        void push_back(const Json &value) {
            if (this->is_array())
                this->value.data_array->push_back(value);
            else if (this->is_null()) {
                this->data_type = json_array;
                this->value.data_array = new std::vector<Json>();
                this->value.data_array->push_back(value);
            } else
                throw std::logic_error("function Json::push_back: This object has been defined as another type, if you want to force changes to the properties of this object, call the clear() function first");
        }

        void push_back(Json &&value) {
            if (this->is_array())
                this->value.data_array->push_back(std::move(value));
            else if (this->is_null()) {
                this->data_type = json_array;
                this->value.data_array = new std::vector<Json>();
                this->value.data_array->push_back(std::move(value));
            } else
                throw std::logic_error("function Json::push_back: This object has been defined as another type, if you want to force changes to the properties of this object, call the clear() function first");
        }

        void push_front(const Json &value) {
            if (this->is_array())
                this->value.data_array->insert(this->value.data_array->begin(), value);
            else if (this->is_null()) {
                this->data_type = json_array;
                this->value.data_array = new std::vector<Json>();
                this->value.data_array->push_back(value);
            } else
                throw std::logic_error("function Json::push_front: This object has been defined as another type, if you want to force changes to the properties of this object, call the clear() function first");
        }

        void push_front(Json &&value) {
            if (this->is_array())
                this->value.data_array->insert(this->value.data_array->begin(), std::move(value));
            else if (this->is_null()) {
                this->data_type = json_array;
                this->value.data_array = new std::vector<Json>();
                this->value.data_array->push_back(std::move(value));
            } else
                throw std::logic_error("function Json::push_front: This object has been defined as another type, if you want to force changes to the properties of this object, call the clear() function first");
        }

        void erase(int index) {
            if (this->is_array()) {
                int size = this->value.data_array->size();
                if (index >= 0 || index < size) {
                    auto iter = this->value.data_array->begin() + index;
                    iter->clear();
                    this->value.data_array->erase(iter);
                } else
                    throw std::out_of_range("function Json::erase: index out of range");
            } else
                throw std::logic_error("function Json::erase: type error");
        }

        void erase(const char *key) {
            std::string str(key);
            erase(str);
        }

        void erase(const std::string &key) {
            if (this->is_object()) {
                auto iter = this->value.data_object->find(key);
                if (iter != this->value.data_object->end()) {
                    iter->second.clear();
                    this->value.data_object->erase(iter);
                }
            } else
                throw std::logic_error("function Json::erase: type error");
        }

        Json &operator=(const Json &other) {
            this->clear();
            this->copy(other);
            return *this;
        }

        Json &operator=(Json &&other) {
            this->clear();
            this->data_type = other.data_type;
            this->value = other.value;
            other.data_type = json_null;
            return *this;
        }

        bool operator==(const Json &other) const {
            if (this->data_type != other.data_type)
                return false;
            switch (this->data_type) {
            case json_null:
                return true;
            case json_bool:
                return this->value.data_bool == other.value.data_bool;
            case json_int:
                return this->value.data_int == other.value.data_int;
            case json_double:
                return this->value.data_double == other.value.data_double;
            case json_string:
                return *this->value.data_string == *other.value.data_string;
            case json_array:
                return *this->value.data_array == *other.value.data_array;
            case json_object:
                return *this->value.data_object == *other.value.data_object;
            default:
                break;
            }
            return false;
        }

        bool operator!=(const Json &other) const {
            return !((*this) == other);
        }

        Json &operator[](int index) {
            if (this->is_array()) {
                int size = this->value.data_array->size();
                if (index >= 0 && index < size) {
                    return this->value.data_array->at(index);
                }
                throw std::out_of_range("function Json::operator[]: index out of range");
            } else
                throw std::logic_error("function Json::operator[]: type error");
        }

        Json &operator[](const char *key) {
            std::string str(key);
            return (*this)[str];
        }

        Json &operator[](const std::string &key) {
            if (this->is_object()) {
                auto iter = this->value.data_object->find(key);
                if (iter != this->value.data_object->end())
                    return iter->second;
                else {
                    this->value.data_object->insert(std::make_pair(key, Json()));
                    return (*this)[key];
                }
            } else if (this->is_null()) {
                this->data_type = json_object;
                this->value.data_object = new std::map<std::string, Json>();
                return (*this)[key];
            } else
                throw std::logic_error("function Json::operator[]: type error");
        }

        operator bool() const {
            if (this->is_bool())
                return this->value.data_bool;
            else
                throw std::logic_error("function Json::operator bool(): type error");
        }

        operator int() const {
            if (this->is_int())
                return this->value.data_int;
            else
                throw std::logic_error("function Json::operator int(): type error");
        }

        operator double() const {
            if (this->is_double())
                return this->value.data_double;
            else
                throw std::logic_error("function Json::operator double(): type error");
        }

        operator std::string() const {
            if (this->is_string())
                return *this->value.data_string;
            else
                throw std::logic_error("function Json::operator std::string(): type error");
        }

        operator std::vector<Json>() const {
            if (this->is_array())
                return *this->value.data_array;
            else
                throw std::logic_error("function Json::operator std::vector<Json>(): type error");
        }

        operator std::map<std::string, Json>() const {
            if (this->is_object())
                return *this->value.data_object;
            else
                throw std::logic_error("function Json::operator std::map<std::string, Json>(): type error");
        }

        // 请暂时不要使用这个函数，我无法保证它的正确性
        // 如需输出请使用 to_string() 函数
        // friend std::ostream &operator<<(std::ostream &os, const Json &json);

        void parse(const char *json) {
            std::string str(json);
            this->parse(str);
        }

        void parse(const std::string &json) {
            this->clear();
            Parser parser(json);
            *this = parser.parse();
        }

        void parse(std::ifstream &file) {
            this->clear();
            std::string json;
            std::string line;
            if (!file.is_open())
                throw std::runtime_error("function Json::parse: file is not open");
            while (std::getline(file, line))
                json += line;
            this->parse(json);
        }

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

        void copy(const Json &other) {
            data_type = other.data_type;
            switch (data_type) {
            case json_bool:
                value.data_bool = other.value.data_bool;
                break;
            case json_int:
                value.data_int = other.value.data_int;
                break;
            case json_double:
                value.data_double = other.value.data_double;
                break;
            case json_string:
                value.data_string = new std::string(*other.value.data_string);
                break;
            case json_array:
                value.data_array = new std::vector<Json>(*other.value.data_array);
                break;
            case json_object:
                value.data_object = new std::map<std::string, Json>(*other.value.data_object);
                break;
            default:
                break;
            }
        }

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
