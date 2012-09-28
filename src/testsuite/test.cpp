#include "../dlo/output.hpp"

//using namespace dlo;

int main(int argc, char **argv){
	try{
		if(argc > 1){
			dlo::set_logfile(argv[argc-1]);
		}
		dlo::set_debug_level(10);
		dlo::set_verbosity(2);
		debug(0, "foo ", "bar", " baz ", 3, " ", 4, ' ');
		debugf(1, "some debugmessage with data: %s and %s", 1, 2);
		dlo::note(0, "some note");
		dlo::notef(1, "some other, %s note", "formated");
		dlo::warn("And now: commit suicide!!!");
		dlo::fatalf("Yeah!!!!", ' ');
	}
	catch(...){
		return -1;
	}
}
