#ifndef _JSON_OBJECT_HPP_
#define _JSON_OBJECT_HPP_
#include <sys/types.h>

#include <cstddef>
#include <stdexcept>
#include <string>
#include <vector>

#include "json_data.hpp"
//#include "json_parser.hpp"
// TODO I think I need to do less on JSON_Object, and do more on JSON_Data. I
// should make the JSON_Data handle how to get value and how to set value, this
// related part in JSON_Object need to be just a portal.
namespace s2ujson {

class JSON_Object {
 public:
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

  inline void add(const std::string &key, const JSON_Object &d_object) {
    object.insert(std::make_pair(key, JSON_Data(d_object)));
  }
  inline void add(const std::string &&key, const JSON_Object &d_object) {
    add(key, d_object);
  }

  inline double get_number(const std::string &key) {
    is_key_valid(key);
    return object.find(key)->second.get_number();
  }
  inline double get_number(const std::string &&key) { return get_number(key); }

  inline std::string get_string(const std::string &key) {
    is_key_valid(key);
    return object.find(key)->second.get_string();
  }
  inline std::string get_string(const std::string &&key) {
    return get_string(key);
  }

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

  inline std::vector<JSON_Data> get_array(const std::string &key) {
    is_key_valid(key);
    return object.find(key)->second.get_array();
  }
  inline std::vector<JSON_Data> get_array(const std::string &&key) {
    return get_array(key);
  }

  inline JSON_Object get_object(const std::string &key) {
    is_key_valid(key);
    return *(object.find(key)->second.get_object());
  }
  inline JSON_Object get_object(const std::string &&key) {
    return get_object(key);
  }

  inline JSON_Data &operator[](std::string &key) {
    is_key_valid(key);
    return object.find(key)->second;
  }

  inline JSON_Data &operator[](std::string &&key) {
    is_key_valid(key);
    return object.find(key)->second;
  }

 private:
  std::map<std::string, JSON_Data> object;
  inline void is_key_valid(const std::string &key) {
    if (object.find(key) == object.end()) {
      throw std::invalid_argument("invalid key");
    }
  }
  inline void is_key_valid(const std::string &&key) { is_key_valid(key); }
};
}  // namespace s2ujson
#endif