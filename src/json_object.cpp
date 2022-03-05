#include "json_object.hpp"

#include <initializer_list>
#include <iostream>
#include <string>
#include <utility>

#include "json_data.h"

using JSON_Data = s2ujson::JSON_Data;
using JSON_Object = s2ujson::JSON_Object;
using JSON_Ref = s2ujson::JSON_Ref;

JSON_Object::JSON_Object(
    std::initializer_list<std::pair<std::string, JSON_Data>> list) {
  for (auto i : list) {
    if (i.first == "object") {
    }
    object.insert(i.first, i.second);
  }
}

JSON_Ref::JSON_Ref(
    std::initializer_list<std::pair<std::string, JSON_Ref>> list) {
  JSON_Object object;
  for (auto i : list) {
    object.push_back(i.first, i.second.data);
  }
  data = object;
}
