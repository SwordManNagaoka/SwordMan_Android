﻿#include "Pause.h"
#include "../../GameController/GameController.h"
#include "../../ECS/ECS.hpp"
//アーキタイプ
#include "../../ArcheType/Button.hpp"
//コンポーネント
#include "../../Components/BlendMode.hpp"


namespace Scene
{
	Pause::Pause()
	{
		for (int i = 0; i < 3; ++i)
		{
			ECS::Entity* e = ECS::ButtonArcheType()("pauseUI", Vec2(300 + i * 150, 300), Vec2(i * 96, 0), Vec2(96, 144), 40);
			e->AddGroup(ENTITY_GROUP::PauseUI);
		}
		ECS::Entity* entity = &ECS::EcsSystem::GetManager().AddEntity();
		entity->AddComponent<ECS::Position>();
		entity->AddComponent<ECS::SimpleDraw>("fade");
		entity->AddComponent<ECS::BlendMode>().SetAlpha(128);
		entity->AddGroup(ENTITY_GROUP::Back1);
	}
	Pause::~Pause()
	{
		auto entity = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Back1);
		for (auto& e : entity)
		{
			e->Destroy();
		}
		auto pause = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::PauseUI);
		for (auto& e : pause)
		{
			e->Destroy();
		}
	}
	void Pause::Update()
	{
		ECS::EcsSystem::GetManager().Update();
	}
	void Pause::Draw()
	{
		ECS::EcsSystem::GetManager().OrderByDraw(ENTITY_GROUP::Max);
	}
}