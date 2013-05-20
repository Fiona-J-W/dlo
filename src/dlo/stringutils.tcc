#ifndef STRINGUTILS_HPP
	#error "don't include stringutils.tcc directly, use stringutils.hpp instead."
#endif

#ifndef STRINGUTILS_TCC
#define STRINGUTILS_TCC

#include <utility>

namespace dlo{
namespace stringutils{

namespace impl{

template<typename T>
inline void text_helper(stringstream& stream, T&& arg){
	stream << std::forward<T>(arg);
}

template<typename T, typename... Targs>
inline void text_helper(stringstream& stream, T&& arg,  Targs&&...args){
	stream << std::forward<T>(arg);
	text_helper(stream, std::forward<Targs>(args)...);
}

} //namespace ompl

template<typename... T>
string text(T&&...args){
	stringstream returnstream;
	impl::text_helper(returnstream, std::forward<T>(args)...);
	return returnstream.str();
}


namespace impl{

template<typename T>
inline void textf_helper(vector<string>& vec, T&& arg){
	stringstream tmpstream;
	tmpstream << std::forward<T>(arg);
	vec.push_back(tmpstream.str());
}

template<typename T, typename... Targs>
inline void textf_helper(vector<string>& vec, T&& arg, Targs&&...args){
	stringstream tmpstream;
	tmpstream << std::forward<T>(arg);
	vec.push_back(tmpstream.str());
	textf_helper(vec, std::forward<Targs>(args)...);
}

string textf_impl(const std::vector<string>& strings);

} //namespace ompl

template<typename...T>
string textf(const string& formatstring, T&&...args){
	vector<string> strings;
	strings.push_back(formatstring);
	impl::textf_helper(strings, std::forward<T>(args)...);
	return impl::textf_impl(strings);
}

} //namespace stringutils
} //namespace dlo


#endif
