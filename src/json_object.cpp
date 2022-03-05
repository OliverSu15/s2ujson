#include "json_object.h"

#include <initializer_list>
#include <iostream>
#include <string>
#include <utility>

#include "json_data.h"

using JSON_Data = s2ujson::JSON_Data;
using JSON_Object = s2ujson::JSON_Object;

JSON_Object::JSON_Object(
    std::initializer_list<std::pair<std::string, JSON_Data>> list) {
  operator=(list);
}

JSON_Object &JSON_Object::operator=(
    std::initializer_list<std::pair<std::string, JSON_Data>> list) {
  for (auto i : list) {
    object.insert(std::make_pair(i.first, i.second));
  }
  return *this;
}
