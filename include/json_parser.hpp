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
#include "json_object.h"

namespace s2ujson {

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

// forward decalration, so it can be used in parsing array
static JSON_Object _JSON_parse_object_iter(std::string::const_iterator &begin);

/**
 * @brief private function that parse `array` type in JSON. use
 * `JSON_parse_array` instead
 *
 * @param begin
 * @return std::vector<JSON_Data>
 */
static std::vector<JSON_Data> _JSON_parse_array_iter(
    std::string::const_iterator &begin) {
  auto iter = begin;
  if (*iter != '[') throw array_is_invalid;
  iter++;
  auto next_start_pos = iter;
  auto next_end_pos = iter;
  std::vector<JSON_Data> array;
  while (*iter != '\0') {
    switch (*iter) {
      case ']':
        begin = iter + 1;
        return array;
      case 'n': {
        array.emplace_back(JSON_parse_null(std::string(iter, iter + 4)));
        iter += 4;
        next_start_pos = next_end_pos = iter;
        break;
      }
      case 't': {
        array.emplace_back(JSON_parse_true(std::string(iter, iter + 4)));
        iter += 4;
        next_start_pos = next_end_pos = iter;
        break;
      }
      case 'f': {
        array.emplace_back(JSON_parse_false(std::string(iter, iter + 5)));
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
inline static std::vector<JSON_Data> _JSON_parse_array_iter(
    std::string::const_iterator &&begin) {
  return _JSON_parse_array_iter(begin);
}
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

/**
 * @brief private function that parse `array` type in JSON. use
 * `JSON_parse_object` instead
 *
 * @param begin
 * @return JSON_Object
 */
static JSON_Object _JSON_parse_object_iter(std::string::const_iterator &begin) {
  auto iter = begin;
  for (; *iter == ' '; iter++)
    ;
  if (*iter != '{') throw object_is_invalid;
  iter++;
  JSON_Object object;
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
inline static JSON_Object _JSON_parse_object_iter(
    std::string::const_iterator &&begin) {
  return _JSON_parse_object_iter(begin);
}
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
#endif