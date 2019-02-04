#ifndef T_P_CAMERA_H_
#define T_P_CAMERA_H_

#include "Actor.h"

// NXFOlÌJ
// »ìÒF½ âQi"Jacky" Ho Siu Kij
class TPCamera : public Actor
{
public:
	// RXgN^
	TPCamera(IWorld* world);

private:
	// XV
	void update(float delta_time) override;
	// `æ
	void draw() const override;

private:
	// JÌñ]
	void rotate();

private:
	Vector3 target_;						// _ivC[j
	float yaw_angle_{ 0.0f };				// y²px
	float pitch_angle_{ 0.0f };				// x²px

	const float RotateSpeed{ 3.0f };		// JÌñ]¬x
	const float CameraHeight{ 22.0f };		// _Ì³
	const float CameraDistance{ 60.0f };	// _ÆÌ£
	const float PitchMax{ 30.0f };			// ÂpÅål
	const float PitchMin{ -20.0f };			// ÂpÅ¬l
};

#endif // !T_P_CAMERA_H_