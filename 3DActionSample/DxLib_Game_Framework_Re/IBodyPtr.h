#ifndef IBODY_PTR_H_
#define IBODY_PTR_H_

#include <memory>

// ポインタ：衝突判定インターフェース
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class IBody;
using IBodyPtr = std::shared_ptr<IBody>;

#endif // !IBODY_PTR_H_