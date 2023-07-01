#pragma once
#include "TileCoord.h"
#include "TileMap.h"
#include <unordered_map>
#include <string>

class Pathfinder
{
public:
	Tilemap* map = nullptr;
private:
	std::unordered_map<Vector2, float,		std::hash<Vector2>, std::equal_to<Vector2>> unvisited;
	std::unordered_map<Vector2, float,		std::hash<Vector2>, std::equal_to<Vector2>> visited;
	std::unordered_map<Vector2, Vector2,	std::hash<Vector2>, std::equal_to<Vector2>> cheapestEdgeTo;

	Vector2 startNode;
	Vector2 goalNode;
	Vector2 currentNode;

public:
	Pathfinder() //Default constructor
	{

	}

	Pathfinder(Tilemap* levelToNavigate, Vector2 startTile, Vector2 endTile) //Constructor to set up new start conditions
	{
		map = levelToNavigate;
		startNode = startTile;
		goalNode = endTile;
		currentNode = startNode;
		Restart();
	}

	void Restart() //Resets pathfinding process with existing start and goal
	{
		visited.clear();
		cheapestEdgeTo.clear();

		//Reset costs
		unvisited.clear();
		for (Vector2 position : map->GetAllTraversibleTiles())
		{
			unvisited[position] = INFINITY;
		}
		unvisited[startNode] = 0;
	}

	std::pair<Vector2, float> GetLowestCostIn(std::unordered_map<Vector2, float> set)
	{
		Vector2 cheapestPosition = { -1, -1 };
		float lowestCost = INFINITY;
		for (auto nodeValuePair : set)
		{
			Vector2 tile = nodeValuePair.first;
			float cost = nodeValuePair.second;

			if (cost < lowestCost)
			{
				cheapestPosition = tile;
				lowestCost = cost;
			}
		}
		return { cheapestPosition, lowestCost };
	}

	bool IsVisited(Vector2 pos) const { return visited.count(pos); }
	bool IsSolved() const { return IsVisited(goalNode); }
	bool IsCompleted() { return IsVisited(goalNode) || GetLowestCostIn(unvisited).second == INFINITY; }

	void MoveToVisitedSet(Vector2 node)
	{
		visited[currentNode] = unvisited[currentNode];
		unvisited.erase(currentNode);
	}

	float GetTotalCostToReach(Vector2 pos) { return unvisited[pos]; }
	void SetCostToReach(Vector2 pos, float newCost) { unvisited[pos] = newCost; }

	float GetCostForTile(Vector2 tile) //CHECK THIS
	{
		return 1.0f;
	}

	void ProcessNextIterationFunctional()
	{
		if (IsCompleted()) return;
		currentNode = GetLowestCostIn(unvisited).first;

		for (auto adjacent : map->GetTraversibleTilesAdjacentTo(currentNode))
		{
			if (IsVisited(adjacent)) continue;

			float costThisWay = GetTotalCostToReach(currentNode) + 1.0f /*GetCostForTile(adjacent)*/; // Plus 1

			float oldCost = GetTotalCostToReach(adjacent);
			if (costThisWay < oldCost)
			{
				SetCostToReach(adjacent, costThisWay);
				cheapestEdgeTo[adjacent] = currentNode;
			}
		}
		
		MoveToVisitedSet(currentNode);
	}

	bool SolvePath() //Runs full algorithm until completion
	{
		Restart();
		while (!IsCompleted())
		{
			ProcessNextIterationFunctional();
		}

		return IsSolved();
	}

	std::list<Vector2> GetSolution()
	{
		std::list<Vector2> solution;
		auto currentNode = goalNode;
		while (currentNode != startNode)
		{
			solution.push_front(currentNode);
			currentNode = cheapestEdgeTo[currentNode];
		}
		return solution;
	}

	void DrawCurrentState()
	{
		//Draw outlines for tiles of interest
		Rectangle recStart = { startNode.x * map->tileSizeX, startNode.y * map->tileSizeY, map->tileSizeX, map->tileSizeY };
		Rectangle recGoal = { goalNode.x * map->tileSizeX, goalNode.y * map->tileSizeY, map->tileSizeX, map->tileSizeY };
		Rectangle recCurrent = { currentNode.x * map->tileSizeX, currentNode.y * map->tileSizeY, map->tileSizeX, map->tileSizeY };
		DrawRectangleLinesEx(recCurrent, 10, BLUE);
		DrawRectangleLinesEx(recStart, 10, GREEN);
		DrawRectangleLinesEx(recGoal, 10, RED);

		//Draw lines to visited nodes
		for (Vector2 tile : map->GetAllTraversibleTiles())
		{
			if (IsVisited(tile))
				DrawLine(map->GetTileCenter(tile).x, map->GetTileCenter(tile).y, 
					map->GetTileCenter(cheapestEdgeTo[tile]).x, map->GetTileCenter(cheapestEdgeTo[tile]).y, GREEN);
		}

		//Draw solution line
		for (Vector2 solution : GetSolution())
		{
			Vector2 start = map->GetTileCenter(solution);
			Vector2 end = map->GetTileCenter(cheapestEdgeTo[solution]);
			
			DrawLineEx(start, end, 10, BLUE);
		}

		//Draw costs for unvisited tiles
		for (Vector2 position : map->GetAllTraversibleTiles())
		{
			if (!IsVisited(position))
			{
				float cost = unvisited[position];
				
				char buffer[30];
				sprintf(buffer, "%u", (int)cost);

				DrawText(buffer, position.x * map->tileSizeX, position.y * map->tileSizeY, 40, GRAY);
			}
		}

		//Draw costs for visited tiles
		for (Vector2 position : map->GetAllTraversibleTiles())
		{
			if (IsVisited(position))
			{
				float cost = visited[position];

				char buffer[30];
				sprintf(buffer, "%u", (int)cost);

				DrawText(buffer, position.x * map->tileSizeX, position.y * map->tileSizeY, 40, GREEN);
			}
		}
	}

};


