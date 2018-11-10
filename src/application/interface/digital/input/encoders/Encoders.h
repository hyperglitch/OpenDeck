/*
    OpenDeck MIDI platform firmware
    Copyright (C) 2015-2018 Igor Petrovic

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "board/Board.h"
#include "database/Database.h"
#include "midi/src/MIDI.h"
#ifdef LEDS_SUPPORTED
#include "interface/digital/output/leds/LEDs.h"
#endif
#ifdef DISPLAY_SUPPORTED
#include "interface/display/Display.h"
#endif
#include "DataTypes.h"
#include "sysex/src/DataTypes.h"
#include "Constants.h"

///
/// \brief Encoder handling.
/// \defgroup interfaceEncoders Encoders
/// \ingroup interfaceDigitalIn
/// @{

class Encoders
{
    public:
    #ifdef LEDS_SUPPORTED
    #ifdef DISPLAY_SUPPORTED
    Encoders(Database &database, MIDI &midi, LEDs &leds, Display &display) :
    #else
    Encoders(Database &database, MIDI &midi, LEDs &leds) :
    #endif
    #else
    #ifdef DISPLAY_SUPPORTED
    Encoders(Database &database, MIDI &midi, Display &display) :
    #else
    Encoders(Database &database, MIDI &midi) :
    #endif
    #endif
    database(database),
    midi(midi)
    #ifdef LEDS_SUPPORTED
    ,leds(leds)
    #endif
    #ifdef DISPLAY_SUPPORTED
    ,display(display)
    #endif
    {}

    void update();

    private:
    Database            &database;
    MIDI                &midi;
    #ifdef LEDS_SUPPORTED
    LEDs        &leds;
    #endif
    #ifdef DISPLAY_SUPPORTED
    Display             &display;
    #endif

    ///
    /// \brief Holds current CC value for all encoders.
    /// Used only if encoder is configured in CC mode.
    ///
    uint8_t             ccValue[MAX_NUMBER_OF_ENCODERS] = { 0 };

    ///
    /// \brief Array holding last movement time for all encoders.
    ///
    uint32_t            lastMovementTime[MAX_NUMBER_OF_ENCODERS] = {};

    ///
    /// \brief Array holding previous encoder direction for all encoders.
    ///
    encoderPosition_t   lastDirection[MAX_NUMBER_OF_ENCODERS] = {};

    ///
    /// \brief Array holding current debounced direction for all encoders.
    ///
    encoderPosition_t   debounceDirection[MAX_NUMBER_OF_ENCODERS] = {};

    ///
    /// \brief Used to detect constant rotation in single direction.
    /// Once n consecutive movements in same direction are detected,
    /// all further movements are assumed to have same direction until
    /// encoder stops moving for DEBOUNCE_RESET_TIME milliseconds *or*
    /// n new consecutive movements are made in the opposite direction.
    /// n = ENCODER_DEBOUNCE_COUNT (defined in Constants.h)
    ///
    uint8_t             debounceCounter[MAX_NUMBER_OF_ENCODERS] = {};
};

/// @}