//-----------------------------------------------------------------------------
// ツリー基底クラス [Tree.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/13
//-----------------------------------------------------------------------------
// 更新日 : 2019/12/01 【菅野 樹】
//-----------------------------------------------------------------------------

//=============================================================================
//【インクルード】
//=============================================================================
#include "Tree.h"
#include "ImguiManager.h"
#include "UtilityFunction.h"
#include "TelopManager.h"

//=============================================================================
//【using宣言】
//=============================================================================
using namespace treeNS;

#pragma region Tree
//=============================================================================
//【グローバル変数】
//=============================================================================
int Tree::numOfTree = 0;

//=============================================================================
// コンストラクタ
//=============================================================================
Tree::Tree(treeNS::TreeData _treeData)
{
	{//オブジェクトタイプと衝突対象の指定
		using namespace ObjectType;
		treeCell.type = TREE;
		treeCell.target = PLAYER|ENEMY| ENEMY_BEAR |BULLET;
	}
	beforeDigital = false;

	treeData = _treeData;
	this->position = treeData.initialPosition;
	leaf.position = treeData.initialPosition;
	leaf.onActive = false;//初期値は非アクティブ

	// treeData.sizeを基にした拡大縮小
	switch (treeData.size)
	{
	case STANDARD:
		scale = D3DXVECTOR3(2.4f, 1.7f, 2.4f);
		setSize(D3DXVECTOR3(1.5f, 10.0f, 1.5f));
		break;
	case LARGE:
		scale = D3DXVECTOR3(8.0f, 5.5f, 8.0f);
		setSize(D3DXVECTOR3(7.5f, 30.0f, 7.5f));		//サイズの設定
		break;
	case VERY_LARGE:
		scale = D3DXVECTOR3(21.0f, 16.0f, 21.0f);
		setSize(D3DXVECTOR3(15.0f, 75.0f, 15.0f));		//サイズの設定
		break;
	}

	leaf.scale = scale;
	

	//半径の設定
	setRadius(size.y);

	// 重力付与
	this->onGravity = true;

	//現在は緑化中でない
	nowAroundGreening = false;

	//初期ツリー情報に応じてステートを設定する
	if (treeData.type == ANALOG_TREE)
	{
		if (treeData.greenState == GREEN)
		{
			state = new GreenState(this);
		}
		else if(treeData.greenState == DEAD)
		{
			state = new AnalogState(this);
		}
	}
	else if (treeData.type == DIGITAL_TREE) 
	{
		playDigitalEffect();
		state = new DigitalState(this);
		disableAroundGreening();//最初は緑化処理をしない
	}
		
	onTransState = false;
	numOfTree++;

	//シフト先として選択されていない
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		selectShift[i] = false;
	}

	culling = false;
}


//=============================================================================
// デストラクタ
//=============================================================================
Tree::~Tree()
{
	numOfTree--;
	SAFE_DELETE(state);
}


//=============================================================================
// 更新処理
//=============================================================================
void Tree::update(float frameTime)
{
	// 重力
	if (this->onGravity)
	{
		D3DXVECTOR3 gravityDirection = D3DXVECTOR3(0, -1, 0);
	}

	//状態別更新
	state->update(frameTime);

	//状態遷移
	if (onTransState)
	{
		AbstractState* tmp = state->transition();
		SAFE_DELETE(state);
		state = tmp;
		onTransState = false;
	}

	// オブジェクトのアップデート
	leaf.update();
	this->Object::update();
}

//=============================================================================
//【周辺の緑化処理（デジタル化時）】
//=============================================================================
void Tree::render()
{
#ifdef _DEBUG
	if (treeData.type == treeNS::DIGITAL_TREE) {
		greeningArea.render();
	}
	box->render(matrixCenter);
#endif
}

//=============================================================================
//【周辺の緑化処理（デジタル化時）】
//=============================================================================
void Tree::greeningAround()
{
	greeningArea.initialize(&center);
	greeningArea.setRadius(1.0f);
	greeningArea.playerNo = playerNo;
	greeningArea.mode = GreeningAreaNS::GREENING_MODE;
	//エフェクトの再生
	GreeningAreaNS::GreeningEffect* greeningEffect 
		= new GreeningAreaNS::GreeningEffect(&greeningArea.position,&greeningArea.scale);
	effekseerNS::play(0, greeningEffect);
	nowAroundGreening = true;
}

