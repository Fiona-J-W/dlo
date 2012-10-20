add_executable( output_test
	output_test.cpp
)
target_link_libraries( output_test
	dlo
)

add_executable( sig_test
	sig_test.cpp
)
target_link_libraries( sig_test
	dlo
)

add_executable( string_test
	string_test.cpp
)
target_link_libraries( string_test
	dlo
)

add_executable( conf_test
	conf_test.cpp
)
target_link_libraries( conf_test
	dlo
)

add_executable( i18n_test
	i18n_test.cpp
)
target_link_libraries( i18n_test
	dlo
)
