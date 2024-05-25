#include "daisy_seed.h"
#include <queue>

using namespace daisy;

DaisySeed hw;
MidiUartHandler midi;

const float PEAK_PITCH_VOLTAGE = 3.3f;

int main(void)
{
    // Daisy Seed config
    hw.Init();

    // DAC config
    DacHandle::Config dac_cfg;
    dac_cfg.bitdepth = DacHandle::BitDepth::BITS_12;
    dac_cfg.buff_state = DacHandle::BufferState::ENABLED;
    dac_cfg.mode = DacHandle::Mode::POLLING;
    dac_cfg.chn = DacHandle::Channel::ONE;
    hw.dac.Init(dac_cfg);

    // Midi Receiver config
    MidiUartHandler::Config midi_config;
    midi.Init(midi_config);
    midi.StartReceive();

    // Debug LED
    bool led_state = false;

    // Store NoteOnEvent info for multiple key presses
    std::vector<NoteOnEvent> notes_pressed;

    int pitch = 0;

    while (1)
    {
        hw.SetLed(led_state);
        hw.dac.WriteValue(DacHandle::Channel::ONE, pitch);

        midi.Listen();
        // Loop through any MIDI Events (ignoring channel for now)
        while (midi.HasEvents())
        {
            MidiEvent msg = midi.PopEvent();
            switch (msg.type)
            {
            case NoteOn:
            {
                notes_pressed.push_back(msg.AsNoteOn());
            }
            break;
            case NoteOff:
            {
                notes_pressed.pop_back();
            }
            break;
            default:
                break;
            }
        }

        led_state = !notes_pressed.empty();

        if (!notes_pressed.empty())
        {
            NoteOnEvent last_pressed = notes_pressed.back();

            /* Calculate note voltage (v/oct)
            Midi middle C is 60, let's center around that.
            Take the note offset from center, multiply by 1/12, and offset by
            the center of our V range [0,PEAK_PITCH_VOLTAGE]. */
            float note_voltage = (last_pressed.note - 60) * (1.0f / 12.0f) + PEAK_PITCH_VOLTAGE / 2.0f;

            /* Convert V ([0,PEAK_PITCH_VOLTAGE]) to DAC ([0, 4095])
            For intervals [a,b] and [c,d]:
            f(x) = c + (d - c) / (b - a) * (x - a); */
            pitch = int(4095 / PEAK_PITCH_VOLTAGE * note_voltage);

            // Clamp pitch between [0,4095]
            if (pitch > 4095)
            {
                pitch = 4095;
            }
            else if (pitch < 0)
            {
                pitch = 0;
            }
        }
        else
        {
            pitch = 0;
        }
    }
}