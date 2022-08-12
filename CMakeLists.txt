project("mgnr")

#set(CMAKE_CXX_FLAGS "-std=c++20 -ggdb")
set(CMAKE_CXX_FLAGS "-std=c++2a -O3")

set(CMAKE_EXE_LINKER_FLAGS "-static")

include_directories(./)

add_library(mgnr_static STATIC
    Binasc.cpp
    kalman.cpp
    MidiFile.cpp
    MidiMessage.cpp  
    Options.cpp
    synth.cpp
    editTable.cpp
    MidiEvent.cpp
    midiLoader.cpp
    midisplit.cpp
    player.cpp
    synthOutput.cpp
    hbb.cpp
    MidiEventList.cpp
    midiMap.cpp
    offline.cpp
    stringPool.cpp)
set_target_properties(mgnr_static PROPERTIES OUT_NAME "mgnr_static")


add_executable(mgnr renderer.cpp)
target_link_libraries(mgnr mgnr_static)


add_library(pymgnr SHARED 
    pymgnr.cpp
    Binasc.cpp
    kalman.cpp
    MidiFile.cpp
    MidiMessage.cpp  
    Options.cpp
    synth.cpp
    editTable.cpp
    MidiEvent.cpp
    midiLoader.cpp
    midisplit.cpp
    player.cpp
    synthOutput.cpp
    hbb.cpp
    MidiEventList.cpp
    midiMap.cpp
    offline.cpp
    stringPool.cpp)
set_target_properties(pymgnr PROPERTIES OUT_NAME "pymgnr")
