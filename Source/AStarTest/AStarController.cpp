// Fill out your copyright notice in the Description page of Project Settings.


#include "AStarController.h"
#include "AStarNode.h"
#include "NodeGrid.h"
#include "Kismet/GameplayStatics.h"

AAStarController::AAStarController() {
	bShowMouseCursor = true;
	bEnableClickEvents = true;
}

void AAStarController::BeginPlay()
{
	Super::BeginPlay();

	GridRef = Cast<ANodeGrid>(UGameplayStatics::GetActorOfClass(this, ANodeGrid::StaticClass()));
	PathFinder::GetInstance()->Grid = GridRef;

	EnableInput(this);
	/*
	Left Click : Set StartNode
	Right click : Set EndNode
	Shift + Left Click : Switching NodeType Wall, None
	Enter : Run Path Find
	C : Node Reset
	*/
	InputComponent->BindAction("SetStartNode", IE_Pressed, this, &AAStarController::SetStartNode);
	InputComponent->BindAction("SetEndNode", IE_Pressed, this, &AAStarController::SetEndNode);
	InputComponent->BindAction("SetWall", IE_Pressed, this, &AAStarController::SetWall);
	InputComponent->BindAction("FindStart", IE_Pressed, this, &AAStarController::PathFind);
	InputComponent->BindAction("Reset", IE_Pressed, this, &AAStarController::Reset);
}

AAStarNode * AAStarController::GetNode()
{
	FHitResult HitTemp;
	GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, HitTemp);

	AAStarNode* NodeTemp = nullptr;

	if (HitTemp.GetActor()) {
		UE_LOG(LogTemp, Error, TEXT("%s"), *HitTemp.GetActor()->GetName());
		NodeTemp = Cast<AAStarNode>(HitTemp.GetActor());
	}
	return NodeTemp;
}

void AAStarController::SetStartNode()
{
	auto Temp = GetNode();
	if (Temp == nullptr) { return; }

	// 스타트 노드 설정 시 해당 노드가 엔드노드라면 해제
	if (EndNode != nullptr && Temp == EndNode) {
		EndNode->SetColor(FVector(0.f, 0.f, 0.f));
		EndNode = nullptr;
	}

	// 스타트 노드가 이미 존재 한다면 기존 스타트 해제, 이미 스타트 노드인 노드를 선택시 선택 해제
	if (StartNode != nullptr) { 
		StartNode->SetColor(FVector(0.f, 0.f, 0.f)); 

		if (StartNode == Temp) {
			StartNode = nullptr;
			return;
		}
	}

	StartNode = Temp;
	StartNode->SetColor(FVector(0.f, 1.f, 0.f));
}

void AAStarController::SetEndNode()
{
	auto Temp = GetNode();
	if (Temp == nullptr) { return; }

	// 엔드 노드 설정 시 해당 노드가 스타트 노드라면 해제
	if (StartNode != nullptr && Temp == StartNode) {
		StartNode->SetColor(FVector(0.f, 0.f, 0.f));
		StartNode = nullptr;
	}

	if (EndNode != nullptr) {
		EndNode->SetColor(FVector(0.f, 0.f, 0.f)); 
		if (EndNode == Temp) {
			EndNode = nullptr;
			return;
		}
	}

	EndNode = Temp;
	EndNode->SetColor(FVector(0.f, 0.f, 1.f));
}

void AAStarController::SetWall()
{
	auto Temp = GetNode();

	if (Temp->Type == NodeType::None) {
		if (StartNode != nullptr && Temp == StartNode) {
			StartNode = nullptr;
		}else if (EndNode != nullptr && Temp == EndNode) {
			EndNode = nullptr;
		}

		Temp->Type = NodeType::Wall;
		Temp->SetColor(FVector(0.5f, 0.5f, 0.5f));
	}
	else {
		Temp->Type = NodeType::None;
		Temp->SetColor(FVector(0.f, 0.f, 0.f));
	}	
}

void AAStarController::PathFind()
{
	if (StartNode == nullptr || EndNode == nullptr) { 
		UE_LOG(LogTemp, Error, TEXT("Please Set StartNode & EndNode"));
		return;
	}

	UE_LOG(LogTemp, Error, TEXT("Run PathFind"));
	PathFinder::GetInstance()->PathFind(StartNode, EndNode);
}

