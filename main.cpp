#include "Json.h"
#include <iostream>

using Json = my_json::Json;

int main() {
    // test code
    Json json1;
    std::cout << json1.to_string() << std::endl;

    Json json2 = true;
    std::cout << json2.to_string() << std::endl;
    json2 = false;
    std::cout << json2.to_string() << std::endl;

    Json json3 = 123;
    std::cout << json3.to_string() << std::endl;
    json3 = 123.456;
    std::cout << json3.to_string() << std::endl;

    json3 = "hello";
    std::cout << json3.to_string() << std::endl;

    Json json4(std::vector<Json>{1, 2, 3, 4, 5});
    std::cout << json4.to_string() << std::endl;
    json4[0] = 2;
    json4.push_back(6);
    json4.push_front(0);
    std::cout << json4.to_string() << std::endl;

    Json json5(std::map<std::string, Json>{
        {"a", 1},
        {"b", 2},
        {"c", 3},
        {"d", 4},
        {"e", 5},
    });
    std::cout << json5.to_string() << std::endl;
    json5["a"] = 2;
    json5["f"] = 6;
    std::cout << json5.to_string() << std::endl;

    Json json6(std::map<std::string, Json>{
        {"json1", json1},
        {"json2", json2},
        {"json3", json3},
        {"json4", json4},
        {"json5", json5},
    });
    std::cout << json6.to_string() << std::endl;

    Json json7;
    json7.parse(json6.to_string());
    json7["json4"][0] = 1;
    std::cout << json7.to_string() << std::endl;

    Json json8;
    std::ifstream file("../.vscode/CMake.code-workspace");
    json8.parse(file);
    std::cout << json8.to_string() << std::endl;

    Json json9;
    json9.parse("[1, 2, 3, 4, 5]");
    std::cout << json9.to_string() << std::endl;
    return 0;
}
