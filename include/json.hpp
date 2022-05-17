/**
 * @file json.hpp
 * @author OliverSu15 (OliverSu15@outlook.com)
 * @brief
 * @version 1.0
 * @date 2022-03-06
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef _JSON_HPP_
#define _JSON_HPP_
#include <cctype>
#include <cstddef>
#include <ctime>
#include <exception>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace s2ujson {

// forward declaration
class JSON_Object;

/**
 * @brief used to represent all the data type in JSON
 *
 */
enum class value_t { NULL_DATA, TRUE, FALSE, NUMBER, STRING, ARRAY, OBJECT };

/**
 * @brief The class which holds all the data, include 'Object'.
 * @details Is basically a wrapper of 'std::variant'. All the 'exceptions' that
 * threw by 'std::variant' need you to handle, if you do something incorrectly
 *
 */
class JSON_Data {
  // use to achieve initialization of array by using initializer_list
  using var = std::variant<std::nullptr_t, bool, double, int, std::string>;

 public:
  // All the Constructors
  constexpr JSON_Data() { set(nullptr); }
  constexpr explicit JSON_Data(const bool d_bool) { set(d_bool); }
  constexpr JSON_Data(const double d_number) { set(d_number); }
  constexpr JSON_Data(const int d_number) {
    set(static_cast<double>(d_number));
  }
  constexpr JSON_Data(const std::string &d_string) { set(d_string); }
  constexpr JSON_Data(const char *ptr) { set(std::string(ptr)); }
  constexpr JSON_Data(const std::vector<JSON_Data> &d_array) { set(d_array); }
  constexpr JSON_Data(const JSON_Object &d_object) { set(d_object); }
  constexpr JSON_Data(const std::nullptr_t n_ptr) { set(n_ptr); };
  constexpr JSON_Data(std::initializer_list<var> list) { operator=(list); }
  constexpr JSON_Data(
      std::initializer_list<std::pair<std::string, JSON_Data>> list) {
    operator=(list);
  }

  // All the getter
  inline std::nullptr_t &get_null() { return std::get<std::nullptr_t>(data); }
  inline bool &get_bool() { return std::get<bool>(data); }
  inline double &get_number() { return std::get<double>(data); }
  inline std::string &get_string() { return std::get<std::string>(data); }
  inline std::vector<JSON_Data> &get_array() {
    return std::get<std::vector<JSON_Data>>(data);
  }
  inline JSON_Object &get_object() {
    return *std::get<std::shared_ptr<JSON_Object>>(data);
  }

  // All the type check
  inline const value_t &get_type() { return type; };
  inline bool is_null() const { return type == value_t::NULL_DATA; }
  inline bool is_boolean() const {
    return (type == value_t::TRUE || type == value_t::FALSE);
  }
  inline bool is_number() const { return type == value_t::NUMBER; }
  inline bool is_string() const { return type == value_t::STRING; }
  inline bool is_array() const { return type == value_t::ARRAY; }
  inline bool is_object() const { return type == value_t::OBJECT; }

  // All the setter
  inline void set(const std::nullptr_t) {
    type = value_t::NULL_DATA;
    data = nullptr;
  }
  inline void set(bool d_bool) {
    if (d_bool) {
      type = value_t::TRUE;
    } else {
      type = value_t::FALSE;
    }
    data = d_bool;
  }
  inline void set(double d_number) {
    type = value_t::NUMBER;
    data = d_number;
  }
  inline void set(const std::string &d_string) {
    type = value_t::STRING;
    data = d_string;
  }
  inline void set(const std::vector<JSON_Data> &d_array) {
    type = value_t::ARRAY;
    data = d_array;
  }
  inline void set(const JSON_Object &d_object) {
    type = value_t::OBJECT;
    data = std::make_shared<JSON_Object>(d_object);
  }

  /**
   * @brief a special setter which used to provied usage like `data.get<bool>()`
   *
   * @tparam T
   * @return const T&
   */
  template <typename T>
  inline const T &get() {
    return std::get<T>(data);
  }

