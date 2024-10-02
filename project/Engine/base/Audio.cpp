#include "Audio.h"
#include<assert.h>
/// <summary>
/// シングルトンインスタンスの取得
/// </summary>
Audio* Audio::GetInstance() {
	static Audio instance;
	return &instance;
}

void Audio::Initialize()
{
	HRESULT result;
	/*xAudioエンジンのインスタンスを生成*/
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(result));
	/*マスターボイスを生成*/
	result = xAudio2->CreateMasteringVoice(&masterVoice);
	assert(SUCCEEDED(result));
}

Audio::SoundData Audio::SoundLordWave(const char* filename)
{
	HRESULT result;

	/*ファイルオープン*/
	/*ファイル入力ストリームのインスタンス*/
	std::ifstream file;
	/*.wavファイルをバイナリモードで開く*/
	file.open(filename, std::ios_base::binary);
	/*ファイルオープン失敗を検出する*/
	assert(file.is_open());

	/*.wavデータ読み込み*/
	/*RIFFヘッダーの読み込み*/
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	/*ファイルがRIFFかチェック*/
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}
	/*タイプがWAVEかチェック*/
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}
	/*Formatチャンクの読み込み*/
	FormatChunk format = {};
	/*チャンクヘッダーの確認*/
	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
		assert(0);
	}

	/*チャンク本体の読み込み*/
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);
	/*Dataチャンクの読み込み*/
	ChunkHeader data;
	//file.read((char*)&data, sizeof(data));
	///*JUNKチャンクを検出した場合*/
	//if (strncmp(data.id, "JUNK", 4) == 0) {
	//	/*読み取り位置をJUNKチャンクの終わりまで進める*/
	//	file.seekg(data.size, std::ios_base::cur);
	//	/*再読み込み*/
	//	file.read((char*)&data, sizeof(data));
	//}
	//if (strncmp(data.id, "data", 4) != 0) {
	//	assert(0);
	//}
	/*dataを読み込み、data チャンクを見つけるまでループする*/
	while (true) {
		file.read(reinterpret_cast<char*>(&data), sizeof(data));
		if (file.eof()) {
			//throw std::runtime_error("No data chunk found in WAV file.");
			assert(0);
		}
		if (strncmp(data.id, "data", 4) == 0) {
			break;
		}
		/*目的のチャンク (data チャンク) を見つけるまで、サイズ分だけ読み飛ばす*/
		file.seekg(data.size, std::ios_base::cur);
	}
	/*Dataチャンクのデータ部（波形データ）の読み込み*/
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	/*ファイルクローズ*/
	/*Waveファイルを閉じる*/
	file.close();
	/*読み込んだ音声データをreturn*/
	/*returnするための音声データ*/
	SoundData soundData = {};

	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	return soundData;
}

void Audio::SoundUnLord(SoundData* soundData)
{
	/*バッファのメモリを解放*/
	delete[] soundData->pBuffer;

	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};
}

void Audio::SoundPlayWave(IXAudio2* xAudio2,SoundData& soundData, bool loop)
{
	//HRESULT result;
	///*波形フォーマットをもとにSourceVoiceの生成*/
	//IXAudio2SourceVoice* pSourceVoice = nullptr;
	//result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
	//assert(SUCCEEDED(result));
	///*再生する波形データの設定*/
	//XAUDIO2_BUFFER buf{};
	//buf.pAudioData = soundData.pBuffer;
	//buf.AudioBytes = soundData.bufferSize;
	//buf.Flags = XAUDIO2_END_OF_STREAM;
	///*波形データの再生*/
	//result = pSourceVoice->SubmitSourceBuffer(&buf);
	//result = pSourceVoice->Start();
	HRESULT result;
	if (!soundData.pSourceVoice) {
		result = xAudio2->CreateSourceVoice(&soundData.pSourceVoice, &soundData.wfex);
		assert(SUCCEEDED(result));
	}

	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	if (loop) {
		buf.LoopBegin = 0;
		buf.LoopLength = XAUDIO2_LOOP_INFINITE; // 無限ループ設定
		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	result = soundData.pSourceVoice->SubmitSourceBuffer(&buf);
	assert(SUCCEEDED(result));

	result = soundData.pSourceVoice->Start();
	assert(SUCCEEDED(result));
}

void Audio::SoundStop(SoundData& soundData)
{
	if (soundData.pSourceVoice) {
		soundData.pSourceVoice->Stop(0);
		soundData.pSourceVoice->FlushSourceBuffers();
	}
}

void Audio::Finalize()
{
	/*XAudio2解放*/
	xAudio2.Reset();
}
