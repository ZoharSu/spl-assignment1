#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist), library() {}
/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    for (const SessionConfig::TrackInfo& track : library_tracks) {
        if (track.type == "MP3") {
            MP3Track *t = new MP3Track {
                track.title, track.artists, track.duration_seconds,
                track.bpm, track.extra_param1, bool(track.extra_param2)
            };
            std::cout << "MP3Track created: " << t->get_bitrate() << " kbps" << std::endl;
            library.push_back(t);
        } else { // WAV
            WAVTrack *t = new WAVTrack {
                track.title, track.artists, track.duration_seconds,
                track.bpm, track.extra_param1, track.extra_param2
            };
            std::cout << "WAVTrack created: " << t->get_sample_rate() << "Hz/"
                      << t->get_bit_depth() << "bit" << std::endl;
            library.push_back(t);
        }
    }
}

/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 * 
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    return playlist.find_track(track_title);
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
    std::cout << "[INFO] Loading playlist: " << playlist_name << std::endl;
    playlist = Playlist(playlist_name);
    int count = 0;

    for (size_t i : track_indices) {
        if (i < 1 ||  i > library.size()) {
            std::cout << "[WARNING] Invalid track index: " << i << std::endl;
            continue;
        }
        AudioTrack *track = library[i - 1]->clone().release();
        if (track == nullptr) {
            std::cout << "[ERROR] Clone failed at loadPlaylistFromIndices" << std::endl;
            continue;
        }
        track->load();
        track->analyze_beatgrid();
        playlist.add_track(track);

        count++;
        std::cout << "Added ’" << track->get_title() << "’ to playlist ’"
                               << playlist_name << "’" << std::endl;
    }
    std::cout << "[INFO] Playlist loaded: " << playlist_name
              << " (" << count << " tracks)" << std::endl;
}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    std::vector<std::string> v;
    for (AudioTrack* t : playlist.getTracks())
        v.push_back(t->get_title());

    return v;
}

DJLibraryService::~DJLibraryService() {
    for (AudioTrack *t : library)
        delete t;
}
