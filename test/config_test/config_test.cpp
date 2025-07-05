#define BOOST_TEST_MODULE config_test
#include <boost/test/included/unit_test.hpp>
#include <iostream>
#include <string>

#undef ERROR
#define json(...) #__VA_ARGS__
#define CO ","
#define LF "\n"
#define BOOST_TEST_OUL_ERROR(ACTUAL, EXPECTED) BOOST_TEST(((int)ACTUAL) == ((int)EXPECTED))

import config;
import usings;
import message;

using namespace oul;
using namespace std;
using namespace boost::unit_test;
using enum ERROR;

#pragma region invalid_configuration_format_tests

void invalid_configuration_format_test(cr<string> source)
{
	BOOST_CHECK_THROW(CONFIG(istringstream(source)), InvalidConfiguration);
	try
	{
		CONFIG(istringstream(source));
	}
	catch (InvalidConfiguration& e)
	{
		BOOST_TEST_OUL_ERROR(e.get_error_code(), invalid_configuration_format);
	}
}
BOOST_AUTO_TEST_CASE(invalid_configuration_format_json)
{
	string source = "{\"project_name\":\"abcd\"";
	invalid_configuration_format_test(source);
}
BOOST_AUTO_TEST_CASE(invalid_configuration_format_yaml)
{
	string source = "project_name: abcd"
					"default_url: http://example.com"
					"components:"
					"- 123";
	invalid_configuration_format_test(source);
}

void root_not_object_test(cr<string> source)
{
	BOOST_CHECK_THROW(CONFIG(istringstream(source)), InvalidConfiguration);
	try
	{
		CONFIG(istringstream(source));
	}
	catch (InvalidConfiguration& e)
	{
		BOOST_TEST_OUL_ERROR(e.get_error_code(), root_not_object);
	}
}
BOOST_AUTO_TEST_CASE(root_not_object_json)
{
	string source = "[\"a\", 123]";
	root_not_object_test(source);
}
BOOST_AUTO_TEST_CASE(root_not_object_yaml)
{
	string source = "- a"
					"- 123";
	root_not_object_test(source);
}

void missing_project_name_test(cr<string> source)
{
	BOOST_CHECK_THROW(CONFIG(istringstream(source)), InvalidConfiguration);
	try
	{
		CONFIG(istringstream(source));
	}
	catch (InvalidConfiguration& e)
	{
		BOOST_TEST_OUL_ERROR(e.get_error_code(), missing_project_name);
	}
}
BOOST_AUTO_TEST_CASE(missing_project_name_json)
{
	string source = json({)
					json("default_url": "http://example.com") CO
					json("components": [])
					json(});
	missing_project_name_test(source);
}
BOOST_AUTO_TEST_CASE(missing_project_name_yaml)
{
	string source = "default_url: http://example.com" LF "components: []";
	missing_project_name_test(source);
}
BOOST_AUTO_TEST_CASE(invalid_project_name_json)
{
	string source = json({)
	                json("project_name": {}) CO
					json("default_url": "http://example.com") CO
					json("components": [])
					json(});
	missing_project_name_test(source);
}
BOOST_AUTO_TEST_CASE(invalid_project_name_yaml)
{
	string source =
		"default_url: http://example.com" LF "project_name:" LF "- invalid" LF "components: []";
	missing_project_name_test(source);
}

void components_not_array_test(cr<string> source)
{
	BOOST_CHECK_THROW(CONFIG(istringstream(source)), InvalidConfiguration);
	try
	{
		CONFIG(istringstream(source));
	}
	catch (InvalidConfiguration& e)
	{
		BOOST_TEST_OUL_ERROR(e.get_error_code(), components_not_array);
	}
}
BOOST_AUTO_TEST_CASE(missing_component_array_json)
{
	string source = json({)
		json("project_name": "test") CO
		json("default_url": "http://example.com")
				json(});
	components_not_array_test(source);
}
BOOST_AUTO_TEST_CASE(missing_component_array_yaml)
{
	string source = "project_name: test" LF "default_url: http://example.com";
	components_not_array_test(source);
}

BOOST_AUTO_TEST_CASE(components_not_array_json)
{
	string source = json({)
		json("project_name": "test") CO
		json("default_url": "http://example.com") CO
		json("components": {"not": "array"})
				json(});
	components_not_array_test(source);
}
BOOST_AUTO_TEST_CASE(components_not_array_yaml)
{
	string source =
		"project_name: test" LF "default_url: http://example.com" LF "components: not array";
	components_not_array_test(source);
}

