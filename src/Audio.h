#pragma once

#define MAX_TRACKS 256


namespace Audio
{

void start(int freq = 44100, int chan = 2, int buf_size = 4096);
void stop();
// Start/stop the audio sub-system
// It is started with default values during initialization
// Only stop+start if you want to change the frequency or number of channels
// The buffer size has to be a power of 2
// In most cases, you shouldn't have to worry about calling these functions

void set_tracks(int num);
// Set the number of tracks
// This represents the number of simulteanous sounds that can be played
// This can be called at any time, even when sound output is active
// Lowering the number of tracks while there currently is a higher amount
// active will cause aribitrary tracks to be stopped immediatly, thus cutting
// the sound playing on that track before it terminates
// The default value is 16
// The ultimate maximum is MAX_TRACK (default 256)

int get_tracks();
// Get the curreny number of tracks

int get_free_track();
// Get the index of a free track
// This does not lock or reserve the track, but simply returns the index
// of the first track with no sample currently playing on it
// Returns the track index, or -1 of there is no free track

int play(SoundClip *sc, bool loop = false, bool start = true, int track = -1);
// Play a sound clip
// 'track' is the track number to use for playing the clip
// If it is -1, the first free track will be used, and its index
// will be returned.  If there is no free track, the sound clip
// will not play and -1 will be returned.
// If a specific track number is specified, that track will be
// used for playback.  If a clip is already playing on that track,
// it will immediatly be stopped and the new one will take its place.
//
// 'start' determines wether the playback starts immediatly (true) or
// is paused (false) until manually resumed
//
// If the audio system is not started (i.e. Audio::stop() was called),
// no error will be produced, and the sound clip will use the track
// as expected.  Playback will begin once audio is started

void stop(int track);
// Stop playback on the given track index

void pause(int track);
void resume(int track);
// Pause/resume playback on the given track index

float get_pos(int track);
// Return the current playback position of the given track
// The position ranges from 0 (start) to 1 (end)
// If the track is not playing, 1 will be returned

void set_pos(int track, float pos);
// Change the current playback position of the given track
// The position ranges from 0 (start) to 1 (end)

float get_volume(const Str& cat);
void set_volume(const Str& cat, float vol);
// Get/set the volume of a category
// If sounds of the specified category are currently playing on active
// tracks, their volume will be changed immediatly
// Volume values cannot be below zero, and shouldn't exceed 1.0

float get_volume();
void set_volume(float vol);
// Get/set the master volume
// This does not change the sound card's mixer data, but rather applies
// a multiplier to every output sample
// Volume values cannot be below zero, and shouldn't exceed 1.0

float get_volume(int track);
void set_volume(int track, float vol);
// Get/set the volume of a track

}
