#include "GetHealthGoal_Evaluator.h"
#include "GetEscapeGoal_Evaluator.h"
#include "../Raven_ObjectEnumerations.h"
#include "Goal_Think.h"
#include "Raven_Goal_Types.h"
#include "misc/Stream_Utility_Functions.h"
#include "Raven_Feature.h"


double GetEscapeGoal_Evaluator::CalculateDesirability(Raven_Bot* pBot)
{
	double Distance = Raven_Feature::DistanceToItem(pBot, type_health);

	if (!pBot->GetTargetBot() || pBot->Health() >= pBot->GetTargetBot()->Health())
	{
		return 0;
	}
	else if (pBot->GetTargetSys()->isTargetShootable() && pBot->GetTargetBot()->GetTargetBot() == pBot)
	{
		const double Tweaker = 0.5;


		double Desirability = Tweaker * (1 - Raven_Feature::DistanceToItem(pBot, type_health)/ 500);

		//ensure the value is in the range 0 to 1
		Clamp(Desirability, 0, 1);

		//bias the value according to the personality of the bot
		Desirability *= m_dCharacterBias;

		return Desirability;
	}
	else
		return 0;
}



//----------------------------- SetGoal ---------------------------------------
//-----------------------------------------------------------------------------
void GetEscapeGoal_Evaluator::SetGoal(Raven_Bot* pBot)
{
	//pBot->GetBrain()->AddGoal_GetItem(type_escape);
	pBot->GetBrain()->AddGoal_Escape();
}

//-------------------------- RenderInfo ---------------------------------------
//-----------------------------------------------------------------------------
void GetEscapeGoal_Evaluator::RenderInfo(Vector2D Position, Raven_Bot* pBot)
{
	gdi->TextAtPos(Position, "H: " + ttos(CalculateDesirability(pBot), 2));
	return;

	std::string s = ttos(1 - Raven_Feature::Health(pBot)) + ", " + ttos(Raven_Feature::DistanceToItem(pBot, type_health));
	gdi->TextAtPos(Position + Vector2D(0, 15), s);
}