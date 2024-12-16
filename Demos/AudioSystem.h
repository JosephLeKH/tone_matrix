#pragma once

#include <QObject>
#include <QAudioSink>
#include <functional>

using AudioCallback = std::function<void (double*, int)>;

class AudioSystem: public QObject {
    Q_OBJECT

public:
    /* These can be called from anywhere. */
    static void play(AudioCallback callback);
    static void stop();

    static int  sampleRate();
    static void setSampleRate(int sampleRate);

    /* All of these can only be called on the Qt GUI thread. */


public slots:
    void handleStateChanged(QAudio::State newState);

private:
    AudioSystem();
    AudioSystem(const AudioSystem&) = delete;
    void operator= (AudioSystem) = delete;

    static AudioSystem* instance();
    void playImpl(AudioCallback toPlay);
    void stopImpl();

    QAudioFormat  format;
    QAudioSink*   audio;
    QIODevice*    device;

    /* Keep track of whether we are intending to play sound so that we
     * can recover from errors.
     */
    enum class State {
        PLAYING, STOPPED
    } state = State::STOPPED;
};
