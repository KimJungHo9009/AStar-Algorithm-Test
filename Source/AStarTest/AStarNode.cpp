// Fill out your copyright notice in the Description page of Project Settings.


#include "AStarNode.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
AAStarNode::AAStarNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("MESH");
	SetRootComponent(Mesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultMesh(TEXT("/Engine/BasicShapes/Cube"));
	if (DefaultMesh.Succeeded()) {
		Mesh->SetStaticMesh(DefaultMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> DefaultMaterial(TEXT("/Game/Material/MAT_Normal_Inst"));
	
	//Mesh->CreateAndSetMaterialInstanceDynamicFromMaterial(0, DefaultMaterial.Object);
	Mesh->SetMaterial(0, DefaultMaterial.Object);
	
}

// Called when the game starts or when spawned
void AAStarNode::BeginPlay()
{
	Super::BeginPlay();
	//Mesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	//Mesh->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(1.f, 0.f, 0.f));
}

void AAStarNode::NodeInit(int32 _Index, int32 _GridSize)
{
	Parent = nullptr;
	FCost = 0;
	GCost = 0;
	HCost = 0;
	Rows = _Index / _GridSize;
	Cols = _Index % _GridSize;
	Type = NodeType::None;
	SetColor(FVector(0.f, 0.f, 0.f));
}

void AAStarNode::SetColor(FVector _Color)
{
	Mesh->SetVectorParameterValueOnMaterials(TEXT("Color"), _Color);
}

int32 AAStarNode::GetFCost()
{
	FCost = GCost + HCost;
	return FCost;
}

// Called every frame
void AAStarNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

