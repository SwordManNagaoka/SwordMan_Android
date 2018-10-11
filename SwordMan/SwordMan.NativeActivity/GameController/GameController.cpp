#include "GameController.h"
#include "../ResourceManager/ResourceManager.hpp"
#include "../../Components/BasicComponents.hpp"
#include "../../Components/Renderer.hpp"
#include "../../Components/Collider.hpp"
#include "../../ArcheType/Map.hpp"
#include "../../ArcheType/Player.hpp"
#include "../Class/TouchInput.hpp"
#include "../../Class/Scene/SceneManager.hpp"
#include "../../Events/EventManager.hpp"
#include "../../Events/AtackEvent.hpp"
#include "../../ArcheType/Enemy.hpp"
#include "../../Events/AddScoreEvent.hpp"
#include "../../Events/GoalEvent.hpp"

void GameController::ResourceLoad()
{
	ResourceManager::GetGraph().Load("image/a.png", "a");
	ResourceManager::GetGraph().Load("image/font_text.png", "font");
	ResourceManager::GetGraph().Load("image/ui/goalMessage.png", "goalMessage");
	ResourceManager::GetGraph().Load("image/ui/pauseButton.png", "pauseButton");
	ResourceManager::GetGraph().Load("image/ui/life.png", "health");
	ResourceManager::GetGraph().Load("image/ui/pauseUI.png", "pauseUI");
	ResourceManager::GetGraph().Load("image/ui/fade.png", "fade");
	ResourceManager::GetGraph().LoadDiv("image/sword.png", "sword", 5, 5, 1, 192, 192);
	ResourceManager::GetGraph().LoadDiv("image/rolling.png", "rolling", 12, 4, 3, 288, 288);	
	ResourceManager::GetGraph().LoadDiv("image/enemy01.png", "enemy1", 2, 2, 1, 96, 96);
	ResourceManager::GetGraph().LoadDiv("image/enemy02.png", "enemy2", 4, 4, 1, 96, 96);
	ResourceManager::GetGraph().LoadDiv("image/enemy03.png", "enemy3", 6, 6, 1, 96, 96);
	ResourceManager::GetGraph().LoadDiv("image/enemy04.png", "enemy4", 4, 4, 1, 96, 96);
	ResourceManager::GetGraph().LoadDiv("image/goal.png", "goal", 1, 1, 1, 144, 192);
	ResourceManager::GetGraph().LoadDiv("image/player.png", "player", 6, 2, 3, 96, 96);
	ResourceManager::GetGraph().LoadDiv("image/effect/hit_weak.png", "hitWeak", 4, 4, 1, 192, 192);
	ResourceManager::GetGraph().LoadDiv("image/effect/bomb.png", "bomb", 4, 4, 1, 192, 192);
	ResourceManager::GetGraph().LoadDiv("image/effect/hit_strong.png", "hitStrong", 5, 5, 1, 192, 192);
}

GameController::GameController()
{
	//最初に必要なリソースやEntityの生成、ロードを行う
	ResourceLoad();
	pManager = &ECS::EcsSystem::GetManager();	
	//初期シーン
	Scene::SceneManager::Get().ChangeScene(Scene::SceneManager::State::Game);

	//イベント関数の登録
	//Event::EventManager().Get().Add(Scene::SceneManager::State::Game, Event::AddScoreEvent::Do);
	Event::EventManager().Get().Add(Scene::SceneManager::State::Game, Event::CollisionEvent::AttackCollisionToEnemy);
	Event::EventManager().Get().Add(Scene::SceneManager::State::Game, Event::CollisionEvent::PlayerToEnemy);
	Event::EventManager().Get().Add(Scene::SceneManager::State::Game, Event::GoalEvent::HitPlayer);
}

void GameController::ResetGame()
{

}

void GameController::Update()
{
	TouchInput::GetInput().Update();
	pManager->Refresh();
	Event::EventManager::Get().Update();
	Scene::SceneManager::Get().Update();

}

void GameController::Draw()
{
	Scene::SceneManager::Get().Draw();
}
