add_executable( output_test
	output_test.cpp
)
target_link_libraries( output_test
	dlo
)

add_executable( testsuite
	main.cpp
	sig_test.cpp
	conf_test.cpp
	string_test.cpp
)
target_link_libraries( testsuite
	dlo
)
