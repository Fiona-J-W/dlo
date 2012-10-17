#include "signalhandling.hpp"

#include "stringutils.hpp"

#include <stdexcept>
#include <string>
#include <cstdint>
#include <climits>

namespace dlo{

using std::vector;

//put the instances of the static vars here:
std::atomic_uint signalhandling::signal;
struct sigaction signalhandling::handler_struct;

//declare the actual signalhandler:
void signal_handler(int signal);


void signalhandling::init(vector<unsigned int> sigs){
	signal.store( 0 );
	handler_struct.sa_handler = signal_handler;
	for(auto it = sigs.begin(); it != sigs.end(); ++it){
		if(*it > UINT_MAX){
			throw std::invalid_argument("the number of a given signal"
				"is to big, to be saved in an unsigned int.");
		}
		sigaction(*it, &handler_struct, NULL);
	}
}


unsigned int signalhandling::get_last_signal(){
	return signal.load();
}

unsigned int signalhandling::reset(){
	return signal.fetch_and(0);
}

void signalhandling::check(){
	auto sig = signal.load();
	if(sig){
		throw signal_exception( stringutils::text("caught signal #", sig), sig );
	}
}

void signal_handler(int signal){
	signalhandling::signal.store( signal );
}


signal_exception::signal_exception(const std::string& what_arg, int sig_num):
	std::runtime_error(what_arg),
	_sig_num(sig_num){}

int signal_exception::sig_num(){
	return _sig_num;
}


} //namespace dlo
