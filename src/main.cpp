#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

#include "json_parser.hpp"

using namespace s2ujson;
// TODO make the API similar to JSON for Modern C++

int test_pass = 0;
int test_count = 0;

template <typename T>
void EXPECT_EQ_BASE(bool equality, int line_num, T expect, T actual) {
  test_count++;
  if (equality) {
    test_pass++;
  } else {
    std::cerr << line_num << ":" << expect << ":" << actual << std::endl;
  }
}
template <typename T>
void EXPECT_EXCEPTION(int line_num, std::string json, data_type type,
                      T expected) try {
  // might have a better way to handle it
  static_assert(std::is_base_of<std::exception, T>::value,
                "T is not a exception");
  test_count++;
  switch (type) {
    case data_type::NULL_DATA:
      JSON_parse_null(json);
      break;
    case data_type::TRUE:
      JSON_parse_true(json);
      break;
    case data_type::FALSE:
      JSON_parse_false(json);
      break;
    case data_type::NUMBER:
      JSON_parse_number(json);
      break;
    case data_type::STRING:
      JSON_parse_string(json);
      break;
    case data_type::ARRAY:
      JSON_parse_array(json);
      break;
    case data_type::OBJECT:
      // TODO
      break;
  }
  std::cerr << line_num << std::endl;
} catch (T &e) {
  string a = e.what();
  string b = expected.what();
  if (string(e.what()) == string(expected.what())) {
    test_pass++;
  } else {
    std::cerr << line_num << expected.what() << " : " << e.what() << std::endl;
  }
}

template <typename T>
void EXPECT_EQ_INT(int line_num, T expect, T actual) {
  EXPECT_EQ_BASE(expect == actual, line_num, expect, actual);
}

void test_parse_null() {
  EXPECT_EQ_INT(__LINE__, nullptr, JSON_parse_null("null"));
  EXPECT_EXCEPTION(__LINE__, "nall", data_type::NULL_DATA,
                   std::invalid_argument("literial \"null\" is not correct"));

  string test = "null";
  auto iter = test.cbegin();
  EXPECT_EQ_INT(__LINE__, nullptr, JSON_parse_null_iter(iter));
}

void test_parse_true() {
  EXPECT_EQ_INT(__LINE__, true, JSON_parse_true("true"));
  EXPECT_EXCEPTION(__LINE__, "taue", data_type::TRUE,
                   std::invalid_argument("literial \"true\" is not correct"));
  string test = "true";
  auto iter = test.cbegin();
  EXPECT_EQ_INT(__LINE__, true, JSON_parse_true_iter(iter));
}

void test_parse_false() {
  EXPECT_EQ_INT(__LINE__, false, JSON_parse_false("false"));
  EXPECT_EXCEPTION(__LINE__, "fulsr", data_type::FALSE,
                   std::invalid_argument("literial \"false\" is not correct"));

  string test = "false";
  auto iter = test.cbegin();
  EXPECT_EQ_INT(__LINE__, false, JSON_parse_false_iter(iter));
}

void TEST_NUMBER(int line_num, double expected, string json) {
  auto iter = json.cbegin();
  EXPECT_EQ_INT(line_num, expected, JSON_parse_number_iter(iter));
}

