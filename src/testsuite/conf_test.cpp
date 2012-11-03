#include "string_test.hpp"

#include <fstream>
#include <iostream>
#include <cstdlib>

#include <unistd.h>
#include <sys/stat.h>

#include "../dlo/configuration.hpp"
#include "../dlo/output.hpp"



int conf_test(){
	using namespace dlo;
	//we need a defined, local_state:
	auto old_config_home = getenv("XDG_CONFIG_HOME");
	setenv("XDG_CONFIG_HOME",".",1);
	
	unsigned int fails = 0;
	
	note(1, "testing whether an uninitialised state throws...");
	try{
		settings S;
		error("failed");
		++fails;
	}
	catch(uninitialised_config_error &e){
		note(1,"success");
	}
	settings::init("test");
	mkdir("test", 0777);
	
	std::ofstream testfile("test/test.ini");
	
	
	swriteln(testfile,
		"foo = \"bar\"\n",
		"#comment\n",
		"reference = foo\n",
		"[ section ]\n",
		"foo = \"baz\"");
	
	settings S("test");
	
	note(1,"testing common key=value...");
	if(S["foo"] == "bar"){
		note(1,"success.");
	}
	else{
		++fails;
		error("failed!");
	}
	
	note(1, "testing reference resolving...");
	if(S["reference"] == "bar"){
		note(1,"success.");
	}
	else{
		++fails;
		error("failed!");
	}
	
	note(1, "testing sections...");
	if(S["section::foo"] == "baz"){
		note(1,"success.");
	}
	else{
		++fails;
		error("failed!");
	}
	
	if(fails){
		errorf("%s tests failed.", fails);
	}
	else{
		note(0,"all tests successfull");
	}
	
	//clean up:
	testfile.close();
	unlink("test/test.ini");
	rmdir("test");
	
	//set the environment back:
	setenv("XDG_CONFIG_HOME",old_config_home,1);
	
	return fails;
}
