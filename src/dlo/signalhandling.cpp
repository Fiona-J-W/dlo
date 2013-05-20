#include "signalhandling.hpp"

#include <atomic>
#include <stdexcept>
#include <string>
#include <cstdint>
#include <climits>

// This should work almost everywhere:
#if ATOMIC_INT_LOCK_FREE != 2
	#error "Your platform doesn't support a lockfree atomic<int>."
#endif

namespace dlo{

namespace signalhandling {
using std::vector;

//put the instances of the static vars here:
namespace {
	std::atomic_int signal = ATOMIC_VAR_INIT(0);
	struct sigaction handler_struct;
}

//declare the actual signalhandler:
extern "C" void signal_handler(int signal);


void init(vector<int> sigs){
	signal.store(0);
	handler_struct.sa_handler = signal_handler;
	for(auto sig: sigs){
		sigaction(sig, &handler_struct, NULL);
	}
}


int get_last_signal(){
	return signal.load();
}

int reset(){
	return signal.fetch_and(0);
}

void check(){
	using std::to_string;
	auto sig = signal.load();
	if(sig){
		throw signal_exception("caught signal #" + to_string(sig), sig);
	}
}


extern "C"{
void signal_handler(int sign){
	signal.store(sign);
}
}

} //namespace signalhandling

signal_exception::signal_exception(const std::string& what_arg, int sig_num):
	std::runtime_error(what_arg),
	_sig_num(sig_num){}

int signal_exception::sig_num(){
	return _sig_num;
}


} //namespace dlo
