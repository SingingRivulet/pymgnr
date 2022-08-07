#include "synth.h"
namespace mgnr{

synth::~synth(){
    clearTracks();
}

void synth::onNoteOn(note * n,int c){
    if(!n->info.empty()){
        if(n->info.at(0)=='@')
            return;
    }
    callJavaNoteOn(n->info.c_str(),c, n->tone, n->volume);
}

void synth::onNoteOff(note * n,int c){
    if(!n->info.empty()){
        if(n->info.at(0)=='@')
            return;
    }
    callJavaNoteOff(n->info.c_str(),c, n->tone);
}
void synth::onUseInfo(const stringPool::stringPtr & info){
    if(info.empty())
        return;
    if(info.at(0)=='@')
        return;
    onLoadName(info);
    loadInstrument(info);
}

}
