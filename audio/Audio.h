#pragma once


namespace Audio
{



void set_tracks(int num);
// Change the number of tracks
// If the number is reduced, and upper tracks are playing, they will be stopped
// The default number of tracks is 64

int play(SoundClip *sc, int track = -1, bool repeat = false, float vol = 1.0f, float pan = 0, float pitch = 1.0f, float pos = 0);
// Play a sound clip
// If 'track' is -1, the first available track will be used.  If there is no available
// tracks, the sound clip will not be played and -1 will be returned
// If 'track' is a valid track index, that track will be used.  If a sound clip is already
// playing on that track, it will be stopped
// 'repeat' controls wether the sound will restart from the beginning once done playing
// 'vol' is the volume to set for the track, 1.0 being normal
// 'pan' is the pan value to set for the track, -1.0 being left, 0.0 center and 1.0 right
// 'pitch' is the pitch value to set for the track, 1.0 being normal
// 'pos' is the position where the sound should start playing, between 0 and 1
// Returns the channel index used for playing

void stop_clip(SoundClip *sc);
// Stop any track playing this clip
// Called during SoundClip's destructor to avoir segfaults

float get_volume();
void set_volume(float vol);
// Get/set the master volume

bool is_track_active(int track);
// Returns true if the given track index is currently active


//
// The following functions act on tracks, referenced by their index
// The functions do not check wether a track is active or not, and do
// the change either way
//

float get_volume(int track);
void set_volume(int track, float vol);
// Get/set a track volume

float get_pan(int track);
void set_pan(int track, float pan);
// Get/set a track's pan value, clamped between -1 and 1

float get_pitch(int track);
void set_pitch(int track, float pitch);
// Get/set a track's pitch value
// A pitch of 0 essentially pauses playback


}
