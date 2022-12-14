#ifndef MGNR_PLAYER
#define MGNR_PLAYER
#include "editTable.h"
#include <set>
#include <list>
#include <chrono>
namespace mgnr{
    class player:public editTable{
        public:
            player();
            void playStart();
            void playStop();
            void playStep();
            void noteOn(note *);
            void noteOff(note *);
            
            double tempo;
            virtual long getTime();
            bool playingStatus;
            std::set<note*> playing;
            
            virtual void onNoteOn(note * n,int c)=0;
            virtual void onNoteOff(note * n,int c)=0;
            virtual void onSetChannelIns(int c,int ins)=0;
            
        private:
            std::set<note*> toPlay;
            long lastTime;
            long playTimes;
            inline float secondsPerTick(){//1秒=多少tick
                return 60.0 / (tempo * TPQ);
            }
            inline float ticksPerSecond(){//1 tick=多少秒
                return (tempo * TPQ) / 60.0;
            }
            void goNextStep();
            int playNum[16];
            int playIns[16];
            int ins2Channel[128];
        public:
            int useInstrument(const stringPool::stringPtr & n);
            int releaseInstrument(const stringPool::stringPtr & n);
    };
}
#endif