void url_not_string_test(cr<string> source)
{
	BOOST_CHECK_THROW(CONFIG(istringstream(source)), InvalidConfiguration);
	try
	{
		CONFIG(istringstream(source));
	}
	catch (InvalidConfiguration& e)
	{
		BOOST_TEST_OUL_ERROR(e.get_error_code(), url_not_string);
	}
}
BOOST_AUTO_TEST_CASE(missing_default_url_json)
{
	string source = json({)
		json("project_name": "test") CO
		json("components": [])
		json(});
	url_not_string_test(source);
}
BOOST_AUTO_TEST_CASE(missing_default_url_yaml)
{
	string source = "project_name: test" LF "components: []";
	url_not_string_test(source);
}
BOOST_AUTO_TEST_CASE(invalid_default_url_json)
{
	string source = json({)
		json("project_name": "test") CO
		json("default_url": {}) CO
		json("components": [])
		json(});
	url_not_string_test(source);
}
BOOST_AUTO_TEST_CASE(invalid_default_url_yaml)
{
	string source = "project_name: test" LF "default_url:" LF "    not: string" LF "components: []";
	url_not_string_test(source);
}

#pragma endregion

#pragma region invalid_component_configuration_tests

void invalid_component_test(cr<string> source)
{
	BOOST_CHECK_THROW(CONFIG(istringstream(source)), InvalidConfiguration);
	try
	{
		CONFIG(istringstream(source));
	}
	catch (InvalidConfiguration& e)
	{
		BOOST_TEST_OUL_ERROR(e.get_error_code(), invalid_component);
	}
}
BOOST_AUTO_TEST_CASE(invalid_component_string_json)
{
	string source = json({)
				    json("project_name": "test") CO
		            json("default_url": "http://example.com") CO
		            json("components":)
		            json([)
		            json("string not component") CO
		            json({)
		            json(    "name": "kompo") CO
		            json(    "location": "components/kompo") CO
		            json(    "include": {})
		            json(}])
		            json(});
	invalid_component_test(source);
}
BOOST_AUTO_TEST_CASE(invalid_component_string_yaml)
{
	string source = "project_name: test" LF "default_url: http://example.com" LF "components:" LF
					"- name: kompo" LF "  location: components/kompo" LF "  include: {}" LF
					"- string not component";
	invalid_component_test(source);
}
BOOST_AUTO_TEST_CASE(invalid_component_array_json)
{
	string source = json({)
		            json("project_name": "test") CO
		            json("default_url": "http://example.com") CO
		            json("components":)
		            json([{)
		            json(    "name": "kompo") CO
		            json(    "location": "components/kompo") CO
		            json(    "include": {})
		            json(}) CO
		            json(["array", "not", "component"])
		            json(])
		            json(});
	invalid_component_test(source);
}
BOOST_AUTO_TEST_CASE(invalid_component_array_yaml)
{
	string source = "project_name: test" LF "default_url: http://example.com" LF "components:" LF
					"- - array" LF "  - not" LF "  - component" LF "- name: kompo" LF
					"  location: components/kompo" LF "  include: {}";
	invalid_component_test(source);
}

void invalid_component_name_test(cr<string> source)
{
	BOOST_CHECK_THROW(CONFIG(istringstream(source)), InvalidConfiguration);
	try
	{
		CONFIG(istringstream(source));
	}
	catch (InvalidConfiguration& e)
	{
		BOOST_TEST_OUL_ERROR(e.get_error_code(), invalid_component_name);
	}
}
BOOST_AUTO_TEST_CASE(missing_component_name_json)
{
	string source = json({)
		            json("project_name": "test") CO
		            json("default_url": "http://example.com") CO
		            json("components":)
		            json([{)
		            json(    "location": "components/kompo") CO
		            json(    "include": {})
		            json(}])
		            json(});
	invalid_component_name_test(source);
}
BOOST_AUTO_TEST_CASE(missing_component_name_yaml)
{
	string source = "project_name: test" LF "default_url: http://example.com" LF "components:" LF
					"- location: components/kompo" LF "  include: {}";
	invalid_component_name_test(source);
}
BOOST_AUTO_TEST_CASE(invalid_component_name_json)
{
	string source = json({)
		            json("project_name": "test") CO
		            json("default_url": "http://example.com") CO
		            json("components":)
		            json([{)
		            json(    "name": null) CO
		            json(    "location": "components/kompo") CO
		            json(    "include": {})
		            json(}])
		            json(});
	invalid_component_name_test(source);
}
BOOST_AUTO_TEST_CASE(invalid_component_name_yaml)
{
	string source = "project_name: test" LF "default_url: http://example.com" LF "components:" LF
					"- name:" LF "  location: components/kompo" LF "  include: {}";
	invalid_component_name_test(source);
}

