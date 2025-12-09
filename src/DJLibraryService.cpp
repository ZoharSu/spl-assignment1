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

DJLibraryService::~DJLibraryService() {
    for (int i = 0; i < library.size(); i++) {
        AudioTrack* tmp = library[i];
        library[i] = nullptr;
        delete tmp;
    }
}

void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    //Todo: Implement buildLibrary method
    // std::cout << "TODO: Implement DJLibraryService::buildLibrary method\n"<< library_tracks.size() << " tracks to be loaded into library.\n";
    for (const SessionConfig::TrackInfo& t : library_tracks) {
        if (t.type == "MP3") {
            MP3Track* track = new MP3Track(t.title, t.artists, t.duration_seconds, t.bpm, t.extra_param1, t.extra_param2);
            library.push_back(track);
            std::cout << "MP3Track created: " << track->get_bitrate() << " kbps" << std::endl;
        } else {
            WAVTrack* track = new WAVTrack(t.title, t.artists, t.duration_seconds, t.bpm, t.extra_param1, t.extra_param2);
            library.push_back(track);
            std::cout << "WAVTrack created: " << track->get_sample_rate() << "Hz/"
                      << track->get_bit_depth() << "bit" << std::endl;
        }
    }
    std::cout << "[INFO] Track library built: " << library.size() << " tracks loaded" << std::endl;
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
    // Your implementation here
    return playlist.find_track(track_title);
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
    // Your implementation here
    std::cout << "[INFO] Loading playlist: " << playlist_name << std::endl;
    playlist = Playlist{playlist_name}; // IMPLEMENT MOVE ASSIGNMENT ON PLAYLIST
    for (int i : track_indices) {
        if (i >= 1 && i <= library.size()) {
            AudioTrack* t = library[i-1];
            PointerWrapper<AudioTrack> clone = t->clone();
            if (clone) {
                clone->load();
                clone->analyze_beatgrid();
                playlist.add_track(clone.release());
                std::cout << "Added \'" << t->get_title() << "\' to playlist \'"
                    << playlist_name << "\'" << std::endl;
            } else {
                std::cerr << "[ERROR] Clone failed at loadPlaylistFromIndices" << std::endl;
            }
        } else {
            std::cout << "[WARNING] Invalid track index: " << i << std::endl;
        }
    }
    std::cout << "[INFO] Playlist loaded: " << playlist_name << " (" << playlist.get_track_count()
    << " tracks)" << std::endl;
    // // For now, add a placeholder to fix the linker error
    // (void)playlist_name;  // Suppress unused parameter warning
    // (void)track_indices;  // Suppress unused parameter warning
}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    // Your implementation here
    std::vector<std::string> titles;
    for (AudioTrack* t : playlist.getTracks()) {
        titles.push_back(t->get_title());
    }
    return titles; // Placeholder
}
