﻿#include "Menu.h"
#include "../../ArcheType/ArcheType.hpp"
#include "../../Class/DXFilieRead.hpp"

namespace Scene
{
	void Menu::indexAdd()
	{
		++index;
		if (index % 3 == 0)
		{
			index = 0;
		}
	}
	void Menu::indexSub()
	{
		--index;
		if (index < 0)
		{
			index = 2;
		}
	}

	Menu::Menu(IOnSceneChangeCallback * sceneTitleChange, Parameter * parame)
		: AbstractScene(sceneTitleChange)
	{
		//平坦なのしか出さないのでステージパラメーターはなんでもいい
		stageLoader.LoadStage("stage/stageparam03.csv");
		stageLoader.LoadStageConstitution();
		//以下のようにしないと動的にマップチップを切り替えられない
		ResourceManager::GetGraph().RemoveGraph(stageLoader.GetStageParam().mapImage);
		ResourceManager::GetGraph().Load("image/menu/cursor.png", "cursor");
		ResourceManager::GetGraph().Load("image/menu/stage1.png", "stage1UI");
		ResourceManager::GetGraph().Load("image/menu/stage2.png", "stage2UI");
		ResourceManager::GetGraph().Load("image/menu/stage3.png", "stage3UI");
		ResourceManager::GetGraph().Load("image/menu/hiscore.png", "hiscore");
		const_cast<StageParam&>(stageLoader.GetStageParam()).mapImage = "stage1";
		stageCreator.SetMapParam(stageLoader.GetStageParam());
		stageCreator.FillUpFlatMap();
		//ステージの生成
		stageCreator.Run(nullptr, nullptr, nullptr);
		cursor_L = ECS::ArcheType()("cursor", Vec2{ 0.f,300 }, ENTITY_GROUP::GameUI);
		cursor_R = ECS::ArcheType()("cursor", Vec2{ System::SCREEN_WIDIH - 160.f,300 }, ENTITY_GROUP::GameUI);
		cursor_R->GetComponent<ECS::SimpleDraw>().DoTurn(true);
		stageUI[0] = ECS::ArcheType()("stage1UI", Vec2{ System::SCREEN_WIDIH / 2.f,System::SCREEN_HEIGHT / 2.6f }, ENTITY_GROUP::GameUI);
		stageUI[0]->GetComponent<ECS::SimpleDraw>().DoCenter(true);
		stageUI[1] = ECS::ArcheType()("stage2UI", Vec2{ System::SCREEN_WIDIH / 2.f,System::SCREEN_HEIGHT / 2.6f }, ENTITY_GROUP::GameUI);
		stageUI[1]->GetComponent<ECS::SimpleDraw>().DoCenter(true);
		stageUI[1]->GetComponent<ECS::SimpleDraw>().DrawDisable();
		stageUI[2] = ECS::ArcheType()("stage3UI", Vec2{ System::SCREEN_WIDIH / 2.f,System::SCREEN_HEIGHT / 2.6f }, ENTITY_GROUP::GameUI);
		stageUI[2]->GetComponent<ECS::SimpleDraw>().DoCenter(true);
		stageUI[2]->GetComponent<ECS::SimpleDraw>().DrawDisable();
		ECS::ArcheType()("hiscore", Vec2{ System::SCREEN_WIDIH / 2.f,System::SCREEN_HEIGHT - 140.f}, ENTITY_GROUP::GameUI)
			->GetComponent<ECS::SimpleDraw>().DoCenter(true);
		ECS::Cloud()("cloud");

		//セーブデータのロード
		//1
		{
			int stageNo = 1;
			std::string stageName = "stage" + stageNo;
			stageName += ".dat";
			FileSystem().Load(stageName, &score[0]);
		}
		//2
		{
			int stageNo = 2;
			std::string stageName = "stage" + stageNo;
			stageName += ".dat";
			FileSystem().Load(stageName, &score[1]);
		}
		//3
		{
			int stageNo = 3;
			std::string stageName = "stage" + stageNo;
			stageName += ".dat";
			FileSystem().Load(stageName, &score[2]);
		}
		
	}
	void Menu::Finalize()
	{
		ResourceManager::GetGraph().RemoveGraph("cursor");
		ResourceManager::GetGraph().RemoveGraph("stage1UI");
		ResourceManager::GetGraph().RemoveGraph("stage2UI");
		ResourceManager::GetGraph().RemoveGraph("stage3UI");
		ResourceManager::GetGraph().RemoveGraph("hiscore");
		auto entity = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::GameUI);
		for (auto& e : entity)
		{
			e->Destroy();
		}
	}
	void Menu::Update()
	{
		ECS::EcsSystem::GetManager().Update();
		for (auto& it : stageUI)
		{
			it->GetComponent<ECS::SimpleDraw>().DrawDisable();
		}
		preIndex = index;
		stageUI[index]->GetComponent<ECS::SimpleDraw>().DrawEnable();
		//左端
		if (TouchInput::GetInput().GetBtnPress(0) == 1 && 
			Collision::BoxAndBox(
				TouchInput::GetInput().GetTouchIDPos(0), Vec2{ 1.f,1.f },
				Vec2{0.f,0.f}, Vec2{ 160.f,720.f }))
		{
			indexSub();
		}
		//右端
		else if (TouchInput::GetInput().GetBtnPress(0) == 1 &&
			Collision::BoxAndBox(
				TouchInput::GetInput().GetTouchIDPos(0), Vec2{ 1.f,1.f }, Vec2{ System::SCREEN_WIDIH - 160.f,0.f }, 
				Vec2{ System::SCREEN_WIDIH ,720.f }))
		{
			indexAdd();
		}
		//真ん中
		else if(TouchInput::GetInput().GetBtnPress(0) == 1)
		{
			switch (index)
			{
			case 0:
			{
				auto param = std::make_unique<Parameter>();
				param->Set<int>("stageNum", 1);
				CommonData::StageNum::val = 1;
				Finalize();
				GetCallback().OnSceneChange(SceneName::Game, param.get(), SceneStack::OneClear);
				return;
				break;
			}
			case 1:
			{
				auto param = std::make_unique<Parameter>();
				param->Set<int>("stageNum", 2);
				CommonData::StageNum::val = 2;
				Finalize();
				GetCallback().OnSceneChange(SceneName::Game, param.get(), SceneStack::OneClear);
				return;
				break;
			}
			case 2:
			{
				auto param = std::make_unique<Parameter>();
				param->Set<int>("stageNum", 3);
				CommonData::StageNum::val = 3;
				Finalize();
				GetCallback().OnSceneChange(SceneName::Game, param.get(), SceneStack::OneClear);
				return;
				break;
			}
			}
		}
		if (index != preIndex && index == 2)
		{
			const_cast<StageParam&>(stageLoader.GetStageParam()).mapImage = "stage3";
			stageCreator.SetMapParam(stageLoader.GetStageParam());
			auto entity = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Ground);

			for (auto& e : entity)
			{
				e->Destroy();
			}
			auto back = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Back1);
			for (auto& b : back)
			{
				b->Destroy();
			}
			stageCreator.FillUpFlatMap();
		}
		else if (index != preIndex && index == 0)
		{
			const_cast<StageParam&>(stageLoader.GetStageParam()).mapImage = "stage1";
			stageCreator.SetMapParam(stageLoader.GetStageParam());
			auto entity = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Ground);
			for (auto& e : entity)
			{
				e->Destroy();
			}
			auto back = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Back1);
			for (auto& b : back)
			{
				b->Destroy();
			}
			stageCreator.FillUpFlatMap();
		}
		else if (index != preIndex && index == 1)
		{
			const_cast<StageParam&>(stageLoader.GetStageParam()).mapImage = "stage2";
			stageCreator.SetMapParam(stageLoader.GetStageParam());
			auto entity = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Ground);
			for (auto& e : entity)
			{
				e->Destroy();
			}
			auto back = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Back1);
			for (auto& b : back)
			{
				b->Destroy();
			}
			stageCreator.FillUpFlatMap();
		}
		stageCreator.Run(nullptr, nullptr, nullptr);
		cloud.Run();
	}
	void Menu::Draw()
	{
		ECS::EcsSystem::GetManager().OrderByDraw(ENTITY_GROUP::Max);
		DrawFormatString(100, 500, 0, "1::%d", score[0]);
		DrawFormatString(100, 520, 0, "2::%d", score[1]);
		DrawFormatString(100, 540, 0, "3::%d", score[2]);
	}
}