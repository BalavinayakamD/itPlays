/**
 * @file PlayerState.h
 * @brief Defines the PlayerState class for shared player configuration.
 *
 * This class holds configuration settings that are shared between
 * the main window and the video screen widget.
 */

#ifndef PLAYERSTATE_H
#define PLAYERSTATE_H
#pragma once

/**
 * @class PlayerState
 * @brief Stores shared configuration settings for the video player.
 *
 * This class acts as a central configuration holder that can be
 * passed between components to share playback settings.
 */
class PlayerState
{
public:
    /**
     * @brief Seek offset in milliseconds for forward/backward navigation.
     *
     * Default: 5000ms (5 seconds)
     */
    int seekOffset = 5000;
    float volumeOffset = 0.2;
};

#endif // PLAYERSTATE_H
