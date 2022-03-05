/**
 * @file json_data.h
 * @author OliverSu15 (OliverSu15@outlook.com)
 * @version 1.0
 * @date 2022-03-05
 *
 * @copyright Copyright (c) 2022
 *
 */

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

// forward declaration
class JSON_Object;

/**
 * @brief used to represent all the data type in JSON
 *
 */
enum class data_type { NULL_DATA, TRUE, FALSE, NUMBER, STRING, ARRAY, OBJECT };

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

  // All the setter
  inline void set(const std::nullptr_t) {
    type = data_type::NULL_DATA;
    data = nullptr;
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
  std::string output();

 private:
  /**
   * @brief output array style JSON string for array
   *
   * @return std::string
   */
  std::string array_output();
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
  data_type type = data_type::NULL_DATA;
};

}  // namespace s2ujson

#endif