#ifndef ANIMALS_AS_METER_RECORDINGCALLBACK_H
#define ANIMALS_AS_METER_RECORDINGCALLBACK_H

#include <oboe/Definitions.h>
#include <oboe/AudioStream.h>
#include "Obtain.h"
#include "SoundRecording.h"
#include "logging_macros.h"
#include "DrawParams.h"

#ifndef MODULE_NAME
#define MODULE_NAME  "RecordingCallback"
#endif

class RecordingCallback : public oboe::AudioStreamCallback {

private:
    const char* TAG = "RecordingCallback:: %s";
    SoundRecording* mSoundRecording = nullptr;
    DrawParams mDrawData{};
    obtain::Obtain obtainBeatDetector;

public:
    RecordingCallback() = default;

    explicit RecordingCallback(SoundRecording* recording, int32_t sampleRate) :
        obtainBeatDetector(obtain::Obtain(sampleRate)) {
        mSoundRecording = recording;
    }

    const DrawParams& GetDrawParams();

    oboe::DataCallbackResult
    onAudioReady(oboe::AudioStream *audioStream, void *audioData, int32_t numFrames);

    oboe::DataCallbackResult
    processRecordingFrames(oboe::AudioStream *audioStream, float *audioData, int32_t numFrames);
};

#endif //ANIMALS_AS_METER_RECORDINGCALLBACK_H