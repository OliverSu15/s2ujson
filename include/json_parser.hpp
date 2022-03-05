#ifndef _JSON_PARSER_H_
#define _JSON_PARSER_H_
#include <cassert>
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

//#include "json_data.h"
#include "json_object.hpp"

namespace s2ujson {

using string = std::string;

inline bool isHex(char ch) {
  return (std::isdigit(ch) != 0) ||
         ((ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F'));
}

inline unsigned int JSON_parse_string_hex_helper(std::string &&json) try {
  const int hex = 16;
  if (isHex(json[0]) && isHex(json[1]) && isHex(json[2]) && isHex(json[3])) {
    return std::stoul(json, nullptr, hex);
  }
  throw std::invalid_argument("Invalid Unicode HEX");
} catch (...) {
  throw std::invalid_argument("Invalid Unicode HEX");
}

std::string JSON_parse_string_utf_helper(unsigned int hex) {
  string result;
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
    if (hex > 0x10FFFF) throw std::invalid_argument("invalid HEX");
    result.push_back(0xF0 | ((hex >> 18) & 0xFF));
    result.push_back(0x80 | ((hex >> 12) & 0x3F));
    result.push_back(0x80 | ((hex >> 6) & 0x3F));
    result.push_back(0x80 | (hex & 0x3F));
  }

  return result;
}
// Parse True Part

bool JSON_parse_true_iter(std::string::const_iterator &begin) {
  auto iter = begin;
  while (*iter == ' ') {
    iter++;
  }
  if (*(iter++) == 't' && *(iter++) == 'r' && *(iter++) == 'u' &&
      *(iter++) == 'e') {
    begin = iter;
    return true;
  }
  throw std::invalid_argument("literial \"true\" is not correct");
}

inline bool JSON_parse_true(const std::string &&json) {
  auto iter = json.cbegin();
  return JSON_parse_true_iter(iter);
}

inline bool JSON_parse_true(const std::string &json) {
  auto iter = json.cbegin();
  return JSON_parse_true_iter(iter);
}

// Parse False Part

bool JSON_parse_false_iter(std::string::const_iterator &begin) {
  auto iter = begin;
  while (*iter == ' ') {
    iter++;
  }
  if (*(iter++) == 'f' && *(iter++) == 'a' && *(iter++) == 'l' &&
      *(iter++) == 's' && *(iter++) == 'e') {
    begin = iter;
    return false;
  }
  throw std::invalid_argument("literial \"false\" is not correct");
}

inline bool JSON_parse_false(const string &&json) {
  auto iter = json.cbegin();
  return JSON_parse_false_iter(iter);
}

inline bool JSON_parse_false(const string &json) {
  auto iter = json.cbegin();
  return JSON_parse_false_iter(iter);
}

// Parse Null Part
std::nullptr_t JSON_parse_null_iter(std::string::const_iterator &begin) {
  auto iter = begin;
  while (*iter == ' ') {
    iter++;
  }
  if (*(iter++) == 'n' && *(iter++) == 'u' && *(iter++) == 'l' &&
      *(iter++) == 'l') {
    begin = iter;
    return nullptr;
  }
  throw std::invalid_argument("literial \"null\" is not correct");
}

inline std::nullptr_t JSON_parse_null(const string &&json) {
  auto iter = json.cbegin();
  return JSON_parse_null_iter(iter);
}

inline std::nullptr_t JSON_parse_null(const string &json) {
  auto iter = json.cbegin();
  return JSON_parse_null_iter(iter);
}
// Parse Number Part
double JSON_parse_number_iter(std::string::const_iterator &begin) {
  auto end = begin;
  if (*end == '-') end++;
  if (*end == '0') {
    end++;
  } else {
    if (!(*end >= '1' && *end <= '9'))
      throw std::invalid_argument(
          "number is not correct");  // TODO may have a beeter explain
    while (std::isdigit(*end)) end++;
  }

  if (*end == '.') {
    end++;
    if (!std::isdigit(*end))
      throw std::invalid_argument(
          "number is not correct");  // TODO may have a beeter explain
    while (std::isdigit(*end)) end++;
  }
  if ((*end == 'e' || *end == 'E')) {
    end++;
    if ((*end == '+' || *end == '-')) end++;
    if (!std::isdigit(*end))
      throw std::invalid_argument(
          "number is not correct");  // TODO may have a beeter explain
    while (std::isdigit(*end)) end++;
  }
  // char a = *end;
  // char b = *(end + 10);
  // not a good design may need to change latter
  if ((*end != ',') && (*end != '\0') && (*end != ' ') && (*end != ']') &&
      (*end != '}') && (*end != '\n'))
    throw std::invalid_argument(
        "number is not correct");  // TODO may have a beeter explain
  string num_string(begin, end);
  begin = end;
  return std::stod(num_string);
}

inline double JSON_parse_number(const string &&json) {
  auto iter = json.cbegin();
  return JSON_parse_number_iter(iter);
}

inline double JSON_parse_number(const string &json) {
  auto iter = json.cbegin();
  return JSON_parse_number_iter(iter);
}

// Parse String Part
string JSON_parse_string_iter(std::string::const_iterator &begin) {
  auto iter = begin;
  // int pos = 0;
  string result = "";
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
            // might have INVALID_UNICODE_HEX
            unsigned int hex =
                JSON_parse_string_hex_helper(string(iter + 1, iter + 5));
            if (hex >= 0xD800 && hex <= 0xDBFF) {
              iter += 5;
              if (*iter != '\\')
                throw std::invalid_argument(
                    "invalid unicode surrogate");  // INVALID_UNICODE_SURROGATE
              iter++;
              if (*iter != 'u')
                throw std::invalid_argument(
                    "invalid unicode surrogate");  // INVALID_UNICODE_SURROGATE
              // pos += 1;
              //   might have INVALID_UNICODE_HEX
              unsigned int hex2 =
                  JSON_parse_string_hex_helper(string(iter + 1, iter + 5));
              if (hex2 < 0xDC00 || hex2 > 0xDFFF)
                throw std::invalid_argument(
                    "invalid unicode surrogate");  // INVALID_UNICODE_SURROGATE
              hex = (((hex - 0xD800) << 10) | (hex2 - 0xDC00)) + 0x10000;
            }
            if (hex == 0) {
              while (*iter != '\"') iter++;
              begin = ++iter;
              return result;
              // result.insert(, '\0');
              //  result.push_back('\u0000');
            } else {
              string utf_result = JSON_parse_string_utf_helper(hex);
              result.append(utf_result);
            }
            iter += 4;
            break;
          }
          default:
            // INVALID_STRING_ESCAPE
            throw std::invalid_argument(
                "invalid string escape");  // TODO may have a beeter explain
        }
        iter++;
        break;
      default:
        // INVALID_STRING_CHAR
        if (*iter < 0x20)
          throw std::invalid_argument(
              "invalid string char");  // TODO may have a beeter explain
        result.push_back(*iter);
        iter++;
        break;
    }
  }
  // LEPT_PARSE_MISS_QUOTATION_MARK
  throw std::invalid_argument(
      "miss quotation mark");  // TODO may have a beeter explain
}

