#include "offline_freqbuilder.h"
int main(int argc, char** argv) {
    if (argc < 11) {
        printf("%s midi 音源 采样率 fftsize 节奏 小节偏移 间隔 重叠 阈值 输出\n", argv[0]);
        printf(
            "例如：\n%s ../datas/test.mid "
            "../datas/sndfnt.sf2 44100 8192 4 0 16 16 2 out.txt\n",
            argv[0]);
        return 0;
    }
    mgnr::offline_render::getMidiMap(
        argv[1],
        argv[2],
        atoi(argv[3]),
        atoi(argv[4]),
        atoi(argv[5]),
        atoi(argv[6]),
        atoi(argv[7]),
        atoi(argv[8]),
        atoi(argv[9]),
        argv[10]);
    return 0;
}
