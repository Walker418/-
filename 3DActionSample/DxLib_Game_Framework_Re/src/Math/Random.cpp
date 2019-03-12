#include "Random.h"
#include <random>

// クラス：乱数生成
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// 乱数生成器を定義
static std::mt19937 mt;
// 乱数シードを定義
static unsigned int s_seed{ std::mt19937::default_seed };

// 初期化
void Random::randomize()
{
	std::random_device rd;
	randomize(rd());
}

// 初期化
void Random::randomize(unsigned int seed)
{
	s_seed = seed;
	mt = std::mt19937(s_seed);
}

// int型の乱数の生成
int Random::rand_int(int min, int max)
{
	std::uniform_int_distribution<int> dist(min, max);

	return dist(mt);
}

// float型の乱数の生成
float Random::rand_float(float min, float max)
{
	std::uniform_real_distribution<float> dist(min, max);

	return dist(mt);
}

// 乱数シードの取得
unsigned int Random::seed()
{
	return s_seed;
}
