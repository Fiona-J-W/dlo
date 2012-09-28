add_library( dlo
	output.cpp
)

#testlibrary:
add_executable( test
	test.cpp
)
target_link_libraries( test
	dlo
)