void AAStarController::Reset()
{
	StartNode = nullptr;
	EndNode = nullptr;
	
	for (int32 NodeIndex = 0; NodeIndex < GridRef->NodeArr.Num(); NodeIndex++) {
		GridRef->NodeArr[NodeIndex]->SetColor(FVector(0.f, 0.f, 0.f));
		GridRef->NodeArr[NodeIndex]->Type = NodeType::None;
	}	
}


int32 PathFinder::GetDistance(AAStarNode * _A, AAStarNode * _B)
{
	int32 DisX = FMath::Abs(_A->Cols - _B->Cols);
	int32 DisY = FMath::Abs(_A->Rows - _B->Rows);

	return (14 * FMath::Min(DisX, DisY) + 10 * FMath::Abs(DisX - DisY));
}

void PathFinder::PathFind(AAStarNode * _Start, AAStarNode * _End)
{
	OpenNode.Empty();
	CloseNode.Empty();

	StartNode = _Start;
	EndNode = _End;
	StartNode->Parent = nullptr;
	EndNode->Parent = nullptr;

	AAStarNode* CurrentNode = StartNode;

	StartNode->GCost = 0;
	StartNode->HCost = GetDistance(StartNode, EndNode);

	do {

		TArray<AAStarNode*> NeighborNode =  Grid->FindNeighborNode(CurrentNode);
		UE_LOG(LogTemp, Error, TEXT("NeighborNode Num : %d"), NeighborNode.Num());

		for (int32 i = 0; i < NeighborNode.Num(); ++i) {

			if (CloseNode.Find(NeighborNode[i]) != -1) { continue; }
			if (NeighborNode[i]->Type == NodeType::Wall) { continue; }

			int32 _GCost = CurrentNode->GCost + GetDistance(NeighborNode[i], CurrentNode);
			

			if (OpenNode.Find(NeighborNode[i]) == -1 ||
				_GCost < NeighborNode[i]->GCost) {

				int32 _HCost = GetDistance(NeighborNode[i], EndNode);
				NeighborNode[i]->GCost = _GCost;
				NeighborNode[i]->HCost = _HCost;
				NeighborNode[i]->Parent = CurrentNode;

				UE_LOG(LogTemp, Error, TEXT("Node Coord : %d, %d"), NeighborNode[i]->Rows, NeighborNode[i]->Cols);
				UE_LOG(LogTemp, Error, TEXT("_GCost : %d"), _GCost);
				UE_LOG(LogTemp, Error, TEXT("_HCost : %d"), _HCost);

				if (OpenNode.Find(NeighborNode[i]) == -1) {
					OpenNode.Add(NeighborNode[i]);
					UE_LOG(LogTemp, Error, TEXT("OpenNode Num : %d"), OpenNode.Num());
				}
			}
		}

		CloseNode.Add(CurrentNode);
		if (OpenNode.Find(CurrentNode) != -1) {
			OpenNode.Remove(CurrentNode);
		}

		if (OpenNode.Num() > 0) {
			UE_LOG(LogTemp, Error, TEXT("Min Cost Check"));
			AAStarNode* MinCostNode = OpenNode[0];
			MinCostNode->GetFCost();

			for (int32 k = 1; k < OpenNode.Num(); ++k) {
				OpenNode[k]->GetFCost();

				if (MinCostNode->FCost > OpenNode[k]->FCost) {
					MinCostNode = OpenNode[k];
				}
				else if (MinCostNode->FCost == OpenNode[k]->FCost) {
					if (MinCostNode->HCost > OpenNode[k]->HCost) {
						MinCostNode = OpenNode[k];
					}
				}				
			}

			CurrentNode = MinCostNode;
		}

		if (CurrentNode == EndNode) {
			UE_LOG(LogTemp, Error, TEXT("Found Path"));

			PathNode = RetraceNode(CurrentNode);
			for (AAStarNode* Node : PathNode) {
				Node->SetColor(FVector(1.f, 0.f, 1.f));
			}

		}

	} while (OpenNode.Num() > 0);
}

TArray<AAStarNode*> PathFinder::RetraceNode(AAStarNode * _Current)
{
	TArray<AAStarNode*> PathArr;
	AAStarNode* TempNode = _Current;

	while (TempNode != nullptr) {
		PathArr.Add(TempNode);
		TempNode = TempNode->Parent;
	}

	return PathArr;
}
