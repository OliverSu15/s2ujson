/**
 * @file json_object.hpp
 * @author OliverSu15 (OliverSu15@outlook.com)
 * @brief
 * @version 1.0
 * @date 2022-03-05
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef _JSON_OBJECT_HPP_
#define _JSON_OBJECT_HPP_

#include <utility>

#include "json_data.h"
namespace s2ujson {

/**
 * @brief a working `data structure` to let the user can read the data in the
 * order they added
 *
 */
class JSON_Object_Storage {
 public:
  explicit JSON_Object_Storage() = default;
  inline void insert(const std::string &key, JSON_Data &data) {
    key_to_pos.insert(std::make_pair(key, key_to_pos.size()));
    if (real_data.size() >= key_to_pos.size()) {
      real_data[key_to_pos.size() - 1] = data;
    } else {
      real_data.push_back(data);
    }
  }
  inline void insert(const std::string &key, JSON_Data &&data) {
    insert(key, data);
  }

  inline JSON_Data &find(const std::string &key) {
    return real_data[key_to_pos.find(key)->second];
  }

  inline JSON_Data &find(const int pos) { return real_data[pos]; }

  inline bool is_key_invalid(const std::string &key) {
    return key_to_pos.find(key) == key_to_pos.end();
  }

  std::vector<std::pair<std::string, JSON_Data>> get_Storage_In_Order() {
    std::vector<std::pair<std::string, JSON_Data>> result(key_to_pos.size());
    for (auto i : key_to_pos) {
      result[i.second] = std::make_pair(i.first, real_data[i.second]);
    }
    return result;
  }

 private:
  std::map<std::string, int> key_to_pos;
  std::vector<JSON_Data> real_data;
};

class JSON_Object {
 public:
  // All Constructor
  explicit JSON_Object() = default;
  JSON_Object(std::initializer_list<std::pair<std::string, JSON_Data>> list);

  // All add
  inline void add(const std::string &key) {
    object.insert(std::make_pair(key, JSON_Data()));
  }
  inline void add(const std::string &&key) { add(key); }
  inline void add(const std::string &key, std::nullptr_t) {
    object.insert(std::make_pair(key, JSON_Data()));
  }
  inline void add(const std::string &&key, std::nullptr_t) {
    add(key, nullptr);
  }
  inline void add(const std::string &key, const bool d_bool) {
    object.insert(std::make_pair(key, JSON_Data(d_bool)));
  }
  inline void add(const std::string &&key, const bool d_bool) {
    add(key, d_bool);
  }
  inline void add(const std::string &key, const double d_number) {
    object.insert(std::make_pair(key, JSON_Data(d_number)));
  }
  inline void add(const std::string &&key, const double d_number) {
    add(key, d_number);
  }
  inline void add(const std::string &key, const std::string &d_string) {
    object.insert(std::make_pair(key, JSON_Data(d_string)));
  }
  inline void add(const std::string &&key, const std::string &d_string) {
    add(key, d_string);
  }
  inline void add(const std::string &key,
                  const std::vector<JSON_Data> &d_array) {
    object.insert(std::make_pair(key, JSON_Data(d_array)));
  }
  inline void add(const std::string &&key,
                  const std::vector<JSON_Data> &d_array) {
    add(key, d_array);
  }
  inline void add(const std::string &key, const JSON_Object &d_object) {
    object.insert(std::make_pair(key, JSON_Data(d_object)));
  }
  inline void add(const std::string &&key, const JSON_Object &d_object) {
    add(key, d_object);
  }
  inline void add(const std::string &key, const JSON_Data &d_data) {
    object.insert(std::make_pair(key, d_data));
  }
  inline void add(const std::string &&key, const JSON_Data &d_data) {
    add(key, d_data);
  }

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
  inline std::string output() {
    std::string output_string = "{";
    for (auto i : object) {
      output_string += ("\"" + i.first + "\"" + ":" + i.second.output() + ",");
    }
    output_string.push_back('}');
    return output_string;
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

}  // namespace s2ujson
#endif