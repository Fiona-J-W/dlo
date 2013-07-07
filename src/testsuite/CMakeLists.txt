find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_executable( testsuite
	main.cpp
	sig_test.cpp
	conf_test.cpp
	string_test.cpp
	output_test.cpp
)

add_executable( tests
	tests_main.cpp
	tests_stringutils.cpp
)

target_link_libraries( testsuite
	dlo
)

target_link_libraries( tests
	dlo
	${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)
