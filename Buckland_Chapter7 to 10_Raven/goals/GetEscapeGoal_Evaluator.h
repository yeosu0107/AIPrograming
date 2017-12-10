#ifndef RAVEN_ESCAPE_EVALUATOR
#define RAVEN_ESCAPE_EVALUATOR
#pragma warning (disable:4786)


#include "Goal_Evaluator.h"
#include "../Raven_Bot.h"

class GetEscapeGoal_Evaluator : public Goal_Evaluator
{
public:

	GetEscapeGoal_Evaluator(double bias) :Goal_Evaluator(bias) {}

	double CalculateDesirability(Raven_Bot* pBot);

	void  SetGoal(Raven_Bot* pEnt);

	void RenderInfo(Vector2D Position, Raven_Bot* pBot);
};

#endif