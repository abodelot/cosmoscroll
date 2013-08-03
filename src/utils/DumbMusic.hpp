#ifndef DUMBMUSIC_HPP
#define DUMBMUSIC_HPP

#include <dumb.h>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

/**
 * SFML wrapper around DUMB library for playing module music files
 */
class DumbMusic: public sf::SoundStream
{
public:
	/**
	 * Load DUMB library
	 */
	static void initDumb();

	/**
	 * Unload DUMB library
	 */
	static void exitDumb();

	DumbMusic();

	~DumbMusic();

	/**
	 * Open a music from an audio file
	 */
	bool openFromFile(const std::string& filename);

	/**
	 * Get the total duration of the music
	 */
	sf::Time getDuration() const;

private:
	static const int SAMPLING_RATE = 44100;
	static const int BUFFER_SIZE   = SAMPLING_RATE;
	static const int NB_CHANNELS   = 2; // stereo

	void close();

	bool onGetData(Chunk& data);

	void onSeek(sf::Time timeOffset);

	sf::Int16        m_samples[BUFFER_SIZE];
	DUH*             m_module;
	DUH_SIGRENDERER* m_player;
};


#endif // DUMBMUSIC_HPP
