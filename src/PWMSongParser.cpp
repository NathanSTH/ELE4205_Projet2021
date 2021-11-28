/*
 * PWMSongParser.cpp
 *
 *  Created on: Aug 13, 2015
 *      Author: Lucas Malo Bélanger
 *  Modified on: Nov 8, 2016
 *           by: R. Gourdeau
 */

#include "../include/PWMSongParser.h"

using namespace std;


PWMSongParser::PWMSongParser() {
	system("cd /sys/devices/");
	system("modprobe pwm-meson");
	system("modprobe pwm-ctrl");
	system("cd");

	frequencyPath = "/sys/devices/pwm-ctrl.42/freq0";
	enablePath = "/sys/devices/pwm-ctrl.42/enable0";
	dutyPath = "/sys/devices/pwm-ctrl.42/duty0";
	ofstream ofs (dutyPath);
	if(ofs.is_open())
		ofs << 700;
	else
		cout << "error opening " << dutyPath << endl;
	initMap();
	ofs.close();
}


PWMSongParser::~PWMSongParser() {
	ofstream ofs (enablePath);
	if(ofs.is_open())
		ofs << 0;
	else
		cout << "error opening " << enablePath << endl;
	ofs.close();
	//system((std::string("echo 0 > ") + enablePath).c_str());
}

void PWMSongParser::play() const
{
	const std::vector<command>& commands = getCommands();
	for(std::vector<command>::const_iterator it = commands.begin(); it != commands.end(); it++)
	{
		tone(it->frequency,it->length);
		usleep((it->delay - it->length) * 1000);
	}
}


void PWMSongParser::tone(uint32_t frequency, uint32_t length) const
{
	std::stringstream ss_freq;
	ss_freq << frequency;

	ofstream ofs_freq (frequencyPath);
	if(ofs_freq.is_open())
		ofs_freq << ss_freq.str();
	else
		cout << "error opening " << frequency << endl;
	ofs_freq.close();

	ofstream ofs_en (enablePath);

	if(frequency != 0){
		if(ofs_en.is_open())
			ofs_en << 1;
		else
			cout << "error opening " << enablePath << endl;
	}
	ofs_en.close();

	usleep(length*1000);

	ofs_en.open(enablePath);

	if(ofs_en.is_open())
		ofs_en << 0;
	else
		cout << "error opening " << enablePath << endl;
	ofs_en.close();
}

void PWMSongParser::readFile(std::string filename)
{
	std::string data1,data2,data3, data4, data5, frequency, length, delay;
	_commands.clear();

	command c;

	std::ifstream file(filename.c_str(), std::ifstream::in);
	if (file.fail()) {
        	std::cout << "Failed to open "+filename << std::endl;
		exit(-1);
	}	
	while(!file.eof())
	{
		file >> data1 >> data2 >> data3;
		file.ignore(1);

		file >> data4 >> data5;
		file.ignore(1);

		frequency.assign(data2, 10,3);
		length.assign(data3,7,3);
		delay.assign(data5, 0, 3);

		c.frequency = atoi(frequency.c_str());
		c.delay = atoi(delay.c_str());
		c.length = atoi(length.c_str());

		_commands.push_back(c);
	}
	file.close();
}

void PWMSongParser::readBuffer(std::string buffer)
{
	std::string bpm,data,len;
	_commands.clear();

	command c;

	std::stringstream ss;
	ss.str(buffer);

	if (ss.fail()) {
		std::cout << "Failed to read buffer" << std::endl;
		exit(-1);
	}

	ss >> bpm;
	float beat = atoi(bpm.c_str());
	float b_len = 1000*60/(beat);

	while(ss >> data){
		data = data.substr(0,data.length()-1);

		ss.unget();
		ss >> len;

		int tmp = 0;
		switch(atoi(len.c_str())){
		case 1:
			tmp = 4*b_len;
			break;
		case 2:
			tmp = 2*b_len;
			break;
		case 4:
			tmp = 1*b_len;
			break;
		case 8:
			tmp = 0.5*b_len;
			break;
		default:
			cout << "wrong length number" << endl;
			break;
		}

		if(data == "R"){
			c.frequency = 0;
			c.delay = c.length;
			c.length = 0;
		}else{
			c.frequency = notes.find(data)->second;
			c.length = tmp;
			c.delay = c.length;
		}
		_commands.push_back(c);
	}

}


const std::vector<PWMSongParser::command>& PWMSongParser::getCommands() const
{
	return _commands;
}

void PWMSongParser::initMap(void){
	notes.insert(std::pair<string, int>("A", 440));
	notes.insert(std::pair<string, int>("A#", 466));
	notes.insert(std::pair<string, int>("B", 494));
	notes.insert(std::pair<string, int>("C", 523));
	notes.insert(std::pair<string, int>("C#", 554));
	notes.insert(std::pair<string, int>("D", 587));
	notes.insert(std::pair<string, int>("D#", 622));
	notes.insert(std::pair<string, int>("E", 659));
	notes.insert(std::pair<string, int>("F", 698));
	notes.insert(std::pair<string, int>("F#", 734));
	notes.insert(std::pair<string, int>("G", 784));
	notes.insert(std::pair<string, int>("G#", 831));
	notes.insert(std::pair<string, int>("a", 880));
	notes.insert(std::pair<string, int>("a#", 932));
	notes.insert(std::pair<string, int>("b", 988));
	notes.insert(std::pair<string, int>("c", 1046));
	notes.insert(std::pair<string, int>("c#", 1109));
	notes.insert(std::pair<string, int>("d", 1175));
	notes.insert(std::pair<string, int>("d#", 1245));
	notes.insert(std::pair<string, int>("e", 1319));
	notes.insert(std::pair<string, int>("f", 1397));
	notes.insert(std::pair<string, int>("f#", 1480));
	notes.insert(std::pair<string, int>("g", 1568));
	notes.insert(std::pair<string, int>("g#", 1661));
}
