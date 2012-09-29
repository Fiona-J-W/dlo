
#include "stringutils.hpp"

#include <fstream>
#include <stdexcept>
#include <iostream>
#include <cstdlib>

namespace dlo{
namespace stringutils{

using namespace std;

string strip(string str){
	size_t pos=0;
	while(str[pos] == ' ' || str[pos] == '\t'){
		++pos;
	}
	str.erase(0,pos);
	if(str == ""){
		return "";
	}
	pos=str.size() - 1;
	while(str[pos] == ' ' || str[pos] == '\t'){
		//str[pos] = 0;
		--pos;
	}
	str.erase(pos+1);
	return str;
}



vector<string> split(string str, string delim){
	size_t pos = str.find(delim);
	vector<string> data;
	while( pos != string::npos ){
		data.push_back( str.substr(0, pos) );
		str.erase(0,pos+delim.size());
		pos=str.find(delim);
	}
	data.push_back(str);
	return data;
}

pair<string,string> split_once(string str, string delim){
	pair<string, string> data;
	size_t pos = str.find(delim);
	if( pos == string::npos ){
		throw invalid_argument("splitOnce(): delimeter not found");
	}
	data.first=str.substr(0,pos);
	data.second=str.substr(pos+delim.size());
	return data;
}

string decode(string str){
	str=strip(str);
	string returnstr="";
	if(str[0] != '"' || str[str.size()-1] != '"'){
		throw invalid_argument("decode(): no valid formatstring");
	}
	for(size_t pos=1; pos<str.size()-1 ; ++pos){
		if( str[pos] != '\\'){
			if( str[pos] == '"' ){
				throw invalid_argument(
					"decode(): no valid formatstring"
				);
			}
			returnstr+=str[pos];
		}
		else{
			if(pos+1 >= str.size()-1){
				throw invalid_argument(
					"decode(): no valid formatstring"
				);
			}
			++pos;
			switch(str[pos]){
				case '\\': returnstr+='\\'; break;
				case 'n':  returnstr+='\n'; break;
				case 't':  returnstr+='\t'; break;
				case '"':  returnstr+='"';  break;
				default : throw invalid_argument(
						"decode(): no valid formatstring"
					);
			}
		}
	}
	return returnstr;
}

string _textf_impl(const std::vector<string>& strings){
	if(strings.empty()){
		throw std::invalid_argument("no formatstring");
	}
	stringstream resultstream;
	stringstream formatstream(strings[0]);
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
				}
				catch(std::out_of_range &e){
					throw std::invalid_argument("invalid formatstring");
				}
				formatstream.ignore();
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

} //namespace stringutils
} //namespace dlo