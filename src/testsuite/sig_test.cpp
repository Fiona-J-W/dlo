
#include "../dlo/output.hpp"
#include "../dlo/signalhandling.hpp"


int sig_test(){
	using namespace dlo;
	signalhandling::init();
	unsigned int fails = 0;
	
	note(1,"This test assumes that it won't recieve signals by other processes.");
	
	note(1,"testing dlo::signalhandling::get_last_signal() without signal...");
	if(signalhandling::get_last_signal() == 0){
		note(1,"success");
	}
	else{
		++fails;
		error("failed");
	}
	
	note(1,"testing dlo::signalhandling::check() without signal...");
	try{
		dlo::signalhandling::check();
		note(1,"success");
	}
	catch(dlo::signal_exception &e){
		++fails;
		error("failed");
	}
	
	note(1, "sending SIGINT to testprocess");
	raise(SIGTERM);
	
	note(1,"testing dlo::signalhandling::get_last_signal() with SIGTERM...");
	if(signalhandling::get_last_signal() == SIGTERM){
		note(1, "success");
	}
	else{
		++fails;
		error("failed");
	}
	
	note(1,"testing dlo::signalhandling::check() with SIGTERM...");
	try{
		dlo::signalhandling::check();
		++fails;
		error("failed");
	}
	catch(dlo::signal_exception &e){
		note(1,"success");
	}
	
	note(1,"testing dlo::signalhandling::reset()...");
	if(signalhandling::reset() == SIGTERM){
		if(signalhandling::get_last_signal() == 0){
			note(1,"success");
		}
		else{
			++fails;
			error("failed (signal was not reset)");
		}
	}
	else{
		if(signalhandling::get_last_signal() == 0){
			++fails;
			error("failed (did not return last signal)");
		}
		else{
			++fails;
			error("failed (did nothing)");
		}
	}
	
	if(fails){
		errorf("%s tests failed", fails);
	}
	else{
		note(0, "all tests were succesfull");
	}
	
	return fails;
}
