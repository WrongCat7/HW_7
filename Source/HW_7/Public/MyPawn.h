// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPawn.generated.h"

class UCapsuleComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class UInputActionValue;
class UFloatingPawnMovement;

UCLASS()
class HW_7_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn")
	UCapsuleComponent* rootcomp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn")
	USkeletalMeshComponent* meshcomp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn")
	USpringArmComponent* springarmcomp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn")
	UCameraComponent* cameracomp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	UFloatingPawnMovement* movementcomp;

	UFUNCTION()
	void StartMove(const FInputActionValue& value);
	UFUNCTION()
	void StopMove(const FInputActionValue& value);
	UFUNCTION()
	void StartLook(const FInputActionValue& value);
	UFUNCTION()
	void StopLook(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float jumpstrength = 0;
	float maxjumpstrength = 200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float speed = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float gravity = 500.0f;
	bool isjumping = false;
	bool islooking = false;
	bool ismoving = false;
	bool IsGrounded() const;
	float pitch = 0;
	FVector movedelta;
	FVector2D lookinput;
	FRotator yaw;
	FRotator Pitch;
	float mousesens = 100.0f;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
