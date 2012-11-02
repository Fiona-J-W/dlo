#include "string_test.hpp"

#include "../dlo/output.hpp"
#include "../dlo/stringutils.hpp"

int string_test(){
	using namespace dlo::stringutils;
	using dlo::notef;
	using dlo::note;
	using dlo::error;
	using dlo::errorf;
	
	unsigned int fails = 0;
	
	notef(1, "testing dlo::stringutils::%s()...","text");
	if(text(1,2.5,"foo",'b','a','r') == "12.5foobar"){
		note(1,"success.");
	}
	else{
		++fails;
		error("failed!");
	}
	
	notef(1, "testing dlo::stringutils::%s()...","textf");
	if(textf("foo %s bar %3s, %2s, %0s","1",2,3.5,'c') 
		== "foo 1 bar 3.5, 2, foo %s bar %3s, %2s, %0s"){
		note(1,"success.");
	}
	else{
		++fails;
		error("failed!");
	}
	
	notef(1, "testing dlo::stringutils::%s()...","strip");
	if(strip(" \t foo bar \t ") == "foo bar"){
		note(1,"success.");
	}
	else{
		++fails;
		error("failed!");
	}
	
	notef(1, "testing dlo::stringutils::%s()...","split");
	if(split("foo:bar:baz",":") 
		== (std::vector<std::string>{"foo", "bar", "baz"}) ){
		note(1,"success.");
	}
	else{
		++fails;
		error("failed!");
	}
	
	
	notef(1, "testing dlo::stringutils::%s()...","split_once");
	auto tmp = split_once("foo:bar:baz",":");
	if((tmp.first == "foo")&&(tmp.second == "bar:baz")){
		note(1,"success.");
	}
	else{
		++fails;
		error("failed!");
	}
	
	
	notef(1, "testing dlo::stringutils::%s()...","decode");
	if(decode("\"foo\\t\\n\\\\\\\"\"") == "foo\t\n\\\""){
		note(1,"success.");
	}
	else{
		++fails;
		error("failed!");
	}
	
	
	notef(1, "testing dlo::stringutils::%s()...","prefix_and_align");
	if(prefix_and_align("foo ","bar\nbaz") == "foo bar\n    baz"){
		note(1,"success.");
	}
	else{
		++fails;
		error("failed!");
	}
	
	notef(1, "testing dlo::stringutils::%s() with utf8...","prefix_and_align");
	if( prefix_and_align(u8"äöüß ","bar\nbaz") == u8"äöüß bar\n     baz"){
		note(1,"success.");
	}
	else{
		++fails;
		error("failed!");
	}
		
	notef(1, "testing dlo::stringutils::%s()...","u8len");
	if( u8len( u8"aäß¿") == 4 ){
		note(1,"success.");
	}
	else{
		++fails;
		error("failed!");
		note(1, "Result was: ", u8len(u8"aäß¿"));
	}
	
	if(fails){
		errorf("%s tests failed", fails);
		return 1;
	}
	else{
		note(0,"all tests were succesfull");
		return 0;
	}
	return fails;
}
