#pragma once

#include "GraphNet.h"

class Agent
{
public:
	Agent();
	~Agent();

	void AgentTick(); // evaluate k steps in a tick, Sum and collect the actions.
	void DoAction(); // Do the selected action and clear action cache.

	GraphNet m_brain;
};
