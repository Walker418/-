#ifndef FIELD_PTR_H_
#define FIELD_PTR_H_

#include <memory>

// ポインタ：フィールド
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class Field;
using FieldPtr = std::shared_ptr<Field>;

// フィールドの生成
template <class T, class... Args>
inline FieldPtr new_field(Args... args)
{
	return std::make_shared<T>(args...);
}

#endif // !FIELD_PTR_H_