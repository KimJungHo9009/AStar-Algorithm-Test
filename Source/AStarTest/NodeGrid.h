// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NodeGrid.generated.h"

UCLASS()
class ASTARTEST_API ANodeGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANodeGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:

	UFUNCTION()
	void CreateGrid(int32 _Size);

	UFUNCTION()
	TArray<class AAStarNode*> FindNeighborNode(class AAStarNode* _CurrentNode);

	UFUNCTION()
	bool NodeCheck(int32 _row, int32 _col);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GridSize;

	TSubclassOf<class AAStarNode> NodeClass;
	TArray<class AAStarNode*> NodeArr;
};
