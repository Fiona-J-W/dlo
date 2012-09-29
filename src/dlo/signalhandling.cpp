#include "signalhandling.hpp"

#include "stringutils.hpp"

#include <stdexcept>
#include <string>
#include <cstdint>

namespace dlo{

using std::vector;

//put the instances of the static vars here:
sig_atomic_t signalhandling::signal;
struct sigaction signalhandling::handler_struct;

//declare the actual signalhandler:
void signal_handler(int signal);


void signalhandling::init(vector<unsigned int> sigs){
	signal = 0;
	handler_struct.sa_handler = signal_handler;
	for(auto it = sigs.begin(); it != sigs.end(); ++it){
		if(*it > SIG_ATOMIC_MAX){
			throw std::invalid_argument("the number of a given signal"
				"is to big, to be saved in a sig_atomic_t");
		}
		if(*it < 1){
			throw std::invalid_argument("the number of a given signal"
				"is to small (<1) to be handled by this class");
		}
		sigaction(*it, &handler_struct, NULL);
	}
}


unsigned int signalhandling::get_last_signal(){
	return signal;
}

unsigned int signalhandling::reset(){
	unsigned int returnval = signalhandling::signal;
	//a signal might arrive here and destroy everything; this is bad as people
	//might reset the stuff and believe they broke nothing
	signalhandling::signal = 0;
	return returnval;
}

void signalhandling::check(){
	sig_atomic_t sig = signalhandling::signal;
	if(sig){
		throw signal_exception( stringutils::text("caught signal #", sig), sig );
	}
}

void signal_handler(int signal){
	signalhandling::signal = signal;
}


signal_exception::signal_exception(const std::string& what_arg, int sig_num):
	std::runtime_error(what_arg),
	_sig_num(sig_num){}

int signal_exception::sig_num(){
	return _sig_num;
}


} //namespace dlo