//=============================================================================
//【周辺の枯木処理（枯木化時）】
//=============================================================================
void Tree::deadAround()
{
	greeningArea.initialize(&center);
	greeningArea.setRadius(1.0f);
	greeningArea.mode = GreeningAreaNS::DEAD_MODE;
	//エフェクトの再生
	GreeningAreaNS::DeadingEffect* deadingEffect 
		= new GreeningAreaNS::DeadingEffect(&greeningArea.position,&greeningArea.scale);
	effekseerNS::play(0, deadingEffect);
	nowAroundGreening = true;
}

//=============================================================================
//【状態遷移を行う】
//=============================================================================
void Tree::transState()
{
	onTransState = true;
}

//=============================================================================
//【デジタルエフェクトの再生】
//=============================================================================
void Tree::playDigitalEffect()
{
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		//VISION時
		digitalEffect[i] = new treeNS::DigitTree(i+1,&position);
		effekseerNS::play(i+1,digitalEffect[i]);
	}
}

//=============================================================================
//【デジタルエフェクトの停止】
//=============================================================================
void Tree::stopDigitalEffect()
{
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		effekseerNS::stop(i+1,digitalEffect[i]);
	}
}

//=============================================================================
//【デジタルエフェクトの表示/非表示】
//=============================================================================
void Tree::switchingShownDigitalEffect(bool shown,int playerNo) 
{
	if(digitalEffect[playerNo])
		digitalEffect[playerNo]->setShown(shown);
}

//=============================================================================
//【シフト先として選択されたかどうかを切り替える】
//=============================================================================
void Tree::switchingSelected(bool selected,int playerNo) 
{
	selectShift[playerNo] = selected;
}

//=============================================================================
// 重力発生メッシュ（接地メッシュ）の設定
//=============================================================================
void Tree::setAttractor(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix)
{
	attractorMesh = _attractorMesh;
	attractorMatrix = _attractorMatrix;
}

//=============================================================================
// Getter
//=============================================================================
Object* Tree::getLeaf() { return &leaf; }
Object* Tree::getTrunk() { return this; }
Object* Tree::getGreeningArea() { return &greeningArea; }
int Tree::getNumOfTree(){ return numOfTree; }
void Tree::resetNumOfTree(){ numOfTree = 0; }
TreeData* Tree::getTreeData() { return &treeData; }
LPD3DXMESH Tree::getMesh() {
	using namespace staticMeshNS;
	StaticMesh model;
	switch (treeData.model)
	{
	case treeNS::A_MODEL:	model = *reference(A_TRUNK);		break;
	case treeNS::B_MODEL:	model = *reference(B_TRUNK);		break;
	case treeNS::C_MODEL:	model = *reference(GREEN_TREE_002);	break;
	default:				model = *reference(A_TRUNK);		break;
	}
	return model.mesh;
};
bool Tree::isAroundGreening()		{ return nowAroundGreening; }
bool Tree::getTransState()			{return onTransState;}
bool Tree::getSelected(int playerNo) { return selectShift[playerNo]; }

//=============================================================================
// Setter
//=============================================================================
void Tree::setDataToTree(TreeData _treeData) { treeData = _treeData; }
void Tree::addHp(int value,int playerNo) { 
	if (treeData.greenState == treeNS::GREEN)return;//緑化していれば加算しない
	treeData.hp += value; 
	if (treeData.hp >=MAX_HP)
	{
		//エフェクトの再生
		//DA変換エフェクト
		effekseerNS::Instance* instance = new effekseerNS::Instance();
		instance->effectNo = effekseerNS::DAC;
		instance->position = position;
		effekseerNS::play(0,instance);

		//デジタルツリーエフェクトの再生
		playDigitalEffect();

		this->playerNo = playerNo;

		//状態遷移
		transState();
	}
}
void Tree::reduceHp(int value)
{
	if (treeData.greenState == treeNS::DEAD)return; //枯れ木は対象外
	treeData.hp -= value;
	if (treeData.hp <= 0)
	{
		//状態遷移
		transState();
		
		// デジタルツリーが破壊された場合の、ツリー攻撃中フラグとマーカーを切る
		// ツリーが破壊されずエネミー起因で処理する場合はEventMakaerに記載
		treeData.isAttaked = false;
		MarkerRenderer::get()->attackedTree = NULL;
	}
}
//緑化エリアのスケールを設定
void Tree::setGreeningArea(float value)
{
	greeningArea.size = D3DXVECTOR3(value, value, value);
	greeningArea.sphere->setScale(value);
}
//周囲への緑化を終了：（枯木化も内包）
void Tree::disableAroundGreening()	{ 
	nowAroundGreening = false; 
	greeningArea.treeCell.remove();//衝突空間から離脱
}

