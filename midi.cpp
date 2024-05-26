#include <queue>

#include "daisy_seed.h"

/**
 *
 */
void parse_midi(
    daisy::MidiUartHandler &midi,
    std::vector<daisy::NoteOnEvent> &pressed_notes)
{
    midi.Listen();
    // Loop through any MIDI Events (ignoring channel for now)
    while (midi.HasEvents())
    {
        daisy::MidiEvent msg = midi.PopEvent();
        switch (msg.type)
        {
        case daisy::NoteOn:
        {
            pressed_notes.push_back(msg.AsNoteOn());
        }
        break;
        case daisy::NoteOff:
        {
            // TODO: don't just pop last, remove released note
            pressed_notes.pop_back();
        }
        break;
        default:
            break;
        }
    }
}