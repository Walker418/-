#ifndef LINE_H_
#define LINE_H_

#include "Vector3.h"

// クラス：線分
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class Line
{
public:
	// デフォルトコンストラクタ
	Line() = default;
	// コンストラクタ
	Line(const Vector3& start, const Vector3& end) :
		start{ start }, end{ end } { }
	// 始点
	Vector3 start{ 0.0f,0.0f,0.0f };
	// 終点
	Vector3 end{ 0.0f,0.0f,0.0f };
};

#endif // !LINE_H_