//=============================================================================
// 接地処理
//=============================================================================
void Tree::grounding()
{
	D3DXVECTOR3 gravityDirection = D3DXVECTOR3(0, -1, 0);
	Ray gravityRay;
	gravityRay.update(this->position, gravityDirection);

	if (gravityRay.rayIntersect(attractorMesh, *attractorMatrix))
	{
		position.y -= gravityRay.distance-0.5f;
		leaf.position = position;
		// 重力切る
		this->onGravity = false;
		this->setGravity(D3DXVECTOR3(0,0,0), 0);
	}
	else {
		position =
			D3DXVECTOR3(rand() % 400, 150, rand() % 480);
		position -= D3DXVECTOR3(200, 0, 240);
	}
}
#pragma endregion

#pragma region DigitalState
//=============================================================================
//【コンストラクタ：デジタル状態】
//=============================================================================
DigitalState::DigitalState(Tree* target)
{
	tree = target;
	start();
}

//=============================================================================
//【デストラクタ：デジタル状態】
//=============================================================================
DigitalState::~DigitalState()
{
}

//=============================================================================
//【開始：デジタル状態】
//=============================================================================
void DigitalState::start()
{
	aroundGreenTimer = 0.0f;
	//サイズに応じて緑化範囲を設定する
	switch (tree->getTreeData()->size)
	{
	case treeNS::TREE_SIZE::STANDARD:
		aroundGreenRange = AROUND_GREEN_RANGE_S; break;
	case treeNS::TREE_SIZE::LARGE:
		aroundGreenRange = AROUND_GREEN_RANGE_L; break;
	case treeNS::TREE_SIZE::VERY_LARGE:
		aroundGreenRange = AROUND_GREEN_RANGE_V; break;
	}
	tree->getGreeningArea()->scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f)*aroundGreenRange;
	tree->beforeDigital = true;
	//葉をアクティブ状態にする
	tree->getLeaf()->onActive = true;

	//周囲への緑化を開始する
	tree->greeningAround();

}

//=============================================================================
//【終了：デジタル状態】
//=============================================================================
void DigitalState::end()
{

}

//=============================================================================
//【更新：デジタル状態】
//=============================================================================
void DigitalState::update(float frameTime)
{
	if (aroundGreenTimer < AROUND_GREEN_TIME)
	{
		aroundGreenTimer += frameTime;
		//緑化時間終了
		if (aroundGreenTimer > AROUND_GREEN_TIME)
		{
			aroundGreenTimer = AROUND_GREEN_TIME;	//タイマー停止
			tree->disableAroundGreening();			//周囲緑化フラグを切る
		}
	}

	//
	if (tree->isAroundGreening())
	{
		float rate = aroundGreenTimer/AROUND_GREEN_TIME;
		tree->setGreeningArea(UtilityFunction::lerp(1.0f, aroundGreenRange, rate));
	}

}

//=============================================================================
//【状態遷移：デジタル状態】
//=============================================================================
AbstractState* DigitalState::transition()
{
	return new AnalogState(tree);
}

#pragma endregion

#pragma region AnalogState
//=============================================================================
//【コンストラクタ：アナログ状態】
//=============================================================================
AnalogState::AnalogState(Tree* target)
{
	tree = target;
	start();
}

//=============================================================================
//【デストラクタ：アナログ状態】
//=============================================================================
AnalogState::~AnalogState()
{

}

