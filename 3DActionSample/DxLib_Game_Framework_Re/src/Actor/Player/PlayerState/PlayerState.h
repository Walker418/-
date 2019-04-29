#ifndef PLAYER_STATE_ID_H_
#define PLAYER_STATE_ID_H_

// ñ^FvC[ÌóÔ
// »ìÒF½ âQi"Jacky" Ho Siu Kij
enum class PlayerState
{
	Normal,			// Êí
	Slash1,			// Ui1iÚj
	Slash2,			// Ui2iÚj
	Slash3,			// Ui3iÚj
	JumpAttack1,	// WvUi1iÚj
	JumpAttack2,	// WvUi2iÚj
	Damage,			// ¯Ý
	Guard,			// K[h
	Blocking,		// K[hÉæémbNobN
	GuardAttack,	// K[hU
	GuardEnd,		// K[hI¹
	Death,			// S
	ForwardEvasion,	// Oñð
	LeftEvasion,	// ¶ñð
	RightEvasion,	// Eñð
};

#endif // !PLAYER_STATE_ID_H_