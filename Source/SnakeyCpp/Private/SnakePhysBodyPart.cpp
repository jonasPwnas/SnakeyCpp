// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakePhysBodyPart.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"


// Cons cons constructor
ASnakePhysBodyPart::ASnakePhysBodyPart()
{
	PrimaryActorTick.bCanEverTick = false;
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	SetRootComponent(BodyMesh);
	
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BodyMesh->SetCollisionProfileName(TEXT("PhysicsActor"));
	
	BodyMeshConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("BodyConstraint"));
	BodyMeshConstraint->SetupAttachment(RootComponent);
}

void ASnakePhysBodyPart::SetupConstraint(float HalfSegmentLength)
{
	BodyMeshConstraint->SetRelativeLocation(FVector(HalfSegmentLength, 0.0f, 0.0f));
}

void ASnakePhysBodyPart::LinkToPreviousBody(UPrimitiveComponent* PrevBody)
{
	if (!PrevBody) return;
	
	BodyMeshConstraint->SetConstrainedComponents
	(
	PrevBody,	 NAME_None,
	BodyMesh, NAME_None
	);
	
	//BodyMesh->SetLinearDamping(1.0f);
	//BodyMesh->SetAngularDamping(1.0f);
	
	BodyMeshConstraint->SetDisableCollision(true);
	
	BodyMeshConstraint->SetLinearXLimit(LCM_Limited, 60.f);
	BodyMeshConstraint->SetLinearYLimit(LCM_Limited, 60.f);
	BodyMeshConstraint->SetLinearZLimit(LCM_Limited, 60.f);
	
	BodyMeshConstraint->SetAngularSwing1Limit(	ACM_Limited, 45.0f); //SwingLimitAngle);
	BodyMeshConstraint->SetAngularSwing2Limit(	ACM_Limited, 45.0f); //SwingLimitAngle);
	BodyMeshConstraint->SetAngularTwistLimit(	ACM_Limited,  45.0f); //wistLimitAngle);
	
	BodyMesh->SetSimulatePhysics(true);
}