inline string JSON_parse_string(const string &&json) {
  auto iter = json.cbegin();
  return JSON_parse_string_iter(iter);
}

inline string JSON_parse_string(const string &json) {
  auto iter = json.cbegin();
  return JSON_parse_string_iter(iter);
}

// Parse Array Part
// extern this function, so it can be used in JSON_parse_array_iter
JSON_Object JSON_parse_object_iter(std::string::const_iterator &begin);

std::vector<JSON_Data> JSON_parse_array_iter(
    std::string::const_iterator &begin) {
  auto iter = begin;
  if (*iter != '[') throw std::invalid_argument("array is invalid");
  iter++;
  auto next_start_pos = iter;
  auto next_end_pos = iter;
  // data_type next_data_type = data_type::NULL_DATA;
  std::vector<JSON_Data> array;
  while (*iter != '\0') {
    switch (*iter) {
      case ']':
        begin = iter + 1;
        return array;
      case 'n': {
        // if (next_start_pos == next_end_pos)
        array.emplace_back(JSON_parse_null(string(iter, iter + 4)));
        iter += 4;
        next_start_pos = next_end_pos = iter;
        break;
      }
      case 't': {
        // if (next_start_pos == next_end_pos)
        array.emplace_back(JSON_parse_true(string(iter, iter + 4)));
        iter += 4;
        next_start_pos = next_end_pos = iter;
        break;
      }
      case 'f': {
        // if (next_start_pos == next_end_pos)
        array.emplace_back(JSON_parse_false(string(iter, iter + 5)));
        iter += 5;
        next_start_pos = next_end_pos = iter;
        break;
      }
      case '\"': {
        array.emplace_back(JSON_parse_string_iter(iter));
        break;
      }
      case '[': {
        array.emplace_back(JSON_parse_array_iter(iter));
        break;
      }
      case '{': {
        array.emplace_back(JSON_parse_object_iter(iter));
        break;
      }
      default: {
        if (*iter != ' ' && *iter != ',' && *iter != '\n') {
          array.emplace_back(JSON_parse_number_iter(iter));
        } else
          iter++;
      }
    }
  }
  // if (json[pos] != ']')
  throw std::invalid_argument("array is invalid");
  // return array;
}

