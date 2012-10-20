#include "i18n.hpp"

#include <stdexcept>
#include <unordered_map>

namespace dlo{

using std::string;

namespace impl{
	::std::unordered_map<string,string> dict;
}

string i18n(const string& str){
	if( impl::dict.empty() ){
		return str;
	}
	try{
		return impl::dict.at(str);
	}
	catch(std::out_of_range &e){
		return str;
	}
}

void set_lang(string lang);

void set_dict(std::unordered_map<string, string> dict){
	impl::dict = dict;
}

}
