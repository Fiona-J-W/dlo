#include "../dlo/output.hpp"
#include "../dlo/signalhandling.hpp"

//using namespace dlo
using dlo::signalhandling;


int main(int argc, char **argv){
	try{
		if(argc > 1){
			dlo::set_logfile(argv[argc-1]);
		}
		signalhandling::init();
		signalhandling::check();
		dlo::set_debug_level(10);
		dlo::set_verbosity(2);
		debug(0, "foo ", "bar", " baz ", 3, " ", 4, ' ');
		debugf(1, "some debugmessage with data: %s and %s", 1, 2);
		dlo::note(0, "some note");
		dlo::notef(1, "some other, %s note", "formated");
		dlo::warn("And now: commit suicide!!!");
		signalhandling::check();
		dlo::fatalf("Yeah!!!!", ' ');
	}
	catch(dlo::signal_exception &e){
		dlo::print_and_log(e.what());
		return e.sig_num();
	}
	catch(...){
		return -1;
	}
}
