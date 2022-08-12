#include "offline_freqbuilder.h"
#include "sampler.h"
extern "C" {
void pymgnr_render(const char* midi,
                   const char* sf,
                   int sampleRate,
                   int fftSize,
                   int section,       //节拍
                   int sectionShift,  //节拍偏移
                   int skip,          //处理时跳过帧
                   int blockNum,      //每一次有多少个block，剩下的补0
                   float minFreq,     //频谱被标记的阈值
                   void (*callback)(int, const float* freq, const int* tag)) {
    printf("输入：%s\n", midi);
    printf("音源：%s\n", sf);
    printf("采样率：%d\n", sampleRate);
    printf("节拍：%d\n", section);
    printf("节拍偏移：%d\n", sectionShift);
    printf("阈值：%f\n", minFreq);
    printf("开始执行渲染\n");
    int count = 0;
    mgnr::offline_render::getMidiMap(
        midi, sf, sampleRate,
        fftSize, section, sectionShift, skip, blockNum, minFreq,
        [&](const float* freq, const int* tag, mgnr::offline& renderer) {
            //printf("%lf\n", renderer.lookAtX);
            callback(count, freq, tag);
            ++count;
        });
    printf("共%d帧\n", count);
}
void pymgnr_sample(const char* midi,
                   void (*callback)(int, const int* id, const int* type)) {
    printf("输入：%s\n", midi);
    mgnr::offline renderer;
    renderer.loadMidi(midi);
    float buf[64];
    renderer.updateTimeMax();
    int lenInBeat = (renderer.noteTimeMax / renderer.TPQ);
    std::map<int, int> frame;

    int list_id[128];
    int list_type[128];
    int list_index;

    for (int i = 0; i < lenInBeat; i = i + 1) {
        for (int j = 0; j < 4; ++j) {
            auto beat = i + j * 0.25;
            constexpr auto len = 0.25;
            frame.clear();
            //开始采样
            mgnr::fetch(renderer, beat + len * 0.3, [&](mgnr::note* n) {
                //解析音轨
                char info[128];
                snprintf(info, 128, "%s", n->info.c_str());
                char* infop = info;
                while (*infop) {
                    if (*infop == '.') {
                        ++infop;
                        if (*infop != '\0') {
                            frame[n->tone] = atoi(infop);
                        }
                        return;
                    }
                    ++infop;
                }
            });
            list_index = 0;
            for (auto& it : frame) {
                if (list_index < 128) {
                    list_id[list_index] = it.first;
                    list_type[list_index] = it.second;
                    ++list_index;
                }
            }
            callback(list_index, list_id, list_type);
        }
    }
}
}