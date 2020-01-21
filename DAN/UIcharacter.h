//============================
//[UIcharacter.h]
// ����@�W��
//============================

//============================
//�C���N���[�h
//============================
#include "Result_chara.h"
#include "Player_Chara.h"
#include "Greenig_Chara.h"
#include "Greening_Moji.h"
#include "Defeat_Chara.h"
#include "Defeat_Moji.h"
#include "WholeGreening.h"
#include "Persent.h"

//============================
//���O��� �萔
//============================
namespace uiCharacterNS
{
	//UI�̎��
	enum UICHARACTER_TYPE
	{
		RESULT,
		PLAYER1,
		PLAYER2,
		GREENIG,
		RYOKUKA,
		PERSENT,
		DEFEAT,
		GEKITAI,
		UICHARACTER_MAX,	//UI(����)�̑���
	};

	//���U���g�̃t�F�C�Y
	enum RESULT_PHASE
	{
		PHASE_01,		//���U���g���S�̃X���C�h�C��
		PHASE_02,		//���\��
		PHASE_03,		//���ʕ\��(�����Ȃ�)
		PHASE_04,		//�^�C�g���ɖ߂�t�F�C�Y
		PHASE_05,
		PHASE_MAX,		//�t�F�C�Y�̑���
	};

	
}
//============================
//�N���X��`
//============================
class UIcharacter:public BasicUI
{
public: //�����o�[�ϐ�
	Result_Chara* result;
	Player_Chara* player;
	Greening_Chara* greenChara;
	Greening_Moji* greenMoji;
	Defeat_Chara* defeatChara;
	Defeat_Moji* defeatMoji;
	WholeGreening* greening;
	Persent* persent;

public: //�����o�[�֐�
	UIcharacter();						//�R���X�g���N�^
	~UIcharacter();						//�f�X�g���N�^
	void initialize(int playerType);	//������
	void render(int resultPhase);		//�`��
	void update(int resultPhase, int playerType,float flametime);		//�X�V
	
};


