// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPawn.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include "MyPlayerController.h"
#include "GameFramework/FloatingPawnMovement.h"


// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	rootcomp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("rootcomp"));
	meshcomp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("meshcomp"));
	meshcomp->SetupAttachment(rootcomp);
	springarmcomp = CreateDefaultSubobject<USpringArmComponent>(TEXT("springarmcomp"));
	springarmcomp->SetupAttachment(rootcomp);
	springarmcomp->TargetArmLength = 300.0f;
	springarmcomp->bUsePawnControlRotation = false;
	springarmcomp->bInheritYaw = true;
	cameracomp = CreateDefaultSubobject<UCameraComponent>(TEXT("cameracomp"));
	cameracomp->SetupAttachment(springarmcomp);
	///Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> meshasset(TEXT("/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny"));
	if (meshasset.Succeeded()) {
		meshcomp->SetSkeletalMesh(meshasset.Object);
	}
	movementcomp = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("movementcomp"));
	rootcomp->SetSimulatePhysics(false);
	meshcomp->SetSimulatePhysics(false);
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (ismoving) AddActorLocalOffset(movedelta * speed * DeltaTime);
	if (islooking) AddActorLocalRotation(yaw * DeltaTime * mousesens);
	if (!IsGrounded()) {
			jumpstrength -= gravity * DeltaTime; // 델타타임 안곱하면 프레임마다 gravity 빼지고, 곱하면 1초마다 gravity가 빼진다.
			FVector z(0.0f, 0.0f, jumpstrength * DeltaTime); // 속도 * 시간
			AddActorLocalOffset(z, true);
	}
	else jumpstrength = 0;
	Pitch = FRotator(pitch * DeltaTime * mousesens, 0.0f, 0.0f);
	if (!Pitch.IsZero()) springarmcomp->SetRelativeRotation(Pitch);
}
void AMyPawn::StartMove(const FInputActionValue& value) {
	if (!Controller) return;

	const FVector2D movementvector = value.Get<FVector2D>();
	movedelta = FVector(movementvector.X, movementvector.Y, 0.0f);
	ismoving = true;
}
void AMyPawn::StopMove(const FInputActionValue& value) {
		ismoving = false;
}

void AMyPawn::StartLook(const FInputActionValue& value) {
	lookinput = value.Get<FVector2D>();
	yaw = FRotator(0.0f, lookinput.X, 0.0f);
	if (pitch < 80.0f && pitch > -90.0f)pitch += lookinput.Y;
	else if (pitch > 80.0f) pitch -= 1.0f;
	else if (pitch < -90.0f) pitch += 1.0f;
	islooking = true;
}
void AMyPawn::StopLook(const FInputActionValue& value) {
	islooking = false;
}
void AMyPawn::StartJump(const FInputActionValue& value) {
	if (value.Get<bool>()) {
		if (IsGrounded() && movementcomp) {
			jumpstrength = maxjumpstrength;
			FVector jumpvelocity = FVector(0.0f, 0.0f, jumpstrength);
			movementcomp->Velocity += jumpvelocity;

			isjumping = true;
		}
	}
}
void AMyPawn::StopJump(const FInputActionValue& value) {
	if (value.Get<bool>()) {
		isjumping = false;
	}
}
bool AMyPawn::IsGrounded() const {
	FVector start = GetActorLocation(); //레이 시작점
	FVector end = start - FVector(0.0f, 0.0f, 93.0f); // 레이 끝점 / 기준좌표 - 아래로
	FHitResult hit; // LineTrace에서 충돌 정보를 받을 구조체

	FCollisionQueryParams params; //레이/충돌 검사 설정
	params.AddIgnoredActor(this); //자기 자신은 검사에서 무시

	return GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Visibility, params);
}
// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* enhancedinput = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		if (AMyPlayerController* playercontroller = Cast<AMyPlayerController>(GetController())) {
			if (playercontroller->IAmove) {
				enhancedinput->BindAction(
					playercontroller->IAmove,
					ETriggerEvent::Triggered,
					this,
					&AMyPawn::StartMove
				);
				enhancedinput->BindAction(
					playercontroller->IAmove,
					ETriggerEvent::Completed,
					this,
					&AMyPawn::StopMove
				);

				enhancedinput->BindAction(
					playercontroller->IAjump,
					ETriggerEvent::Triggered,
					this,
					&AMyPawn::StartJump
				);
				enhancedinput->BindAction(playercontroller->IAsprint,
					ETriggerEvent::Completed,
					this,
					&AMyPawn::StopJump
				);
				enhancedinput->BindAction(
					playercontroller->IAlook,
					ETriggerEvent::Triggered,
					this,
					&AMyPawn::StartLook
				);
				enhancedinput->BindAction(
					playercontroller->IAlook,
					ETriggerEvent::Completed,
					this,
					&AMyPawn::StopLook
				);
			}
		}
		
	}
}

