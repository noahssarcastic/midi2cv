#include <queue>

#include "daisy_seed.h"

#include "./midi.cpp"
#include "./pitch.cpp"

enum class NotePriority
{
    HIGH,
    LOW,
    LAST
};

const float PEAK_PITCH_VOLTAGE = 3.3f;
const NotePriority NOTE_PRIORITY = NotePriority::LAST;

int main(void)
{
    daisy::DaisySeed hw;
    daisy::MidiUartHandler midi;

    // Daisy Seed config
    hw.Init();

    // DAC config
    daisy::DacHandle::Config dac_cfg;
    dac_cfg.bitdepth = daisy::DacHandle::BitDepth::BITS_12;
    dac_cfg.buff_state = daisy::DacHandle::BufferState::ENABLED;
    dac_cfg.mode = daisy::DacHandle::Mode::POLLING;
    dac_cfg.chn = daisy::DacHandle::Channel::ONE;
    hw.dac.Init(dac_cfg);

    // Midi Receiver config
    daisy::MidiUartHandler::Config midi_config;
    midi.Init(midi_config);
    midi.StartReceive();

    // Debug LED
    bool led_state = false;

    // Store NoteOnEvent info for multiple key presses
    std::vector<daisy::NoteOnEvent> pressed_notes;

    int pitch = 0;

    while (1)
    {
        hw.SetLed(led_state);
        hw.dac.WriteValue(daisy::DacHandle::Channel::ONE, pitch);

        parse_midi(midi, pressed_notes);

        led_state = !pressed_notes.empty();

        if (!pressed_notes.empty())
        {
            daisy::NoteOnEvent note_to_play;
            switch (NOTE_PRIORITY)
            {
            case NotePriority::LAST:
                note_to_play = pressed_notes.back();
                break;
            case NotePriority::HIGH:
                // TODO: implement
                note_to_play = pressed_notes.back();
                break;
            case NotePriority::LOW:
                // TODO: implement
                note_to_play = pressed_notes.back();
                break;
            default:
                note_to_play = pressed_notes.back();
            }
            pitch = get_pitch(note_to_play, PEAK_PITCH_VOLTAGE);
        }
    }
}