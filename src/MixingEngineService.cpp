#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
    : decks{nullptr, nullptr}, active_deck(0), auto_sync(false), bpm_tolerance(0)
{
    std::cout << "[MixingEngineService] Initialized with 2 empty decks" << std::endl;
}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService() {
    std::cout << "[MixingEngineService] Cleaning up decks..." << std::endl;
    for (int i = 0; i < 2; i++) {
        if (decks[i] != nullptr) {
            delete decks[i];
            decks[i] = nullptr;
        }
    }
}


/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    std::cout << "\n=== Loading Track to Deck === " << std::endl;
    PointerWrapper<AudioTrack> clone = track.clone();
    if (!clone)
        return -1;

    int i = 1 - active_deck;
    std::cout << "[Deck Switch] Target deck: " << active_deck << std::endl;

    if (decks[i] != nullptr)
        delete decks[i];

    clone->load();
    clone->analyze_beatgrid();

    if (auto_sync && !can_mix_tracks(clone))
        sync_bpm(clone);

    decks[i] = clone.release();

    std::cout << "[Load Complete] ’" << track.get_title()
              << "’ is now loaded on deck " << i << std::endl;

    if (decks[active_deck] != nullptr) {
        std::cout << "[Unload] Unloading previous deck " << active_deck <<
            " (" << decks[active_deck]->get_title() << ")" << std::endl;
        delete decks[active_deck];
        decks[active_deck] = nullptr;
    }
    std::cout << "[Active Deck] Switched to deck " << i << std::endl;
    active_deck = i;

    return 0;
}

/**
 * @brief Display current deck status
 */
void MixingEngineService::displayDeckStatus() const {
    std::cout << "\n=== Deck Status ===\n";
    for (size_t i = 0; i < 2; ++i) {
        if (decks[i])
            std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
        else
            std::cout << "Deck " << i << ": [EMPTY]\n";
    }
    std::cout << "Active Deck: " << active_deck << "\n";
    std::cout << "===================\n";
}

/**
 * TODO: Implement can_mix_tracks method
 * 
 * Check if two tracks can be mixed based on BPM difference.
 * 
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const {
    int bpm_diff = track->get_bpm() - decks[active_deck]->get_bpm();
    if (bpm_diff < 0) bpm_diff *= -1; // apply absolute value

    return bpm_diff <= bpm_tolerance;
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
    // Your implementation here
}
