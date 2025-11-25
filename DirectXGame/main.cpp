#include <Windows.h>

#include "KamataEngine.h"

using namespace KamataEngine;

// テクスチャハンドル
uint32_t textureHandle_ = 0;

uint32_t textureEnemy_ = 0;

uint32_t textureBullet_ = 0;

// スプライト
KamataEngine::Sprite* sprite_ = nullptr;

KamataEngine::Sprite* spriteFont_[100] = {};
KamataEngine::Sprite* spriteCommand_[100] = {};
KamataEngine::Sprite* spriteEnemy_ = nullptr;
KamataEngine::Sprite* spriteBullet_ = nullptr;

// 構造体
struct Bullet {
	float x, y;
	bool isAlive;
};
struct Enemy {
	float x, y;
	bool isAlive;
};


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// エンジンの初期化
	KamataEngine::Initialize(L"キーファイア");

	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("word.png");

	textureEnemy_ = TextureManager::Load("MonsterEN.png");

	textureBullet_ = TextureManager::Load("bullet1.png");
	// スプライトインスタンスの生成
	sprite_ = Sprite::Create(textureHandle_, {100, 10});
	spriteBullet_ = Sprite::Create(textureBullet_, {0, 0});
	spriteEnemy_ = Sprite::Create(textureEnemy_, {800, 400});

	for (int i = 0; i < 100; i++) {
		spriteFont_[i] = Sprite::Create(textureHandle_, {100.0f + i * 32, 200});
		spriteFont_[i]->SetSize({32, 32});
	}
	for (int i = 0; i < 100; i++) {
		spriteCommand_[i] = Sprite::Create(textureHandle_, {100.0f + i * 32, 100});
		spriteCommand_[i]->SetSize({32, 32});
	}
	// --- ゲーム状態 ---
	std::string commands[] = {"aaaaa", "bbbbbbb", "hit", "run", "shot", "zap"}; // 短く簡単
	std::string currentCommand = "";
	std::string inputText = "";
	Bullet bullet = {0, 0, false};
	Enemy enemy = {1200.0f, 360.0f, true};
	float timer = 0.0f;
	float limitTime = 180.0f; // 約3秒
	bool commandActive = false;
	bool success = false;
	int score = 0;
	int life = 3;
	int level = 1;

	//今までにキーを押したか？
	bool inputPush = false;

	// メインループ
	while (true) {
		// エンジンの更新
		if (KamataEngine::Update()) {
			break;
		}
		/*if (life <= 0) {
			Novice::ScreenPrintf(560, 360, "GAME OVER");
			Novice::EndFrame();
			if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0)
				break;
			continue;
		}*/
		// 敵がいないなら新しく出す
		if (!enemy.isAlive) {
			enemy.x = 1100.0f;
			enemy.y = 360.0f;
			enemy.isAlive = true;
		}
		// コマンド発生
		if (!commandActive) {
			currentCommand = commands[rand() % (int)(sizeof(commands) / sizeof(commands[0]))];
			inputText = "";
			timer = limitTime;
			commandActive = true;
			success = false;
		}
		// 制限時間
		if (commandActive) {
			timer--;
			if (timer <= 0) {
				commandActive = false;
			}
		}
		// 入力処理（完全対応版）
		char inputKey = 0; //入力していない
		if (Input::GetInstance()->PushKey(DIK_A))inputKey += 'a';
		if (Input::GetInstance()->PushKey(DIK_B))inputKey += 'b';
		if (Input::GetInstance()->PushKey(DIK_C))inputKey += 'c';
		if (Input::GetInstance()->PushKey(DIK_D))inputKey += 'd';
		if (Input::GetInstance()->PushKey(DIK_E))inputKey += 'e';
		if (Input::GetInstance()->PushKey(DIK_F))inputKey += 'f';
		if (Input::GetInstance()->PushKey(DIK_G))inputKey += 'g';
		if (Input::GetInstance()->PushKey(DIK_H))inputKey += 'h';
		if (Input::GetInstance()->PushKey(DIK_I))inputKey += 'i';
		if (Input::GetInstance()->PushKey(DIK_J))inputKey += 'j';
		if (Input::GetInstance()->PushKey(DIK_K))inputKey += 'k';
		if (Input::GetInstance()->PushKey(DIK_L))inputKey += 'l';
		if (Input::GetInstance()->PushKey(DIK_M))inputKey += 'm';
		if (Input::GetInstance()->PushKey(DIK_N))inputKey += 'n';
		if (Input::GetInstance()->PushKey(DIK_O))inputKey += 'o';
		if (Input::GetInstance()->PushKey(DIK_P))inputKey += 'p';
		if (Input::GetInstance()->PushKey(DIK_Q))inputKey += 'q';
		if (Input::GetInstance()->PushKey(DIK_R))inputKey += 'r';
		if (Input::GetInstance()->PushKey(DIK_S))inputKey += 's';
		if (Input::GetInstance()->PushKey(DIK_T))inputKey += 't';
		if (Input::GetInstance()->PushKey(DIK_U))inputKey += 'u';
		if (Input::GetInstance()->PushKey(DIK_V))inputKey += 'v';
		if (Input::GetInstance()->PushKey(DIK_W))inputKey += 'w';
		if (Input::GetInstance()->PushKey(DIK_X))inputKey += 'x';
		if (Input::GetInstance()->PushKey(DIK_Y))inputKey += 'y';
		if (Input::GetInstance()->PushKey(DIK_Z))inputKey += 'z';
		//Enterで判定
		if (Input::GetInstance()->PushKey(DIK_RETURN)  && commandActive) {
		    if (inputText == currentCommand) {
		    		bullet = {100.0f, 360.0f, true};
		    	success = true;
		    	}
		    	commandActive = false;
		     }
		//今までキーを離していれば
		if (inputPush == false) {
			//入力があれば
			if (inputKey != 0) {
				inputText += inputKey; //文字列に追加
				inputPush = true;      //押していることを記憶
			}
		}
		//今までキーを押していれば
		else
		{
			//入力がなければ
			if (inputKey == 0)
			{
				inputPush = false;    //離していることを記憶
			}
		}
		/*if (preKeys[DIK_A] == 0 && keys[DIK_A] != 0)
			inputText += 'a';
		if (preKeys[DIK_B] == 0 && keys[DIK_B] != 0)
			inputText += 'b';
		if (preKeys[DIK_C] == 0 && keys[DIK_C] != 0)
			inputText += 'c';
		if (preKeys[DIK_D] == 0 && keys[DIK_D] != 0)
			inputText += 'd';
		if (preKeys[DIK_E] == 0 && keys[DIK_E] != 0)
			inputText += 'e';
		if (preKeys[DIK_F] == 0 && keys[DIK_F] != 0)
			inputText += 'f';
		if (preKeys[DIK_G] == 0 && keys[DIK_G] != 0)
			inputText += 'g';
		if (preKeys[DIK_H] == 0 && keys[DIK_H] != 0)
			inputText += 'h';
		if (preKeys[DIK_I] == 0 && keys[DIK_I] != 0)
			inputText += 'i';
		if (preKeys[DIK_J] == 0 && keys[DIK_J] != 0)
			inputText += 'j';
		if (preKeys[DIK_K] == 0 && keys[DIK_K] != 0)
			inputText += 'k';
		if (preKeys[DIK_L] == 0 && keys[DIK_L] != 0)
			inputText += 'l';
		if (preKeys[DIK_M] == 0 && keys[DIK_M] != 0)
			inputText += 'm';
		if (preKeys[DIK_N] == 0 && keys[DIK_N] != 0)
			inputText += 'n';
		if (preKeys[DIK_O] == 0 && keys[DIK_O] != 0)
			inputText += 'o';
		if (preKeys[DIK_P] == 0 && keys[DIK_P] != 0)
			inputText += 'p';
		if (preKeys[DIK_Q] == 0 && keys[DIK_Q] != 0)
			inputText += 'q';
		if (preKeys[DIK_R] == 0 && keys[DIK_R] != 0)
			inputText += 'r';
		if (preKeys[DIK_S] == 0 && keys[DIK_S] != 0)
			inputText += 's';
		if (preKeys[DIK_T] == 0 && keys[DIK_T] != 0)
			inputText += 't';
		if (preKeys[DIK_U] == 0 && keys[DIK_U] != 0)
			inputText += 'u';
		if (preKeys[DIK_V] == 0 && keys[DIK_V] != 0)
			inputText += 'v';
		if (preKeys[DIK_W] == 0 && keys[DIK_W] != 0)
			inputText += 'w';
		if (preKeys[DIK_X] == 0 && keys[DIK_X] != 0)
			inputText += 'x';
		if (preKeys[DIK_Y] == 0 && keys[DIK_Y] != 0)
			inputText += 'y';
		if (preKeys[DIK_Z] == 0 && keys[DIK_Z] != 0)
			inputText += 'z';*/
		// バックスペース
		//if (preKeys[DIK_BACKSPACE] == 0 && keys[DIK_BACKSPACE] != 0 && !inputText.empty()) {
		//	inputText.pop_back();
		//}
		//// Enterで判定
		//if (preKeys[DIK_RETURN] == 0 && keys[DIK_RETURN] != 0 && commandActive) {
		//	if (inputText == currentCommand) {
		//		bullet = {100.0f, 360.0f, true};
		//		success = true;
		//	}
		//	commandActive = false;
		//}
		// 弾移動
		if (bullet.isAlive) {
			bullet.x += 10.0f;
			if (bullet.x > 1280)
				bullet.isAlive = false;
		}
		// 敵移動
		if (enemy.isAlive) {
			enemy.x -= 3.0f + level * 0.5f;
			if (enemy.x < 0) {
				enemy.isAlive = false;
				life--;
			}
		}
		// 当たり判定
		if (bullet.isAlive && enemy.isAlive) {
			float dx = bullet.x - enemy.x;
			float dy = bullet.y - enemy.y;
			float dist = sqrtf(dx * dx + dy * dy);
			if (dist < 20.0f) {
				bullet.isAlive = false;
				enemy.isAlive = false;
				score += 100;
				if (score % 300 == 0 && limitTime > 60) {
					level++;
					limitTime -= 15;
				}
			}
		}

		spriteBullet_->SetPosition({bullet.x, bullet.y});
		spriteEnemy_->SetPosition({enemy.x, enemy.y});

		// 描画開始
		dxCommon->PreDraw();

		// ここに描画処理を記述する

		// スプライト描画前処理
		Sprite::PreDraw();
		// 弾描画
		if (bullet.isAlive) {
			/*Novice::DrawEllipse((int)bullet.x, (int)bullet.y, 10, 10, 0.0f, 0xFFAA00FF, kFillModeSolid);*/
			spriteBullet_->Draw();
		}
		// 敵描画
		if (enemy.isAlive) {
			/*Novice::DrawBox((int)enemy.x - 15, (int)enemy.y - 15, 30, 30, 0.0f, 0xFF3333FF, kFillModeSolid);*/
			spriteEnemy_->Draw();
		}
		sprite_->Draw();

		// 文字列を描画する
		int i = 0;
		while (inputText[i] != '\0') {
			/*int x = msg[i] - 'A';*/ // 　文字を数値に変換
			int x = inputText[i] - 'a';//文字を数値に変換
			x = x * 32;           // 幅を掛ける
			spriteFont_[i]->SetTextureRect({(float)x, 0}, {32, 32});

			spriteFont_[i]->Draw();

			i++;
		}
		//コマンド表示
		 i = 0;
		while (currentCommand[i] != '\0') {
			/*int x = msg[i] - 'A';*/   // 　文字を数値に変換
			int x = currentCommand[i] - 'a'; // 文字を数値に変換
			
			x = x * 32;                 // 幅を掛ける
			spriteCommand_[i]->SetTextureRect({(float)x, 0}, {32, 32});

			spriteCommand_[i]->Draw();

			i++;
		}
		// スプライト描画後処理
		Sprite::PostDraw();

		// 描画終了
		dxCommon->PostDraw();
	}

	delete sprite_;

	for (int i = 0; i < 100; i++) {
		delete spriteFont_[i];
	}
	for (int i = 0; i < 100; i++) {
		delete spriteCommand_[i];
	}
	delete spriteBullet_;
	delete spriteEnemy_;

	// エンジンの終了処理
	KamataEngine::Finalize();

	return 0;
}
