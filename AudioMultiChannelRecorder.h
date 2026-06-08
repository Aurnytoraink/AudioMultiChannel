#ifndef AudioMultiChannelRecorder_h_
#define AudioMultiChannelRecorder_h_

#include "Arduino.h"
#include "AudioStream.h"
#include "Audio.h"

#include "FsLib/FsLib.h"
#include "EventResponder.h"

class AudioMultiChannelRecorder : public AudioStream {
public:
  AudioMultiChannelRecorder(unsigned char ninput, audio_block_t **iqueue);
  AudioMultiChannelRecorder(void)
    : AudioMultiChannelRecorder(2, inputQueueArray){}; // By default, it will record a stereo audio stream
  ~AudioMultiChannelRecorder(void)  // Called when class is been cleared
  {
    if (!isStopped()) stop();
  }
  void record(FsFile input_file);
  void pause(void);
  void stop(void);
  bool isRecording(void);
  bool isPaused(void);
  bool isStopped(void);
  virtual void update(void);

  friend class AudioRecorder;

private:
  unsigned char chanCnt;
  uint8_t state;
  enum state_e { STATE_STOP,
                 STATE_RECORDING };
  audio_block_t *inputQueueArray[2];  // For default value
  void flushBuffer();
  FsFile file;
};

class AudioRecorder : public AudioMultiChannelRecorder {
  public:
    AudioRecorder(): AudioMultiChannelRecorder(1, inputQueueArray) {};
};

#endif