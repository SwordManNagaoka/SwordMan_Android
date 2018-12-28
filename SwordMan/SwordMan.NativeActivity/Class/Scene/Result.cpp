﻿#include "Result.h"

//アーキタイプ
#include "../../ArcheType/Button.hpp"
//コンポーネント
#include "../../Components/ImageFontDraw.hpp"
#include "../../Components/BlendMode.hpp"
#include "../../Utility/Converter.hpp"
#include "../../Components/GradationColor.hpp"
#include "../../Components/EasingMove.hpp"

#include <fstream>
#include "../../Class/DXFilieRead.hpp"
#include "../../Class/RankSelector.hpp"
#include "../../Utility/Input.hpp"

namespace Scene
{
	Result::Result(IOnSceneChangeCallback* sceneTitleChange, Parameter* parame)
		: AbstractScene(sceneTitleChange)
	{
		ECS::Entity* btn = ECS::ButtonArcheType()("pauseUI", Vec2(680, 500), Vec2(0, 0), Vec2(96, 144), 50);
		btn->GetComponent<ECS::CircleColiider>().SetOffset(48, 48);
		btn->AddComponent<ECS::BackTitleButtonTag>();
		btn->AddGroup(ENTITY_GROUP::GameUI);

		ECS::Entity* menuBtn = ECS::ButtonArcheType()("pauseUI", Vec2(480, 500), Vec2(192, 0), Vec2(96, 144), 50);
		menuBtn->GetComponent<ECS::CircleColiider>().SetOffset(48, 48);
		menuBtn->AddComponent<ECS::BackMenuButtonTag>();
		menuBtn->AddGroup(ENTITY_GROUP::GameUI);

		ECS::Entity* backFade = &ECS::EcsSystem::GetManager().AddEntity();
		backFade->AddComponent<ECS::Position>(0, 0);
		backFade->AddComponent<ECS::SimpleDraw>("fade").DrawDisable();
		backFade->AddComponent<ECS::BlendMode>().SetAlpha(50);
		backFade->AddGroup(ENTITY_GROUP::Fade1);

		int scoreData = CommonData::TotalScore::val;
		int stageNo = CommonData::StageNum::val;

		//---スコアの表示---//
		ECS::Entity* scoreUI = &ECS::EcsSystem::GetManager().AddEntity();
		scoreUI->AddComponent<ECS::Color>(0,0,255);
		scoreUI->AddComponent<ECS::Position>(Vec2(-200,100));
		scoreUI->AddComponent<ECS::ImageFontDraw>("font",Vec2(32,32),16).SetFontImageKind(false);
		scoreUI->GetComponent<ECS::ImageFontDraw>().SetDrawData("Score");
		scoreUI->GetComponent<ECS::Scale>().val = 2.0f;
		scoreUI->AddComponent<ECS::EasingMove>(Easing::ExpoOut,ECS::EasingMove::DirectionState::LeftAndRight).SetBeginToEndPoint(-200, 200);
		scoreUI->GetComponent<ECS::EasingMove>().SetTimeToDuration(30.0f, 80.0f);
		scoreUI->AddGroup(ENTITY_GROUP::GameUI);

		ECS::Entity* scoreParam = &ECS::EcsSystem::GetManager().AddEntity();
		scoreParam->AddComponent<ECS::Color>(255, 0, 0);
		scoreParam->AddComponent<ECS::Position>(Vec2(1400, 100));
		scoreParam->AddComponent<ECS::ImageFontDraw>("font", Vec2(32, 32), 16).SetFontImageKind(false);
		scoreParam->GetComponent<ECS::ImageFontDraw>().SetDrawData(Converter::ToString(scoreData).c_str());
		scoreParam->GetComponent<ECS::Scale>().val = 2.0f;
		scoreParam->AddComponent<ECS::GradationColor>().SetGradationPower(Vec3(2, 1, 4));
		scoreParam->AddComponent<ECS::EasingMove>(Easing::ExpoOut, ECS::EasingMove::DirectionState::LeftAndRight).SetBeginToEndPoint(1400, 800);
		scoreParam->GetComponent<ECS::EasingMove>().SetTimeToDuration(30.0f, 80.0f);
		scoreParam->AddGroup(ENTITY_GROUP::GameUI);
		
		//---ランク表示---//
		ECS::Entity* rankUI = &ECS::EcsSystem::GetManager().AddEntity();
		rankUI->AddComponent<ECS::Color>(0, 0, 255);
		rankUI->AddComponent<ECS::Position>(Vec2(-400,300));
		rankUI->AddComponent<ECS::ImageFontDraw>("font", Vec2(32, 32), 16).SetFontImageKind(false);
		rankUI->GetComponent<ECS::ImageFontDraw>().SetDrawData("Rank");
		rankUI->GetComponent<ECS::Scale>().val = 2.0f;
		rankUI->AddComponent<ECS::EasingMove>(Easing::ExpoOut, ECS::EasingMove::DirectionState::LeftAndRight).SetBeginToEndPoint(-400, 200);
		rankUI->GetComponent<ECS::EasingMove>().SetTimeToDuration(50.0f, 130.0f);
		rankUI->AddGroup(ENTITY_GROUP::GameUI);
		
		std::string rankName = RankSelector().execute(scoreData);

		ECS::Entity* rankData = &ECS::EcsSystem::GetManager().AddEntity();
		rankData->AddComponent<ECS::Color>(255, 0, 0);
		rankData->AddComponent<ECS::Position>(Vec2(1400, 300));
		rankData->AddComponent<ECS::ImageFontDraw>("font", Vec2(32, 32), 16).SetFontImageKind(false);
		rankData->AddComponent<ECS::EasingMove>(Easing::ExpoOut, ECS::EasingMove::DirectionState::LeftAndRight).SetBeginToEndPoint(1400, 800);
		rankData->GetComponent<ECS::EasingMove>().SetTimeToDuration(50.0f, 130.0f);
		rankData->GetComponent<ECS::ImageFontDraw>().SetDrawData(rankName.c_str());
		rankData->GetComponent<ECS::Scale>().val = 2.0f;
		rankData->AddComponent<ECS::GradationColor>().SetGradationPower(Vec3(1,3,5));
		rankData->AddGroup(ENTITY_GROUP::GameUI);
		
		std::string stageName = "stage" + stageNo;
		stageName += ".dat";
		if (FileSystem().HighScoreSave(stageName, &scoreData))
		{
			//---新記録の表示---//
			ECS::Entity* newRecord = &ECS::EcsSystem::GetManager().AddEntity();
			newRecord->AddComponent<ECS::Color>(128, 128, 0);
			newRecord->AddComponent<ECS::Position>(Vec2(650, 200));
			newRecord->AddComponent<ECS::ImageFontDraw>("font", Vec2(32, 32), 16).SetFontImageKind(false);
			newRecord->GetComponent<ECS::ImageFontDraw>().SetDrawData("NEW RECORD");
			newRecord->GetComponent<ECS::Scale>().val = 2.0f;
			newRecord->AddComponent<ECS::GradationColor>().SetGradationPower(Vec3(3, 2, 1));
			newRecord->AddGroup(ENTITY_GROUP::GameUI);
		}
		const auto& button = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::GameUI);
		for (auto& b : button)
		{
			if (b->HasComponent<ECS::BackTitleButtonTag>())
			{
				b->GetComponent<ECS::PushButton>().SetSceneCallBack(&GetCallback());
				auto changeFunc = [](Scene::IOnSceneChangeCallback* callBack)
				{
					callBack->OnSceneChange(SceneName::Game, nullptr, SceneStack::AllClear);
					return;
				};
				b->GetComponent<ECS::PushButton>().SetEventFunction(changeFunc);
			}
			if (b->HasComponent<ECS::BackMenuButtonTag>())
			{
				b->GetComponent<ECS::PushButton>().SetSceneCallBack(&GetCallback());
				auto changeFunc = [](Scene::IOnSceneChangeCallback* callBack)
				{
					const auto& fades = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Fade1);
					for (auto& fade : fades)
					{
						fade->GetComponent<ECS::BlendMode>().FadeIn(50, 255, 5);
					}
					return;
				};
				b->GetComponent<ECS::PushButton>().SetEventFunction(changeFunc);
			}
		}
	}

	Result::~Result()
	{
		ECS::EcsSystem::GetManager().AllKill();
		ResourceManager::GetSound().Remove("BGM");
	}
	
	void Result::Update()
	{
		if (Input::Get().GetKeyFrame(KEY_INPUT_S) == 1)
		{
			GetCallback().OnSceneChange(SceneName::Game, nullptr, SceneStack::AllClear);
			return;
		}
		else if (Input::Get().GetKeyFrame(KEY_INPUT_A) == 1)
		{
			GetCallback().OnSceneChange(SceneName::Menu, nullptr, SceneStack::AllClear);
			return;
		}
		const auto& button = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::GameUI);
		const auto& player = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Player);
		for (const auto& it : player) { it->Update(); }
		for (auto& b : button) { b->Update(); }

		const auto& fades = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Fade1);
		for (auto& fade : fades)
		{
			if (!fade->HasComponent<ECS::BlendMode>()) { break; }
			if (fade->GetComponent<ECS::BlendMode>().isEnd())
			{
				GetCallback().OnSceneChange(SceneName::Menu, nullptr, SceneStack::AllClear);
				return;
			}
		}

	}
	void Result::Draw()
	{
		ECS::EcsSystem::GetManager().OrderByDraw(ENTITY_GROUP::Max);
	}
}