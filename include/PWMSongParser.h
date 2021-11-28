/*
 * PWMSongParser.h
 *
 *  Created on: Aug 13, 2015
 *      Author: Lucas Malo Bélanger
 */

#ifndef PWMSONGPARSER_H_
#define PWMSONGPARSER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stdint.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <algorithm>
#include <sstream>
#include <map>

/**
	\class PWMSongParser
	\brief Class that handles the music player.
	**/
class PWMSongParser {
public:
	PWMSongParser();
	virtual ~PWMSongParser();

	/**	\brief This function reads a txt file and parses it into usable commands.
	 	*@param filename File to be read.
		**/
	void readFile(std::string filename);

	/**	\brief This function reads a string and parses it into usable commands.
	 	*@param buffer String buffer to be read.
		**/
	void readBuffer(std::string buffer);

	/**	\brief This function plays music from the commands.
		**/
	virtual void play() const;

	/** \var frequencyPath
	 	\brief Path to the PWM frequency file in the Odroid.
	 	 **/
	std::string frequencyPath;
	/** \var enablePath
	 	\brief Path to the PWM output enable file in the Odroid.
	 	 **/
	std::string enablePath;
	/** \var dutyPath
	 	\brief Path to the duty cycle file in the Odroid.
	 	 **/
    std::string dutyPath;
private:
	/** \brief This function assigns the parameter commands to the Odroid files.
	 	*@param frequency The PWM frequency to be set.
	 	*@param length The desired note duration to be played.
	 **/
	virtual void tone(uint32_t frequency, uint32_t length) const;

	/** \brief This function initializes the note frequency map.
	 **/
	void initMap(void);

	/** \var command
	 	\brief Structure that contains parameters for each note.
	**/
	struct command
	{
		uint32_t frequency;
		uint32_t length;
		uint32_t delay;
	};
	
	/** \var notes
	 	\brief Map that contains all the notes and associated frequencies.
	**/
	std::map<std::string, int> notes;

	/** \var _commands
	 	\brief List of commands that composes the music.
	**/
	std::vector<command> _commands;
protected :
	/**
	 	\brief This function accesses the commands.
	**/
	const std::vector<command>& getCommands() const;
};

#endif /* PWMSONGPARSER_H_ */
