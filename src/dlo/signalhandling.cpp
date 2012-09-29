
#include "signalhandling.hpp"


#include <stdexcept>
#include <string>
#include <cstdint>

namespace dlo{

using std::vector;

//put the instances of the static vars here:
sig_atomic_t signalhandling::signal;
struct sigaction signalhandling::handler_struct;

//declare the actual signalhandler:
void signalHandler(int signal);


void signalhandling::init(vector<unsigned int> sigs){
	signal = 0;
	handler_struct.sa_handler = signalHandler;
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

void signal_handler(int signal){
	signalhandling::signal = signal;
}

} //namespace dlo