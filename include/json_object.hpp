#ifndef _JSON_OBJECT_HPP_
#define _JSON_OBJECT_HPP_
#include <sys/types.h>

#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "json_data.h"
//#include "json_parser.hpp"
namespace s2ujson {

class JSON_Object_Storage {
 public:
  explicit JSON_Object_Storage() = default;
  explicit JSON_Object_Storage(std::uint64_t size) : real_data(size) {}
  inline void insert(const std::string &key, JSON_Data &&data) {
    insert(key, data);
  }
  inline void insert(const std::string &key, JSON_Data &data) {
    key_to_pos.insert(std::make_pair(key, key_to_pos.size()));
    if (real_data.size() >= key_to_pos.size()) {
      real_data[key_to_pos.size() - 1] = data;
    } else {
      real_data.push_back(data);
    }
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

class JSON_Ref {
  using var = std::variant<std::nullptr_t, bool, double, int, std::string>;

 public:
  JSON_Ref(std::nullptr_t) : data(nullptr) {}
  JSON_Ref(int number) : data(number) {}
  JSON_Ref(double number) : data(number) {}
  explicit JSON_Ref(bool d_bool) : data(d_bool) {}
  JSON_Ref(std::string d_string) : data(d_string) {}
  JSON_Ref(const char *ptr) : data(ptr) {}
  JSON_Ref(std::initializer_list<std::pair<std::string, JSON_Ref>> list);
  JSON_Ref(std::initializer_list<var> list) {
    std::vector<JSON_Data> d_array;
    for (auto i : list) {
      d_array.push_back(convert(i));
    }
    data = d_array;
  }
  //: data(list) {}
  JSON_Data data;

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
};
class JSON_Object {
 public:
  explicit JSON_Object() = default;
  JSON_Object(std::initializer_list<std::pair<std::string, JSON_Data>> list);
  JSON_Object &operator=(
      std::initializer_list<std::pair<std::string, JSON_Data>> list);
  inline void add(const std::string &key, const bool d_bool) {
    object.insert(key, JSON_Data(d_bool));
  }
  inline void add(const std::string &&key, const bool d_bool) {
    add(key, d_bool);
  }

  inline void add(const std::string &key, const double d_number) {
    object.insert(key, JSON_Data(d_number));
  }
  inline void add(const std::string &&key, const double d_number) {
    add(key, d_number);
  }

  inline void add(const std::string &key, const std::string &d_string) {
    object.insert(key, JSON_Data(d_string));
  }
  inline void add(const std::string &&key, const std::string &d_string) {
    add(key, d_string);
  }

  inline void add(const std::string &key,
                  const std::vector<JSON_Data> &d_array) {
    object.insert(key, JSON_Data(d_array));
  }
  inline void add(const std::string &&key,
                  const std::vector<JSON_Data> &d_array) {
    add(key, d_array);
  }

  inline void add(const std::string &key) { object.insert(key, JSON_Data()); }
  inline void add(const std::string &&key) { add(key); }

  inline void add(const std::string &key, std::nullptr_t) {
    object.insert(key, JSON_Data());
  }
  inline void add(const std::string &&key, std::nullptr_t) {
    add(key, nullptr);
  }

  inline void add(const std::string &key, const JSON_Object &d_object) {
    object.insert(key, JSON_Data(d_object));
  }
  inline void add(const std::string &&key, const JSON_Object &d_object) {
    add(key, d_object);
  }

  inline void push_back(const std::string &key, JSON_Data &d_data) {
    object.insert(key, d_data);
  }

  inline double get_number(const std::string &key) {
    is_key_valid(key);
    return object.find(key).get_number();
  }
  inline double get_number(const std::string &&key) { return get_number(key); }

  inline std::string get_string(const std::string &key) {
    is_key_valid(key);
    return object.find(key).get_string();
  }
  inline std::string get_string(const std::string &&key) {
    return get_string(key);
  }

  inline std::nullptr_t get_null(const std::string &key) {
    is_key_valid(key);
    return object.find(key).get_null();
  }
  inline std::nullptr_t get_null(const std::string &&key) {
    return get_null(key);
  }

  inline bool get_bool(const std::string &key) {
    is_key_valid(key);
    return object.find(key).get_bool();
  }
  inline bool get_bool(const std::string &&key) { return get_bool(key); }

  inline std::vector<JSON_Data> get_array(const std::string &key) {
    is_key_valid(key);
    return object.find(key).get_array();
  }
  inline std::vector<JSON_Data> get_array(const std::string &&key) {
    return get_array(key);
  }

  inline JSON_Object get_object(const std::string &key) {
    is_key_valid(key);
    return (object.find(key).get_object());
  }
  inline JSON_Object get_object(const std::string &&key) {
    return get_object(key);
  }

  inline JSON_Data &operator[](std::string &key) {
    // is_key_valid(key);
    if (!object.is_key_invalid(key)) {
      return object.find(key);
    }
    object.insert(key, JSON_Data());
    return object.find(key);
  }

  inline JSON_Data &operator[](std::string &&key) { return operator[](key); }

  inline std::string output() {
    auto data = object.get_Storage_In_Order();
    std::string output_string = "{";
    for (auto i : data) {
      output_string += ("\"" + i.first + "\"" + ":" + i.second.output() + ",");
    }
    output_string.push_back('}');
    return output_string;
  }

 private:
  JSON_Object_Storage object;
  inline void is_key_valid(const std::string &key) {
    if (object.is_key_invalid(key)) {
      throw std::invalid_argument("invalid key");
    }
  }
  inline void is_key_valid(const std::string &&key) { is_key_valid(key); }
};

}  // namespace s2ujson
#endif