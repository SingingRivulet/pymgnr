project("midirender")

#set(CMAKE_CXX_FLAGS "-std=c++20 -ggdb")
set(CMAKE_CXX_FLAGS "-std=c++2a -O3")

set(CMAKE_EXE_LINKER_FLAGS "-static")

include_directories(./)

add_library(midirender_static STATIC
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
set_target_properties(midirender_static PROPERTIES OUT_NAME "midirender_static")


add_executable(midirender main.cpp)
target_link_libraries(midirender midirender_static)


add_library(pymidirender SHARED 
    pymidirender.cpp
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
set_target_properties(pymidirender PROPERTIES OUT_NAME "pymidirender")