#include <Arduino.h>
#include "AudioMultiChannelRecorder.h"


AudioMultiChannelRecorder::AudioMultiChannelRecorder(unsigned char ninput, audio_block_t **iqueue) :
  AudioStream(ninput, iqueue),
	chanCnt(ninput), state(STATE_STOP)
	{}

// Initialize file and start record
void AudioMultiChannelRecorder::record(FsFile input_file) {
	if (state == STATE_RECORDING) { // Ensure recording is not already running
		Serial.println("Already recording!");
		return;
	}
	file = input_file;
	state = STATE_RECORDING;
}

// check if current state != stopped
// check if audio data
// ensure everything buffered gets written out
// truncate file (is this really useful for raw data ?)
// close file
void AudioMultiChannelRecorder::stop(void) {
	if (state != STATE_RECORDING) {
		Serial.println("Recording already stopped!");
		return;
	}
	state = STATE_STOP;
	file.flush(); // Make sure all data have been written
	size_t pos = file.position();
	file.truncate(pos); // Truncate file
	file.close();
}

// Interleave all channels to create a single formatted according to PCM specs
static void interleave(int16_t* buffer, int16_t** data, uint16_t chanCnt) {
	int i = 0;
	for (int sample = 0; sample < AUDIO_BLOCK_SAMPLES; sample++) {
			for (int channel = 0; channel < chanCnt; channel++) {
					buffer[i++] = data[channel][sample];
			}
	}
}

// Write audio block into a buffer
void AudioMultiChannelRecorder::update(void) {
	audio_block_t* blocks[chanCnt]; // Create a list of blocks sized for number of channel
	int16_t* data[chanCnt];
	int16_t interleavedData[chanCnt * AUDIO_BLOCK_SAMPLES]; // Interveaved data formatted in PCM format
	bool needWrite = false;

	// Acquire block per channel
	for (int channel = 0; channel < chanCnt; channel++) {
		blocks[channel] = receiveReadOnly(channel);
		if (nullptr != blocks[channel]) // Verify block as data
		{
			data[channel] = blocks[channel]->data;
		}		
	}

	if (state == STATE_RECORDING) //Check module is recording
	{
		interleave(interleavedData, data, chanCnt);
		needWrite = true;
	}

	// Release them #LETTHEMBEFREE
	for (int channel = 0; channel < chanCnt; channel++) {
		if (nullptr != blocks[channel]) // stock release() can't cope with NULL pointer
		{
			release(blocks[channel]);
		}
	}

	if (needWrite) {
		// SD card are more performant when writing 512 bytes
		size_t result = file.write(reinterpret_cast<const char*>(&interleavedData), sizeof interleavedData); // Write into SD card (might want to delay that function in the future)
		if (!result) Serial.println("writing failed :/");
	}
}

// Transfert data from buffer to file
void AudioMultiChannelRecorder::flushBuffer(void) {}

// Return if recording is running
bool AudioMultiChannelRecorder::isRecording(void)
{
	uint8_t s = *(volatile uint8_t *)&state; // Create a defferenced copy (not sure)
	return (s == STATE_RECORDING);
}

// Return if recording is stopped
bool AudioMultiChannelRecorder::isStopped(void)
{
	uint8_t s = *(volatile uint8_t *)&state;
	return (s == STATE_STOP);
}