#include "Random.h"
#include <random>

// �N���X�F��������
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// ������������`
static std::mt19937 mt;
// �����V�[�h���`
static unsigned int s_seed{ std::mt19937::default_seed };

// ������
void Random::randomize()
{
	std::random_device rd;
	randomize(rd());
}

// ������
void Random::randomize(unsigned int seed)
{
	s_seed = seed;
	mt = std::mt19937(s_seed);
}

// int�^�̗����̐���
int Random::rand_int(int min, int max)
{
	std::uniform_int_distribution<int> dist(min, max);

	return dist(mt);
}

// float�^�̗����̐���
float Random::rand_float(float min, float max)
{
	std::uniform_real_distribution<float> dist(min, max);

	return dist(mt);
}

// �����V�[�h�̎擾
unsigned int Random::seed()
{
	return s_seed;
}
