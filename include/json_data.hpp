#ifndef _JSON_DATA_HPP_
#define _JSON_DATA_HPP_

#include <cctype>
#include <cstddef>
#include <ctime>
#include <exception>
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

// enum parse_result {
//   PARSE_OK = 0,
//   PARSE_EXPECT_VALUE,
//   PARSE_INVALID_VALUE,
//   PARSE_ROOT_NOT_SINGULAR,
//   PARSE_NUMBER_OUT_OF_RANGE,
// };

class JSON_Data {
 public:
  JSON_Data() { set(nullptr); }
  explicit JSON_Data(bool d_bool) { set(d_bool); }
  explicit JSON_Data(double d_number) { set(d_number); }
  explicit JSON_Data(const std::string &d_string) { set(d_string); }
  explicit JSON_Data(const std::vector<JSON_Data> &d_array) { set(d_array); }
  explicit JSON_Data(const JSON_Object &d_object) { set(d_object); }
  explicit JSON_Data(const std::nullptr_t n_ptr) { set(n_ptr); };
  inline bool &get_bool() {
    if (type == data_type::TRUE || type == data_type::FALSE) {
      return std::get<bool>(data);
    }
    throw std::bad_variant_access();  // TODOthrow type unmatched, just use the
                                      // variant exception, may change
  }
  // DONE didn't know how to handle Null in C++, need to change later
  inline std::nullptr_t &get_null() { return std::get<std::nullptr_t>(data); }

  inline double &get_number() {
    if (type == data_type::NUMBER) {
      return std::get<double>(data);
    }
    throw std::bad_variant_access();  // TODOthrow type unmatched, just use the
                                      // variant exception, may chang
  }

  inline std::string &get_string() {
    if (type == data_type::STRING) {
      return std::get<std::string>(data);
    }
    throw std::bad_variant_access();  // TODOthrow type unmatched
  }

  inline std::vector<JSON_Data> &get_array() {
    if (type == data_type::ARRAY) {
      return std::get<std::vector<JSON_Data>>(data);
    }
    throw std::bad_variant_access();  // TODO unmatched type
  }

  inline std::shared_ptr<JSON_Object> &get_object() {
    if (type == data_type::OBJECT) {
      return std::get<std::shared_ptr<JSON_Object>>(data);
    }
    throw std::bad_variant_access();  // TODO unmatched type
  }
  // TODO not sure is a good desgin for bool type and null type, might need to
  // have a new design
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
  // TODO due to the hardship to process null, I choose to have a separate
  // function. may change later
  inline void set(const std::nullptr_t) {
    type = data_type::NULL_DATA;
    data = nullptr;
  }

  template <typename T>
  inline const T &get() {
    return std::get<T>(data);
  }

  template <typename T>
  inline T &get() {
    return std::get<T>(data);
  }

  template <typename T>
  inline T get() {
    return std::get<T>(data);
  }

  inline JSON_Data &operator=(std::nullptr_t) {
    if (type == data_type::NULL_DATA) {
      data = nullptr;
      return *this;
    }
    throw std::invalid_argument("wrong type to set");
  }

  inline JSON_Data &operator=(const bool d_bool) {
    if (type == data_type::FALSE || type == data_type::TRUE) {
      data = d_bool;
      return *this;
    }
    throw std::invalid_argument("wrong type to set");
  }

  inline JSON_Data &operator=(const double d_number) {
    if (type == data_type::NUMBER) {
      data = d_number;
      return *this;
    }
    throw std::invalid_argument("wrong type to set");
  }

  inline JSON_Data &operator=(const std::string &d_string) {
    if (type == data_type::STRING) {
      data = d_string;
      return *this;
    }
    throw std::invalid_argument("wrong type to set");
  }

  inline JSON_Data &operator=(const std::vector<JSON_Data> &d_array) {
    if (type == data_type::ARRAY) {
      data = d_array;
      return *this;
    }
    throw std::invalid_argument("wrong type to set");
  }

  inline JSON_Data &operator=(const JSON_Object &d_object) {
    if (type == data_type::OBJECT) {
      data = std::make_shared<JSON_Object>(d_object);
      return *this;
    }
    throw std::invalid_argument("wrong type to set");
  }

 private:
  std::variant<std::nullptr_t, bool, double, std::string,
               std::vector<JSON_Data>, std::shared_ptr<JSON_Object>>
      data;
  data_type type{data_type::NULL_DATA};
};
}  // namespace s2ujson

#endif