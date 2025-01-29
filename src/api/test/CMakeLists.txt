add_definitions(-DAUDIO_FILE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/dialog_clip_stereo.wav")
unit_test(pitch_shift_api_test pitch_shift_api_test.cc "pitch-shift-api;AudioFile")