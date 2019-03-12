#ifndef SOUND_H_
#define SOUND_H_

#include <string>
#include <unordered_map>

// �N���X�F�T�E���h
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class Sound
{
public:
	// ������
	static void initialize();
	// BGM�t�@�C���̓ǂݍ���
	static void load_bgm(int id, const std::string& file_name);
	// BGM�̍Đ�
	static void play_bgm(int id);
	// BGM�̒�~
	static void stop_bgm();
	// BGM�̍폜
	static void delete_bgm(int id);
	// SE�t�@�C���̓ǂݍ���
	static void load_se(int id, const std::string& file_name);
	// SE�̍Đ�
	static void play_se(int id);
	// SE�̍폜
	static void delete_se(int id);
	// �I��
	static void finalize();

private:
	// BGM�n���h��
	static std::unordered_map<int, int> bgm_map_;
	// �Đ�����BGM
	static int current_bgm_;
	// SE�n���h��
	static std::unordered_map<int, int> se_map_;
};

#endif // !SOUND_H_
