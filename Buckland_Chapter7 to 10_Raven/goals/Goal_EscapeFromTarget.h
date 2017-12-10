#ifndef GOAL_ESCAPEFROMTARGET_H
#define GOAL_ESCAPEFROMTARGET_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   Goal_EscapeFromTarget.h
//
//  Author: È«·¡ Jeong
//
//  Desc:   
//
//-----------------------------------------------------------------------------
#include "Goals/Goal_Composite.h"
#include "Raven_Goal_Types.h"
#include "../Raven_Bot.h"





class Goal_EscapeFromTarget : public Goal_Composite<Raven_Bot>
{
private:
	Vector2D m_vector;
public:

	Goal_EscapeFromTarget(Raven_Bot* pOwner) :Goal_Composite<Raven_Bot>(pOwner, goal_escape_from_target)
	{}

	void Activate();

	int  Process();

	void Terminate() { m_iStatus = completed; }

};






#endif