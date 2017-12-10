#include "Raven_TargetingSystem.h"
#include "Raven_Bot.h"
#include "Raven_SensoryMemory.h"



//-------------------------------- ctor ---------------------------------------
//-----------------------------------------------------------------------------
Raven_TargetingSystem::Raven_TargetingSystem(Raven_Bot* owner) :m_pOwner(owner),
m_pCurrentTarget(0)
{}



//----------------------------- Update ----------------------------------------
//	��� ������ �Ÿ��� 100 - ���� HP�� �̿��ؼ� ������ å��,
//	�� �� ������ ���� ���� ���� ���� �켱�� Ÿ�� 
//-----------------------------------------------------------------------------
void Raven_TargetingSystem::Update()
{
	std::pair<double, Raven_Bot*> ScoreNTarget(MaxDouble, nullptr);
	
	int MemoryHitDamage = 0;			// ���� ���ظ� ���� ���� ���ؿ� ���� ���� 
	m_pCurrentTarget = 0;

	//grab a list of all the opponents the owner can sense
	std::list<Raven_Bot*> SensedBots;
	std::map<Raven_Bot*, MemoryRecord> SeneoryMemorys;

	SensedBots = m_pOwner->GetSensoryMem()->GetListOfRecentlySensedOpponents();
	SeneoryMemorys = m_pOwner->GetSensoryMem()->GetMemoryMap();

	std::list<Raven_Bot*>::const_iterator curBot = SensedBots.begin();
	for (curBot; curBot != SensedBots.end(); ++curBot)
	{
		//make sure the bot is alive and that it is not the owner
		if ((*curBot)->isAlive() && (*curBot != m_pOwner))
		{
			double dist = Vec2DDistanceSq((*curBot)->Pos(), m_pOwner->Pos()) / 100.0;
			MemoryHitDamage = (*curBot)->GetMaxHealth() - SeneoryMemorys[*curBot].nHittedDamage;

			if (ScoreNTarget.first > (double)MemoryHitDamage + dist)
				ScoreNTarget = std::pair<double, Raven_Bot*>((double)MemoryHitDamage + dist, *curBot);
		}
	}

	m_pCurrentTarget = ScoreNTarget.second;
}




bool Raven_TargetingSystem::isTargetWithinFOV()const
{
	return m_pOwner->GetSensoryMem()->isOpponentWithinFOV(m_pCurrentTarget);
}

bool Raven_TargetingSystem::isTargetShootable()const
{
	return m_pOwner->GetSensoryMem()->isOpponentShootable(m_pCurrentTarget);
}

Vector2D Raven_TargetingSystem::GetLastRecordedPosition()const
{
	return m_pOwner->GetSensoryMem()->GetLastRecordedPositionOfOpponent(m_pCurrentTarget);
}

double Raven_TargetingSystem::GetTimeTargetHasBeenVisible()const
{
	return m_pOwner->GetSensoryMem()->GetTimeOpponentHasBeenVisible(m_pCurrentTarget);
}

double Raven_TargetingSystem::GetTimeTargetHasBeenOutOfView()const
{
	return m_pOwner->GetSensoryMem()->GetTimeOpponentHasBeenOutOfView(m_pCurrentTarget);
}
