//
// Created by admin on 2022/2/24.
//

#define TSF_IMPLEMENTATION
#include "offline.h"

namespace mgnr {

    void offline::rebuildNoteLen() {}

    void offline::drawNote_begin() {}

    void offline::drawNote(int fx, int fy, int tx, int ty, int volume,
                           const stringPool::stringPtr &info,
                           bool selected, bool onlydisplay) {}

    void offline::drawNote_end() {}

    void offline::drawDescriptions(float p, const stringPool::stringPtr &title,
                                   const std::string &content) {}

    void offline::drawDescriptionsPadd() {}

    void offline::drawCaption(float p, const std::string &str) {}

    void offline::callAddDescription(int tick) {}

    void offline::callEditDescription(int tick, const stringPool::stringPtr &title,
                                      const std::string &content) {}

    void offline::drawTableRaw(int from, int to, int left, int right, int t) {}

    void offline::drawTimeCol(float p) {}

    void offline::drawSectionCol(float p, int n) {}

    void offline::drawTempo(float p, double t) {}

    void offline::drawTempoPadd() {}

    void offline::drawScroll() {}

    void offline::scrollBuilder_onGetNoteArea() {}

    void offline::scrollBuilder_onGetAllNotePos(note *) {}

    void offline::scrollBuilder_onSwap() {}

    void offline::callJavaNoteOn(const char *info, int channel, int tone, int vol) {}

    void offline::callJavaNoteOff(const char *info, int channel, int tone) {}

    void offline::onLoadName(const stringPool::stringPtr &name) {}

    void offline::onSelectedChange(int len) {}

    void offline::onNoteOn(note *n, int c) {
        if (!n->info.empty()) {
            if (n->info[0] == '@')
                return;
        }
        
        if(soundfont){
            tsf_channel_note_on(soundfont, c, n->tone, n->volume / 128.0);
        }
    }

    void offline::onNoteOff(note *n, int c) {
        if (!n->info.empty()) {
            if (n->info[0] == '@')
                return;
        }
        if(soundfont){
            tsf_channel_note_off(soundfont, c, n->tone);
        }
    }

    void offline::onSetChannelIns(int c, int ins) {
        if(soundfont){
            tsf_channel_set_presetnumber(soundfont, c, ins);
        }
    }

    long offline::getTime() {
        return (long) nowTime;
    }

    offline::offline(const char *sf, int sampleRate) {
        soundfont = tsf_load_filename(sf);
        this->sampleRate = sampleRate;
        tsf_set_output(soundfont, TSF_MONO, sampleRate, 0.8);
        tsf_channel_set_bank_preset(soundfont, 9, 128, 0);
    }

    offline::offline() {
        soundfont = nullptr;
    }

    offline::~offline() {
        if(soundfont){
            tsf_close(soundfont);
        }
    }

    bool offline::renderStep(float *buffer) {
        nowTime = nowTime_point * 1000. / sampleRate;
        //::__android_log_print(ANDROID_LOG_INFO,
        //                      "offline_render",
        //                      "nowTime:%f nowTime_point:%d", nowTime, nowTime_point);
        if (!playingStatus) {
            playStart();
        }
        playStep();
        
        if(soundfont){
            tsf_render_float(soundfont, buffer, 64, 0);
        }
        nowTime_point += 64;
        return noteTimeMax > lookAtX;
    }
}
