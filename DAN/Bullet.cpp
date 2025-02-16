//===================================================================================================================================
//【Bullet.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/11/05
// [更新日]2019/11/25
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Bullet.h"
#include "UtilityFunction.h"
#include "EffekseerManager.h"

//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace bulletNS;

//////////////////////////////////////////////////////////////////////////////////////////////////////
// [バレットマネージャ]
//////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region Bullet
//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
Bullet::Bullet(Ray shootingRay,int playerNo, bool isPowUp)
{
	this->playerNo = playerNo;
	//パラメータの初期化
	digitalPower = DIGITAL_POWER;

	//引数の代入
	this->launchPosition	= shootingRay.start;												//発射位置
	this->speed				= shootingRay.direction*SPEED;										//速度
	this->initialCollide	= launchPosition + shootingRay.direction * shootingRay.distance;	//初期衝突位置

	//弾道の初期化
	ballisticRay.initialize(launchPosition, shootingRay.direction);
	ballisticRay.color = D3DXCOLOR(0, 255, 120, 255);

	//弾本体
	if (isPowUp)
	{
		effect = new BulletBody2(&matrixWorld);
	}
	else
	{
		effect = new BulletBody(&matrixWorld);
	}
	effekseerNS::play(0, effect);

	{//オブジェクトタイプと衝突対象の指定
		using namespace ObjectType;
		treeCell.type = BULLET;
		treeCell.target = ENEMY_PARTS | ENEMY | TREE;
	}

	Object::initialize(&launchPosition);							//バレットモデルの初期化
	postureControl(axisZ.direction, -shootingRay.direction,1.0f);	//モデルを進行方向へ姿勢制御する
	Object::update();
	existenceTimer = EXIST_TIME;
	endPoint = launchPosition + speed * EXIST_TIME;					//何にも衝突しなかった場合の終着位置
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
Bullet::~Bullet()
{
	effekseerNS::stop(0, effect);
}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void Bullet::update(float frameTime)
{
	if (existenceTimer <= 0)return;
	existenceTimer -= frameTime;
	//位置更新
	D3DXVec3Lerp(&position, &ballisticRay.start, &endPoint,1.0f - existenceTimer/EXIST_TIME);
	Object::update();
}

//===================================================================================================================================
//【描画】
//===================================================================================================================================
void Bullet::render()
{
	float length = Base::between2VectorLength(ballisticRay.start, position);
	ballisticRay.render(length);
	debugRender();
}

//===================================================================================================================================
//【衝突判定】
//===================================================================================================================================
bool Bullet::collide(LPD3DXMESH targetMesh, D3DXMATRIX targetMatrix)
{
	//弾道レイの書き換え防止
	Ray ray;
	ray.initialize(ballisticRay.start, ballisticRay.direction);
	
	//レイ判定
	bool hit = ray.rayIntersect(targetMesh, targetMatrix);

	if (hit)
	{
		//距離判定
		hit = (Base::between2VectorLength(ballisticRay.start, position)
		> ray.distance);
	}
	

	return hit;
}
//===================================================================================================================================
//【getter】
//===================================================================================================================================
int Bullet::getDigitalPower(){return digitalPower;}
bool Bullet::isCollideInitial() { 
	float initial = Base::between2VectorLength(ballisticRay.start, initialCollide);
	if (initial >= SPEED*EXIST_TIME)return false;

	float now = Base::between2VectorLength(ballisticRay.start, position);
	return now >= initial; 
}
D3DXVECTOR3 Bullet::getBulletSpeed() { return this->speed; }
//===================================================================================================================================
//【setter】
//===================================================================================================================================
void Bullet::setDigitalPower(float value)
{
	digitalPower = digitalPower * value;
}



//===================================================================================================================================
//【削除】
//===================================================================================================================================
void Bullet::destroy()
{
	existenceTimer = 0.0f;
	//サウンドの再生
	PLAY_PARAMETERS hitSE;
	hitSE = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_HitBulletTree, false ,NULL,false,NULL};
	SoundInterface::SE->playSound(&hitSE);
	//エフェクトの再生
	effekseerNS::Instance* instance = new effekseerNS::Instance();
	instance->position = position;
	effekseerNS::play(0,instance);

}
#pragma endregion

//////////////////////////////////////////////////////////////////////////////////////////////////////
// [バレットマネージャ]
//////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region BulletManager
//===================================================================================================================================
//【コンストラクタ：バレットマネージャー】
//===================================================================================================================================
BulletManager::BulletManager()
{
	bulletList		= new LinkedList<Bullet*>;
	//renderer		= new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::SAMPLE_SCISSORS));
	remaining		= MAGAZINE_NUM;
	intervalTimer	= 0.0f;
	reloadTimer		= 0.0f;
	reloading		= false;
	isLaunched		= false;
	launchFactTime	= 0.0f;
	powerRate		= 1.0f;
	isPowerUp		= false;

	//サウンドの設定
	shotSE = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_Shot, false ,NULL,false,NULL};
	reroadSE = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_Reload, false ,NULL,false,NULL};
}

