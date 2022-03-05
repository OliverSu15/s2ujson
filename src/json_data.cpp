#include "json_data.h"

#include <iostream>

#include "json_object.hpp"

using JSON_Data = s2ujson::JSON_Data;
using JSON_Object = s2ujson::JSON_Object;
// namespace s2ujson {

template <>
inline const JSON_Object &JSON_Data::get<JSON_Object>() {
  return *std::get<std::shared_ptr<JSON_Object>>(data);
}

// template <typename Key, typename Value>
// JSON_Data &JSON_Data::operator=(
//     std::initializer_list<std::pair<const Key, Value>> list) {
//   return *this;
// }

JSON_Data &JSON_Data::operator[](std::string &key) {
  if (type != data_type::OBJECT) {
    set(JSON_Object());
  }
  return (*std::get<std::shared_ptr<JSON_Object>>(data))[key];
}
JSON_Data &JSON_Data::operator[](std::string &&key) {
  // if (type != data_type::OBJECT)
  //   throw std::invalid_argument("wrong type to get");
  // return (*std::get<std::shared_ptr<JSON_Object>>(data))[key];
  return operator[](key);
}

std::string JSON_Data::array_output() {
  std::string output_string = "[";
  auto array = get_array();
  for (auto i : array) {
    output_string += (i.output() + ",");
  }
  output_string += "]";
  return output_string;
}

std::string JSON_Data::output() {
  switch (type) {
    case data_type::NULL_DATA:
      return "null";
    case data_type::TRUE:
      return "true";
    case data_type::FALSE:
      return "false";
    case data_type::NUMBER:
      return std::to_string(get<double>());
    case data_type::STRING:
      return "\"" + get_string() + "\"";
    case data_type::ARRAY:
      return array_output();
    case data_type::OBJECT:
      return get_object().output();
    default:
      throw std::invalid_argument("not implented");
  }
}
// JSON_Data::JSON_Data(
//     std::initializer_list<std::pair<std::string, JSON_Data>> par) {
//   JSON_Object object;
//   for (auto i : par) {
//     object.add(i.first, i.second);
//   }
// }
// constexpr JSON_Data::JSON_Data(
//     std::initializer_list<std::pair<std::string, JSON_Data>> list) {}

// JSON_Data::JSON_Data(const std::pair<std::string, JSON_Data> pair) {}

// JSON_Data &JSON_Data::operator=(
//     std::initializer_list<
//         std::pair<const std::string,
//                   std::variant<std::nullptr_t, bool, double, int,
//                   std::string,
//                                char *, std::vector<JSON_Data>,
//                                JSON_Object>>>
//         list) {
//   return operator=(JSON_Object(list));
// }

// JSON_Data::JSON_Data(std::pair<std::string, JSON_Data> pair) {
//   std::cout << "1" << std::endl;
// }

//}  // namespace s2ujson
