/*
 * core.cpp
 *
 *  Created on: Jul 25, 2014
 *      Author: ruwan
 */

#include "edge.h"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

namespace aire {

Sound::Sound(Video vid)
	:video(vid)
{
	IO io = IO(video);
	io.createAudioFile();
}

Sound::~Sound() {
	// TODO Auto-generated destructor stub
}

using namespace std;


bool Sound::getWoodenHit(){

	int threshold = 100;
	// Now we are pretty sure that argv[1] holds a parameter.
	// We create a configuration for the MultiChannelAudioFileReader,
	// which will read the samples from the file
	CLAM::MultiChannelAudioFileReaderConfig cfg;
	// We set the 'SourceFile' config attribute to the parameter we received
	cfg.SetSourceFile( "/home/ruwan/match00.wav" );
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
	for (unsigned int i = 0; i < result.size(); ++i) {
		if (threshold <= accumulate(result.at(i).begin(),result.at(i).end(),0.0)){
			cout << "selecting shot sound value::" << threshold << endl;//####
			return true;
		}
	}
	return false;
}

} /* namespace aire */