inline std::vector<JSON_Data> JSON_parse_array(const string &&json) {
  auto iter = json.cbegin();
  return JSON_parse_array_iter(iter);
}

inline std::vector<JSON_Data> JSON_parse_array(const string &json) {
  auto iter = json.cbegin();
  return JSON_parse_array_iter(iter);
}

// Parse Object Part
JSON_Object JSON_parse_object_iter(std::string::const_iterator &begin) {
  auto iter = begin;
  for (; *iter == ' '; iter++)
    ;
  if (*iter != '{') throw std::invalid_argument("object is invalid");
  iter++;
  JSON_Object object;
  bool required_key = false;
  bool required_value = false;
  string key;
  while (*iter != '\0') {
    switch (*iter) {
      case '}': {
        begin = ++iter;
        return object;
      }
      case 'n': {
        if (required_key && required_value) {
          object.add(key, JSON_parse_null_iter(iter));
        } else {
          throw std::invalid_argument("object is invalid");
        }
        break;
      }
      case 't': {
        if (required_key && required_value) {
          object.add(key, JSON_parse_true_iter(iter));
        } else {
          throw std::invalid_argument("object is invalid");
        }
        break;
      }
      case 'f': {
        if (required_key && required_value) {
          object.add(key, JSON_parse_false_iter(iter));
        } else {
          throw std::invalid_argument("object is invalid");
        }
        break;
      }
      case '\"': {
        if (!required_key) {
          auto end_pos = iter + 1;
          for (; *end_pos != '\"'; end_pos++)
            ;
          key = string(iter + 1, end_pos);
          required_key = true;
          iter = end_pos + 1;
        } else {
          object.add(key, JSON_parse_string_iter(iter));
          // required_key = false;
          // required_value = false;
          // key = "";
        }
        break;
      }
      case '[': {
        if (required_key && required_value) {
          object.add(key, JSON_parse_array_iter(iter));
          // required_key = false;
          // required_value = false;
          // key = "";
        } else {
          throw std::invalid_argument("object is invalid");
        }
        break;
      }
      case '{': {
        if (required_key && required_value) {
          object.add(key, JSON_parse_object_iter(iter));
          // required_key = false;
          // required_value = false;
          // key = "";
        } else {
          throw std::invalid_argument("object is invalid");
        }
        break;
      }
      case ':': {
        if (required_key && !required_value) {
          required_value = true;
          iter++;
        } else
          throw std::invalid_argument("object is invalid");
        break;
      }
      case ',': {
        if (required_key && required_value) {
          required_key = false;
          required_value = false;
          key = "";
          iter++;
        } else {
          throw std::invalid_argument("object is invalid");
        }
        break;
      }
      default: {
        if (required_key && required_value && *iter != ' ' &&
            *iter != '\n' /*&& *iter != ':'*/) {
          // auto end_pos = iter + 1;
          // for (; *end_pos != ' ' && *end_pos != ','; end_pos++)
          //{   ;
          object.add(key, JSON_parse_number_iter(iter));
          // required_key = false;
          // required_value = false;
          // key = "";
        } else {
          iter++;
        }
      }
    }
  }
  throw std::invalid_argument("object is invalid");
}

inline JSON_Object JSON_parse_object(const string &&json) {
  auto iter = json.cbegin();
  return JSON_parse_object_iter(iter);
}

inline JSON_Object JSON_parse_object(const string &json) {
  auto iter = json.cbegin();
  return JSON_parse_object_iter(iter);
}
}  // namespace s2ujson
#endif