#ifndef SIGNALHANDLING_HPP
#define SIGNALHANDLING_HPP

#include <vector>
#include <csignal>

namespace dlo{

/**
 * This class bundles everythin you need for signalhandling.
 * 
 * This class is static.
 * 
 * To use it, call the init method with a vector of signals you want to have managed
 * by this class once (as early as possible) in your programm.
 * 
 * After that you have to call the getLastSig method in short intervalls and react 
 * to it in a proper way.
 * 
 */
class signalhandling{
public:
	/**
	 * We don't need a constructor, so let's delete it.
	 */
	signalhandling() = delete;
	
	/**
	 * init function for signalhandling. This has to be called early.
	 * @param sigs vector of the signals, that should be handled (defaults to 
	 *        SIGINT and SIGTERM)
	 * @throws std::invalid_argument if a given signals number cannot be stored
	 *         in sig_atomic_t or is smaller than 1
	 */
	static void init(std::vector<unsigned int> sigs 
		= std::vector<unsigned int>{SIGINT, SIGTERM});
	
	
	/**
	 * query for the last signal; this will return 0 if no signal has been caught.
	 */
	static unsigned int get_last_signal();
	
	/**
	 * reset the saved signal to 0. 
	 * 
	 * As it limits to sabotage to ignore signals, don't use this unless you 
	 * have very good reasons to do so. 
	 * 
	 * @returns the value of signalhandling::signal before setting it to zero.
	 *          WARNING: this value might be outdated, if a signal appears
	 *          between copying the value and reseting signalhandling::signal.
	 */
	static unsigned int reset();
	
private:
	
	/**
	 * the actual handlerfunction, that will set signalhandling::signal to 
	 * the new value
	 */
	friend void signal_handler(int signal);
	
	//attributes:
	
	/**
	 * number of the last recieved signal; init will set this to 0.
	 */
	static sig_atomic_t signal;
	
	/**
	 * struct that contains the information, what should be done after recieving
	 * a signal
	 */
	static struct sigaction handler_struct;
};

} //namespace ldo


#endif
