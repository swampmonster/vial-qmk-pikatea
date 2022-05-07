/* Copyright 2020 Jack Kester
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
#include "analog.h"

// Define potentiometer pin
#define POT_PIN F4

// Get midi device handle
extern MidiDevice midi_device;
// Store last read value from pot
int last_val = 0;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_all(
        KC_F13,   KC_F14, KC_F15,   KC_F16,   KC_MEDIA_PREV_TRACK,    KC_MEDIA_PLAY_PAUSE,    KC_MEDIA_NEXT_TRACK,    KC_MUTE
    ),
    [1] = LAYOUT_all(
        _______,   _______,   _______,   _______,   _______,    _______,    _______,    _______
    ),
    [2] = LAYOUT_all(
        _______,   _______,   _______,   _______,   _______,    _______,    _______,    _______
    )
};

// Re-map the value read from the ADC pin to specified range (stolen from arduino).
long remap(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Because we want the value from the pot to be updated as quickly as possible we put
// the reading and sending of the value in this function so that it runs on every loop.
void matrix_scan_user(void) {
    uint16_t pot_value = analogReadPin(POT_PIN); // read pot value.
    int val = remap(pot_value, 0, 1023, 0, 100); // remap from 0-1024 to 0-100 (in a pretty lossy way).

    // Check if the value has changed so we don't spam updates.
    if (val != last_val) {
        last_val = val;
        midi_send_cc(&midi_device, 1, 7, val);
    }
}
