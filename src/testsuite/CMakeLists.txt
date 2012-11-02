add_executable( testsuite
	main.cpp
	sig_test.cpp
	conf_test.cpp
	string_test.cpp
	output_test.cpp
)
target_link_libraries( testsuite
	dlo
)
