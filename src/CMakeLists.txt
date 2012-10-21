
add_subdirectory(dlo)

if(BUILD_TESTS MATCHES ON)
	message("creating the testsuite.")
	add_subdirectory(testsuite)
else()
	message("Won't create the testsuite.")
endif()


