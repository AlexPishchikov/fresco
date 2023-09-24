#include <QDataStream>
#include <QFile>
#include <QString>

#include "wav_parser.h"


unsigned int get_wav_duration(const QString &file_path) {
    QFile wav_file(file_path);
    wav_file.open(QIODevice::ReadOnly);

    QDataStream headers(&wav_file);
    WavData wav_data;

    headers.setByteOrder(QDataStream::LittleEndian);
    headers.readRawData(wav_data.chunk_id, 4);
    headers >> wav_data.size;
    headers.readRawData(wav_data.format, 4);
    headers.readRawData(wav_data.subchunk_id, 4);
    headers >> wav_data.subchunk_size;
    headers >> wav_data.audio_format;
    headers >> wav_data.num_channels;
    headers >> wav_data.sample_rate;
    headers >> wav_data.byte_rate;
    headers >> wav_data.block_align;
    headers >> wav_data.bits_per_sample;

    return wav_data.size * 8.0 / (wav_data.bits_per_sample * wav_data.sample_rate) * 1000.0;
}
