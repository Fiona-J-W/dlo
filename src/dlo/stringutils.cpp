
#include "stringutils.hpp"

#include <fstream>
#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <cctype>
#include <cassert>

namespace dlo{
namespace stringutils{

using std::string;

string strip(string str){
	std::size_t pos=0;
	while( pos < str.size() && isspace(str[pos])){
		++pos;
	}
	str.erase(0,pos);
	if(str.empty()){
		return string();
	}
	pos=str.size() - 1;
	while( pos > 0 && isspace(str[pos])){
		--pos;
	}
	str.erase(pos+1);
	return str;
}



vector<string> split(string str, const string& delim){
	std::size_t pos = str.find(delim);
	vector<string> data;
	while( pos != string::npos ){
		data.push_back( str.substr(0, pos) );
		str.erase(0,pos+delim.size());
		pos=str.find(delim);
	}
	data.push_back(str);
	return data;
}

std::pair<string,string> split_once(string str, string delim){
	std::pair<string, string> data;
	std::size_t pos = str.find(delim);
	if( pos == string::npos ){
		throw std::invalid_argument("splitOnce(): delimeter not found");
	}
	data.first=str.substr(0,pos);
	data.second=str.substr(pos+delim.size());
	return data;
}

string decode(string str){
	str=strip(str);
	string returnstr="";
	if(str[0] != '"' || str[str.size()-1] != '"'){
		throw std::invalid_argument("decode(): no valid formatstring");
	}
	for(std::size_t pos=1; pos<str.size()-1 ; ++pos){
		if( str[pos] != '\\'){
			if( str[pos] == '"' ){
				throw std::invalid_argument(
					"decode(): no valid formatstring"
				);
			}
			returnstr+=str[pos];
		}
		else{
			if(pos+1 >= str.size()-1){
				throw std::invalid_argument(
					"decode(): no valid formatstring"
				);
			}
			++pos;
			switch(str[pos]){
				case '\\': returnstr+='\\'; break;
				case 'n':  returnstr+='\n'; break;
				case 't':  returnstr+='\t'; break;
				case '"':  returnstr+='"';  break;
				default : throw std::invalid_argument(
						"decode(): no valid formatstring"
					);
			}
		}
	}
	return returnstr;
}

namespace impl{
string textf_impl(const std::vector<string>& strings){
	//this is an internal function, so this can be done via an assert:
	//(if people use it from outside without care it is THEIR fault, if this explodes)
	assert(!strings.empty());
	
	std::ostringstream resultstream;
	std::istringstream formatstream(strings[0]);
	string tmp;
	unsigned int unspecified_inserts = 1;
	while(getline(formatstream, tmp, '%')){
		resultstream << tmp;
		if( formatstream.eof() ){
			break;
		}
		switch(formatstream.peek()){
			case '%': 
				resultstream << '%'; 
				formatstream.ignore();
				break;
			case 's':
				try{
					resultstream << strings.at(unspecified_inserts);
					++unspecified_inserts;
					formatstream.ignore();
				}
				catch(std::out_of_range &e){
					throw std::invalid_argument("invalid formatstring");
				}
				break;
			default:
				getline(formatstream, tmp, 's');
				try{
					resultstream << strings.at(stoi(tmp));
				}
				catch(std::out_of_range &e){
					throw std::invalid_argument("invalid formatstring");
				}
		}
	}
	return resultstream.str();
}
} // namespace impl


std::string prefix_and_align(const std::string& prefix, const std::string& text){
	std::stringstream textstream(text);
	string tmp;
	string returnstring;
	getline(textstream, tmp);
	returnstring = prefix + tmp;
	if(!textstream.eof()){
		//keep in mind, that this is not a perfect solution
		auto prefix_size = u8len( prefix );
		string alignstr;
		for( decltype(prefix_size) i = 0; i <  prefix_size; ++i ){
			alignstr += ' ';
		}
		while( getline(textstream, tmp) ){
			returnstring += "\n" + alignstr + tmp;
		}
	}
	if(text.back() == '\n'){
		returnstring += '\n';
	}
	return returnstring;
}

std::size_t u8len(const std::string& str){
	std::size_t returnval = 0;
	for(const auto c: str){
		if( (~c) & (1 << 7) ){
			++returnval;
		}
		else if( (c & (1 << 7)) && (c & (1 << 6)) ){
			++returnval;
		}
	}
	return returnval;
}

} //namespace stringutils
} //namespace dlo
