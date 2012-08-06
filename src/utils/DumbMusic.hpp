#ifndef DUMBMUSIC_HPP
#define DUMBMUSIC_HPP


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
	/**
	 * Quitter le module DUMB en fin de programme
	 */
	static void Exit();

	DumbMusic();
	~DumbMusic();

	bool openFromFile(const std::string& name);

private:
	bool onGetData(Chunk& data);

	void onSeek(sf::Time timeOffset);

	sf::Int16 samples_[BUFFER_SIZE * N_CHANNELS];

	DUH* module_;
	DUH_SIGRENDERER* player_;
	static bool inited_;
};


#endif // DUMBMUSIC_HPP