//=============================================================================
//【開始：アナログ状態】
//=============================================================================
void AnalogState::start()
{
	tree->getTreeData()->type = treeNS::ANALOG_TREE;
	tree->getTreeData()->greenState = treeNS::DEAD;
	tree->getLeaf()->onActive = false;
	aroundDeadTimer = AROUND_DEAD_TIME;//デフォルトは周囲への枯木化を行わない

	//前の状態がデジタルツリーで枯木に状態遷移してきた場合
	//周囲へ枯木処理を行う
	if (tree->beforeDigital)
	{
		aroundDeadTimer = 0.0f;
		//サイズに応じて緑化範囲を設定する
		switch (tree->getTreeData()->size)
		{
		case treeNS::TREE_SIZE::STANDARD:
			aroundDeadRange = AROUND_GREEN_RANGE_S; break;
		case treeNS::TREE_SIZE::LARGE:
			aroundDeadRange = AROUND_GREEN_RANGE_L; break;
		case treeNS::TREE_SIZE::VERY_LARGE:
			aroundDeadRange = AROUND_GREEN_RANGE_V; break;
		}
		tree->getGreeningArea()->scale = D3DXVECTOR3(1.0f,1.0f,1.0f)*aroundDeadRange;

		tree->deadAround();
		tree->beforeDigital = false;
	}
}

//=============================================================================
//【終了：アナログ状態】
//=============================================================================
void AnalogState::end()
{

}

//=============================================================================
//【更新：アナログ状態】
//=============================================================================
void AnalogState::update(float frameTime)
{
	if (aroundDeadTimer < AROUND_DEAD_TIME)
	{
		aroundDeadTimer += frameTime;
		//枯木時間終了
		if (aroundDeadTimer > AROUND_DEAD_TIME)
		{
			aroundDeadTimer = AROUND_DEAD_TIME;	//タイマー停止
			tree->disableAroundGreening();		//周囲枯木化フラグを切る

			TelopManager* telopManager = TelopManager::get();
			telopManager->playOrder(telopManagerNS::WITHER);
			PLAY_PARAMETERS playParameters = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_Green_Down, false ,NULL,false,NULL };
			SoundInterface::SE->playSound(&playParameters);	//SE再生
		}
	}

	//
	if (tree->isAroundGreening())
	{
		float rate = aroundDeadTimer / AROUND_DEAD_TIME;
		tree->setGreeningArea(UtilityFunction::lerp(1.0f, aroundDeadRange, rate));
	}

}

//=============================================================================
//【状態遷移：アナログ状態】
//=============================================================================
AbstractState* AnalogState::transition()
{
	//緑化する
	tree->getTreeData()->greenState = treeNS::GREEN;
	//HPの増加による緑化
	if (tree->getTreeData()->hp >= MAX_HP)
	{
		//デジタル化
		tree->getTreeData()->type = treeNS::DIGITAL_TREE;
		return new DigitalState(tree);
	}
	else {
		//アナログ状態のまま緑化
		tree->getTreeData()->type = treeNS::ANALOG_TREE;
		return new GreenState(tree);
	}
}

#pragma endregion

#pragma region GreenState
//=============================================================================
//【コンストラクタ：緑化状態】
//=============================================================================
GreenState::GreenState(Tree* target)
{
	tree = target;
	start();
}

//=============================================================================
//【デストラクタ：緑化状態】
//=============================================================================
GreenState::~GreenState()
{

}

//=============================================================================
//【開始：緑化状態】
//=============================================================================
void GreenState::start()
{
	//緑化したアナログツリー
	tree->getTreeData()->greenState = treeNS::GREEN;
	tree->getTreeData()->type = treeNS::ANALOG_TREE;
	tree->getLeaf()->onActive = true;
}

//=============================================================================
//【終了：緑化状態】
//=============================================================================
void GreenState::end()
{

}

//=============================================================================
//【更新：緑化状態】
//=============================================================================
void GreenState::update(float frameTime)
{

}

//=============================================================================
//【更新：緑化状態】
//=============================================================================
AbstractState* GreenState::transition()
{
	//再度アナログ状態に戻る
	return new AnalogState(tree);
}

#pragma endregion

