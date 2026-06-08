#include <Audio.h>
#include <Wire.h>
#include "SdFat.h"
#include "AudioMultiChannelRecorder.h"

#define RECORD_DURATION_S 5*60
#define RECORD_DURATION_MS 1000*RECORD_DURATION_S
#define N_WAVEFORM 32


SdFs sd;      // Use fast SDIO protocol
FsFile file;  // Support FAT16/FAT32 and exFAT filesystem
String filename = "32_WAVEFORMS_5MIN.RAW";

audio_block_t *iqa[N_WAVEFORM];

AudioMultiChannelRecorder  recorder(N_WAVEFORM, iqa);
AudioSynthWaveformSine   waveforms[N_WAVEFORM];

// TODO find an better and modular way to initiate the AudioConnection
AudioConnection          patchCord1(waveforms[0], 0, recorder, 0);
AudioConnection          patchCord2(waveforms[1], 0, recorder, 1);
AudioConnection          patchCord3(waveforms[2], 0, recorder, 2);
AudioConnection          patchCord4(waveforms[3], 0, recorder, 3);
AudioConnection          patchCord5(waveforms[4], 0, recorder, 4);
AudioConnection          patchCord6(waveforms[5], 0, recorder, 5);
AudioConnection          patchCord7(waveforms[6], 0, recorder, 6);
AudioConnection          patchCord8(waveforms[7], 0, recorder, 7);
AudioConnection          patchCord9(waveforms[8], 0, recorder, 8);
AudioConnection          patchCord10(waveforms[9], 0, recorder, 9);
AudioConnection          patchCord11(waveforms[10], 0, recorder, 10);
AudioConnection          patchCord12(waveforms[11], 0, recorder, 11);
AudioConnection          patchCord13(waveforms[12], 0, recorder, 12);
AudioConnection          patchCord14(waveforms[13], 0, recorder, 13);
AudioConnection          patchCord15(waveforms[14], 0, recorder, 14);
AudioConnection          patchCord16(waveforms[15], 0, recorder, 15);
AudioConnection          patchCord17(waveforms[16], 0, recorder, 16);
AudioConnection          patchCord18(waveforms[17], 0, recorder, 17);
AudioConnection          patchCord19(waveforms[18], 0, recorder, 18);
AudioConnection          patchCord20(waveforms[19], 0, recorder, 19);
AudioConnection          patchCord21(waveforms[20], 0, recorder, 20);
AudioConnection          patchCord22(waveforms[21], 0, recorder, 21);
AudioConnection          patchCord23(waveforms[22], 0, recorder, 22);
AudioConnection          patchCord24(waveforms[23], 0, recorder, 23);
AudioConnection          patchCord25(waveforms[24], 0, recorder, 24);
AudioConnection          patchCord26(waveforms[25], 0, recorder, 25);
AudioConnection          patchCord27(waveforms[26], 0, recorder, 26);
AudioConnection          patchCord28(waveforms[27], 0, recorder, 27);
AudioConnection          patchCord29(waveforms[28], 0, recorder, 28);
AudioConnection          patchCord30(waveforms[29], 0, recorder, 29);
AudioConnection          patchCord31(waveforms[30], 0, recorder, 30);
AudioConnection          patchCord32(waveforms[31], 0, recorder, 31);

// Recording State Machine
enum MODES {INIT, RUN, STOPPED};
int mode = INIT;


void setup() {
  Serial.begin(9600);
  Serial.setTimeout(5000); // Set timeout for 5 seconds
  while (!Serial) {}

  // Tell me if I did smth wrong :(
  if (CrashReport) {
    Serial.print(CrashReport);
    delay(5000);
  }

  // Audio connections require memory, and the record queue
  // uses this memory to buffer incoming audio.
  AudioMemory(60);

  /* Configure waveforms
   * For this example, first waveform start at 100 Hz, then each of the following
   * is increased by 200 Hz
  */
  int freq = 100;
  for (int i = 0; i < N_WAVEFORM; i++) {
    waveforms[i].frequency(freq);
    waveforms[i].phase(0.0);
    waveforms[i].amplitude(1.0);
    freq += 200;
  }

  // Config SD Card using SDIO protocol
  sd.begin(SdioConfig(FIFO_SDIO));

  // Open (or create) file
	if(!file.open(filename.c_str(), O_WRONLY | O_CREAT)) {
		Serial.println("Open file failed #sad ;(");
		exit(1);
	}
  Serial.print("Recording save at /");
  Serial.println(filename);

  Serial.print("Recording duration is set to ");
  Serial.print(RECORD_DURATION_S);
  Serial.println("s");
}


void loop() {

  if (mode == INIT) {
    recorder.record(file);
    mode = RUN;
    Serial.println("Start recording");
  }

  if (mode == RUN) {
    elapsedMillis waiting;     // "waiting" starts at zero
    while (waiting <= (RECORD_DURATION_MS)) {
        // Serial.print(waiting);
        // Serial.println(" ms");
    }
    mode = STOPPED;
  }

  // Time has elapsed, run this code to stop the recording
  if (mode == STOPPED) {
    recorder.stop();
    Serial.println("Stop recording");
    sd.end(); // Eject SD card
    exit(0); // Stop program and after a while Teensy will go into program mode (not ideal, should add a IDLE state maybe)
  }
}
