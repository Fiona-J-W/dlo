#define BOOST_TEST_MODULE dlo test
#include <boost/test/unit_test.hpp>

#include "../dlo/stringutils.hpp"
using namespace dlo::stringutils;

BOOST_AUTO_TEST_CASE(variadic_text_test){
	BOOST_CHECK_EQUAL(text(1), "1");
	BOOST_CHECK_EQUAL(text("foo"), "foo");
	BOOST_CHECK_EQUAL(text(1,2,3), "123");
	BOOST_CHECK_EQUAL(text("a",'b',static_cast<char>('c'+3-3)), "abc");
	BOOST_CHECK_EQUAL(text("foo","bar"), "foobar");
}

BOOST_AUTO_TEST_CASE(variadic_textf_test){
	BOOST_CHECK_EQUAL(textf("%s",1), "1");
	BOOST_CHECK_EQUAL(textf("%s",'\t'), "\t");
	BOOST_CHECK_EQUAL(textf("%s%s%s%s%%",1,2,3,4), "1234%");
	BOOST_CHECK_EQUAL(textf("%s,%s,%s",1,"foo","bar"), "1,foo,bar");
}

BOOST_AUTO_TEST_CASE(strip_test){
	BOOST_CHECK_EQUAL(strip(""),"");
	BOOST_CHECK_EQUAL(strip(" \t "),"");
	BOOST_CHECK_EQUAL(strip("foo"),"foo");
	BOOST_CHECK_EQUAL(strip(" \t foo"),"foo");
	BOOST_CHECK_EQUAL(strip("foo \t "),"foo");
	BOOST_CHECK_EQUAL(strip(" \t foo \t "),"foo");
	BOOST_CHECK_EQUAL(strip(" \t foo bar \t "),"foo bar");
}

BOOST_AUTO_TEST_CASE(split_test){
	BOOST_CHECK(split("") == std::vector<std::string>{""});
	BOOST_CHECK(split("foo") == std::vector<std::string>{"foo"});
	
	std::vector<std::string>expected_result;
	
	expected_result = {"foo","bar"};
	BOOST_CHECK(split("foo,bar") == expected_result);
	
	expected_result = {"foo","bar", "baz"};
	BOOST_CHECK(split("foo,bar,baz") == expected_result);
	
	expected_result = {"","foo","bar",""};
	BOOST_CHECK(split(",foo,bar,") == expected_result);
	
	expected_result = {"foo","bar"};
	BOOST_CHECK(split("foo_bar","_") == expected_result);
	
	expected_result = {"foo","bar", "baz"};
	BOOST_CHECK(split("foo_bar_baz","_") == expected_result);
	
	expected_result = {"foo", "bar,baz"};
	BOOST_CHECK(split("foo_bar,baz","_") == expected_result);
	
	expected_result = {"","foo","bar",""};
	BOOST_CHECK(split("_foo_bar_","_") == expected_result);
	
	expected_result = {"foo","bar", "baz"};
	BOOST_CHECK(split("fooabcbarabcbaz","abc") == expected_result);
	
	expected_result = {"foo", "bar,baz"};
	BOOST_CHECK(split("fooabcbar,baz","abc") == expected_result);
	
	expected_result = {"","foo","bar",""};
	BOOST_CHECK(split("abcfooabcbarabc","abc") == expected_result);
}

BOOST_AUTO_TEST_CASE(split_once_test){
	BOOST_CHECK_THROW(split_once(""), std::invalid_argument);
	BOOST_CHECK_THROW(split_once("", ""), std::invalid_argument);
	BOOST_CHECK_THROW(split_once("foobar"), std::invalid_argument);
	BOOST_CHECK_THROW(split_once("foobar", ""), std::invalid_argument);
	
	std::pair<std::string, std::string> expected_result;
	
	expected_result = {"",""};
	BOOST_CHECK(split_once(",") == expected_result);
	expected_result = {"",",,,"};
	BOOST_CHECK(split_once(",,,,") == expected_result);
	expected_result = {"",""};
	BOOST_CHECK(split_once("abc","abc") == expected_result);
	expected_result = {"","ababab"};
	BOOST_CHECK(split_once("abababab", "ab") == expected_result);
	
	expected_result = {"foo","bar"};
	BOOST_CHECK(split_once("foo,bar") == expected_result);
	expected_result = {"foo","bar,baz"};
	BOOST_CHECK(split_once("foo,bar,baz") == expected_result);
}

BOOST_AUTO_TEST_CASE(decode_test){
	BOOST_CHECK_THROW(decode(""),std::invalid_argument);
	BOOST_CHECK_THROW(decode("\"a"),std::invalid_argument);
	BOOST_CHECK_THROW(decode("a\""),std::invalid_argument);
	BOOST_CHECK_THROW(decode("a"),std::invalid_argument);
	BOOST_CHECK_EQUAL(decode(R"("")"),"");
	BOOST_CHECK_EQUAL(decode(R"("abc")"),"abc");
	BOOST_CHECK_EQUAL(decode(R"("\"\"")"),"\"\"");
	BOOST_CHECK_EQUAL(decode(R"("\t\n")"),"\t\n");
}