void invalid_original_name_test(cr<string> source)
{
	BOOST_CHECK_THROW(CONFIG(istringstream(source)), InvalidConfiguration);
	try
	{
		CONFIG(istringstream(source));
	}
	catch (InvalidConfiguration& e)
	{
		BOOST_TEST_OUL_ERROR(e.get_error_code(), invalid_original_name);
	}
}
BOOST_AUTO_TEST_CASE(invalid_original_name_json)
{
	string source = json({)
		            json("project_name": "test") CO
		            json("default_url": "http://example.com") CO
		            json("components":)
		            json([{)
		            json(    "name": "kompo") CO
		            json(    "original_name": ["invalid", "name"]) CO
		            json(    "location": "components/kompo") CO
		            json(    "include": {})
		            json(}])
		            json(});
	invalid_original_name_test(source);
}
BOOST_AUTO_TEST_CASE(invalid_original_name_yaml)
{
	string source = "project_name: test" LF "default_url: http://example.com" LF "components:" LF
					"- name: kompo" LF "  location: components/kompo" LF "  original_name:" LF
					"    invalid: name" LF "  include: {}";
	invalid_original_name_test(source);
}

void missing_location_test(cr<string> source)
{
	BOOST_CHECK_THROW(CONFIG(istringstream(source)), InvalidConfiguration);
	try
	{
		CONFIG(istringstream(source));
	}
	catch (InvalidConfiguration& e)
	{
		BOOST_TEST_OUL_ERROR(e.get_error_code(), missing_location);
	}
}
BOOST_AUTO_TEST_CASE(invalid_location_json)
{
	string source = json({)
		            json("project_name": "test") CO
		            json("default_url": "http://example.com") CO
		            json("components":)
		            json([{)
		            json(    "name": "kompo", ) CO
		            json(    "location": null, ) CO
		            json(    "include": {})
		            json(}])
		            json(});
	missing_location_test(source);
}
BOOST_AUTO_TEST_CASE(invalid_location_yaml)
{
	string source = "project_name: test" LF "default_url: http://example.com" LF "components:" LF
					"- name: kompo" LF "  location: null" LF "  include: {}";
	missing_location_test(source);
}
BOOST_AUTO_TEST_CASE(missing_location_json)
{
	string source = json({)
		            json("project_name": "test") CO
		            json("default_url": "http://example.com") CO
		            json("components":)
		            json([{)
		            json(    "name": "kompo", ) CO
		            json(    "include": {})
		            json(}])
		            json(});
	missing_location_test(source);
}
BOOST_AUTO_TEST_CASE(missing_location_yaml)
{
	string source = "project_name: test" LF "default_url: http://example.com" LF "components:" LF
					"- name: kompo" LF "  include: {}";
	missing_location_test(source);
}

void missing_include_test(cr<string> source)
{
	BOOST_CHECK_THROW(CONFIG(istringstream(source)), InvalidConfiguration);
	try
	{
		CONFIG(istringstream(source));
	}
	catch (InvalidConfiguration& e)
	{
		BOOST_TEST_OUL_ERROR(e.get_error_code(), missing_include);
	}
}
BOOST_AUTO_TEST_CASE(missing_include_json)
{
	string source = json({)
		            json("project_name": "test") CO
		            json("default_url": "http://example.com") CO
		            json("components":)
		            json([{)
		            json(    "name": "kompo") CO
		            json(    "location": "components/kompo") CO
		            json(    "exclude": {})
		            json(}])
		            json(});
	missing_include_test(source);
}
BOOST_AUTO_TEST_CASE(missing_include_yaml)
{
	string source = "project_name: test" LF "default_url: http://example.com" LF "components:" LF
					"- name: kompo" LF "  location: components/kompo" LF "  exclude: abcd";
	missing_include_test(source);
}

void invalid_exclude_test(cr<string> source)
{
	BOOST_CHECK_THROW(CONFIG(istringstream(source)), InvalidConfiguration);
	try
	{
		CONFIG(istringstream(source));
	}
	catch (InvalidConfiguration& e)
	{
		BOOST_TEST_OUL_ERROR(e.get_error_code(), invalid_exclude);
	}
}
BOOST_AUTO_TEST_CASE(invalid_exclude_json)
{
	string source = json({)
		            json("project_name": "test") CO
		            json("default_url": "http://example.com") CO
		            json("components":)
		            json([{)
		            json(    "name": "kompo") CO
		            json(    "location": "components/kompo") CO
		            json(    "include": {}) CO
		            json(    "exclude": 123)
		            json(}])
		            json(});
	invalid_exclude_test(source);
}
BOOST_AUTO_TEST_CASE(invalid_exclude_yaml)
{
	string source =
		"project_name: test" LF "default_url: http://example.com" LF "components:" LF
		"- name: kompo" LF "  location: components/kompo" LF "  include: {}" LF "  exclude: abcd";
	invalid_exclude_test(source);
}

#pragma endregion