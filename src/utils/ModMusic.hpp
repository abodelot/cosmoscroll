#ifndef MODMUSIC_HPP
#define MODMUSIC_HPP

#include <SFML/Audio.hpp>

// DUMB library forward declarations
class DUH;
class DUH_SIGRENDERER;

/**
 * SFML wrapper around DUMB library for playing module music files (mod, s3m, xm, it)
 */
class ModMusic: public sf::SoundStream
{
public:
    ModMusic();

    ~ModMusic();

    /**
     * Open a music from an audio file
     */
    bool openFromFile(const std::string& filename);

    /**
     * Get the total duration of the music
     */
    sf::Time getDuration() const;

private:
    static struct Init
    {
        Init();  // static ctor
        ~Init(); // static dtor
    } s_init;

    static const int SAMPLING_RATE = 44100;
    static const int BUFFER_SIZE   = SAMPLING_RATE;
    static const int NB_CHANNELS   = 2; // Stereo

    void close();

    bool onGetData(Chunk& data);

    void onSeek(sf::Time timeOffset);

    sf::Int16        m_samples[BUFFER_SIZE];
    DUH*             m_module;
    DUH_SIGRENDERER* m_player;
};


#endif // MODMUSIC_HPP
