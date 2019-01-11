#ifndef FIELD_H_
#define FIELD_H_

// �N���X�F�t�B�[���h
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class Field
{
public:
	// �R���X�g���N�^
	Field(int stage, int skybox);
	// �`��
	void draw() const;

	// �R�s�[�֎~
	Field(const Field& other) = delete;
	Field& operator = (const Field& other) = delete;

private:
	// �X�e�[�W���f��
	int stage_;
	// �X�J�C�{�b�N�X���f��
	int skybox_;
};

#endif // !FIELD_H_