#pragma once

// TODO: strides in vision & offset inside ticks?
constexpr auto VISION_WIDTH = 7;
constexpr auto VISION_DEPTH = 7;
constexpr auto VISION_CHANNEL = 3;
constexpr auto TOUCH_DEPTH = 5;
constexpr auto STATUS_DEPTH = 8;
constexpr auto INPUT_COUNT = VISION_DEPTH * VISION_WIDTH * VISION_CHANNEL + TOUCH_DEPTH + STATUS_DEPTH;

constexpr auto OUTPUT_DIRC_DEPTH = 4;
constexpr auto OUTPUT_ACTION_DEPTH = 6;
constexpr auto OUTPUT_COUNT = OUTPUT_DIRC_DEPTH + OUTPUT_ACTION_DEPTH;

constexpr auto IO_COUNT = INPUT_COUNT + OUTPUT_COUNT;

constexpr auto MINIMAL_AGENT_COUNT = 30;

constexpr auto SPECIES_COUNT = 4;

constexpr auto MAPSIZE_X = 64;
constexpr auto MAPSIZE_Y = 64;

class Agent;

enum TerrainType // ch1
{
	Meadow,		 // move costs more stamina, have more seed and nuts.
	Grass,		 // regular, spawns food and few seed but no nuts.
	Dirt,		 // Meta-data: seed (if any) growth time; nothing special, move costs less stamina.
	SeedDirt,	 // Seeded dirt
	Rock,		 // Spawns stones, move costs less stamina.
	Obstacle,	 // Cannot pass through
};

enum ObjectType  // ch2
{
	None,		 // Nothing
	Seed,		 // Eatable seed (Low energy), could grow up to 2-4 food and 0-1 seed, only plantable in dirt
	Food,		 // Regular food
	Nut,		 // Higher energy than food, but need stone to open
	Stone,		 // Open nuts or attack front agent
	AgentSaying, // Agent currently talking
};

enum OutputDirection
{
	Up,
	Down,
	Left,
	Right
};

enum OutputAction
{
	Move,
	Run,
	Stay,
	Say,
	Attack_Use,
	Pick_Drop,
};

enum NodeType
{
	Hidden,
	Error,

	// Do not use them in a gene ( it should be added automatically when build the graph )
	InputNeuron,
	OutputNeuron,
};

enum Activation
{
	Identity,
	ReLU
};

struct MapGrid
{
	TerrainType terrain;
	float terrainMeta;
	ObjectType object;
	bool hasAgentTalking;
	int agentSpecies;
	Agent* currentGridAgent;
};

struct InputSignal
{
	float vision[VISION_WIDTH * VISION_DEPTH * VISION_CHANNEL]; // 7x7x3, Terrain - Species - Objects & Information
	float touch[TOUCH_DEPTH]; // ground has object - ground object - hand has object - hand object - current terrain
	float status[STATUS_DEPTH]; // health% - low health - health cost last frame - stamina% - low stamina - stamina cost last frame - mateable - tick clock
};

struct AgentOutput
{
	OutputAction action;
	OutputDirection direction;
};

struct VertexNode
{
	int innovationNum;

	int index;			 // Index of this vertex
	NodeType nodeType;	 // Node type (hidden / error)

	Activation act;		 // Activation function
	float bias;			 // Initial bias ( only when preActivation higher than this, output of the node will be activated as preActivation - bias ( biased ReLU ) )
	float learningDecay; // How much gradients will it back-propagate
	float valueDecay;	 // fixed node value decay = 0.6 ( value decay after each forward step )

	float preActivation; // For computation
	float beforeActValue;// For computaiton
	float value;		 // For computation
};

struct EdgeNode
{
	int innovationNum;

	int startVertex;	 // vertexIndex < IO_Count = I/O nodes. 0 ~ 159 = Input nodes, 160 ~ 169 = Output nodes.
	int endVertex;
	bool enabled;		 // Edges with low weights will become DISABLED with high prob. and DISABLED edge will be randomly removed, kept, or re-enabled.

	float weight;		 // Initial weight value.
						 // float learningRate;
};

int directionVector[4][2] = { {0, 1}, {0, -1}, {-1, 0}, {1, 0} };
float terrainStaminaCost[6] = {1.5, 1.0, 0.7, 0.7, 0.7, 10.0};

int directionMapping[4][4] = { {0, 1, 2, 3}, {1, 0, 3, 2}, {2, 3, 1, 0}, {3, 2, 0, 1}, };
/*
int getSpawnPointX(int index)
{
	int lenth = 1, walkLenth = 0, turnCount = 0, cDirc = 0;
	int dirc[4] = {1, 0, -1, 0};
	int x = 0;
	for (int i = 0; i < index; i++)
	{
		x += dirc[cDirc];
		walkLenth++;

		if (walkLenth >= lenth)
		{
			turnCount++;
			cDirc++;
			cDirc %= 4;

			if (turnCount % 2 == 0)
			{
				lenth += 1;
			}

			walkLenth = 0;
		}
	}

	return x;
}

int getSpawnPointY(int index)
{
	int lenth = 1, walkLenth = 0, turnCount = 0, cDirc = 0;
	int dirc[4] = { 0, 1, 0, -1 };
	int y = 0;
	for (int i = 0; i < index; i++)
	{
		y += dirc[cDirc];
		walkLenth++;

		if (walkLenth >= lenth)
		{
			turnCount++;
			cDirc++;
			cDirc %= 4;

			if (turnCount % 2 == 0)
			{
				lenth += 1;
			}

			walkLenth = 0;
		}
	}

	return y;
}
*/