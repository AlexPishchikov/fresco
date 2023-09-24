#pragma once

#include <QString>


struct WavData {
    char chunk_id[4];
    quint32 size;
    char format[4];
    char subchunk_id[4];
    quint32 subchunk_size;
    quint16 audio_format;
    quint16 num_channels;
    quint32 sample_rate;
    quint32 byte_rate;
    quint16 block_align;
    quint16 bits_per_sample;
};

unsigned int get_wav_duration(const QString &file_path);
