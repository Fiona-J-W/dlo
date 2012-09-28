#include "../dlo/output.hpp"

//using namespace logging;

int main(int argc, char **argv){
	try{
		if(argc > 1){
			logging::set_logfile(argv[argc-1]);
		}
		logging::set_debug_level(10);
		logging::set_verbosity(2);
		debug(0, "foo ", "bar", " baz ", 3, " ", 4, ' ');
		debugf(1, "some debugmessage with data: %s and %s", 1, 2);
		logging::note(0, "some note");
		logging::notef(1, "some other, %s note", "formated");
		logging::warn("And now: commit suicide!!!");
		logging::fatalf("Yeah!!!!", ' ');
	}
	catch(...){
		return -1;
	}
}
