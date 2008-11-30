#ifndef NO_AUDIO

#include "Music.hpp"
#include "MediaManager.hpp"


Music::Music(const char* name)
{
	dumb_register_stdfiles();
	Initialize(N_CHANNELS, SAMPLING_RATE);
	module_ = dumb_load_mod_quick(name);
}


Music::~Music()
{
	duh_end_sigrenderer(player_);
	unload_duh(module_);
	dumb_exit();
}


bool Music::OnStart()
{ 
	const int sig = 0;
	const long pos = 0;
	player_ = duh_start_sigrenderer(module_, sig, N_CHANNELS, pos);
	return true;
}


bool Music::OnGetData(Chunk& data)
{
	duh_render(player_, 16, 0, 1.0f, DELTA, BUFFER_SIZE / 2, samples_); // FIXME: le /2 est magique [sans lui, skipping]

	data.Samples = samples_;
	data.NbSamples = BUFFER_SIZE; // nombre de samples
	
	return true;
}

#endif /* NO_AUDIO */
