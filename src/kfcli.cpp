// kfcli - a tiny libKeyFinder command-line wrapper.
//
// Reads raw 32-bit float, mono, little-endian PCM from a file and prints the
// estimated musical key (one line, e.g. "D minor" or "Bb major", or "SILENCE").
// Decode audio to that format with the bundled scripts/keyfinder.py helper.
//
//   usage: kfcli <raw_float32_mono_file> <samplerate>
//
// Copyright (C) 2026.  Licensed under GPL-3.0-or-later (links libKeyFinder).
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <keyfinder/keyfinder.h>
#include <keyfinder/audiodata.h>

// Order matches KeyFinder::key_t in <keyfinder/constants.h>.
static const char* KEY_NAMES[] = {
  "A major", "A minor", "Bb major", "Bb minor", "B major", "B minor",
  "C major", "C minor", "Db major", "Db minor", "D major", "D minor",
  "Eb major", "Eb minor", "E major", "E minor", "F major", "F minor",
  "Gb major", "Gb minor", "G major", "G minor", "Ab major", "Ab minor",
  "SILENCE"
};

int main(int argc, char** argv) {
  if (argc < 3) {
    fprintf(stderr, "usage: kfcli <raw_float32_mono_file> <samplerate>\n");
    return 2;
  }
  const char* path = argv[1];
  int sr = atoi(argv[2]);
  if (sr <= 0) { fprintf(stderr, "invalid samplerate: %s\n", argv[2]); return 2; }

  FILE* f = fopen(path, "rb");
  if (!f) { fprintf(stderr, "cannot open %s\n", path); return 3; }
  fseek(f, 0, SEEK_END);
  long bytes = ftell(f);
  fseek(f, 0, SEEK_SET);
  size_t n = (size_t)(bytes / sizeof(float));
  std::vector<float> samples(n);
  if (n && fread(samples.data(), sizeof(float), n, f) != n) {
    fprintf(stderr, "read error\n"); fclose(f); return 4;
  }
  fclose(f);

  KeyFinder::AudioData a;
  a.setFrameRate(sr);
  a.setChannels(1);
  a.addToSampleCount((unsigned int)n);
  for (size_t i = 0; i < n; i++) a.setSample((unsigned int)i, (double)samples[i]);

  KeyFinder::KeyFinder kf;
  KeyFinder::key_t key = kf.keyOfAudio(a);
  int idx = (int)key;
  if (idx < 0 || idx > 24) idx = 24;
  printf("%s\n", KEY_NAMES[idx]);
  return 0;
}