  // All overloaded `=` to do `object["key"] = something` or `object["key"] =
  // {{"name", "val"}}` or `object["key"] = {1, "good", false, nullptr}`
  inline JSON_Data &operator=(std::nullptr_t) {
    set(nullptr);
    return *this;
  }
  inline JSON_Data &operator=(const bool d_bool) {
    set(d_bool);
    return *this;
  }
  inline JSON_Data &operator=(const double d_number) {
    set(d_number);
    return *this;
  }
  inline JSON_Data &operator=(const int d_number) {
    return operator=(static_cast<double>(d_number));
  }
  inline JSON_Data &operator=(const std::string &d_string) {
    set(d_string);
    return *this;
  }
  inline JSON_Data &operator=(const std::string &&d_string) {
    return operator=(d_string);
  }
  inline JSON_Data &operator=(const char *ptr) {
    return operator=(std::string(ptr));
  }
  inline JSON_Data &operator=(const std::vector<JSON_Data> &d_array) {
    set(d_array);
    return *this;
  }
  inline JSON_Data &operator=(const std::vector<JSON_Data> &&d_array) {
    return operator=(d_array);
  }
  inline JSON_Data &operator=(const JSON_Object &d_object) {
    set(d_object);
    return *this;
  }
  inline JSON_Data &operator=(const JSON_Object &&d_object) {
    return operator=(d_object);
  }
  JSON_Data &operator=(
      std::initializer_list<std::pair<std::string, JSON_Data>> list);
  JSON_Data &operator=(std::initializer_list<var> list);

  // All overloaded `[]` to do `object["key1"]["key2"] = something"
  JSON_Data &operator[](std::string &&key);
  JSON_Data &operator[](std::string &key);

  /**
   * @brief output the JSON string of this data
   * @return std::string
   */
  std::string to_string();

 private:
  /**
   * @brief output array style JSON string for array
   *
   * @return std::string
   */
  std::string array_to_string();
  /**
   * @brief get the data out of var
   *
   * @param d_var
   * @return JSON_Data
   */
  JSON_Data convert(var d_var);

 private:
  std::variant<std::nullptr_t, bool, double, std::string,
               std::vector<JSON_Data>, std::shared_ptr<JSON_Object>>
      data;
  value_t type = value_t::NULL_DATA;
};
}  // namespace s2ujson

/**
 * @brief a working `data structure` to let the user can read the data in the
 * order they added
 * @deprecated working in progress
 */
// class JSON_Object_Storage {
//  public:
//   explicit JSON_Object_Storage() = default;
//   inline void insert(const std::string &key, JSON_Data &data) {
//     key_to_pos.insert(std::make_pair(key, key_to_pos.size()));
//     if (real_data.size() >= key_to_pos.size()) {
//       real_data[key_to_pos.size() - 1] = data;
//     } else {
//       real_data.push_back(data);
//     }
//   }
//   inline void insert(const std::string &key, JSON_Data &&data) {
//     insert(key, data);
//   }

//   inline JSON_Data &find(const std::string &key) {
//     return real_data[key_to_pos.find(key)->second];
//   }

//   inline JSON_Data &find(const int pos) { return real_data[pos]; }

//   inline bool is_key_invalid(const std::string &key) {
//     return key_to_pos.find(key) == key_to_pos.end();
//   }

//   std::vector<std::pair<std::string, JSON_Data>> get_Storage_In_Order() {
//     std::vector<std::pair<std::string, JSON_Data>> result(key_to_pos.size());
//     for (auto i : key_to_pos) {
//       result[i.second] = std::make_pair(i.first, real_data[i.second]);
//     }
//     return result;
//   }

//  private:
//   std::map<std::string, int> key_to_pos;
//   std::vector<JSON_Data> real_data;
// };
namespace s2ujson {
class JSON_Object {
 public:
  using iterator = std::map<std::string, JSON_Data>::iterator;
  using const_iterator = std::map<std::string, JSON_Data>::const_iterator;
  using reverse_iterator = std::map<std::string, JSON_Data>::reverse_iterator;
  using const_reverse_iterator =
      std::map<std::string, JSON_Data>::const_reverse_iterator;

