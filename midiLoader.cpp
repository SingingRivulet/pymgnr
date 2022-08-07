#include "editTable.h"
#include "MidiFile.h"
#include "Options.h"
#include <iostream>
#include <map>
#include <string>
namespace mgnr{

using namespace std;
using namespace smf;

static const char * instrumentName[] = {
    "Piano",
    "BrightPiano",
    "ElectricPiano",
    "HonkyTonkPiano",
    "RhodesPiano",
    "ChorusedPiano",
    "Harpsichord",
    "Clavinet",
    "Celesta",
    "Glockenspiel",
    "MusicBoX",
    "Vibraphone",
    "Marimba",
    "Xylophone",
    "TubularBells",
    "Dulcimer",
    "HammondOrgan",
    "PercussiveOrgan",
    "RockOrgan",
    "ChurchOrgan",
    "ReedOrgan",
    "Accordion",
    "Harmonica",
    "TangoAccordian",
    "Guitar-nylon",
    "Guitar-steel",
    "Guitar-jazz",
    "Guitar-clean",
    "Guitar-muted",
    "OverdrivenGuitar",
    "DistortionGuitar",
    "GuitarHarmonics",
    "AcousticBass",
    "ElectricBass-finger",
    "ElectricBass-pick",
    "FretlessBass",
    "SlapBass1",
    "SlapBass2",
    "SynthBass1",
    "SynthBass2",
    "Violin",
    "Viola",
    "Cello",
    "Contrabass",
    "TremoloStrings",
    "PizzicatoStrings",
    "OrchestralHarp",
    "Timpani",
    "StringEnsemble1",
    "StringEnsemble2",
    "SynthStrings1",
    "SynthStrings2",
    "ChoirAahs",
    "VoiceOohs",
    "SynthVoice",
    "OrchestraHit",
    "Trumpet",
    "Trombone",
    "Tuba",
    "MutedTrumpet",
    "FrenchHorn",
    "BrassSection",
    "SynthBrass1",
    "SynthBrass2",
    "SopranoSaX",
    "AltoSaX",
    "TenorSaX",
    "BaritoneSaX",
    "Oboe",
    "EnglishHorn",
    "Bassoon",
    "Clarinet",
    "Piccolo",
    "Flute",
    "Record",
    "PanFlute",
    "BottleBlow",
    "Skakuhachi",
    "Whistle",
    "Ocarina",
    "Lead1-square",
    "Lead2-sawtooth",
    "Lead3-calliope",
    "Lead4-chiff",
    "Lead5-charang",
    "Lead6-voice",
    "Lead7-fifths",
    "Lead8-bass",
    "Pad1-newage",
    "Pad2-warm",
    "Pad3-polysynth",
    "Pad4-choir",
    "Pad5-bowed",
    "Pad6-metallic",
    "Pad7-halo",
    "Pad8-sweep",
    "FX1-rain",
    "FX2-soundtrack",
    "FX3-crystal",
    "FX4-atmosphere",
    "FX5-brightness",
    "FX6-goblins",
    "FX7-echoes",
    "FX8-sci-fi",
    "Sitar",
    "Banjo",
    "Shamisen",
    "Koto",
    "Kalimba",
    "Bagpipe",
    "Fiddle",
    "Shanai",
    "Tinkle Bell",
    "Agogo",
    "SteelDrums",
    "Woodblock",
    "TaikoDrum",
    "MelodicTom",
    "SynthDrum",
    "ReverseCymbal",
    "GuitarFretNoise",
    "BreathNoise",
    "Seashore",
    "BirdTweet",
    "TelephoneRing",
    "Helicopter",
    "Applause",
    "Gunshot"
};

void editTable::addNoteWithId(float position,float tone,float dur,int v,int ins) {
    char infoBuf[128];
    int instrumentId = ins;
    if(instrumentId<0 || instrumentId>=128){
        instrumentId = 0;
    }
    snprintf(infoBuf,sizeof(infoBuf),"%s.1",instrumentName[instrumentId]);
    midiMap::addNote(position , tone , dur , v , strPool.create(infoBuf));
}

void editTable::instrument2Id_init(){
    for(int i=0;i<128;++i){
        instrument2Id[instrumentName[i]] = i;
    }
}
void editTable::loadInstrument(int id){
    if(id<0||id>=128)
        return;
    if(instrumentLoaded[id])
        return;
    instrumentLoaded[id] = true;

    //::__android_log_print(ANDROID_LOG_INFO, "mgenner","require instrument:%s",instrumentName[id]);
}
int editTable::getInstrumentId(const stringPool::stringPtr & name){
    char n[128];
    snprintf(n,128,"%s",name.c_str());
    for(int i=0;i<128;++i){
        if(n[i]=='\0'){
            break;
        }else if(n[i]=='.'){
            n[i] = '\0';
            break;
        }
    }
    auto it = instrument2Id.find(n);
    if(it==instrument2Id.end()){
        return 0;
    }else{
        return it->second;
    }
}
void editTable::loadMidi(const std::string & str){
    MidiFile midifile;
    midifile.read(str);
    midifile.doTimeAnalysis();
    midifile.linkNotePairs();
    
    TPQ=midifile.getTicksPerQuarterNote();
    rebuildNoteLen();
    //::__android_log_print(ANDROID_LOG_INFO, "mgenner","TPQ:%d",TPQ);
    int tracks = midifile.getTrackCount();
    
    if (tracks > 1){
        //::__android_log_print(ANDROID_LOG_INFO, "mgenner","TRACKS:%d",tracks);
    }
    
    std::set<int> iset;
    
    for (int track=0; track<tracks; track++) {

        char infoBuf[128];

        int instrumentId = 0;

        auto &track_obj = midifile[track];

        for (int event = 0; event < track_obj.size(); event++) {

            auto &event_obj = track_obj[event];

            if (event_obj.isNoteOn() && event_obj.size() >= 3) {
                int position = event_obj.tick;
                int delay = event_obj.getTickDuration();
                int delayS = event_obj.getDurationInSeconds();
                int tone = (int) event_obj[1];
                int v = (int) event_obj[2];
                snprintf(infoBuf, sizeof(infoBuf), "%s.%d", instrumentName[instrumentId], track);
                addNote(position, tone, delay, v, strPool.create(infoBuf));
                iset.insert(instrumentId);
            } else if (event_obj.isTimbre()) {
                instrumentId = event_obj.getP1();
                if (instrumentId < 0)
                    instrumentId = 0;
                else if (instrumentId > 128)
                    instrumentId = 128;
            } else if (event_obj.isMetaMessage()) {
                if (event_obj.isMarkerText()) {
                    int position = event_obj.tick;
                    snprintf(infoBuf, sizeof(infoBuf), "%s.%d", instrumentName[instrumentId],
                             track);
                    addDescription(strPool.create(infoBuf), position, event_obj.getMetaContent());
                }
            }
        }
    }

    auto numTracks = midifile.getNumTracks();
    //::__android_log_print(ANDROID_LOG_INFO, "mgenner","load controls numTracks:%d",numTracks);
    for(int trackIndex=0;trackIndex<numTracks;++trackIndex) {
        for (int i = 0; i < midifile.getNumEvents(trackIndex); i++) {
            if (midifile.getEvent(trackIndex, i).isTempo()) {//是设置时间
                double tp = midifile.getEvent(trackIndex, i).getTempoBPM();
                addTempo(midifile.getEvent(trackIndex, i).tick, tp);
            }
        }
    }

    //::__android_log_print(ANDROID_LOG_INFO, "mgenner","load instruments");
    for (auto it : iset){
        loadInstrument(it);
    }

    //::__android_log_print(ANDROID_LOG_INFO, "mgenner","load midi success");
}

void editTable::onScriptCmd(const char *){}

std::string editTable::exportString(){
    struct noteMap_t{
        int tone,volume,time;
        bool isNoteOn;
        int  instrument;
    };

    std::string res;
    std::vector<noteMap_t*> noteMap;

    for(auto it:notes){
        if(it->info.empty()){
        }else{
            if(it->info.at(0)!='@') {//为@是控制字符
                auto p1 = new noteMap_t;
                p1->tone = it->tone;
                p1->volume = it->volume > 100 ? 100 : it->volume;
                p1->time = it->begin;
                p1->isNoteOn = true;

                auto p2 = new noteMap_t;
                p2->tone = it->tone;
                p2->volume = 0;
                p2->time = it->begin + it->delay;
                p2->isNoteOn = false;

                p1->instrument = getInstrumentId(it->info);
                p2->instrument = p1->instrument;
                noteMap.push_back(p1);
                noteMap.push_back(p2);
            }
        }
    }
    sort(noteMap.begin(),noteMap.end(),[](noteMap_t * a,noteMap_t * b){
        if(a->time < b->time){
            return true;
        }
        if(a->time == b->time && b->isNoteOn && !a->isNoteOn){
            return true;
        }
        return false;
    });
    char buf[256];
    for(auto & it:noteMap){
        bzero(buf,sizeof(buf));
        if(it->isNoteOn) {
            snprintf(buf, sizeof(buf), "B %d %d %d %d\n",it->time , it->tone , it->instrument , it->volume);
        }else{
            snprintf(buf, sizeof(buf), "E %d %d %d\n",it->time , it->tone , it->instrument);
        }
        res+=buf;
        delete it;
    }
    return res;
}

void editTable::exportMidi(const std::string & filename) {
    map<stringPool::stringPtr, int> tracks;
    int trackNum = 1;//0音轨存没有info的音符
    int track;
    MidiFile midifile;

    midifile.setTPQ(TPQ);//0音轨
    midifile.addTrack();//0音轨

    struct noteMap_t {
        int tone, volume, time;
        bool isNoteOn;
        bool isMarker;
        std::string markerMessage{};
    };

    std::map<int, std::pair<int, std::vector<noteMap_t *> > > noteMap;

    //添加音符
    for (auto it:notes) {
        if (it->info.empty()) {
            track = 0;

        } else {
            if (it->info.at(0) != '@') {//为@是控制字符
                auto tit = tracks.find(it->info);

                if (tit == tracks.end()) {//没有音轨
                    midifile.addTrack();
                    tracks[it->info] = trackNum;
                    track = trackNum;
                    ++trackNum;

                    auto p1 = new noteMap_t;
                    p1->tone = it->tone;
                    p1->volume = it->volume > 100 ? 100 : it->volume;
                    p1->time = it->begin;
                    p1->isNoteOn = true;
                    p1->isMarker = false;

                    auto p2 = new noteMap_t;
                    p2->tone = it->tone;
                    p2->volume = 0;
                    p2->time = it->begin + it->delay;
                    p2->isNoteOn = false;
                    p2->isMarker = false;

                    auto &lst = noteMap[track];
                    lst.first = getInstrumentId(it->info);
                    lst.second.push_back(p1);
                    lst.second.push_back(p2);


                } else {
                    track = tit->second;


                    auto p1 = new noteMap_t;
                    p1->tone = it->tone;
                    p1->volume = it->volume > 100 ? 100 : it->volume;
                    p1->time = it->begin;
                    p1->isNoteOn = true;
                    p1->isMarker = false;

                    auto p2 = new noteMap_t;
                    p2->tone = it->tone;
                    p2->volume = 0;
                    p2->time = it->begin + it->delay;
                    p2->isNoteOn = false;
                    p2->isMarker = false;

                    auto &lst = noteMap[track];
                    lst.second.push_back(p1);
                    lst.second.push_back(p2);

                }

            }
        }
    }
    //添加文本信息
    for (auto &it_title:descriptions) {

        auto tit = tracks.find(it_title.first);
        int ins = getInstrumentId(it_title.first);

        if (tit == tracks.end()) {//没有音轨
            midifile.addTrack();
            tracks[it_title.first] = trackNum;
            track = trackNum;
            ++trackNum;

            for (auto &it_content:it_title.second) {
                if(it_content.first>=0) {
                    auto p = new noteMap_t;
                    p->time = it_content.first;
                    p->isNoteOn = false;
                    p->isMarker = true;
                    p->markerMessage = it_content.second;

                    auto &lst = noteMap[track];
                    lst.first = ins;
                    lst.second.push_back(p);
                }
            }

        } else {
            track = tit->second;

            for (auto &it_content:it_title.second) {
                if(it_content.first>=0) {
                    auto p = new noteMap_t;
                    p->time = it_content.first;
                    p->isNoteOn = false;
                    p->isMarker = true;
                    p->markerMessage = it_content.second;

                    auto &lst = noteMap[track];
                    lst.first = ins;
                    lst.second.push_back(p);
                }
            }
        }
    }

    for (auto it:timeMap) {//添加time map
        midifile.addTempo(0, it.first, it.second);
    }
    for (auto itlst:noteMap) {
        int tk = itlst.first;
        int ch = tk;
        if (ch > 15)
            ch = 15;

        sort(itlst.second.second.begin(), itlst.second.second.end(),
             [](noteMap_t *a, noteMap_t *b) {
                 if (a->time < b->time) {
                     return true;
                 }
                 if (a->time == b->time && b->isNoteOn && !a->isNoteOn) {
                     return true;
                 }
                 return false;
             });

        midifile.addTimbre(tk, 0, ch, itlst.second.first);

        for (auto it:itlst.second.second) {//扫描音轨
            if (it->isMarker) {
                midifile.addMarker(tk, it->time, it->markerMessage);
            } else {
                if (it->isNoteOn) {
                    midifile.addNoteOn(tk, it->time, ch, it->tone, it->volume);
                } else {
                    midifile.addNoteOff(tk, it->time, ch, it->tone);
                }
            }
            delete it;
        }
    }
    midifile.write(filename);
    editStatus = false;
}

}
