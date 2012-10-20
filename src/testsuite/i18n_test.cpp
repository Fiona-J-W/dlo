#include "../dlo/output.hpp"
#include "../dlo/i18n.hpp"

#include <unordered_map>
#include <string>

int main(){
	using dlo::i18n;
	using dlo::notef;
	using dlo::note;
	using dlo::error;
	using dlo::errorf;
	using std::string;
	
	unsigned int fails = 0;
	
	note(0, "testing dlo::i18n() without dict...");
	if(i18n("foo") == "foo" ){
		note(0,"success.");
	}
	else{
		++fails;
		error("failed!");
	}
		
	note(0, "testing dlo::i18n() without dict and empty string...");
	if(i18n("") == "" ){
		note(0,"success.");
	}
	else{
		++fails;
		error("failed!");
	}
	
	std::unordered_map<string, string> dict = {
		{"foo", "bar"},
		{"bla", "blub"}
	};
		
	note(0, "trying to set language");
	dlo::set_dict(dict);
	
	note(0, "testing dlo::i18n() with dict and word in dict...");
	if(i18n("foo") == "bar" ){
		note(0,"success.");
	}
	else{
		++fails;
		error("failed!");
	}
		
	note(0, "testing dlo::i18n() with dict and word not in dict...");
	if(i18n("baz") == "baz" ){
		note(0,"success.");
	}
	else{
		++fails;
		error("failed!");
	}
	
	note(0, "testing dlo::i18n() with dict and empty string...");
	if(i18n("") == "" ){
		note(0,"success.");
	}
	else{
		++fails;
		error("failed!");
	}
	
	if(fails){
		errorf("%s tests failed", fails);
		return 1;
	}
	else{
		note(0,"all tests were succesfull");
		return 0;
	}
}
