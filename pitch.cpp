#include <queue>

#include "daisy_seed.h"

/**
 * Calculate note voltage (v/oct).
 */
float note_to_volts(uint8_t note, float v_upper)
{
    /* Midi middle C is 60, let's center around that.
    Take the note offset from center, multiply by 1/12, and offset by
    the center of our V range [0,v_upper]. */
    return (note - 60) * (1.0f / 12.0f) + v_upper / 2.0f;
}

/**
 * Clamp a DAC value between [0,4095].
 */
int clamp_dac_value(int i)
{

    if (i > 4095)
    {
        return 4095;
    }
    else if (i < 0)
    {
        return 0;
    }
    return i;
}

/**
 * Convert V ([0,v_upper]) to DAC ([0, 4095]).
 */
int volts_to_dac(float note_voltage, float v_upper)
{
    /* For intervals [a,b] and [c,d]:
    f(x) = c + (d - c) / (b - a) * (x - a); */
    return clamp_dac_value(int(4095 / v_upper * note_voltage));
}

/**
 *
 */
int get_pitch(daisy::NoteOnEvent note_to_play, float v_upper)
{
    float note_voltage = note_to_volts(note_to_play.note, v_upper);
    return volts_to_dac(note_voltage, v_upper);
}