 public:
  // All Constructor
  explicit JSON_Object() = default;
  JSON_Object(std::initializer_list<std::pair<std::string, JSON_Data>> list);

  // All add
  inline void add(const std::string &key) {
    object.insert(std::make_pair(key, JSON_Data()));
  }
  // inline void add(const std::string &&key) { add(key); }
  inline void add(const std::string &key, std::nullptr_t) {
    object.insert(std::make_pair(key, JSON_Data()));
  }
  // inline void add(const std::string &&key, std::nullptr_t) {
  //   add(key, nullptr);
  // }
  inline void add(const std::string &key, const bool d_bool) {
    object.insert(std::make_pair(key, JSON_Data(d_bool)));
  }
  // inline void add(const std::string &&key, const bool d_bool) {
  //   add(key, d_bool);
  // }
  inline void add(const std::string &key, const int d_number) {
    object.insert(std::make_pair(key, JSON_Data(d_number)));
  }
  inline void add(const std::string &key, const double d_number) {
    object.insert(std::make_pair(key, JSON_Data(d_number)));
  }
  // inline void add(const std::string &&key, const double d_number) {
  //   add(key, d_number);
  // }
  inline void add(const std::string &key, const std::string &d_string) {
    object.insert(std::make_pair(key, JSON_Data(d_string)));
  }
  // inline void add(const std::string &&key, const std::string &d_string) {
  //   add(key, d_string);
  // }
  inline void add(const std::string &key,
                  const std::vector<JSON_Data> &d_array) {
    object.insert(std::make_pair(key, JSON_Data(d_array)));
  }
  // inline void add(const std::string &&key,
  //                 const std::vector<JSON_Data> &d_array) {
  //   add(key, d_array);
  // }
  inline void add(const std::string &key, const JSON_Object &d_object) {
    object.insert(std::make_pair(key, JSON_Data(d_object)));
  }
  // inline void add(const std::string &&key, const JSON_Object &d_object) {
  //   add(key, d_object);
  // }
  inline void add(const std::string &key, const JSON_Data &d_data) {
    object.insert(std::make_pair(key, d_data));
  }
  // inline void add(const std::string &&key, const JSON_Data &d_data) {
  //   add(key, d_data);
  // }

  // All getter
  inline std::nullptr_t get_null(const std::string &key) {
    is_key_valid(key);
    return object.find(key)->second.get_null();
  }
  inline std::nullptr_t get_null(const std::string &&key) {
    return get_null(key);
  }
  inline bool get_bool(const std::string &key) {
    is_key_valid(key);
    return object.find(key)->second.get_bool();
  }
  inline bool get_bool(const std::string &&key) { return get_bool(key); }
  inline double get_number(const std::string &key) {
    is_key_valid(key);
    return object.find(key)->second.get_number();
  }
  inline double get_number(std::string &&key) { return get_number(key); }
  inline const std::string &get_string(const std::string &key) {
    is_key_valid(key);
    return object.find(key)->second.get_string();
  }
  inline const std::string &get_string(std::string &&key) {
    return get_string(key);
  }
  inline const std::vector<JSON_Data> &get_array(const std::string &key) {
    is_key_valid(key);
    return object.find(key)->second.get_array();
  }
  inline const std::vector<JSON_Data> &get_array(std::string &&key) {
    return get_array(key);
  }
  inline JSON_Object &get_object(const std::string &key) {
    is_key_valid(key);
    return (object.find(key)->second.get_object());
  }
  inline JSON_Object &get_object(std::string &&key) { return get_object(key); }
  /**
   * @brief a special setter which used to provied usage like
   * `object.get<bool>("key")`
   *
   * @tparam T
   * @return const T&
   */
  template <typename T>
  inline const T &get(const std::string &key) {
    return object.find(key)->second.get<T>();
  }

  // All operator
  inline JSON_Data &operator[](const std::string &key) {
    auto iter = object.find(key);
    if (iter != object.end()) {
      return iter->second;
    }
    return object.insert(std::make_pair(key, JSON_Data())).first->second;
  }
  inline JSON_Data &operator[](const std::string &&key) {
    return operator[](key);
  }

