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
		settings S("");
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
		"foo = \"bar\"\n"
		"#comment\n"
		"reference = foo\n"
		
		"ref1 = ref_base\n"
		"ref2 = ref1\n"
		"ref3 = ref2\n"
		"ref4 = ref3\n"
		"ref5 = ref4\n"
		"ref6 = ref5\n"
		"ref7 = ref6\n"
		"ref8 = ref7\n"
		"ref9 = ref8\n"
		"ref10 = ref9\n"
		"ref11 = ref10\n"
		"ref12 = ref11\n"
		"ref13 = ref12\n"
		"ref_base = \"ref_base_text\"\n"
		
		"[ section ]\n"
		"foo = \"baz\"\n"
		);
	
	settings S("test");
	
	note(1,"testing common key=value...");
	if(S["foo"] == "bar"){
		note(1,"success.");
	}
	else{
		++fails;
		error("failed!");
	}
	
	note(1, "testing backward-reference resolving...");
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
	
	note(1, "testing forward-reference-resolving...");
	if(S["ref13"] == "ref_base_text"){
		note(1,"success.");
	}
	else{
		++fails;
		error("failed!");
		notef(2, "\twith ref_base=„%s“", S["ref_base"]);
		notef(2, "\twith ref1=„%s“", S["ref_1"]);
		notef(2, "\twith ref13=„%s“", S["ref_13"]);
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
