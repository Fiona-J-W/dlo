#ifndef output_hpp
#define output_hpp

#include <vector>
#include <string>
#include <sstream>

#include "stringutils.hpp"

namespace dlo {

using std::string;
using std::stringstream;
using std::vector;



/**
 * set the logfile
 *
 * @param filename logfile; if empty, logging will be disabled
 * @returns 0 if the logfile could be opened for writing, otherwise -1
 */
int set_logfile(const string& filename);

#ifdef DEBUG

/**
 * Print a debugmessage; this won't do anything in release-builds.
 * @param level debuglevel (lower means less important)
 * @param ... parameters that will be converted to a string and printed
 */
#define debug(level, ...) ::dlo::_debug( __FILE__, __LINE__, (level), ::dlo::stringutils::text(__VA_ARGS__) )

/**
 * Print a debugmessage; this won't do anything in release-builds.
 * @param level debuglevel (lower means less important)
 * @param ... the first parameter is a formatstring that will, 
 *            together with the rest, create the message
 */
#define debugf(level, ...) ::dlo::_debug( __FILE__, __LINE__, (level), ::dlo::stringutils::textf(__VA_ARGS__) )

/**
 * Set the debug-level: only debug-messages with a level higher that it will be processed.
 * @param level the new debuglevel
 */
void set_debug_level(int level);


#else // not DEBUG-mode
// these declarations are just here to enable compilation:
#define debug(level, ...)
#define debugf(level, ...)

// let's hope the compiler will really inline this; 
// otherwise there will be ugly linker-errors (seems
// to work with gcc):
inline void set_debug_level(int){}
#endif


/**
 * Set the verbosity level.
 * @param level the highest level, that will be printed
 */
void set_verbosity(int level);


/**
 * Print a note with the given urgency-level.
 * @param level urgency-level ot the message
 * @param args the args that will be converted to a string and then concatenated to 
 *             form the message, that will be printed
 */
template<typename... T>
void note(int level, T...args);


/**
 * Print a note with the given urgency-level.
 * @param level urgency-level ot the message
 * @param formatstring a string that describes how the message should be build and 
 *                     uses the other args to build the final text.
 * @param args the args that will be converted to a string
 */
template<typename... T>
void notef(int level, const string& formatstring, T...args);


/**
 * Print a warning to stderr and log it, if a logfile is set.
 * @param args the args that will be converted to a string and then concatenated to 
 *             form the message, that will be printed
 */
template<typename... T>
void warn(T... args);

/**
 * Print a warning to stderr and log it, if a logfile is set.
 * @param formatstring a string that describes how the message should be build and 
 *                     uses the other args to build the final text.
 * @param args the args that will be converted to a string
 */
template<typename... T>
void warnf(const string& formatstring, T... args);


/**
 * Print an error to stderr and log it, if a logfile is set.
 * @param args the args that will be converted to a string and then concatenated to 
 *             form the message, that will be printed
 */
template<typename... T>
void error(T... args);

/**
 * Print an error to stderr and log it, if a logfile is set.
 * @param formatstring a string that describes how the message should be build and 
 *                     uses the other args to build the final text.
 * @param args the args that will be converted to a string
 */
template<typename... T>
void errorf(const string& formatstring, T... args);

/**
 * Print a fatal error. This will throw a private exception to terminate the
 * program. You may want to ensure that the destructors are executed by encapsulating
 * your main with a catch(...){return 1;} block.
 *
 * @param args the args that will be converted to a string and then concatenated to 
 *             form the message, that will be printed
 */
template<typename... T>
void fatal(T... args);

/**
 * Print a fatal error. This will throw a private exception to terminate the
 * program. You may want to ensure that the destructors are executed by encapsulating
 * your main with a catch(...){return 1;} block.
 *
 * @param formatstring a string that describes how the message should be build and 
 *                     uses the other args to build the final text.
 * @param args the args that will be converted to a string
 */
template<typename... T>
void fatalf(const string& formatstring, T... args);

/**
 * Get the current time.
 *
 * @returns a string with the format "DD/MM/YY hh:mm:ss"
 */
string get_timestamp();


/**
 * disable printing to stdout from all printfunctions of this unit.
 * @param quiet true = disable printing, false = enable
 */
void set_stdout_quiet(bool quiet = true);


/**
 * disable printing to stderr from all printfunctions of this unit.
 * @param quiet true = disable printing, false = enable
 */
void set_stderr_quiet(bool quiet = true);

/**
 * Write a message to stdout or stderr and to the log.
 * 
 * @param msg the message that will be printed
 * @param normal whether the message is normal behaviour and should be 
 * printed to stdout or an error to be printed to stderr
 */
void print_and_log(const string& msg, bool normal = true);

} //dlo namespace

//include the template-implementations:
#include "output.tcc"

#endif
