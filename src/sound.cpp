/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

// Welcome to the "AudioFileReading" example. In this example, we will show you
// how to use CLAM::MultiChannelAudioFileReader, a Processing that allows to read
// simultaneously several channels at once from the same file.
//
// Before attempting to follow this example you should take a look first on
// the "FileInfo" example ( examples/FileInfo_example.cxx )

// We must include these headers to have access to classes
// AudioFile, MultiChannelAudioFileReader and MultiChannelAudioFileReaderConfig
#include "sound.h"

#include <iostream>
#include <CLAM/AudioFile.hxx>
#include <CLAM/MultiChannelAudioFileReader.hxx>
// Also we need the declaration of CLAM::Audio object, the object which
// represents an ( or portion of an ) audio signal
#include <CLAM/Audio.hxx>
// For accessing <cmath> header in a cross-platform way
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

#include "io.h"

using namespace std;

namespace aire {

Sound::Sound(string videofile, Log log)
	:LOG(log)
{
	video_file = videofile;
}

Sound::Sound(Video video)
	:LOG(video.LOG)
{
	video_file = video.video_file;
}


Sound::~Sound() {
	// TODO Auto-generated destructor stub
}


bool Sound::getWoodenHit()
{

	IO io = IO(video_file);
	string audiofile = io.createAudioFile(video_file);


	int threshold = 800;
	// Now we are pretty sure that argv[1] holds a parameter.
	// We create a configuration for the MultiChannelAudioFileReader,
	// which will read the samples from the file
	CLAM::MultiChannelAudioFileReaderConfig cfg;
	// We set the 'SourceFile' config attribute to the parameter we received
	cfg.SetSourceFile(audiofile);
	// Intantiate the processing
	CLAM::MultiChannelAudioFileReader reader;

	// And configure it
	if ( !reader.Configure(cfg) )
	{
		std::cerr
			<< "Error opening file: " << cfg.GetSourceFile() << std::endl
			<< reader.GetConfigErrorMessage() << std::endl;
		exit(-1);
	}

	// First of all, we must decide which will be our 'read size' i.e. the number
	// of samples we will want to get from each channel at once.
	const CLAM::TSize readSize = 1024;  //reader.GetHeader().GetSamples();


	// And now we must setup the CLAM::Audio to hold the incoming samples from
	// each file channel.
	std::vector<CLAM::Audio> outputs;
	outputs.resize( reader.GetHeader().GetChannels() );

	std::vector<CLAM::Audio> audiovector;

	// And now we set the size of each Audio object to our intended 'read size'
	for ( unsigned i = 0; i < outputs.size(); i++ )
	{
		outputs[i].SetSize( readSize );
	}

	// Now we can safely Start() the processing
	reader.Start();

	// We will implement a crappy text-based VUmeter just to some elementary
	// aspects of working with Audio's. What's a VUmeter? A vumeter is just
	// sort of gauge that indicates the loudness of a given signal. This
	// 'loudness' can be computed for each samples as:
	//     loudness[ n ] = 20. * log10( fabs( x[n] ) )
	// Note that this will give a number in the [0,-inf] range, in dB scale. What
	// is interesting is to see which is the range of this loudness for a given
	// audio fragment: it might be understood as a measure of how 'loud' is
	// that fragment.

	// We will use this counter for tracking the current frame index
	//CLAM::TIndex frameCount = 1;

	// When will the loop end? Whenever the EOF is reached ;) i.e. when the Processing
	// cannot generate any more Audio objects.
	while ( reader.Do( outputs ) )
	{
		audiovector.push_back(outputs.at(0));
	}
	// And stop it:
	reader.Stop();



	std::vector<std::vector<float> > result;
	for (unsigned int i = 0; i < audiovector.size(); ++i) {


		CLAM::Audio myaudio = audiovector.at(i);
		// Spectrum settings
		CLAM::SpectrumConfig ssettings;
		ssettings.GetType().bMagPhase=true;
		ssettings.GetType().bComplex=false;
		ssettings.SetSize(myaudio.GetSize()/2+1);

		// Spectrum creation
		CLAM::Spectrum myspectrum(ssettings);

		// Processing object configuration
		CLAM::FFTConfig fconfig;
		fconfig.SetAudioSize(myaudio.GetSize());

		// Processing object creation
		CLAM::FFT myfft(fconfig);

		// Processing object execution

		// This puts the object in execution mode.
		myfft.Start();

		// And this performs the computation.
		myfft.Do(myaudio,myspectrum);

		CLAM::DataArray magarray = myspectrum.GetMagBuffer();
		magarray.Size();
		magarray.GetPtr();

		std::vector<CLAM::DataArray> myvec;
		myvec.push_back(magarray);


		//std::cout << myarray.Size() << std::endl;	//####

		std::vector<float> data;
		for (int j = 0; j < magarray.Size(); ++j) {
			float mydata = *(magarray.GetPtr()+j);
			data.push_back(mydata);
			//std::cout << mydata << std::endl;
		}
		result.push_back(data);


	}

	std::stringstream content;
	cout << endl;//####
	for (unsigned int i = 0; i < result.size(); ++i) {
		//cout << accumulate(result.at(i).begin(),result.at(i).end(),0.0) << ", ";
		if (threshold <= accumulate(result.at(i).begin(),result.at(i).end(),0.0)){
			cout << "selecting shot sound value::" << threshold << endl;//####
			return true;

		}
	}
	cout << endl;//####
	return false;
}

}
