#ifndef SIGNALHANDLING_HPP
#define SIGNALHANDLING_HPP

#include <vector>
#include <csignal>
#include <stdexcept>


namespace dlo{

/**
 * This namespace bundles everything you need for signalhandling.
 * 
 * To use it, call the init method with a vector of signals you want to have managed
 * by this class once (as early as possible) in your programm.
 * 
 * After that you have to call getLastSig() or check() in short intervals and react 
 * to their output in a proper way.
 * 
 */
namespace signalhandling{
	/**
	 * init function for signalhandling. This has to be called early.
	 * @param sigs vector of the signals, that should be handled (defaults to 
	 *        SIGINT and SIGTERM)
	 */
	void init(std::vector<int> sigs = {SIGINT, SIGTERM});
	
	/**
	 * query for the last signal; this will return 0 if no signal has been caught.
	 */
	int get_last_signal();
	
	/**
	 * reset the saved signal to 0. 
	 * @returns the value of signalhandling::signal before setting it to zero.
	 */
	int reset();
	
	/**
	 * check if signalhandling::signal is set and throw a signal_exception if this 
	 * is the case; otherwise do nothing.
	 * @throws signal_exception if signalhandling::signal is set to another value 
	 *                          than zero.
	 */
	void check();
}



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

} //namespace dlo


#endif
