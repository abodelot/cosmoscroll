#ifndef Music_HPP
#define Music_HPP

#include <dumb.h>
#include <SFML/system.hpp>
#include <SFML/audio.hpp>

#define SAMPLING_RATE 44100
#define N_CHANNELS 2
#define DELTA 65536.0f / SAMPLING_RATE
#define BUFFER_LENGTH 1	// Fixme: Magique?
#define BUFFER_SIZE SAMPLING_RATE / BUFFER_LENGTH

class Music : public sf::SoundStream
{
public:
    Music(char* name);
    ~Music();
	
	virtual bool OnStart();

	virtual bool OnGetData(Chunk& data);
	
private:

	sf::Int16 samples_[BUFFER_SIZE * N_CHANNELS];

	DUH* module_;
	DUH_SIGRENDERER* player_;
};


#endif /* guard Music_HPP */

