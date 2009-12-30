#ifndef DUMBMUSIC_HPP
#define DUMBMUSIC_HPP

#ifndef NO_DUMB_MUSIC

#include <dumb.h>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

#define SAMPLING_RATE  44100
#define N_CHANNELS     2
#define BUFFER_LENGTH  1	// Fixme: Magique?
#define BUFFER_SIZE    SAMPLING_RATE / BUFFER_LENGTH

/**
 * Musique chargée avec libdumb (support du format .mod)
 */
class DumbMusic: public sf::SoundStream
{
public:
	DumbMusic(const char* name);
	~DumbMusic();

	/**
	 * Initaliser le module DUMB en début de programme
	 */
	static void Init();
	
	/**
	 * Quitter le module DUMB en fin de programme
	 */
	static void Exit();

private:
	bool OnStart();

	bool OnGetData(Chunk& data);
	
	sf::Int16 samples_[BUFFER_SIZE * N_CHANNELS];

	DUH* module_;
	DUH_SIGRENDERER* player_;
};

#endif // NO_DUMB_MUSIC

#endif // DUMBMUSIC_HPP