void test_parse_number() {
  EXPECT_EQ_INT(__LINE__, 0.0, JSON_parse_number("0"));
  EXPECT_EQ_INT(__LINE__, 0.0, JSON_parse_number("-0"));
  EXPECT_EQ_INT(__LINE__, 0.0, JSON_parse_number("-0.0"));
  EXPECT_EQ_INT(__LINE__, 1.0, JSON_parse_number("1"));
  EXPECT_EQ_INT(__LINE__, -1.0, JSON_parse_number("-1"));
  EXPECT_EQ_INT(__LINE__, 1.5, JSON_parse_number("1.5"));
  EXPECT_EQ_INT(__LINE__, -1.5, JSON_parse_number("-1.5"));
  EXPECT_EQ_INT(__LINE__, 3.1416, JSON_parse_number("3.1416"));
  EXPECT_EQ_INT(__LINE__, 1E10, JSON_parse_number("1E10"));
  EXPECT_EQ_INT(__LINE__, 1e10, JSON_parse_number("1e10"));
  EXPECT_EQ_INT(__LINE__, 1E+10, JSON_parse_number("1E+10"));
  EXPECT_EQ_INT(__LINE__, 1E-10, JSON_parse_number("1E-10"));
  EXPECT_EQ_INT(__LINE__, -1E10, JSON_parse_number("-1E10"));
  EXPECT_EQ_INT(__LINE__, -1e10, JSON_parse_number("-1e10"));
  EXPECT_EQ_INT(__LINE__, -1E+10, JSON_parse_number("-1E+10"));
  EXPECT_EQ_INT(__LINE__, -1E-10, JSON_parse_number("-1E-10"));
  EXPECT_EQ_INT(__LINE__, 1.234E+10, JSON_parse_number("1.234E+10"));
  EXPECT_EQ_INT(__LINE__, 1.234E-10, JSON_parse_number("1.234E-10"));

  EXPECT_EQ_INT(
      __LINE__, 1.0000000000000002,
      JSON_parse_number("1.0000000000000002")); /* the smallest number > 1 */
  EXPECT_EQ_INT(
      __LINE__, 2.2250738585072014e-308,
      JSON_parse_number(
          "2.2250738585072014e-308")); /* Min normal positive double */
  EXPECT_EQ_INT(__LINE__, -2.2250738585072014e-308,
                JSON_parse_number("-2.2250738585072014e-308"));
  EXPECT_EQ_INT(__LINE__, 1.7976931348623157e+308,
                JSON_parse_number("1.7976931348623157e+308")); /* Max double */
  EXPECT_EQ_INT(__LINE__, -1.7976931348623157e+308,
                JSON_parse_number("-1.7976931348623157e+308"));
  //------------------------------
  TEST_NUMBER(__LINE__, 0.0, ("0"));
  TEST_NUMBER(__LINE__, 0.0, ("-0"));
  TEST_NUMBER(__LINE__, 0.0, ("-0.0"));
  TEST_NUMBER(__LINE__, 1.0, ("1"));
  TEST_NUMBER(__LINE__, -1.0, ("-1"));
  TEST_NUMBER(__LINE__, 1.5, ("1.5"));
  TEST_NUMBER(__LINE__, -1.5, ("-1.5"));
  TEST_NUMBER(__LINE__, 3.1416, ("3.1416"));
  TEST_NUMBER(__LINE__, 1E10, ("1E10"));
  TEST_NUMBER(__LINE__, 1e10, ("1e10"));
  TEST_NUMBER(__LINE__, 1E+10, ("1E+10"));
  TEST_NUMBER(__LINE__, 1E-10, ("1E-10"));
  TEST_NUMBER(__LINE__, -1E10, ("-1E10"));
  TEST_NUMBER(__LINE__, -1e10, ("-1e10"));
  TEST_NUMBER(__LINE__, -1E+10, ("-1E+10"));
  TEST_NUMBER(__LINE__, -1E-10, ("-1E-10"));
  TEST_NUMBER(__LINE__, 1.234E+10, ("1.234E+10"));
  TEST_NUMBER(__LINE__, 1.234E-10, ("1.234E-10"));

  TEST_NUMBER(__LINE__, 1.0000000000000002,
              ("1.0000000000000002")); /* the smallest number > 1 */
  TEST_NUMBER(__LINE__, 2.2250738585072014e-308,
              ("2.2250738585072014e-308")); /* Min normal positive double */
  TEST_NUMBER(__LINE__, -2.2250738585072014e-308, ("-2.2250738585072014e-308"));
  TEST_NUMBER(__LINE__, 1.7976931348623157e+308,
              ("1.7976931348623157e+308")); /* Max double */
  TEST_NUMBER(__LINE__, -1.7976931348623157e+308, ("-1.7976931348623157e+308"));
}
void test_parse_number_out_of_range() {
  EXPECT_EXCEPTION(__LINE__, "1e-10000", data_type::NUMBER,
                   std::out_of_range("stod"));
  EXPECT_EXCEPTION(__LINE__, "4.9406564584124654e-324", data_type::NUMBER,
                   std::out_of_range("stod"));
  EXPECT_EXCEPTION(__LINE__, "-4.9406564584124654e-324", data_type::NUMBER,
                   std::out_of_range("stod"));
  EXPECT_EXCEPTION(__LINE__, "2.2250738585072009e-308", data_type::NUMBER,
                   std::out_of_range("stod"));
  EXPECT_EXCEPTION(__LINE__, "-2.2250738585072009e-308", data_type::NUMBER,
                   std::out_of_range("stod"));
  EXPECT_EXCEPTION(__LINE__, "1e309", data_type::NUMBER,
                   std::out_of_range("stod"));
  EXPECT_EXCEPTION(__LINE__, "-1e309", data_type::NUMBER,
                   std::out_of_range("stod"));
}

