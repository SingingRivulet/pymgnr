#include "offline_freqbuilder.h"
extern "C" {
void midirender_render(const char* midi,
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
}