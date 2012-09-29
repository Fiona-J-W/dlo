
#include "../dlo/output.hpp"
#include "../dlo/signalhandling.hpp"

#include <unistd.h>

int main(){
	try{
		dlo::signalhandling::init();
		
		while(true){
			dlo::signalhandling::check();
			usleep(1000);
		}
		
		return 0;
	}
	catch(dlo::signal_exception &e){
		dlo::note(0, "Terminating: ", e.what());
		return e.sig_num();
	}
	catch(...){
		return -1;
	}
}