  /**
   * @brief allow user to use
   * `JSON_Object object = {{"key","value"},{"key","value"}}`
   *
   * @param list
   * @return JSON_Object&
   */
  JSON_Object &operator=(
      std::initializer_list<std::pair<std::string, JSON_Data>> list);

  /**
   * @brief return object's JSON string
   *
   * @return std::string
   */
  inline std::string to_string() {
    std::string output_string = "{";
    for (auto i : object) {
      output_string +=
          ("\"" + i.first + "\"" + ":" + i.second.to_string() + ",");
    }
    output_string.push_back('}');
    return output_string;
  }

  // All STL-like access
  // iterator
  inline iterator begin() { return object.begin(); }
  inline const_iterator cbegin() const { return object.cbegin(); };
  inline iterator end() { return object.end(); }
  inline const_iterator cend() const { return object.cend(); }
  inline reverse_iterator rbegin() { return object.rbegin(); }
  inline const_reverse_iterator crbegin() const { return object.crbegin(); }
  inline reverse_iterator rend() { return object.rend(); }
  inline const_reverse_iterator crend() const { return object.crend(); }
  // capacity
  inline bool empty() const { return object.empty(); }
  inline size_t size() const { return object.size(); };
  inline size_t max_size() const { return object.max_size(); }
  // modifer
  inline void clear() { object.clear(); }
  // finder
  inline iterator find(const std::string &key) { return object.find(key); }
  inline const_iterator find(const std::string &key) const {
    return object.find(key);
  }
  inline size_t count(const std::string &key) const {
    return object.count(key);
  }

 private:
  inline void is_key_valid(const std::string &key) {
    if (object.find(key) == object.end()) {
      throw std::invalid_argument("invalid key");
    }
  }
  inline void is_key_valid(const std::string &&key) { is_key_valid(key); }

 private:
  std::map<std::string, JSON_Data> object;
};

/**
 * @brief specified version of `get` to get the `JSON_Object` ranther than
 * pointer
 *
 * @tparam
 * @return const JSON_Object&
 */
template <>
inline const JSON_Object &JSON_Data::get<JSON_Object>() {
  return *std::get<std::shared_ptr<JSON_Object>>(data);
}

JSON_Data &JSON_Data::operator=(
    std::initializer_list<std::pair<std::string, JSON_Data>> list) {
  JSON_Object object;
  for (auto i : list) {
    object.add(i.first, i.second);
  }
  set(object);
  return *this;
}

JSON_Data &JSON_Data::operator=(std::initializer_list<var> list) {
  std::vector<JSON_Data> array(list.size());
  int pos = 0;
  for (auto i : list) {
    array[pos++] = convert(i);
  }
  set(array);
  return *this;
}

JSON_Data &JSON_Data::operator[](std::string &key) {
  if (type != value_t::OBJECT) {
    // if not exist create one
    set(JSON_Object());
  }
  return (*std::get<std::shared_ptr<JSON_Object>>(data))[key];
}
JSON_Data &JSON_Data::operator[](std::string &&key) { return operator[](key); }

std::string JSON_Data::to_string() {
  switch (type) {
    case value_t::NULL_DATA:
      return "null";
    case value_t::TRUE:
      return "true";
    case value_t::FALSE:
      return "false";
    case value_t::NUMBER:
      return std::to_string(get<double>());
    case value_t::STRING:
      return "\"" + get_string() + "\"";
    case value_t::ARRAY:
      return array_to_string();
    case value_t::OBJECT:
      return get_object().to_string();
    default:
      throw std::invalid_argument("not implented");
  }
}
std::string JSON_Data::array_to_string() {
  std::string output_string = "[";
  auto array = get_array();
  for (auto i : array) {
    output_string += (i.to_string() + ",");
  }
  output_string += "]";
  return output_string;
}

JSON_Data JSON_Data::convert(var d_var) {
  switch (d_var.index()) {
    case 0:
      return JSON_Data(std::get<std::nullptr_t>(d_var));
    case 1:
      return JSON_Data(std::get<bool>(d_var));
    case 2:
      return JSON_Data(std::get<double>(d_var));
    case 3:
      return JSON_Data(std::get<int>(d_var));
    case 4:
      return JSON_Data(std::get<std::string>(d_var));
    default:
      throw std::invalid_argument("wrong");
  }
}

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
}  // namespace s2ujson
// Exception
#define invalid_Unicode_HEX std::invalid_argument("Invalid Unicode HEX")
#define invalid_HEX std::invalid_argument("invalid HEX")
#define literial_true_is_not_correct \
  std::invalid_argument("literial \"true\" is not correct")
