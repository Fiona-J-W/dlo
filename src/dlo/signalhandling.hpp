#ifndef SIGNALHANDLING_HPP
#define SIGNALHANDLING_HPP

#include <vector>
#include <csignal>
#include <stdexcept>

namespace dlo{

/**
 * This class bundles everything you need for signalhandling.
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
	//as doxygen on debian doesn't understand ctor=delete, make it private:
	/**
	 * We don't need a constructor, so let's delete it.
	 */
	signalhandling() = delete;
	
public:
	/**
	 * init function for signalhandling. This has to be called early.
	 * @param sigs vector of the signals, that should be handled (defaults to 
	 *        SIGINT and SIGTERM)
	 * @throws std::invalid_argument if a given signals number cannot be stored
	 *         in sig_atomic_t or is smaller than 1
	 */
	static void init(std::vector<unsigned int> sigs = {SIGINT, SIGTERM});
	
	/**
	 * query for the last signal; this will return 0 if no signal has been caught.
	 */
	static unsigned int get_last_signal();
	
	/**
	 * reset the saved signal to 0. 
	 * 
	 * Be carefull with this: If you don't have very good reasons to use this
	 * function, don't use it.
	 * 
	 * @returns the value of signalhandling::signal before setting it to zero.
	 *          WARNING: this value might be outdated, if a signal appears
	 *          between copying the value and reseting signalhandling::signal.
	 */
	static unsigned int reset();
	
	/**
	 * check if signalhandling::signal is set and throw a signal_exception if this 
	 * is the case; otherwise do nothing.
	 * @throws signal_exception if signalhandling::signal is set to another value 
	 *                          than zero.
	 */
	static void check();
	
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



/**
 * Exception that indicates that a signal got caught.
 */
class signal_exception: public std::runtime_error{
public:
	/**
	 * The ctor takes the number of the signal as an additional argument
	 * @param what_arg a short message
	 * @param sig_num the number of the signal
	 */
	signal_exception(const std::string& what_arg, int sig_num);
	
	/**
	 * Get the number of the signal.
	 */
	int sig_num();
private:
	/**
	 * the number of the signal.
	 */
	int _sig_num;
};

} //namespace ldo


#endif
