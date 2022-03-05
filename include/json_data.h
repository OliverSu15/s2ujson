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
 public:
  // Construct
  constexpr JSON_Data() { set(nullptr); }

  constexpr explicit JSON_Data(const bool d_bool) { set(d_bool); }
  constexpr JSON_Data(const double d_number) { set(d_number); }
  // constexpr explicit JSON_Data(const int d_number) {
  //   set(static_cast<double>(d_number));
  // }
  constexpr JSON_Data(const int d_number) {
    set(static_cast<double>(d_number));
  }
  constexpr JSON_Data(const std::string &d_string) { set(d_string); }
  constexpr JSON_Data(const std::string &&d_string) { set(d_string); }
  constexpr JSON_Data(const char *ptr) { set(std::string(ptr)); }
  constexpr JSON_Data(const std::vector<JSON_Data> &d_array) { set(d_array); }
  constexpr JSON_Data(const JSON_Object &d_object) { set(d_object); }
  constexpr JSON_Data(const std::nullptr_t n_ptr) { set(n_ptr); };
  // JSON_Data(std::pair<std::string, JSON_Data> pair);
  // JSON_Data(std::initializer_list<std::pair<std::string, JSON_Data>> par) {}
  // template <typename T>
  JSON_Data(std::initializer_list<JSON_Data> list) {
    std::vector<JSON_Data> array(list.begin(), list.end());
    // int pos = 0;
    // for (auto i : list) {
    //   array[pos] = i;
    //   pos++;
    // }
    set(array);
  }

  inline bool &get_bool() {
    // if (type == data_type::TRUE || type == data_type::FALSE) {
    return std::get<bool>(data);
    //}
    // throw std::bad_variant_access();  // TODOthrow type unmatched, just use
    // the
    // variant exception, may change
  }
  // DONE didn't know how to handle Null in C++, need to change later
  inline std::nullptr_t &get_null() { return std::get<std::nullptr_t>(data); }

  inline double &get_number() {
    // if (type == data_type::NUMBER) {
    return std::get<double>(data);
    //}
    // throw std::bad_variant_access();  // TODOthrow type unmatched, just use
    // the variant exception, may chang
  }

  inline std::string &get_string() {
    // if (type == data_type::STRING) {
    return std::get<std::string>(data);
    //}
    // throw std::bad_variant_access();  // TODOthrow type unmatched
  }

  inline std::vector<JSON_Data> &get_array() {
    // if (type == data_type::ARRAY) {
    return std::get<std::vector<JSON_Data>>(data);
    //}
    // throw std::bad_variant_access();  // TODO unmatched type
  }

  inline JSON_Object &get_object() {
    // if (type == data_type::OBJECT) {
    return *std::get<std::shared_ptr<JSON_Object>>(data);
    //}
    // throw std::bad_variant_access();  // TODO unmatched type
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

  // inline JSON_Data &operator=(const char *ptr) {
  //   return operator=(std::string(ptr));
  // }

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
  std::variant<std::nullptr_t, bool, double, std::string,
               std::vector<JSON_Data>, std::shared_ptr<JSON_Object>>
      data;
  data_type type = data_type::NULL_DATA;
};

}  // namespace s2ujson

#endif