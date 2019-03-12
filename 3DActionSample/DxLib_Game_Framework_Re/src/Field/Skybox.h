#ifndef SKYBOX_H_
#define SKYBOX_H_

#include <string>
#include "../Graphic/ModelAsset.h"

// �N���X�F�X�J�C�{�b�N�X
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class Skybox
{
public:
	// ������
	static void initialize();
	// �I������
	static void finalize();
	// �ǂݍ���
	static bool load(int id, const std::string& file_name);
	// �폜
	static void erase(int id);
	// ���b�V���̃o�C���h
	static void bind(int id);
	// �`��
	static void draw();

private:
	// �o�C���h�̃��f��
	static int			model_;
	// ���f���A�Z�b�g
	static ModelAsset	asset_;
};

#endif // !SKYBOX_H_