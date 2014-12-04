/*
 * core.h
 *
 *  Created on: Jul 25, 2014
 *      Author: ruwan
 */
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


#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <numeric>
#include <string>

#include <aire/video.h>


#ifndef SOUND_H_
#define SOUND_H_

using namespace std;
using namespace aire;

namespace aire {

class Sound {
public:
	Sound(Video vid);
	Sound(string videofile, Log log);
	virtual ~Sound();
	bool getWoodenHit();
	Log LOG;
private:
	string video_file;
	string audio_file;
};

} /* namespace aire */
#endif /* SOUND_H_ */
