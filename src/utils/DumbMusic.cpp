#ifndef NO_DUMB_MUSIC


#include <cassert>
#include "DumbMusic.hpp"

#define DELTA          65536.0f / SAMPLING_RATE


DumbMusic::DumbMusic(const char* name)
{
	Initialize(N_CHANNELS, SAMPLING_RATE);
	module_ = dumb_load_mod_quick(name);
	player_ = NULL;
}


DumbMusic::~DumbMusic()
{
	duh_end_sigrenderer(player_);
	unload_duh(module_);
}


void DumbMusic::Init()
{
	dumb_register_stdfiles();
}


void DumbMusic::Exit()
{
	dumb_exit();
}


bool DumbMusic::OnStart()
{
	const int sig = 0;
	const long pos = 0;
	player_ = duh_start_sigrenderer(module_, sig, N_CHANNELS, pos);
	return true;
}


bool DumbMusic::OnGetData(Chunk& data)
{
	// FIXME: le /2 est magique [sans lui, skipping]
	duh_render(player_, 16, 0, 1.0f, DELTA, BUFFER_SIZE / 2, samples_);

	data.Samples = samples_;
	data.NbSamples = BUFFER_SIZE; // nombre de samples
	return true;
}


void		DumbMusic::SetVolume(float &vol)
{
  this->volume_ = vol / 100.f;
}

#endif // NO_DUMB_MUSIC