void test_parse_invalid_argument() {
  EXPECT_EXCEPTION(__LINE__, "+0", data_type::NUMBER,
                   std::invalid_argument("number is not correct"));
  EXPECT_EXCEPTION(__LINE__, "+1", data_type::NUMBER,
                   std::invalid_argument("number is not correct"));
  EXPECT_EXCEPTION(
      __LINE__, ".123", data_type::NUMBER,
      std::invalid_argument(
          "number is not correct")); /* at least one digit before '.' */
  EXPECT_EXCEPTION(
      __LINE__, "1.", data_type::NUMBER,
      std::invalid_argument(
          "number is not correct")); /* at least one digit after '.' */
  EXPECT_EXCEPTION(__LINE__, "INF", data_type::NUMBER,
                   std::invalid_argument("number is not correct"));
  EXPECT_EXCEPTION(__LINE__, "inf", data_type::NUMBER,
                   std::invalid_argument("number is not correct"));
  EXPECT_EXCEPTION(__LINE__, "NAN", data_type::NUMBER,
                   std::invalid_argument("number is not correct"));
  EXPECT_EXCEPTION(__LINE__, "nan", data_type::NUMBER,
                   std::invalid_argument("number is not correct"));
  EXPECT_EXCEPTION(__LINE__, string("0123"), data_type::NUMBER,
                   std::invalid_argument("number is not correct"));
  EXPECT_EXCEPTION(__LINE__, "0x0", data_type::NUMBER,
                   std::invalid_argument("number is not correct"));
  EXPECT_EXCEPTION(__LINE__, "0x123", data_type::NUMBER,
                   std::invalid_argument("number is not correct"));
}

void TEST_STRING(int line_num, string expected, string json) {
  auto iter = json.cbegin();
  EXPECT_EQ_INT(line_num, expected, JSON_parse_string_iter(iter));
}

void test_parse_string() {
  EXPECT_EQ_INT(__LINE__, string(""), JSON_parse_string("\"\""));
  EXPECT_EQ_INT(__LINE__, string("Hello"), JSON_parse_string("\"Hello\""));
  EXPECT_EQ_INT(__LINE__, string("Hello\nWorld"),
                JSON_parse_string("\"Hello\\nWorld\""));
  EXPECT_EQ_INT(__LINE__, string("\" \\ / \b \f \n \r \t"),
                JSON_parse_string("\"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\""));
  EXPECT_EQ_INT(__LINE__, string("Hello\0World"),
                JSON_parse_string("\"Hello\\u0000World\""));
  EXPECT_EQ_INT(__LINE__, string("\x24"),
                JSON_parse_string("\"\\u0024\"")); /* Dollar sign U+0024*/
  EXPECT_EQ_INT(__LINE__, string("\xC2\xA2"),
                JSON_parse_string("\"\\u00A2\"")); /* Cents sign U+00A2 */
  EXPECT_EQ_INT(__LINE__, string("\xE2\x82\xAC"),
                JSON_parse_string("\"\\u20AC\"")); /* Euro sign U+20AC */
  EXPECT_EQ_INT(
      __LINE__, string("\xF0\x9D\x84\x9E"),
      JSON_parse_string("\"\\uD834\\uDD1E\"")); /* G clef sign U+1D11E */
  EXPECT_EQ_INT(
      __LINE__, string("\xF0\x9D\x84\x9E"),
      JSON_parse_string("\"\\ud834\\udd1e\"")); /* G clef sign U+1D11E */
  //------------------------------------------
  TEST_STRING(__LINE__, string(""), ("\"\""));
  TEST_STRING(__LINE__, string("Hello"), ("\"Hello\""));
  TEST_STRING(__LINE__, string("Hello\nWorld"), ("\"Hello\\nWorld\""));
  TEST_STRING(__LINE__, string("\" \\ / \b \f \n \r \t"),
              ("\"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\""));
  TEST_STRING(__LINE__, string("Hello\0World"), ("\"Hello\\u0000World\""));
  TEST_STRING(__LINE__, string("\x24"),
              ("\"\\u0024\"")); /* Dollar sign U+0024*/
  TEST_STRING(__LINE__, string("\xC2\xA2"),
              ("\"\\u00A2\"")); /* Cents sign U+00A2 */
  TEST_STRING(__LINE__, string("\xE2\x82\xAC"),
              ("\"\\u20AC\"")); /* Euro sign U+20AC */
  TEST_STRING(__LINE__, string("\xF0\x9D\x84\x9E"),
              ("\"\\uD834\\uDD1E\"")); /* G clef sign U+1D11E */
  TEST_STRING(__LINE__, string("\xF0\x9D\x84\x9E"),
              ("\"\\ud834\\udd1e\"")); /* G clef sign U+1D11E */
}

