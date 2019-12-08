// Fill out your copyright notice in the Description page of Project Settings.


#include "NodeGrid.h"
#include "AStarNode.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

// Sets default values
ANodeGrid::ANodeGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANodeGrid::BeginPlay()
{
	Super::BeginPlay();
	CreateGrid(8);
}

void ANodeGrid::CreateGrid(int32 _Size)
{
	GridSize = _Size;

	int32 MakeCount = 0;

	float Center = ((float)GridSize / 2);
	
	for (int32 row = 0; row < GridSize; ++row) {
		for (int32 col = 0; col < GridSize; ++col) {

			FActorSpawnParameters param;
			param.Owner = this;
			
			FVector NodePosition;
			NodePosition = FVector((row*100.f), (col*100.f), 0);

			AAStarNode* NewNode = GetWorld()->SpawnActor<AAStarNode>(AAStarNode::StaticClass(), NodePosition, FRotator::ZeroRotator, param);

			NewNode->NodeInit(MakeCount, GridSize);
			NodeArr.Add(NewNode);

			MakeCount++;
		}
	}
}

TArray<AAStarNode*> ANodeGrid::FindNeighborNode(AAStarNode * _CurrentNode)
{
	TArray<AAStarNode*> FoundNode;

	for (int32 row = -1; row <= 1; ++row) {
		for (int32 col = -1; col <= 1; ++col) {
			if ((row == 0 && col == 0)) { continue; }

			if (NodeCheck(_CurrentNode->Rows + row, _CurrentNode->Cols + col)) {
				int32 X = _CurrentNode->Rows + row;
				int32 Y = _CurrentNode->Cols + col;

				int32 FoundIndex = (GridSize * X) + Y;
				UE_LOG(LogTemp, Error, TEXT("Search Index : %d"), FoundIndex);
				FoundNode.Add(NodeArr[FoundIndex]);
			}
		}
	}

	return FoundNode;
}

bool ANodeGrid::NodeCheck(int32 _row, int32 _col)
{
	if (_row < 0 || _row >= GridSize) { return false; }
	if (_col < 0 || _col >= GridSize) { return false; }

	return true;
}

// Called every frame
void ANodeGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

