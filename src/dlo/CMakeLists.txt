
set(DLO_SOURCES
	output.cpp
	signalhandling.cpp
	configuration.cpp
	stringutils.cpp
)

if(BUILD_SHARED MATCHES ON)
	message("creating shared lib")
	add_library( dlo SHARED ${DLO_SOURCES})
else()
	message("creating static lib")
	add_library( dlo STATIC ${DLO_SOURCES})
endif()


