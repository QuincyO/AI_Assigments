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
	std::unordered_map<TileCoord, float, std::hash<TileCoord>, std::equal_to<TileCoord>> unvisited;
	std::unordered_map<TileCoord, float, std::hash<TileCoord>, std::equal_to<TileCoord>> visited;
	std::unordered_map<TileCoord, TileCoord, std::hash<TileCoord>, std::equal_to<TileCoord>> cheapestEdgeTo;

	TileCoord startNode;
	TileCoord goalNode;
	TileCoord currentNode;

	float TotalCost = 0;

public:
	Pathfinder() //Default constructor
	{

	}

	Pathfinder(Tilemap* levelToNavigate, TileCoord startTile, TileCoord endTile) //Constructor to set up new start conditions
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
		for (TileCoord position : map->GetAllTraversibleTiles())
		{
			unvisited[position] = INFINITY;
		}
		unvisited[startNode] = 0;
	}

	std::pair<TileCoord, float> GetLowestCostIn(std::unordered_map<TileCoord, float> set)
	{
		TileCoord cheapestPosition = { -1, -1 };
		float lowestCost = INFINITY;
		for (auto nodeValuePair : set)
		{
			TileCoord tile = nodeValuePair.first;
			float cost = nodeValuePair.second;

			if (cost < lowestCost)
			{
				cheapestPosition = tile;
				lowestCost = cost;
			}
		}
		return { cheapestPosition, lowestCost };
	}

	bool IsVisited(TileCoord pos) const { return visited.count(pos); }
	bool IsSolved() const { return IsVisited(goalNode); }
	bool IsCompleted() { return IsVisited(goalNode) || GetLowestCostIn(unvisited).second == INFINITY; }

	void MoveToVisitedSet(TileCoord node)
	{
		visited[currentNode] = unvisited[node];
		unvisited.erase(currentNode);
	}

	float GetTotalCostToReach(TileCoord pos)
	{
		float totalCost = 0.0f;
		TileCoord current = pos;
		while (current != TileCoord{ -1,-1 })
		{
			totalCost += 1.0f;
			auto it = cheapestEdgeTo.find(current);
			if (it != cheapestEdgeTo.end())
			{
				current = it->second;

			}
			else
			{
				current = TileCoord{ -1,-1 };
			}
		}
		return totalCost;

	}


	void SetCostToReach(TileCoord pos, float newCost) { unvisited[pos] = newCost; }

	float GetCostForTile(TileCoord tile) //CHECK THIS
	{
		return 1.0f;
	}

	void ProcessNextIterationFunctional()
	{
		if (IsCompleted()) return;

		currentNode = unvisited.begin()->first;
		auto Test = map->GetAllTraversibleTiles();
		unvisited.erase(currentNode);

		visited.insert(std::make_pair(currentNode, GetCostForTile(currentNode)));
		if (currentNode == goalNode)
		{
			return;
		}
		auto TEST2 = map->GetTraversibleTilesAdjacentTo(currentNode);
		for (auto adjacent : map->GetTraversibleTilesAdjacentTo(currentNode))
		{
			if (IsVisited(adjacent)) continue;
			float costThisWay = GetTotalCostToReach(currentNode) + 1;

			float oldCost = GetTotalCostToReach(adjacent);

			if (costThisWay < TotalCost)
			{
				cheapestEdgeTo.insert(std::make_pair(adjacent, currentNode));

			}

		}
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

	std::list<TileCoord> GetSolution()
	{
		std::list<TileCoord> solution;
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
		for (TileCoord tile : map->GetAllTraversibleTiles())
		{
			if (IsVisited(tile))
				DrawLine(map->GetTileCenter(tile).x, map->GetTileCenter(tile).y, 
					map->GetTileCenter(cheapestEdgeTo[tile]).x, map->GetTileCenter(cheapestEdgeTo[tile]).y, GREEN);
		}

		//Draw solution line
		for (TileCoord solution : GetSolution())
		{
			Vector2 start = map->GetTileCenter(solution);
			Vector2 end = map->GetTileCenter(cheapestEdgeTo[solution]);
			
			DrawLineEx(start, end, 10, BLUE);
		}

		//Draw costs for unvisited tiles
		for (TileCoord position : map->GetAllTraversibleTiles())
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
		for (TileCoord position : map->GetAllTraversibleTiles())
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


