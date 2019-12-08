// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AStarController.generated.h"

/*
AStar �˰���

FCost : GCost�� HCost�� ���� ��(������ ���� ��ǥ ��� ���� �ʿ��� �� ���)
GCost : ���� ������ ���� ���� ������ ���(����)
HCost : ���� ������ ��ǥ ���� ������ ���

1. ���³�� ����Ʈ�� Ŭ������ ����Ʈ�� �غ�
2. ���� ���� ��ǥ ��� ����, ���� ���� ��ǥ ��� ������ �Ÿ��� ���Ѵ�.
3. ���� ��带 ���� ���� �����Ѵ�.

4. ���� ����� �ֺ� ��带 ã�� ���³�� ����Ʈ�� �ִ´�.
5. ���� ��带 ���³�忡�� ���� Ŭ������ ����Ʈ�� �ִ´�.
6. ���� ��� ����Ʈ�� �ִ� ������ �������� Cost(GCost, HCost, FCost)�� ���Ѵ�.
7. �� ������ FCost ���� ���Ͽ� ���� ���� ���� ������ ��带 ���� ���� �����ϰ� 4�� �������� �ǵ��ư���.
   (FCost�� ���� ��� GCost�� ���Ѵ�.)

8. ��ǥ ��忡 �����ϸ� ����

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