void test_parse_string_error() {
  // LEPT_PARSE_MISS_QUOTATION_MARK
  EXPECT_EXCEPTION(__LINE__, "\"", data_type::STRING,
                   std::invalid_argument("miss quotation mark"));
  EXPECT_EXCEPTION(__LINE__, "\"abc", data_type::STRING,
                   std::invalid_argument("miss quotation mark"));

  // LEPT_PARSE_INVALID_STRING_ESCAPE
  EXPECT_EXCEPTION(__LINE__, "\"\\v\"", data_type::STRING,
                   std::invalid_argument("invalid string escape"));
  EXPECT_EXCEPTION(__LINE__, "\"\\'\"", data_type::STRING,
                   std::invalid_argument("invalid string escape"));
  EXPECT_EXCEPTION(__LINE__, "\"\\0\"", data_type::STRING,
                   std::invalid_argument("invalid string escape"));
  EXPECT_EXCEPTION(__LINE__, "\"\\x12\"", data_type::STRING,
                   std::invalid_argument("invalid string escape"));

  // LEPT_PARSE_INVALID_STRING_CHAR
  EXPECT_EXCEPTION(__LINE__, "\"\x01\"", data_type::STRING,
                   std::invalid_argument("invalid string char"));
  EXPECT_EXCEPTION(__LINE__, "\"\x1F\"", data_type::STRING,
                   std::invalid_argument("invalid string char"));

  // LEPT_PARSE_INVALID_UNICODE_HEX
  EXPECT_EXCEPTION(__LINE__, "\"\\u\"", data_type::STRING,
                   std::invalid_argument("Invalid Unicode HEX"));
  EXPECT_EXCEPTION(__LINE__, "\"\\u0\"", data_type::STRING,
                   std::invalid_argument("Invalid Unicode HEX"));
  EXPECT_EXCEPTION(__LINE__, "\"\\u01\"", data_type::STRING,
                   std::invalid_argument("Invalid Unicode HEX"));
  EXPECT_EXCEPTION(__LINE__, "\"\\u012\"", data_type::STRING,
                   std::invalid_argument("Invalid Unicode HEX"));
  EXPECT_EXCEPTION(__LINE__, "\"\\u/000\"", data_type::STRING,
                   std::invalid_argument("Invalid Unicode HEX"));
  EXPECT_EXCEPTION(__LINE__, "\"\\uG000\"", data_type::STRING,
                   std::invalid_argument("Invalid Unicode HEX"));
  EXPECT_EXCEPTION(__LINE__, "\"\\u0/00\"", data_type::STRING,
                   std::invalid_argument("Invalid Unicode HEX"));
  EXPECT_EXCEPTION(__LINE__, "\"\\u0G00\"", data_type::STRING,
                   std::invalid_argument("Invalid Unicode HEX"));
  EXPECT_EXCEPTION(__LINE__, "\"\\u00/0\"", data_type::STRING,
                   std::invalid_argument("Invalid Unicode HEX"));
  EXPECT_EXCEPTION(__LINE__, "\"\\u00G0\"", data_type::STRING,
                   std::invalid_argument("Invalid Unicode HEX"));
  EXPECT_EXCEPTION(__LINE__, "\"\\u000/\"", data_type::STRING,
                   std::invalid_argument("Invalid Unicode HEX"));
  EXPECT_EXCEPTION(__LINE__, "\"\\u000G\"", data_type::STRING,
                   std::invalid_argument("Invalid Unicode HEX"));
  EXPECT_EXCEPTION(__LINE__, "\"\\u 123\"", data_type::STRING,
                   std::invalid_argument("Invalid Unicode HEX"));

  // LEPT_PARSE_INVALID_UNICODE_SURROGATE
  EXPECT_EXCEPTION(__LINE__, "\"\\uD800\"", data_type::STRING,
                   std::invalid_argument("invalid unicode surrogate"));
  EXPECT_EXCEPTION(__LINE__, "\"\\uDBFF\"", data_type::STRING,
                   std::invalid_argument("invalid unicode surrogate"));
  EXPECT_EXCEPTION(__LINE__, "\"\\uD800\\\\\"", data_type::STRING,
                   std::invalid_argument("invalid unicode surrogate"));
  EXPECT_EXCEPTION(__LINE__, "\"\\uD800\\uDBFF\"", data_type::STRING,
                   std::invalid_argument("invalid unicode surrogate"));
  EXPECT_EXCEPTION(__LINE__, "\"\\uD800\\uE000\"", data_type::STRING,
                   std::invalid_argument("invalid unicode surrogate"));
}

