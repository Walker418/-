#ifndef RANDOM_H_
#define RANDOM_H_

// �N���X�F��������
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class Random
{
public:
	// ������
	static void randomize();
	// ������
	static void randomize(unsigned int seed);
	// int�^�̗����̐���
	static int rand_int(int min, int max);
	// float�^�̗����̐���
	static float rand_float(float min, float max);
	// �����V�[�h�̎擾
	static unsigned int seed();
};

#endif // !RANDOM_H_