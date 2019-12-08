// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AStarNode.generated.h"

UENUM()
enum class NodeType : uint8 {
	None,
	Wall,
};

UCLASS()
class ASTARTEST_API AAStarNode : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAStarNode();

	UFUNCTION()
	void NodeInit(int32 _Index, int32 _GridSize);

	UFUNCTION()
	void SetColor(FVector _Color);

	UFUNCTION()
	int32 GetFCost();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Mesh;

	AAStarNode* Parent;

	int32 NodeIndex;
	int32 Cols;
	int32 Rows;

	int32 FCost;
	int32 GCost;
	int32 HCost;

	NodeType Type;
};
