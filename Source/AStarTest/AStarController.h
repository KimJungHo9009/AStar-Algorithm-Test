// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AStarController.generated.h"

/*
AStar 알고리즘

FCost : GCost와 HCost를 더한 값(시작점 부터 목표 노드 까지 필요한 총 비용)
GCost : 현재 지점과 시작 지점 사이의 비용(누적)
HCost : 현재 지점과 목표 지점 사이의 비용

1. 오픈노드 리스트와 클로즈노드 리스트를 준비
2. 시작 노드와 목표 노드 세팅, 시작 노드와 목표 노드 사이의 거리를 구한다.
3. 시작 노드를 현재 노드로 세팅한다.

4. 현재 노드의 주변 노드를 찾아 오픈노드 리스트에 넣는다.
5. 현재 노드를 오픈노드에서 빼고 클로즈노드 리스트에 넣는다.
6. 오픈 노드 리스트에 있는 노드들을 기준으로 Cost(GCost, HCost, FCost)를 구한다.
7. 각 노드들의 FCost 값을 비교하여 가장 적은 값을 가지느 노드를 현재 노드로 설정하고 4번 과정으로 되돌아간다.
   (FCost가 같을 경우 GCost를 비교한다.)

8. 목표 노드에 도착하면 종료

*/
class PathFinder {
private:
	PathFinder() {};
	~PathFinder() {};

	PathFinder operator=(PathFinder&) = delete;
	PathFinder(PathFinder&&) = delete;
	PathFinder(const PathFinder&) = delete;

	static PathFinder* PathFinderInstance;

public:
	static PathFinder* GetInstance() {
		if (PathFinderInstance == nullptr) {
			PathFinderInstance = new PathFinder();
		}
		return PathFinderInstance;
	}

public:
	TArray<class AAStarNode*> OpenNode;
	TArray<class AAStarNode*> CloseNode;
	TArray<class AAStarNode*> PathNode;

	class AAStarNode* StartNode;
	class AAStarNode* EndNode;

	class ANodeGrid* Grid;

public:
	int32 GetDistance(class AAStarNode* _A, class AAStarNode* _B);
	void PathFind(class AAStarNode* _Start, class AAStarNode* _End);
	TArray<AAStarNode*> RetraceNode(AAStarNode* _Current);

};
PathFinder* PathFinder::PathFinderInstance = nullptr;
/**
 * 
 */
UCLASS()
class ASTARTEST_API AAStarController : public APlayerController
{
	GENERATED_BODY()

public:
	AAStarController();
	virtual void BeginPlay() override;
	
	class AAStarNode* GetNode();

	void SetStartNode();
	void SetEndNode();
	void SetWall();

	void PathFind();
	void Reset();

public:
	class AAStarNode* StartNode;
	class AAStarNode* EndNode;
	class ANodeGrid* GridRef;
};
