#ifndef ISCENE_H_
#define ISCENE_H_

// �C���^�[�t�F�[�X�F�V�[��
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

enum class Scene;

class IScene
{
public:
	// ���z�f�X�g���N�^
	virtual ~IScene() {}
	// �J�n
	virtual void start() = 0;
	// �X�V
	virtual void update(float delta_time) = 0;
	// �`��
	virtual void draw() const = 0;
	// �I�����Ă��邩
	virtual bool is_end() const = 0;
	// ���̃V�[���̎擾
	virtual Scene next() const = 0;
	// �I��
	virtual void end() = 0;
};

#endif // !ISCENE_H_