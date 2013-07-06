#include "configuration.hpp"
#include "stringutils.hpp"
#include "output.hpp"

#include <list>
#include <tuple>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <cstdlib>

namespace dlo{

using namespace std;
using namespace stringutils;

//put the static members here:
std::string settings::app_name = "";
unordered_map<string,unordered_map<string,string> > settings::global_settings;


unordered_map<string, string> read_config_file(std::string filename, bool sections){
	ifstream file(filename.c_str());
	if(! file.is_open()){
		throw runtime_error("read_config_file(): can't open file");
	}
	unordered_map<string, string> data;
	string line;
	string section="";
	pair<string,string> tmp;
	std::string key;
	std::string value;
	unsigned int lineNumber=0;
	// in order to prevent iterator-invalidation, we need a std::list
	std::list<std::tuple<std::string, int, std::string>> unfound_references;
	while( getline(file, line) ){
		++lineNumber;
		line=strip(line);
		//ignore empty lines and comments:
		if(line.empty() || line[0] == '#'){
			continue;
		}
		if( sections && (line[0] == '[') && (line[line.size()-1] == ']') ){
			line.erase(0,1);
			line.erase(line.size()-1,1);
			section = strip(line);
			continue;
		}
		try{
			std::tie(key, value) = split_once(line,"=");
			key = strip(key);
			value = strip(value);
			
			if( sections && !section.empty() ){
				key = section + "::" + key;
			}
		}
		catch(const invalid_argument &e){
			errorf("invalid line in configfile (%s, %s): No delimiter (“%s”)", 
				filename, lineNumber, line);
			continue;
		}
		if( value.length() && value[0] != '"' ){
			if( sections && !section.empty() ){
				value = section + "::" + value;
			}
			if( data.count(value) ){
				value = data[value];
			}
			else{
				unfound_references.emplace_back( make_tuple(key, lineNumber,value) );
				continue;
			}
		}
		else{
			try{
				value = decode( value );
			}
			catch(invalid_argument& e){
			errorf("invalid line in configfile (%s, %s): Cannot decode string (“%s”)", 
				filename, lineNumber, line);
				continue;
			}
		}
		data.insert(std::make_pair(strip(key), value));
	}
	//now see whether some references can be resolved:
	bool found_reference = false;
	do{
		found_reference = false;
		for(auto it = unfound_references.begin(); it!=unfound_references.end();){
			auto match_it = data.find(std::get<2>(*it));
			if(match_it != data.end()){
				found_reference = true;
				data.insert(std::make_pair(std::get<0>(*it), match_it->second));
				it = unfound_references.erase(it);
			}
			else{
				++it;
			}
			
		}
	} while(found_reference);
	for(const auto& ref : unfound_references){
		errorf("undefined reference in configfile (%s, %s) for “%s” to “%s”",
			filename, std::get<1>(ref), std::get<0>(ref), std::get<2>(ref));
	}
	return data;
}

void settings::init(std::string app_name_){
	app_name = app_name_;
}

settings::settings(string config)
	: file( config )
{
	settings::enforce_initialization();
	if(config.empty()){
		return;
	}
	if(! global_settings.count(config) ){
		global_settings[config] = read_config_file( get_conf_dir() + config + ".ini");
	}
}

string settings::operator[](string key){
	settings::enforce_initialization();
	return global_settings[this->file][key];
}

std::string settings::get_value(std::string file, std::string key){
	settings::enforce_initialization();
	if( ! global_settings.count(file) ){
		global_settings[file] = read_config_file(get_conf_dir() + file + ".ini");
	}
	return global_settings[file][key];
}

std::string settings::get_conf_dir(){
	settings::enforce_initialization();
	std::string general_conf_dir;
	auto xdg_config_home = getenv("XDG_CONFIG_HOME");
	if(xdg_config_home){
		general_conf_dir = xdg_config_home;
	}
	else {
		auto home = getenv("HOME");
		if(!home){
			throw std::runtime_error{"neither $XDG_CONFIG_HOME nor $HOME are defined"};
		}
		general_conf_dir = home;
		general_conf_dir += "/.config";
	}
	return general_conf_dir + "/" + app_name + "/";
}

void settings::enforce_initialization(){
	if(app_name.empty()){
		throw uninitialised_config_error("The name of the programm was not set properly.");
	}
}

} //namespace dlo
