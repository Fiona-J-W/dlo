#include "../dlo/output.hpp"

using namespace logging;

int main(int argc, char **argv){
	if(argc > 1){
		set_logfile(argv[argc-1]);
	}
	set_debug_level(10);
	set_verbosity(2);
	debug(0,"foo ", "bar", " baz ", 3, " ", 4, ' ');
	debugf(1,"some debugmessage with data: %s and %s", 1, 2);
	note(0, "some note");
	note(1, "some other note");
	warn("Programm is going to terminate NOW!");
	set_verbosity(1);
}
