#ifndef STRINGUTILS_HPP
	#error "don't include stringutils.tcc directly, use stringutils.hpp instead."
#endif

#ifndef STRINGUTILS_TCC
#define STRINGUTILS_TCC

namespace dlo{
namespace stringutils{

template<typename T>
void _text_helper(stringstream& stream, T arg){
	stream << arg;
}

template<typename T, typename... argT>
void _text_helper(stringstream& stream, T arg,  argT...args){
	stream << arg;
	_text_helper(stream, args...);
}

template<typename... T>
string text(T...args){
	stringstream returnstream;
	_text_helper(returnstream, args...);
	return returnstream.str();
}



template<typename T>
void _textf_helper(vector<string>& vec, T arg){
	stringstream tmpstream;
	tmpstream << arg;
	vec.push_back(tmpstream.str());
}

template<typename T, typename... argT>
void _textf_helper(vector<string>& vec, T arg, argT...args){
	stringstream tmpstream;
	tmpstream << arg;
	vec.push_back(tmpstream.str());
	_textf_helper(vec, args...);
}

string _textf_impl(const std::vector<string>& strings);

template<typename...T>
string textf(const string& formatstring, T...args){
	vector<string> strings;
	strings.push_back(formatstring);
	_textf_helper(strings, args...);
	return _textf_impl(strings);
}

} //namespace stringutils
} //namespace dlo


#endif