void test_parse_array() {
  auto array = JSON_parse_array("[ ]");
  EXPECT_EQ_INT(__LINE__, true, array.empty());

  array = JSON_parse_array("[ null , false , true , 123 , \"abc\" ]");
  EXPECT_EQ_INT(__LINE__, nullptr, array[0].get_null());
  EXPECT_EQ_INT(__LINE__, false, array[1].get_bool());
  EXPECT_EQ_INT(__LINE__, true, array[2].get_bool());
  EXPECT_EQ_INT(__LINE__, string("abc"), array[4].get_string());
  EXPECT_EQ_INT(__LINE__, 123.0, array[3].get_number());

  array = JSON_parse_array("[ [ ] , [ 0 ] , [ 0 , 1 ] , [ 0 , 1 , 2 ] ]");
  EXPECT_EQ_INT(__LINE__, true, array[0].get_array().empty());
  EXPECT_EQ_INT(__LINE__, 0.0, array[1].get_array()[0].get_number());
  EXPECT_EQ_INT(__LINE__, 0.0, array[2].get_array()[0].get_number());
  EXPECT_EQ_INT(__LINE__, 1.0, array[2].get_array()[1].get_number());
  EXPECT_EQ_INT(__LINE__, 0.0, array[3].get_array()[0].get_number());
  EXPECT_EQ_INT(__LINE__, 1.0, array[3].get_array()[1].get_number());
  EXPECT_EQ_INT(__LINE__, 2.0, array[3].get_array()[2].get_number());

  array = JSON_parse_array("[0, -0, -0.0,1, -1,1.5, -1.5, 3.1416,1E10,1e10 ]");
  EXPECT_EQ_INT(__LINE__, 0.0, array[0].get_number());
  EXPECT_EQ_INT(__LINE__, -0.0, array[1].get_number());
  EXPECT_EQ_INT(__LINE__, -0.0, array[2].get_number());
  EXPECT_EQ_INT(__LINE__, 1.0, array[3].get_number());
  EXPECT_EQ_INT(__LINE__, -1.0, array[4].get_number());
  EXPECT_EQ_INT(__LINE__, 1.5, array[5].get_number());
  EXPECT_EQ_INT(__LINE__, -1.5, array[6].get_number());
  EXPECT_EQ_INT(__LINE__, 3.1416, array[7].get_number());
  EXPECT_EQ_INT(__LINE__, 1E10, array[8].get_number());
  EXPECT_EQ_INT(__LINE__, 1e10, array[9].get_number());

  // array = JSON_parse_array("[1,[1,[]]]");

  EXPECT_EXCEPTION(__LINE__, "[\"a\", nul]", data_type::ARRAY,
                   std::invalid_argument("literial \"null\" is not correct"));

  //-------------------
  string test = "[ ]";
  auto iter = test.cbegin();
  auto array2 = JSON_parse_array_iter(iter);
  EXPECT_EQ_INT(__LINE__, true, array2.empty());

  test = "[ null , false , true , 123 , \"abc\" ]";
  iter = test.cbegin();
  array2 = JSON_parse_array_iter(iter);
  EXPECT_EQ_INT(__LINE__, nullptr, array2[0].get_null());
  EXPECT_EQ_INT(__LINE__, false, array2[1].get_bool());
  EXPECT_EQ_INT(__LINE__, true, array2[2].get_bool());
  EXPECT_EQ_INT(__LINE__, string("abc"), array2[4].get_string());
  EXPECT_EQ_INT(__LINE__, 123.0, array2[3].get_number());

  test = "[ [ ] , [ 0 ] , [ 0 , 1 ] , [ 0 , 1 , 2 ] ]";
  iter = test.cbegin();
  array2 = JSON_parse_array_iter(iter);
  EXPECT_EQ_INT(__LINE__, true, array2[0].get_array().empty());
  EXPECT_EQ_INT(__LINE__, 0.0, array2[1].get_array()[0].get_number());
  EXPECT_EQ_INT(__LINE__, 0.0, array2[2].get_array()[0].get_number());
  EXPECT_EQ_INT(__LINE__, 1.0, array2[2].get_array()[1].get_number());
  EXPECT_EQ_INT(__LINE__, 0.0, array2[3].get_array()[0].get_number());
  EXPECT_EQ_INT(__LINE__, 1.0, array2[3].get_array()[1].get_number());
  EXPECT_EQ_INT(__LINE__, 2.0, array2[3].get_array()[2].get_number());

  test = "[0, -0, -0.0,1, -1,1.5, -1.5, 3.1416,1E10,1e10 ]";
  iter = test.cbegin();
  array2 = JSON_parse_array_iter(iter);
  EXPECT_EQ_INT(__LINE__, 0.0, array2[0].get_number());
  EXPECT_EQ_INT(__LINE__, -0.0, array2[1].get_number());
  EXPECT_EQ_INT(__LINE__, -0.0, array2[2].get_number());
  EXPECT_EQ_INT(__LINE__, 1.0, array2[3].get_number());
  EXPECT_EQ_INT(__LINE__, -1.0, array2[4].get_number());
  EXPECT_EQ_INT(__LINE__, 1.5, array2[5].get_number());
  EXPECT_EQ_INT(__LINE__, -1.5, array2[6].get_number());
  EXPECT_EQ_INT(__LINE__, 3.1416, array2[7].get_number());
  EXPECT_EQ_INT(__LINE__, 1E10, array2[8].get_number());
  EXPECT_EQ_INT(__LINE__, 1e10, array2[9].get_number());

  test = "[1,[1,[]]]";
  iter = test.cbegin();
  array2 = JSON_parse_array_iter(iter);
  EXPECT_EQ_INT(__LINE__, 1.0, array2[0].get_number());
}