#define literial_false_is_not_correct \
  std::invalid_argument("literial \"false\" is not correct")
#define literial_null_is_not_correct \
  std::invalid_argument("literial \"null\" is not correct")
#define number_is_not_correct std::invalid_argument("number is not correct")
#define invalid_unicode_surrogate \
  std::invalid_argument("invalid unicode surrogate")
#define invalid_string_escapestd std::invalid_argument("invalid string escape")
#define invalid_string_char std::invalid_argument("invalid string char")
#define miss_quotation_mark std::invalid_argument("miss quotation mark")
#define array_is_invalid std::invalid_argument("array is invalid")
#define object_is_invalid std::invalid_argument("object is invalid")

// helper
inline static bool _isHex(char ch) {
  return (std::isdigit(ch) != 0) ||
         ((ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F'));
}
inline static unsigned int _JSON_parse_string_hex_helper(
    std::string &&json) try {
  if (_isHex(json[0]) && _isHex(json[1]) && _isHex(json[2]) &&
      _isHex(json[3])) {
    return std::stoul(json, nullptr, 16);
  }
  throw invalid_Unicode_HEX;
} catch (...) {
  throw invalid_Unicode_HEX;
}
static std::string _JSON_parse_string_utf_helper(unsigned int hex) {
  std::string result;
  if (hex <= 0x7F) {
    result.push_back((hex & 0xff));
  } else if (hex <= 0x7FF) {
    result.push_back(0xC0 | ((hex >> 6) & 0xFF));
    result.push_back(0x80 | (hex & 0x3F));
  } else if (hex <= 0xFFFF) {
    result.push_back(0xE0 | ((hex >> 12) & 0xFF));
    result.push_back(0x80 | ((hex >> 6) & 0x3F));
    result.push_back(0x80 | (hex & 0x3F));
  } else {
    if (hex > 0x10FFFF) throw invalid_HEX;
    result.push_back(0xF0 | ((hex >> 18) & 0xFF));
    result.push_back(0x80 | ((hex >> 12) & 0x3F));
    result.push_back(0x80 | ((hex >> 6) & 0x3F));
    result.push_back(0x80 | (hex & 0x3F));
  }

  return result;
}

/**
 * @brief private function that parse `true` type in JSON
 *
 *
 * @param begin
 * @return true
 * @return false
 */
static bool _JSON_parse_true_iter(std::string::const_iterator &begin) {
  auto iter = begin;
  while (*iter == ' ') {
    iter++;
  }
  if (*(iter++) == 't' && *(iter++) == 'r' && *(iter++) == 'u' &&
      *(iter++) == 'e') {
    begin = iter;
    return true;
  }
  throw literial_true_is_not_correct;
}
inline static bool _JSON_parse_true_iter(std::string::const_iterator &&begin) {
  return _JSON_parse_true_iter(begin);
}
namespace s2ujson {
/**
 * @brief private function that only exist for testing
 *
 * @param json
 * @return true
 * @return false
 */
inline static bool JSON_parse_true(const std::string &json) {
  auto iter = json.cbegin();
  return _JSON_parse_true_iter(json.cbegin());
}
inline static bool JSON_parse_true(const std::string &&json) {
  return JSON_parse_true(json);
}
}  // namespace s2ujson

/**
 * @brief private function that parse `false` type in JSON
 *
 * @param begin
 * @return true
 * @return false
 */
bool _JSON_parse_false_iter(std::string::const_iterator &begin) {
  auto iter = begin;
  while (*iter == ' ') {
    iter++;
  }
  if (*(iter++) == 'f' && *(iter++) == 'a' && *(iter++) == 'l' &&
      *(iter++) == 's' && *(iter++) == 'e') {
    begin = iter;
    return false;
  }
  throw literial_false_is_not_correct;
}
inline static bool _JSON_parse_false_iter(std::string::const_iterator &&begin) {
  return _JSON_parse_false_iter(begin);
}
namespace s2ujson {
/**
 * @brief private function that only exist for testing
 *
 * @param json
 * @return true
 * @return false
 */
inline static bool JSON_parse_false(const std::string &json) {
  return _JSON_parse_false_iter(json.cbegin());
}
inline static bool JSON_parse_false(const std::string &&json) {
  return JSON_parse_false(json);
}
}  // namespace s2ujson
/**
 * @brief private function that parse `null` type in JSON
 *
 * @param begin
 * @return std::nullptr_t
 */
static std::nullptr_t _JSON_parse_null_iter(
    std::string::const_iterator &begin) {
  auto iter = begin;
  while (*iter == ' ') {
    iter++;
  }
  if (*(iter++) == 'n' && *(iter++) == 'u' && *(iter++) == 'l' &&
      *(iter++) == 'l') {
    begin = iter;
    return nullptr;
  }
  throw literial_null_is_not_correct;
}
inline static std::nullptr_t _JSON_parse_null_iter(
    std::string::const_iterator &&begin) {
  return _JSON_parse_null_iter(begin);
}
namespace s2ujson {
/**
 * @brief private function that only exist for testing
 *
 * @param json
 * @return std::nullptr_t
 */
inline static std::nullptr_t JSON_parse_null(const std::string &json) {
  return _JSON_parse_null_iter(json.cbegin());
}
inline static std::nullptr_t JSON_parse_null(const std::string &&json) {
  return JSON_parse_null(json);
}
}  // namespace s2ujson
/**
 * @brief private function that parse `number` type in JSON
 *
 * @param begin
 * @return double
 */
static double _JSON_parse_number_iter(std::string::const_iterator &begin) {
  auto end = begin;
  if (*end == '-') end++;
  if (*end == '0') {
    end++;
  } else {
    if (!(*end >= '1' && *end <= '9')) throw number_is_not_correct;
    while (std::isdigit(*end)) end++;
  }

  if (*end == '.') {
    end++;
    if (!std::isdigit(*end)) throw number_is_not_correct;
    while (std::isdigit(*end)) end++;
  }
  if ((*end == 'e' || *end == 'E')) {
    end++;
    if ((*end == '+' || *end == '-')) end++;
    if (!std::isdigit(*end)) throw number_is_not_correct;
    while (std::isdigit(*end)) end++;
  }
  if ((*end != ',') && (*end != '\0') && (*end != ' ') && (*end != ']') &&
      (*end != '}') && (*end != '\n'))
    throw number_is_not_correct;
  std::string num_string(begin, end);
  begin = end;
  return std::stod(num_string);
}
inline static double _JSON_parse_number_iter(
    std::string::const_iterator &&begin) {
  return _JSON_parse_number_iter(begin);
}
namespace s2ujson {
/**
 * @brief private function that only exist for testing
 *
 * @param json
 * @return std::nullptr_t
 */
inline static double JSON_parse_number(const std::string &json) {
  return _JSON_parse_number_iter(json.cbegin());
}
inline static double JSON_parse_number(const std::string &&json) {
  return JSON_parse_number(json);
}
}  // namespace s2ujson
/**
 * @brief private function that parse `string` type in JSON
 *
 * @param begin
 * @return std::string
 */
static std::string _JSON_parse_string_iter(std::string::const_iterator &begin) {
  auto iter = begin;
  std::string result = "";
  if (*iter == '\"') iter++;
  while (*iter != '\0') {
    switch (*iter) {
      case '\"':
        begin = iter + 1;
        return result;  // get the right result
      case '\\':
        iter++;
        switch (*iter) {
          case '\\':
            result.push_back('\\');
            break;
          case '\"':
            result.push_back('\"');
            break;
          case '/':
            result.push_back('/');
            break;
          case 'b':
            result.push_back('\b');
            break;
          case 'f':
            result.push_back('\f');
            break;
          case 'n':
            result.push_back('\n');
            break;
          case 'r':
            result.push_back('\r');
            break;
          case 't':
            result.push_back('\t');
            break;
          case 'u': {
            unsigned int hex =
                _JSON_parse_string_hex_helper(std::string(iter + 1, iter + 5));
            if (hex >= 0xD800 && hex <= 0xDBFF) {
              iter += 5;
              if (*iter != '\\') throw invalid_unicode_surrogate;
              iter++;
              if (*iter != 'u') throw invalid_unicode_surrogate;
              unsigned int hex2 = _JSON_parse_string_hex_helper(
                  std::string(iter + 1, iter + 5));
              if (hex2 < 0xDC00 || hex2 > 0xDFFF)
                throw invalid_unicode_surrogate;
              hex = (((hex - 0xD800) << 10) | (hex2 - 0xDC00)) + 0x10000;
            }
            if (hex == 0) {
              while (*iter != '\"') iter++;
              begin = ++iter;
              return result;
            } else {
              result.append(_JSON_parse_string_utf_helper(hex));
            }
            iter += 4;
            break;
          }
          default:
            throw invalid_string_escapestd;
        }
        iter++;
        break;
      default:
        // INVALID_STRING_CHAR
        if (*iter < 0x20) throw invalid_string_char;
        result.push_back(*iter);
        iter++;
        break;
    }
  }
  throw miss_quotation_mark;
}
inline static std::string _JSON_parse_string_iter(
    std::string::const_iterator &&begin) {
  return _JSON_parse_string_iter(begin);
}
namespace s2ujson {
/**
 * @brief private function that only exist for testing
 *
 * @param json
 * @return std::string
 */
inline static std::string JSON_parse_string(const std::string &json) {
  return _JSON_parse_string_iter(json.begin());
}
inline static std::string JSON_parse_string(const std::string &&json) {
  return JSON_parse_string(json);
}
}  // namespace s2ujson
// forward decalration, so it can be used in parsing array
static s2ujson::JSON_Object _JSON_parse_object_iter(
    std::string::const_iterator &begin);

/**
 * @brief private function that parse `array` type in JSON. use
 * `JSON_parse_array` instead
 *
 * @param begin
 * @return std::vector<JSON_Data>
 */
static std::vector<s2ujson::JSON_Data> _JSON_parse_array_iter(
    std::string::const_iterator &begin) {
  auto iter = begin;
  if (*iter != '[') throw array_is_invalid;
  iter++;
  auto next_start_pos = iter;
  auto next_end_pos = iter;
  std::vector<s2ujson::JSON_Data> array;
  while (*iter != '\0') {
    switch (*iter) {
      case ']':
        begin = iter + 1;
        return array;
      case 'n': {
        array.emplace_back(
            s2ujson::JSON_parse_null(std::string(iter, iter + 4)));
        iter += 4;
        next_start_pos = next_end_pos = iter;
        break;
      }
      case 't': {
        array.emplace_back(
            s2ujson::JSON_parse_true(std::string(iter, iter + 4)));
        iter += 4;
        next_start_pos = next_end_pos = iter;
        break;
      }
      case 'f': {
        array.emplace_back(
            s2ujson::JSON_parse_false(std::string(iter, iter + 5)));
        iter += 5;
        next_start_pos = next_end_pos = iter;
        break;
      }
      case '\"': {
        array.emplace_back(_JSON_parse_string_iter(iter));
        break;
      }
      case '[': {
        array.emplace_back(_JSON_parse_array_iter(iter));
        break;
      }
      case '{': {
        array.emplace_back(_JSON_parse_object_iter(iter));
        break;
      }
      default: {
        if (*iter != ' ' && *iter != ',' && *iter != '\n') {
          array.emplace_back(_JSON_parse_number_iter(iter));
        } else
          iter++;
      }
    }
  }
  throw array_is_invalid;
}
inline static std::vector<s2ujson::JSON_Data> _JSON_parse_array_iter(
    std::string::const_iterator &&begin) {
  return _JSON_parse_array_iter(begin);
}
namespace s2ujson {
/**
 * @brief parse `array` type in JSON.
 *
 * @param json
 * @return std::vector<JSON_Data>
 */
inline static std::vector<JSON_Data> JSON_parse_array(const std::string &json) {
  return _JSON_parse_array_iter(json.cbegin());
}
inline static std::vector<JSON_Data> JSON_parse_array(
    const std::string &&json) {
  return JSON_parse_array(json);
}
}  // namespace s2ujson
/**
 * @brief private function that parse `array` type in JSON. use
 * `JSON_parse_object` instead
 *
 * @param begin
 * @return JSON_Object
 */
static s2ujson::JSON_Object _JSON_parse_object_iter(
    std::string::const_iterator &begin) {
  auto iter = begin;
  for (; *iter == ' '; iter++)
    ;
  if (*iter != '{') throw object_is_invalid;
  iter++;
  s2ujson::JSON_Object object;
  bool required_key = false;
  bool required_value = false;
  std::string key;
  while (*iter != '\0') {
    switch (*iter) {
      case '}': {
        begin = ++iter;
        return object;
      }
      case 'n': {
        if (required_key && required_value) {
          object.add(key, _JSON_parse_null_iter(iter));
        } else {
          throw object_is_invalid;
        }
        break;
      }
      case 't': {
        if (required_key && required_value) {
          object.add(key, _JSON_parse_true_iter(iter));
        } else {
          throw object_is_invalid;
        }
        break;
      }
      case 'f': {
        if (required_key && required_value) {
          object.add(key, _JSON_parse_false_iter(iter));
        } else {
          throw object_is_invalid;
        }
        break;
      }
      case '\"': {
        if (!required_key) {
          auto end_pos = iter + 1;
          for (; *end_pos != '\"'; end_pos++)
            ;
          key = std::string(iter + 1, end_pos);
          required_key = true;
          iter = end_pos + 1;
        } else {
          object.add(key, _JSON_parse_string_iter(iter));
        }
        break;
      }
      case '[': {
        if (required_key && required_value) {
          object.add(key, _JSON_parse_array_iter(iter));
        } else {
          throw object_is_invalid;
        }
        break;
      }
      case '{': {
        if (required_key && required_value) {
          object.add(key, _JSON_parse_object_iter(iter));
        } else {
          throw object_is_invalid;
        }
        break;
      }
      case ':': {
        if (required_key && !required_value) {
          required_value = true;
          iter++;
        } else
          throw object_is_invalid;
        break;
      }
      case ',': {
        if (required_key && required_value) {
          required_key = false;
          required_value = false;
          key = "";
          iter++;
        } else {
          throw object_is_invalid;
        }
        break;
      }
      default: {
        if (required_key && required_value && *iter != ' ' && *iter != '\n') {
          object.add(key, _JSON_parse_number_iter(iter));
        } else {
          iter++;
        }
      }
    }
  }
  throw object_is_invalid;
}
inline static s2ujson::JSON_Object _JSON_parse_object_iter(
    std::string::const_iterator &&begin) {
  return _JSON_parse_object_iter(begin);
}
namespace s2ujson {
/**
 * @brief parse `object` type in JSON.
 *
 * @param json
 * @return JSON_Object
 */
inline static JSON_Object JSON_parse_object(const std::string &json) {
  return _JSON_parse_object_iter(json.cbegin());
}
inline static JSON_Object JSON_parse_object(const std::string &&json) {
  return JSON_parse_object(json);
}
}  // namespace s2ujson

static s2ujson::JSON_Data _JSON_parse(std::string::const_iterator &begin) {
  auto iter = begin;
  for (; *iter == ' '; iter++)
    ;
  if (*iter == '{') {
    return _JSON_parse_object_iter(iter);
  } else {
    return _JSON_parse_array_iter(iter);
  }
}
static s2ujson::JSON_Data _JSON_parse(std::string::const_iterator &&begin) {
  return _JSON_parse(begin);
}
namespace s2ujson {
inline static JSON_Data JSON_parse(const std::string &json) {
  return _JSON_parse(json.cbegin());
}
}  // namespace s2ujson

#endif