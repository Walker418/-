#ifndef RANDOM_H_
#define RANDOM_H_

// クラス：乱数生成
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class Random
{
public:
	// 初期化
	static void randomize();
	// 初期化
	static void randomize(unsigned int seed);
	// int型の乱数の生成
	static int rand_int(int min, int max);
	// float型の乱数の生成
	static float rand_float(float min, float max);
	// 乱数シードの取得
	static unsigned int seed();
};

#endif // !RANDOM_H_