//===================================================================================================================================
//【デストラクタ：バレットマネージャー】
//===================================================================================================================================
BulletManager::~BulletManager()
{
	//バレットリストの削除
	bulletList->terminate();
	SAFE_DELETE(bulletList);
}

//===================================================================================================================================
//【更新：バレットマネージャー】
//===================================================================================================================================
void BulletManager::update(float frameTime)
{
	//インターバル時間
	if(intervalTimer > 0)intervalTimer -= frameTime;


	//各バレットの更新
	for (int i = 0; i < bulletList->nodeNum; i++)
	{
		//バレットのポインタ取得
		Bullet* bullet = (*bulletList->getValue(i));
		
		//更新
		bullet->update(frameTime);
	
		if(bullet->existenceTimer > 0 &&	//残存していて
			bullet->isCollideInitial())		//初期衝突予定位置に衝突している場合
		{
			//衝突演出ありの消滅
			bullet->destroy();
		}
	}

	//各バレットの削除処理
	for (int i = 0; i < bulletList->nodeNum; i++)
	{
		//バレットのポインタ取得
		Bullet* bullet = (*bulletList->getValue(i));
		float zeroDistance = Base::between2VectorLength(D3DXVECTOR3(0, 0, 0), bullet->position);

		//生存時間切れ||島（0,0,0）から一定距離離れた場合
		//自然消滅処理
		if (bullet->existenceTimer <= 0 || (zeroDistance > LOST_DISTANCE && currentScene != "Scene -Tutorial-"))
		{
			destroy(bullet, i);
		}
	}

	//リストの更新[削除による対応]
	bulletList->listUpdate();

	//リロード処理
	if (reloadTimer > 0)
	{
		reloadTimer -= frameTime;
	}
	else {
		if (reloading)
		{
			reloading = false;			//リロード終了
			remaining = MAGAZINE_NUM;	//装填
		}
	}

	//LAUNCH_FACT_TIME秒で発射事実を消す
	if (isLaunched)
	{
		launchFactTime += frameTime;
		if (launchFactTime > LAUNCH_FACT_TIME)
		{
			isLaunched = false;
		}
	}
}

//===================================================================================================================================
//【描画：バレットマネージャー】
//===================================================================================================================================
void BulletManager::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
	//弾道レイの描画
	for (int i = 0; i < bulletList->nodeNum; i++)
	{
		(*bulletList->getValue(i))->render();
	}
}

//===================================================================================================================================
//【発射：バレットマネージャー】
//===================================================================================================================================
bool BulletManager::launch(Ray shootingRay,int playerNo)
{
	//インターバル中：発射しない
	if (intervalTimer > 0)return false;	
	
	//リロード中：発射しない
	if (reloading)return false;
	
	//残段数が0：自動リロード
	if (remaining <= 0)
	{
		reload();
		return false;
	}

	//バレットリストへ新たに生成
	Bullet* newBullet = new Bullet(shootingRay,playerNo,isPowerUp);
	newBullet->setDigitalPower(getPowerRate());

	//リストへ追加
	bulletList->insertFront(newBullet);
	
	//リストの更新[追加による対応]
	bulletList->listUpdate();

	//残段数の減算
	remaining--;

	//インターバル時間のセット
	intervalTimer = INTERVAL_TIME;

	//サウンドの再生
	SoundInterface::SE->playSound(&shotSE);
	
	//ゲーム中に発射事実を残す
	isLaunched = true;
	launchFactTime = 0.0f;

	return true;
}

//===================================================================================================================================
//【リロード：バレットマネージャー】
//===================================================================================================================================
void BulletManager::reload()
{
	//残段数が最大：リロードしない
	if (remaining >= MAGAZINE_NUM)return;
	//リロード中：リロードしない
	if (reloading)return;
	reloading = true;							//リロード開始
	reloadTimer = RELOAD_TIME;					//リロードタイムの設定
	SoundInterface::SE->playSound(&reroadSE);	//サウンドの再生

}

//===================================================================================================================================
//【弾削除：バレットマネージャー】
//===================================================================================================================================
void BulletManager::destroy(Bullet* bullet,int nodeNumber)
{
	//削除
	SAFE_DELETE(bullet);									//実体の削除
	bulletList->remove(bulletList->getNode(nodeNumber));	//ポインタそのものの削除
}

//===================================================================================================================================
//【getter：バレットマネージャー】
//===================================================================================================================================
int BulletManager::getRemaining(){return remaining;}
float BulletManager::getReloadTime() { return reloadTimer; }
Bullet* BulletManager::getBullet(int i) { return *bulletList->getValue(i); }
int BulletManager::getNum() { return bulletList->nodeNum; }
bool BulletManager::getIsLaunched() { return isLaunched; }
float BulletManager::getPowerRate() { return powerRate; }

//===================================================================================================================================
//【setter：バレットマネージャー】
//===================================================================================================================================
void BulletManager::setPowerRate(float value)
{
	powerRate = value;
	isPowerUp = true;
}

void BulletManager::setCurrentScene(std::string scene)
{
	currentScene = scene;
}

