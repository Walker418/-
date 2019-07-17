#ifndef SOURCE_ID_H_
#define SOURCE_ID_H_

// �񋓌^�F�f��ID
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
enum
{
	// �X�e�[�W
	MESH_STAGE_CASTLE,		// ��t�B�[���h
	MESH_SKYBOX,			// �X�J�C�{�b�N�X

	// ���f��
	MESH_PALADIN,			// �v���C���[
	MESH_GHOUL,				// �G���G
	MESH_DRAGONBOAR,		// �{�X�G

	// �e�N�X�`��
	TEXTURE_TITLELOGO,		// �^�C�g�����S
	TEXTURE_START,			// �X�^�[�g�\��
	TEXTURE_PLAYER_HPGAUGE,	// �v���C���[�̗̑̓Q�[�W
	TEXTURE_PLAYER_HP,		// �v���C���[�̗͕̑\��
	TEXTURE_P1MESSAGE,		// �t�F�[�Y1�ڕW���b�Z�[�W
	TEXTURE_P2MESSAGE,		// �t�F�[�Y2�ڕW���b�Z�[�W
	TEXTURE_BOSS_HPGAUGE,	// �{�X�̗̑̓Q�[�W
	TEXTURE_BOSS_HP,		// �{�X�̗͕̑\��
	TEXTURE_PAUSE_BG,		// �|�[�Y��ʔw�i
	TEXTURE_PAUSE_TEXT,		// �|�[�Y��ʕ���

	// BGM
	BGM_TITLE,				// �^�C�g�����BGM
	BGM_STAGE,				// �X�e�[�WBGM
	BGM_BOSS,				// �{�X��BGM

	// SE
	SE_START,				// �Q�[���X�^�[�gSE
	SE_SLASH,				// �v���C���[�U��
	SE_BLOCK,				// �v���C���[�h��
	SE_PLAYER_ATK_HIT,		// �v���C���[�U������
	SE_ENEMY_ATK_LIGHT,		// �G�̍U���i���j
	SE_ENEMY_ATK_HEAVY,		// �G�̍U���i��j
	SE_BOSS_TACKLE,			// �{�X�G�̃^�b�N��
	SE_BOSS_ROAR,			// �{�X�G�̙��K

	// �V�F�[�_�[
	SHADER_VERTEX_SHADER,			// ���_�V�F�[�_�[
	SHADER_PIXEL_SHADER,			// �s�N�Z���V�F�[�_�[
	SHADER_SKYBOX_VERTEX_SHADER,	// �X�J�C�{�b�N�X�p���_�V�F�[�_�[
	SHADER_SKYBOX_PIXEL_SHADER,		// �X�J�C�{�b�N�X�p�s�N�Z���V�F�[�_�[

	// �L���[�u�}�b�v
	CUBEMAP_TEST_SPECULAR_HDR,
	CUBEMAP_TEST_DIFFUSE_HDR,
	CUBEMAP_TEST_BRDF,
	CUBEMAP_TEST_ENV_HDR,
};

#endif // !SOURCE_ID_H_