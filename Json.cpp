#include "Json.h"
#include <system_error>
using namespace my_json;

Json::Json() : data_type(json_null) {}

Json::Json(Type type) : data_type(type) {
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

Json::Json(bool value) : data_type(json_bool) {
    this->value.data_bool = value;
}

Json::Json(int value) : data_type(json_int) {
    this->value.data_int = value;
}

Json::Json(double value) : data_type(json_double) {
    this->value.data_double = value;
}

Json::Json(const char *value) : data_type(json_string) {
    this->value.data_string = new std::string(value);
}

Json::Json(std::string value) : data_type(json_string) {
    this->value.data_string = new std::string(value);
}

Json::Json(std::vector<Json> value) : data_type(json_array) {
    this->value.data_array = new std::vector<Json>(value);
}

Json::Json(std::map<std::string, Json> value) : data_type(json_object) {
    this->value.data_object = new std::map<std::string, Json>(value);
}

Json::Json(const Json &other) {
    this->copy(other);
}

Json::Json(Json &&other) {
    this->data_type = other.data_type;
    this->value = other.value;
    other.data_type = json_null;
}

Json::~Json() {
    this->clear();
}

Json::Type Json::type() const {
    return data_type;
}

bool Json::is_null() const {
    return data_type == json_null;
}

bool Json::is_bool() const {
    return data_type == json_bool;
}

bool Json::is_int() const {
    return data_type == json_int;
}

bool Json::is_double() const {
    return data_type == json_double;
}

bool Json::is_string() const {
    return data_type == json_string;
}

bool Json::is_array() const {
    return data_type == json_array;
}

bool Json::is_object() const {
    return data_type == json_object;
}

bool Json::get_bool() const {
    if (this->is_bool())
        return value.data_bool;
    throw std::logic_error("function Json::get_bool: type error");
}

int Json::get_int() const {
    if (this->is_int())
        return value.data_int;
    throw std::logic_error("function Json::get_int: type error");
}

double Json::get_double() const {
    if (this->is_double())
        return value.data_double;
    throw std::logic_error("function Json::get_double: type error");
}

std::string Json::get_string() const {
    if (this->is_string())
        return *value.data_string;
    throw std::logic_error("function Json::get_string: type error");
}

std::vector<Json> Json::get_array() const {
    if (this->is_array())
        return *value.data_array;
    throw std::logic_error("function Json::get_array: type error");
}

std::map<std::string, Json> Json::get_object() const {
    if (this->is_object())
        return *value.data_object;
    throw std::logic_error("function Json::get_object: type error");
}

int Json::size() const {
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

bool Json::empty() const {
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

void Json::clear() {
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

std::string Json::to_string() const {
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

bool Json::find(const char *key) const {
    return this->has_key(key);
}

bool Json::find(const std::string &key) const {
    return this->has_key(key);
}

bool Json::has_key(const char *key) const {
    std::string str(key);
    if (this->is_object())
        return value.data_object->find(str) != value.data_object->end();
    throw std::logic_error("function Json::has_key: type error");
}

bool Json::has_key(const std::string &key) const {
    if (this->is_object())
        return value.data_object->find(key) != value.data_object->end();
    throw std::logic_error("function Json::has_key: type error");
}

void Json::push_back(const Json &value) {
    if (this->is_array())
        this->value.data_array->push_back(value);
    else if (this->is_null()) {
        this->data_type = json_array;
        this->value.data_array = new std::vector<Json>();
        this->value.data_array->push_back(value);
    } else
        throw std::logic_error("function Json::push_back: This object has been defined as another type, if you want to force changes to the properties of this object, call the clear() function first");
}

void Json::push_back(Json &&value) {
    if (this->is_array())
        this->value.data_array->push_back(std::move(value));
    else if (this->is_null()) {
        this->data_type = json_array;
        this->value.data_array = new std::vector<Json>();
        this->value.data_array->push_back(std::move(value));
    } else
        throw std::logic_error("function Json::push_back: This object has been defined as another type, if you want to force changes to the properties of this object, call the clear() function first");
}

void Json::push_front(const Json &value) {
    if (this->is_array())
        this->value.data_array->insert(this->value.data_array->begin(), value);
    else if (this->is_null()) {
        this->data_type = json_array;
        this->value.data_array = new std::vector<Json>();
        this->value.data_array->push_back(value);
    } else
        throw std::logic_error("function Json::push_front: This object has been defined as another type, if you want to force changes to the properties of this object, call the clear() function first");
}

void Json::push_front(Json &&value) {
    if (this->is_array())
        this->value.data_array->insert(this->value.data_array->begin(), std::move(value));
    else if (this->is_null()) {
        this->data_type = json_array;
        this->value.data_array = new std::vector<Json>();
        this->value.data_array->push_back(std::move(value));
    } else
        throw std::logic_error("function Json::push_front: This object has been defined as another type, if you want to force changes to the properties of this object, call the clear() function first");
}

void Json::erase(int index) {
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

void Json::erase(const char *key) {
    std::string str(key);
    erase(str);
}

void Json::erase(const std::string &key) {
    if (this->is_object()) {
        auto iter = this->value.data_object->find(key);
        if (iter != this->value.data_object->end()) {
            iter->second.clear();
            this->value.data_object->erase(iter);
        }
    } else
        throw std::logic_error("function Json::erase: type error");
}

Json &Json::operator=(const Json &other) {
    this->clear();
    this->copy(other);
    return *this;
}

Json &Json::operator=(Json &&other) {
    this->clear();
    this->data_type = other.data_type;
    this->value = other.value;
    other.data_type = json_null;
    return *this;
}

bool Json::operator==(const Json &other) const {
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

bool Json::operator!=(const Json &other) const {
    return !((*this) == other);
}

Json &Json::operator[](int index) {
    if (this->is_array()) {
        int size = this->value.data_array->size();
        if (index >= 0 && index < size) {
            return this->value.data_array->at(index);
        }
        throw std::out_of_range("function Json::operator[]: index out of range");
    } else
        throw std::logic_error("function Json::operator[]: type error");
}

Json &Json::operator[](const char *key) {
    std::string str(key);
    return (*this)[str];
}

// TODO:这里感觉怪怪的
Json &Json::operator[](const std::string &key) {
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

Json::operator bool() const {
    if (this->is_bool())
        return this->value.data_bool;
    else
        throw std::logic_error("function Json::operator bool(): type error");
}

Json::operator int() const {
    if (this->is_int())
        return this->value.data_int;
    else
        throw std::logic_error("function Json::operator int(): type error");
}

Json::operator double() const {
    if (this->is_double())
        return this->value.data_double;
    else
        throw std::logic_error("function Json::operator double(): type error");
}

Json::operator std::string() const {
    if (this->is_string())
        return *this->value.data_string;
    else
        throw std::logic_error("function Json::operator std::string(): type error");
}

Json::operator std::vector<Json>() const {
    if (this->is_array())
        return *this->value.data_array;
    else
        throw std::logic_error("function Json::operator std::vector<Json>(): type error");
}

Json::operator std::map<std::string, Json>() const {
    if (this->is_object())
        return *this->value.data_object;
    else
        throw std::logic_error("function Json::operator std::map<std::string, Json>(): type error");
}

// operator<<
// std::ostream &operator<<(std::ostream &os, const Json &json) {
//     os << json.to_string();
//     return os;
// }

void Json::parse(const char *json) {
    std::string str(json);
    this->parse(str);
}

void Json::parse(const std::string &json) {
    this->clear();
    Parser parser(json);
    *this = parser.parse();
}

void Json::parse(std::ifstream &file) {
    this->clear();
    std::string json;
    std::string line;
    if (!file.is_open())
        throw std::runtime_error("function Json::parse: file is not open");
    while (std::getline(file, line))
        json += line;
    this->parse(json);
}

void Json::copy(const Json &other) {
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
