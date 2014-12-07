/*
 * core.h
 *
 *  Created on: Jul 25, 2014
 *      Author: ruwan
 */
#include <iostream>
#include <CLAM/AudioFile.hxx>
#include <CLAM/MultiChannelAudioFileReader.hxx>
#include <CLAM/Audio.hxx>
#include <CLAM/CLAM_Math.hxx>
#include <CLAM/DataTypes.hxx>
#include <CLAM/FFT.hxx>
#include <CLAM/FFTConfig.hxx>
#include <CLAM/SpectrumConfig.hxx>
#include <CLAM/XMLStorage.hxx>
#include <CLAM/Array.hxx>

#include <vector>
#include <sstream>
#include <fstream>
#include <numeric>


#ifndef SOUND_H_
#define SOUND_H_

namespace aire {

class Sound {
public:
	Sound(Video vid);
	virtual ~Sound();
	bool getWoodenHit();
private:
	Video video;
	Log LOG;
	std::vector<float> edge_changes;
	std::vector<std::vector<float> > section_edge_changes;

};

} /* namespace aire */
#endif /* SOUND_H_ */