void test_parse_object() {
  string test =
      "  { \"n\" : null , \"f\" : false , \"t\" : true , \"i\" : 123 , \"s\" "
      ": "
      "\"abc\", \"a\" : [ 1, 2, 3 ],\"o\" : {\"1\" : 1, \"2\" : 2, \"3\" : 3 "
      "}}";
  auto iter = test.cbegin();
  JSON_Object object2 = JSON_parse_object_iter(iter);

  EXPECT_EQ_INT(__LINE__, nullptr, object2.get_null("n"));
  EXPECT_EQ_INT(__LINE__, false, object2.get_bool("f"));
  EXPECT_EQ_INT(__LINE__, true, object2.get_bool("t"));
  EXPECT_EQ_INT(__LINE__, 123.0, object2.get_number("i"));
  EXPECT_EQ_INT(__LINE__, string("abc"), object2.get_string("s"));
  object2["i"] = 546.0;
  EXPECT_EQ_INT(__LINE__, nullptr, object2.get_null("n"));
  EXPECT_EQ_INT(__LINE__, false, object2.get_bool("f"));
  EXPECT_EQ_INT(__LINE__, true, object2.get_bool("t"));
  EXPECT_EQ_INT(__LINE__, 123.0, object2.get_number("i"));
  EXPECT_EQ_INT(__LINE__, string("abc"), object2.get_string("s"));

  test = R"({
      "glossary" : {
        "title" : "example glossary",
        "GlossDiv" : {
          "title" : "S",
          "GlossList" : {
            "GlossEntry" : {
              "ID" : "SGML",
              "SortAs" : "SGML",
              "GlossTerm" : "Standard Generalized Markup Language",
              "Acronym" : "SGML",
              "Abbrev" : "ISO 8879:1986",
              "GlossDef" : {
                "para" : "A meta-markup language, used to create markup languages such as DocBook.",
                "GlossSeeAlso" : [ "GML", "XML" ]
              },
              "GlossSee" : "markup"
            }
          }
        }
      }
    })";
  iter = test.cbegin();
  object2 = JSON_parse_object_iter(iter);
  JSON_Object ob = object2.get_object("glossary");

  test = R"({"widget": {
    "debug": "on",
    "window": {
        "title": "Sample Konfabulator Widget",
        "name": "main_window",
        "width": 500,
        "height": 500
    },
    "image": { 
        "src": "Images/Sun.png",
        "name": "sun1",
        "hOffset": 250,
        "vOffset": 250,
        "alignment": "center"
    },
    "text": {
        "data": "Click Here",
        "size": 36,
        "style": "bold",
        "name": "text1",
        "hOffset": 250,
        "vOffset": 100,
        "alignment": "center",
        "onMouseUp": "sun1.opacity = (sun1.opacity / 100) * 90;"
    }
}}   )";
  iter = test.cbegin();
  object2 = JSON_parse_object_iter(iter);
  ob = object2;

  test = R"({"web-app": {
  "servlet": [
    {
      "servlet-name": "cofaxCDS",
      "servlet-class": "org.cofax.cds.CDSServlet",
      "init-param": {
        "configGlossary:installationAt": "Philadelphia, PA",
        "configGlossary:adminEmail": "ksm@pobox.com",
        "configGlossary:poweredBy": "Cofax",
        "configGlossary:poweredByIcon": "/images/cofax.gif",
        "configGlossary:staticPath": "/content/static",
        "templateProcessorClass": "org.cofax.WysiwygTemplate",
        "templateLoaderClass": "org.cofax.FilesTemplateLoader",
        "templatePath": "templates",
        "templateOverridePath": "",
        "defaultListTemplate": "listTemplate.htm",
        "defaultFileTemplate": "articleTemplate.htm",
        "useJSP": false,
        "jspListTemplate": "listTemplate.jsp",
        "jspFileTemplate": "articleTemplate.jsp",
        "cachePackageTagsTrack": 200,
        "cachePackageTagsStore": 200,
        "cachePackageTagsRefresh": 60,
        "cacheTemplatesTrack": 100,
        "cacheTemplatesStore": 50,
        "cacheTemplatesRefresh": 15,
        "cachePagesTrack": 200,
        "cachePagesStore": 100,
        "cachePagesRefresh": 10,
        "cachePagesDirtyRead": 10,
        "searchEngineListTemplate": "forSearchEnginesList.htm",
        "searchEngineFileTemplate": "forSearchEngines.htm",
        "searchEngineRobotsDb": "WEB-INF/robots.db",
        "useDataStore": true,
        "dataStoreClass": "org.cofax.SqlDataStore",
        "redirectionClass": "org.cofax.SqlRedirection",
        "dataStoreName": "cofax",
        "dataStoreDriver": "com.microsoft.jdbc.sqlserver.SQLServerDriver",
        "dataStoreUrl": "jdbc:microsoft:sqlserver://LOCALHOST:1433;DatabaseName=goon",
        "dataStoreUser": "sa",
        "dataStorePassword": "dataStoreTestQuery",
        "dataStoreTestQuery": "SET NOCOUNT ON;select test='test';",
        "dataStoreLogFile": "/usr/local/tomcat/logs/datastore.log",
        "dataStoreInitConns": 10,
        "dataStoreMaxConns": 100,
        "dataStoreConnUsageLimit": 100,
        "dataStoreLogLevel": "debug",
        "maxUrlLength": 500}},
    {
      "servlet-name": "cofaxEmail",
      "servlet-class": "org.cofax.cds.EmailServlet",
      "init-param": {
      "mailHost": "mail1",
      "mailHostOverride": "mail2"}},
    {
      "servlet-name": "cofaxAdmin",
      "servlet-class": "org.cofax.cds.AdminServlet"},
 
    {
      "servlet-name": "fileServlet",
      "servlet-class": "org.cofax.cds.FileServlet"},
    {
      "servlet-name": "cofaxTools",
      "servlet-class": "org.cofax.cms.CofaxToolsServlet",
      "init-param": {
        "templatePath": "toolstemplates/",
        "log": 1,
        "logLocation": "/usr/local/tomcat/logs/CofaxTools.log",
        "logMaxSize": "",
        "dataLog": 1,
        "dataLogLocation": "/usr/local/tomcat/logs/dataLog.log",
        "dataLogMaxSize": "",
        "removePageCache": "/content/admin/remove?cache=pages&id=",
        "removeTemplateCache": "/content/admin/remove?cache=templates&id=",
        "fileTransferFolder": "/usr/local/tomcat/webapps/content/fileTransferFolder",
        "lookInContext": 1,
        "adminGroupID": 4,
        "betaServer": true}}],
  "servlet-mapping": {
    "cofaxCDS": "/",
    "cofaxEmail": "/cofaxutil/aemail/*",
    "cofaxAdmin": "/admin/*",
    "fileServlet": "/static/*",
    "cofaxTools": "/tools/*"},
 
  "taglib": {
    "taglib-uri": "cofax.tld",
    "taglib-location": "/WEB-INF/tlds/cofax.tld"}}})";
  iter = test.cbegin();
  object2 = JSON_parse_object_iter(iter);
  ob = object2;

  test = R"({"menu": {
    "header": "SVG Viewer",
    "items": [
        {"id": "Open"},
        {"id": "OpenNew", "label": "Open New"},
        null,
        {"id": "ZoomIn", "label": "Zoom In"},
        {"id": "ZoomOut", "label": "Zoom Out"},
        {"id": "OriginalView", "label": "Original View"},
        null,
        {"id": "Quality"},
        {"id": "Pause"},
        {"id": "Mute"},
        null,
        {"id": "Find", "label": "Find..."},
        {"id": "FindAgain", "label": "Find Again"},
        {"id": "Copy"},
        {"id": "CopyAgain", "label": "Copy Again"},
        {"id": "CopySVG", "label": "Copy SVG"},
        {"id": "ViewSVG", "label": "View SVG"},
        {"id": "ViewSource", "label": "View Source"},
        {"id": "SaveAs", "label": "Save As"},
        null,
        {"id": "Help"},
        {"id": "About", "label": "About Adobe CVG Viewer..."}
    ]
}})";
  iter = test.cbegin();
  object2 = JSON_parse_object_iter(iter);
  ob = object2;

  object2 = JSON_parse_object(
      "  { \"n\" : null , \"f\" : false , \"t\" : true , \"i\" : 123 , \"s\" "
      ": "
      "\"abc\", \"a\" : [ 1, 2, 3 ],\"o\" : {\"1\" : 1, \"2\" : 2, \"3\" : 3 "
      "}}");
  ob = object2;

  object2 = JSON_parse_object(test);
}

int main(int argc, char const *argv[]) {
  // try {
  //   double a = std::stod("nan");
  // } catch (std::exception &e) {
  //   std::cerr << e.what() << std::endl;
  // }
  //   bool temp = (std::stold("3.1416") == 3.1416);
  //   std::cout << "\u0020" /*(char)('\u0000' + 0x20AC)*/ << std::endl;
  test_parse_null();
  test_parse_false();
  test_parse_true();
  test_parse_number();
  test_parse_number_out_of_range();
  test_parse_invalid_argument();
  test_parse_string();
  test_parse_string_error();
  test_parse_array();
  test_parse_object();
  std::cout << "test count:" << test_count << ",test_pass:" << test_pass
            << std::endl;
  return 0;
}
