﻿//----------------------------------------------------
//!@file	Think.hpp
//!@brief	思考(状態に応じた遷移)部分を作成します
//!@author	日比野　真聖
//!@date	2018/9/15
//----------------------------------------------------
#pragma once
#include "../ECS/ECS.hpp"
#include "../Utility/Vec.hpp"
#include "../ResourceManager/ResourceManager.hpp"
#include "../Components/BasicComponents.hpp"
#include "../../Components/ComponentDatas/PlayerData.hpp"
#include "../../Components/Think.hpp"
#include "../../Components/Jump.hpp"
#include "../../Components/AnimationController.hpp"
#include "../../ArcheType/AttackCollision.hpp"


namespace ECS
{
	class Behavior : public Component
	{
	public:
		void	Initialize() override
		{
			if (entity->HasComponent<TriggerJumpMove>())
			{
				jumpMove = &entity->GetComponent<TriggerJumpMove>();
			}
			if (entity->HasComponent<Think>())
			{
				think = &entity->GetComponent<Think>();
			}
		}
		void	Update() override
		{
			if (entity->HasComponent<Think>())
			{
				auto think = entity->GetComponent<Think>();

				switch (think.GetNowState())
				{
				case PlayerData::State::Walk:
					jumpMove->SetJumpTrigger(false);
					break;
				case PlayerData::State::Jump:
				{
					jumpMove->SetJumpTrigger(true);
				}
				break;
				case PlayerData::State::Attack:
					if (think.GetNowMotionCnt().GetCurrentCount() == 0)
					{
						auto pos = entity->GetComponent<Position>().val;
						float sizeX = entity->GetComponent<HitBase>().w() / 2.0f;
						SwordAttackCollision()(Vec2(pos.x + sizeX,pos.y), Vec2(96.0f, 96.0f), 30);
					}
					break;
				case PlayerData::State::JumpAttack:
					if (think.GetNowMotionCnt().GetCurrentCount() == 0)
					{
						entity->DeleteComponent<AnimationDraw>();
						entity->DeleteComponent<AnimationController>();
						entity->AddComponent<AnimationDraw>("rolling");
						entity->AddComponent<AnimationController>(4, 4);

						auto pos = entity->GetComponent<Position>().val;
						float sizeX = entity->GetComponent<HitBase>().w() / 2.0f;
						JumpAttackCollision()(Vec2(pos.x + sizeX,pos.y), Vec2(96.0f, 96.0f), 30);
					}
					if (think.GetNowMotionCnt().GetCurrentCount() > 16)
					{
						entity->DeleteComponent<AnimationDraw>();
						entity->DeleteComponent<AnimationController>();
						entity->AddComponent<ECS::AnimationDraw>("player");
						entity->AddComponent<ECS::AnimationController>(20, 2);
					}
					break;
				case PlayerData::State::Damage:
					break;
				case PlayerData::State::Death:
					break;
				}
			}
		}
		void	Draw2D() override
		{

		}
	private:
		void	Draw3D() override {}
	private:
		TriggerJumpMove* jumpMove;
		Think* think;
	};
}