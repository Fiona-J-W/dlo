#include "configuration.hpp"
#include "stringutils.hpp"
#include "output.hpp"

#include <list>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <cstdlib>

namespace dlo{

using namespace std;
using namespace stringutils;

//put the static member here:
std::string settings::app_name = "";
unordered_map<string,unordered_map<string,string> > settings::globalSettings;


unordered_map<string, string> readConfigFile(std::string filename, bool sections){
	ifstream file(filename.c_str());
	if(! file.is_open()){
		throw runtime_error("readConfigFile(): can't open file");
	}
	unordered_map<string, string> data;
	string line;
	string section="";
	pair<string,string> tmp;
	unsigned int lineNumber=0;
	list< pair<string, pair<int, string> > > unfoundReferences;
	while( getline(file, line) ){
		++lineNumber;
		line=strip(line);
		//ignore empty lines and comments:
		if(!line.length() || line[0] == '#'){
			continue;
		}
		if( sections && (line[0] == '[') && (line[line.size()-1] == ']') ){
			line.erase(0,1);
			line.erase(line.size()-1,1);
			section = strip(line);
			continue;
		}
		try{
			tmp = split_once(line,"=");
		}
		catch(const invalid_argument &e){
			errorf("invalid line in configfile (%s, %s): No delimiter (“%s”)", 
				filename, lineNumber, line);
			continue;
		}
		string value = strip( tmp.second );
		if( value.length() && value[0] != '"' ){
			if( data.count(value) ){
				value = data[value];
			}
			else{
				string key = strip(tmp.first);
				if( sections && !section.empty() ){
					key = section + "::" + key;
				}
				unfoundReferences.push_back( make_pair(key,
					make_pair(lineNumber,value)) );
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
		if( !sections ||  section.empty() ){
			data[ strip( tmp.first ) ] = value;
		}
		else{
			data[ section+"::"+strip(tmp.first) ] = value;
		}
	}
	//now see whether some references can be resolved:
	bool foundReference = false;
	do{
		foundReference = false;
		for(list< pair<string, pair<int, string> > >::iterator it = unfoundReferences.begin();
			it!=unfoundReferences.end();){
			if(data.count(it->second.second)){
				foundReference = true;
				data[it->first] = data[it->second.second];
				it = unfoundReferences.erase(it);
			}
			else{
				++it;
			}
			
		}
	} while(foundReference);
	for(list< pair<string, pair<int, string> > >::iterator it= unfoundReferences.begin();
		it!=unfoundReferences.end();++it){
		errorf("undefined reference in configfile (%s, %s) for “%s” to “%s”",
			filename, it->second.first, it->first, it->second.second);
	}
	return data;
}

void settings::init(std::string app_name_){
	app_name = app_name_;
}

settings::settings(string config)
	: file( config )
{
	if(app_name.empty()){
		throw uninitialised_config_error("The name of the programm was not set properly.");
	}
	if(config.empty()){
		return;
	}
	if(! globalSettings.count(config) ){
		try{
			globalSettings[config] = readConfigFile( text(
				getenv("XDG_CONFIG_HOME"), "/", app_name, "/", config, ".ini" ) );
		}
		catch(std::logic_error& e){
			fatal("XDG_CONFIG_HOME is not defined");
		}
	}
}

string settings::operator[](string key){
	if(app_name.empty()){
		throw uninitialised_config_error("The name of the programm was not set properly.");
	}
	return globalSettings[this->file][key];
}

std::string settings::getValue(std::string file, std::string key){
	if(app_name.empty()){
		throw uninitialised_config_error("The name of the programm was not set properly.");
	}
	if( ! globalSettings.count(file) ){
		try{
			globalSettings[file] = readConfigFile(text(
				getenv("XDG_CONFIG_HOME"), "/", app_name, "/", file, ".ini") );
		}
		catch(std::logic_error& e){
			fatal("XDG_CONFIG_HOME is not defined");
		}
	}
	return globalSettings[file][key];
}

} //namespace dlo
