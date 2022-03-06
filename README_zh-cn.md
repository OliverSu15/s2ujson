
# s2uJSON
[[ENGLISH]](README.md) [[中文]](README_zh-cn.md)
## 简介
一个玩具级别的单头文件JSON库，基于C++17且只使用了标准库，能够完成对JSON的解析和生成的功能。

是 [json-tutorial](https://github.com/miloyip/json-tutorial) 的学习成果。接口部分尽可能的向 [JSON for Modern C++](https://github.com/nlohmann/json) 靠拢了。
## 快速上手
- 引入
  ``` cpp
  #include "json.hpp"
  using namespace s2ujson;
  ```
### `JSON_Object`
`JSON_Object`对应的是JSON的`object`类型，它内部是由`std::map`实现的，因此它的输出不保证按照添加的顺序。
- 创建JSON对象
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
  或者是这样
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
- 添加
  ``` cpp
  object.add("key",1.2);
  object.add("key2",false);
  ```
- 获取
  ``` cpp
  object.get_null("key");
  object.get<std::string>("key2");
  ```
- 生成
  ``` cpp
  object.to_string();
  ```
- 类 STL 的操作
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
`JSON_Data`负责实际存储各类JSON类型，其内部是由`std::variant`来实现的。

`JSON_Object`内部是`std::map<std::string,JSON_Data>`。所以`JSON_Data`也是要保存`JSON_Object`，`JSON_Data`存储`JSON_Object`是保存了其对应的`std::shared_ptr<JSON_Object>`。

- 获取实际的值
  ``` cpp
  data.get_null();
  data.get<std::string>();
  ``` 
- 设置`JSON_Data`的值（不会进行类型检查）
  ``` cpp
  data.set(false);
  data = nullptr;
  ```
- 获取类型
  ``` cpp
  value_t type = data.get_type();
  data.is_null();
  ```
### JSON数组
并不存在一个特定的类型，如果需要使用直接使用`std::vector<JSON_Data>`即可。
## 计划
- [ ] 可以按照添加顺序遍历JSON对象
- [ ] 支持嵌套的JSON数组的语法
- [ ] 更好更全面的测试
- [ ] 接入[Native JSON Benchmark](https://github.com/miloyip/nativejson-benchmark)做测试
## 鸣谢
[miloyip](https://github.com/miloyip/json-tutorial)的`json-tutorial`使我受益匪浅

[nlohmann](https://github.com/nlohmann/json)的`JSON for Modern C++`是本项目想要靠近的目标