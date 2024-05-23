#include "daisy_seed.h"
#include <queue>

using namespace daisy;

DaisySeed hw;
MidiUartHandler midi;

void GetMidiTypeAsString(MidiEvent &msg, char *str)
{
    switch (msg.type)
    {
    case NoteOff:
        strcpy(str, "NoteOff");
        break;
    case NoteOn:
        strcpy(str, "NoteOn");
        break;
    case PolyphonicKeyPressure:
        strcpy(str, "PolyKeyPres.");
        break;
    case ControlChange:
        strcpy(str, "CC");
        break;
    case ProgramChange:
        strcpy(str, "Prog. Change");
        break;
    case ChannelPressure:
        strcpy(str, "Chn. Pressure");
        break;
    case PitchBend:
        strcpy(str, "PitchBend");
        break;
    case SystemCommon:
        strcpy(str, "Sys. Common");
        break;
    case SystemRealTime:
        strcpy(str, "Sys. Realtime");
        break;
    case ChannelMode:
        strcpy(str, "Chn. Mode");
        break;
    default:
        strcpy(str, "Unknown");
        break;
    }
}

int main(void)
{
    // Declare a variable to store the state we want to set for the LED.
    bool led_state;
    led_state = false;

    /** Initialize our hardware */
    hw.Init();

    hw.StartLog();

    MidiUartHandler::Config midi_config;
    midi.Init(midi_config);

    midi.StartReceive();

    std::vector<NoteOnEvent> notes_pressed;

    /** Infinite Loop */
    while (1)
    {
        // Set the onboard LED
        hw.SetLed(led_state);

        /** Process MIDI in the background */
        midi.Listen();

        /** Loop through any MIDI Events (ignoring channel for now) */
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

            led_state = !notes_pressed.empty();
        }
    }
}