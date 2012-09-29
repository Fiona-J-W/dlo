#ifndef STRINGUTILS_HPP
#define STRINGUTILS_HPP

#include <vector>
#include <string>
#include <map>
#include <utility>
#include <sstream>

namespace dlo{
namespace stringutils{
	
using std::vector;
using std::string;
using std::stringstream;


/**
 * convert a list of arguments to a string
 * @param args the arguments
 */
template<typename... T>
string text(T...args);

/**
 * Create a string from a formatstring and some arguments.
 *
 * Unlike printf() this will accept any type with %s as identifier.
 *
 * The formatstring will be printed as is, unless a percent-sign appears. In
 * this case the next character will be used to decide, what to do:
 *
 * If it is another percent-sign, only one of them will be added to the 
 * returnstring and the function will return back to its normal path.
 * 
 * If it is a 's', a counter will be increased and the string-representation of
 * args[counter] will be added to the string.
 * 
 * In all other cases the substring that terminates before the first following
 * 's' will be taken and converted to a int. This int will be used as index to 
 * which arg from args shall be inserted. Note that in this case arg[0] is the
 * formatstring itself.
 *
 * @param formatstring the formatstring
 * @param args the other arguments
 */
template<typename...T>
string textf(const string& formatstring,T...args);


/**
 * Strip the whitespace at the front and the back from a given String
 * @param str unstripped string
 */
std::string strip(std::string str);

/**
 * split the given string at the given delimiter
 * @param str string to be split
 * @param delim Delimeter that will be cut away
 */
std::vector<std::string> split(std::string str, std::string delim=",");

/**
 * split the given string at the first occurance of delim
 * @param str string to be split
 * @param delim Delimeter that will be cut away
 */
std::pair<std::string,std::string> split_once(std::string str, 
	std::string delim=",");

/**
 * decode the given string (for example "\t" would become a tab)
 * @param str encoded string
 */
std::string decode(std::string str);

} //namespace stringutils
} //namespace dlo

#include "stringutils.tcc"

#endif
