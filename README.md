
# s2uJSON
[[ENGLISH]](README.md) [[中文]](README_zh-cn.md)
## Introduction
A toy-level single-file header JSON library which based on c++17 and the stander library. 

A learning result of [json-tutorial](https://github.com/miloyip/json-tutorial). And the API part is trying to be similar with [JSON for Modern C++](https://github.com/nlohmann/json).
## Quick Start
- include the headfile
  ``` cpp
  #include "json.hpp"
  using namespace s2ujson;
  ```
### `JSON_Object`
`JSON_Object` represents the `object` type in JSON. 

It is implemented with `std::map`, so the order is not guaranteed. 
- create JSON object
  ``` cpp
  JSON_Object object;
  object["null"] = nullptr;
  object["null"] = nullptr;
  object["number"] = 1;
  object["float"] = 1.5;
  object["string"] = "this is a string";
  object["boolean"] = true;
  object["array"] = {1, "good", false, nullptr};
  object["animal"] = {{"name", "dragon"}};
  object["user"]["id"] = 10;
  object["user"]["name"] = "Nomango"; 
  ```
  or do it in this way
  ``` cpp
  JSON_Object obj2 = {
      {"nul", nullptr},
      {"number", 1},
      {"float", 1.3},
      {"boolean", false},
      {"array", {1, 2, true, 1.4}},
      //{"array2", {1, 2, {1, 2, 3}}}, Not Support
      {"object", {{"key", "value"}, {"key2", "value2"}}},
      {"object2", {{"key", "val"}, {"key2", {{"key3", "val"}}}}}};
  ```
- add
  ``` cpp
  object.add("key",1.2);
  object.add("key2",false);
  ```
- get
  ``` cpp
  object.get_null("key");
  object.get<std::string>("key2");
  ```
- ouput string
  ``` cpp
  object.to_string();
  ```
- STL-like access
  ``` cpp
  object.begin();
  object.cbegin();
  object.end();
  object.cend();
  object.rbegin();
  object.crbegin();
  object.rend();
  object.crend();
  object.empty();
  object.size();
  object.max_size();
  object.clear();
  object.find();
  object.count("key");
  object.size("key");
  ```
### `JSON_Data`

`JSON_Data` is the type that actually stores the data, and it is implemented with `std::variant`.

Inside `JSON_Object` it is `std::map<std::string,JSON_Data>` which means that `JSON_Data` needs to store `JSON_Object`. `JSON_Object` is stored by `JSON_Data` in `std::shared_ptr<JSON_Object>`.

- get
  ``` cpp
  data.get_null();
  data.get<std::string>();
  ``` 
- set（no type check）
  ``` cpp
  data.set(false);
  data = nullptr;
  ```
- get type
  ``` cpp
  value_t type = data.get_type();
  data.is_null();
  ```
### JSON array
There is no specify type for JSON `array`. You can just use `std::vector<JSON_Data>`.
## TODO
- [ ] can iterate JSON object in order
- [ ] support nested JSON array syntax
- [ ] have more and better test cases
- [ ] import [Native JSON Benchmark](https://github.com/miloyip/nativejson-benchmark) to have a better test
## Thanks
[miloyip](https://github.com/miloyip/json-tutorial)'s `json-tutorial` helped me learn a lot.

[nlohmann](https://github.com/nlohmann/json)'s `JSON for Modern C++` is the target of this project.