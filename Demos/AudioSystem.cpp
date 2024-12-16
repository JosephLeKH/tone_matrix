#include <QMediaDevices>
#include <QAudioSink>
#include <QIODevice>
#include <QtTypes>
#include <QFile>
#include <QTimer>
#include <iostream>
#include <cstdlib>
#include <cstdint>
#include "AudioSystem.h"
#include "error.h"
#include "gthread.h"
#include "gwindow.h"
using namespace std;

namespace {
    /* Number of sound samples to buffer. */
    const qint64 kInternalBufferSize = 4000;

    /* Adapter from a callback function to a QIODevice. */
    class AudioAdapter: public QIODevice {
    public:
        /* Stash audio callback. */
        AudioAdapter(AudioCallback callback) : callback(callback) {

        }

        bool isSequential() const override {
            return false;
        }

        qint64 bytesAvailable() const override {
            //cout << "Asked for byte count." << endl;
            return numeric_limits<qint64>::max();
        }

    protected:
        /* Hand back the data in the buffer. */
        qint64 readData(char* data, qint64 maxSize) override {
            /* Convert from bytes to floats. */
            maxSize /= sizeof(float);

            /* See how many we can actually read, then do the read. */
            qint64 toRead = min(maxSize, kInternalBufferSize);
            callback(callbackBuffer, toRead);

            /* Convert format. */
            for (int i = 0; i < toRead; i++) {
                dataBuffer[i] = clamp(static_cast<float>(callbackBuffer[i]), -1.0f, +1.0f);
            }

            /* Send to the speakers! */
            memcpy(data, dataBuffer, toRead * sizeof(float));
            return toRead * sizeof(float);
        }

        /* Only readable, not writable */
        qint64 writeData(const char*, qint64) override {
            return -1;
        }

    private:
        AudioCallback callback;
        double        callbackBuffer[kInternalBufferSize];
        float         dataBuffer[kInternalBufferSize];
    };

    /* Global sample rate. */
    int theSampleRate = 44100;
}

int AudioSystem::sampleRate() {
    int result;
    GThread::runOnQtGuiThread([&] {
        result = theSampleRate;
    });
    return result;
}

void AudioSystem::setSampleRate(int rate) {
    if (rate <= 0) {
        error("Sample rate must be positive.");
    }

    GThread::runOnQtGuiThread([&] {
        theSampleRate = rate;
    });
}

AudioSystem::AudioSystem() {
    /* Set the audio format. */
    format.setSampleRate(sampleRate());
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Float);

    /* Confirm this audio format is supported. Code adapted from
     * https://doc.qt.io/qt-6/qaudiosink.html.
     */
    QAudioDevice info(QMediaDevices::defaultAudioOutput());
    //std::cout << "Standard output device is " << info.description().toStdString() << std::endl;

    if (!info.isFormatSupported(format)) {
        error("Error: Output format is not supported.");
    }
}

void AudioSystem::play(AudioCallback callback) {
    GThread::runOnQtGuiThread([&] {
        instance()->state = State::PLAYING;
        instance()->playImpl(callback);
    });
}

void AudioSystem::stop() {
    GThread::runOnQtGuiThread([&] {
        instance()->state = State::STOPPED;
        instance()->stopImpl();
    });
}

void AudioSystem::handleStateChanged(QAudio::State newState) {
    //cout << "State is now " << newState << endl;
    //cout << "Error?       " << audio->error() << endl;
    if (newState == QAudio::IdleState) {
        /* If we were asked to stop by the user, stop. */
        if (state == State::STOPPED) {
            stopImpl();
        }

        /* Otherwise there should still be data generated. Restart. */
        audio->start(device);
    }
}

void AudioSystem::playImpl(AudioCallback callback) {
    if (!GThread::iAmRunningOnTheQtGuiThread()) {
        error("Internal threading error. Contact htiek@cs.stanford.edu to report a bug.");
    }

    /* Wire the buffer into an AudioAdapter wrapper. */
    //cout << "Making adapter." << endl;
    device = new AudioAdapter(callback);
    device->open(QIODevice::ReadOnly);

    /* Get an audio sink we can write to. */
    //cout << "Making audio sink." << endl;
    audio = new QAudioSink(QMediaDevices::defaultAudioOutput(), format/*, QThread::currentThread()*/);

    /* Attach to the audio sink so we are notified about state changes. */
    //cout << "Connecting us." << endl;
    connect(audio, &QAudioSink::stateChanged, this, &AudioSystem::handleStateChanged);

    /* Start sound transfer. */
    //cout << "Starting audio." << endl;
    audio->start(device);

    //cout << "Done." << endl;
}

void AudioSystem::stopImpl() {
    if (!GThread::iAmRunningOnTheQtGuiThread()) {
        error("Internal threading error. Contact htiek@cs.stanford.edu to report a bug.");
    }

    if (audio == nullptr) {
        //cout << "Asked to stop, but already stopped." << endl;
        return;
    }

    //std::cout << "   Stopping audio device." << std::endl;
    audio->stop();

    //cout << "    Deleting objects." << endl;
    delete audio;
    delete device;

    /* Null everything out so that multiple stops have no effect. */
    audio  = nullptr;
    device = nullptr;
}

AudioSystem* AudioSystem::instance() {
    if (!GThread::iAmRunningOnTheQtGuiThread()) {
        error("Internal threading error. Contact htiek@cs.stanford.edu to report a bug.");
    }

    static AudioSystem* theInstance = nullptr;
    if (theInstance == nullptr) {
        //cout << "Creating The One And Only AudioDevice." << endl;
        theInstance = new AudioSystem();
    }
    return theInstance;
}
