#ifndef _JSON_DATA_HPP_
#define _JSON_DATA_HPP_

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

class JSON_Object;

enum class data_type { NULL_DATA, TRUE, FALSE, NUMBER, STRING, ARRAY, OBJECT };

class JSON_Data {
  using var = std::variant<std::nullptr_t, bool, double, int, std::string>;

 public:
  // Construct
  constexpr JSON_Data() { set(nullptr); }

  constexpr explicit JSON_Data(const bool d_bool) { set(d_bool); }
  constexpr JSON_Data(const double d_number) { set(d_number); }
  constexpr JSON_Data(const int d_number) {
    set(static_cast<double>(d_number));
  }
  constexpr JSON_Data(const std::string &d_string) { set(d_string); }
  constexpr JSON_Data(const std::string &&d_string) { set(d_string); }
  constexpr JSON_Data(const char *ptr) { set(std::string(ptr)); }
  constexpr JSON_Data(const std::vector<JSON_Data> &d_array) { set(d_array); }
  constexpr JSON_Data(const JSON_Object &d_object) { set(d_object); }
  constexpr JSON_Data(const std::nullptr_t n_ptr) { set(n_ptr); };
  JSON_Data(std::initializer_list<var> list) {
    std::vector<JSON_Data> array;
    for (auto i : list) {
      array.push_back(convert(i));
    }
    set(array);
  }
  JSON_Data(std::initializer_list<std::pair<std::string, JSON_Data>> list);

  inline bool &get_bool() { return std::get<bool>(data); }
  inline std::nullptr_t &get_null() { return std::get<std::nullptr_t>(data); }

  inline double &get_number() { return std::get<double>(data); }

  inline std::string &get_string() { return std::get<std::string>(data); }

  inline std::vector<JSON_Data> &get_array() {
    return std::get<std::vector<JSON_Data>>(data);
  }

  inline JSON_Object &get_object() {
    return *std::get<std::shared_ptr<JSON_Object>>(data);
  }

  inline void set(bool d_bool) {
    if (d_bool) {
      type = data_type::TRUE;
    } else {
      type = data_type::FALSE;
    }
    data = d_bool;
  }

  inline void set(double d_number) {
    type = data_type::NUMBER;
    data = d_number;
  }

  inline void set(const std::string &d_string) {
    type = data_type::STRING;
    data = d_string;
  }

  inline void set(const std::vector<JSON_Data> &d_array) {
    type = data_type::ARRAY;
    data = d_array;
  }

  inline void set(const JSON_Object &d_object) {
    type = data_type::OBJECT;
    data = std::make_shared<JSON_Object>(d_object);
  }

  inline void set(const std::nullptr_t) {
    type = data_type::NULL_DATA;
    data = nullptr;
  }

  template <typename T>
  inline const T &get() {
    return std::get<T>(data);
  }

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

  template <typename T>
  JSON_Data &operator=(std::initializer_list<T> list) {
    // std::vector<JSON_Data> array(list.size());
    // int pos = 0;
    // for (auto i : list) {
    //   array[pos] = i;
    //   pos++;
    // }
    // return operator=(array);
    this(list);
    return *this;
  }

  // template <typename Key, typename Value>
  // JSON_Data &operator=(
  //     std::initializer_list<
  //         std::pair<const std::string,
  //                   std::variant<std::nullptr_t, bool, double, int,
  //                   std::string,
  //                                char *, std::vector<JSON_Data>,
  //                                JSON_Object>>>
  //         list);

  // inline JSON_Data &operator=(std::initializer_list<JSON_Data> list) {
  //   return operator=(std::vector<JSON_Data>(list));
  // }

  inline JSON_Data &operator=(const JSON_Object &d_object) {
    set(d_object);
    return *this;
  }

  inline JSON_Data &operator=(const JSON_Object &&d_object) {
    return operator=(d_object);
  }

  JSON_Data &operator[](std::string &&key);
  JSON_Data &operator[](std::string &key);

  std::string array_output();

  std::string output();

 private:
  JSON_Data convert(var d_var) {
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

  std::variant<std::nullptr_t, bool, double, std::string,
               std::vector<JSON_Data>, std::shared_ptr<JSON_Object>>
      data;
  data_type type = data_type::NULL_DATA;
};

}  // namespace s2ujson

#endif