// Fill out your copyright notice in the Description page of Project Settings.


#include "TCharacter.h"

#include "DrawDebugHelpers.h"
#include "TDashProjectile.h"
#include "TInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ATCharacter::ATCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bDrawDebugArrows = false;

	/*
	 * Camera
	 */
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	// Rotate the camera boom with the controller
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	/*
	 * Interaction
	 */
	InteractionComp = CreateDefaultSubobject<UTInteractionComponent>("InteractionComp");
	SetTickGroup(ETickingGroup::TG_PostUpdateWork); // ensure camera is updated before we tick

	MaxAttackTraceDistance = 3000.f;

	/*
	 * Movement Controller
	 */
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// Don't move the character up and down when the camera rotates
	bUseControllerRotationYaw = false;
}

// Called every frame
void ATCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bDrawDebugArrows) { DrawDebugArrows(); }

}

// Called to bind functionality to input
void ATCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATCharacter::MoveRight);
	PlayerInputComponent->BindAxis("TurnLeft", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this,  &ATCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("SecondaryAttack", IE_Pressed, this,  &ATCharacter::SecondaryAttack);
	PlayerInputComponent->BindAction("TertiaryAttack", IE_Pressed, this,  &ATCharacter::TertiaryAttack);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, InteractionComp, &UTInteractionComponent::PrimaryInteract);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this,  &ACharacter::Jump);
}

// Called when the game starts or when spawned
void ATCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ATCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.f;
	ControlRot.Roll = 0.f;
	
	AddMovementInput(ControlRot.Vector(), Value);
}

void ATCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	
	ControlRot.Pitch = 0.f;
	ControlRot.Roll = 0.f;

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	
	AddMovementInput(RightVector, Value);
}

void ATCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

void ATCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);
	Attack_StartTimer(PrimaryProjectileClass);
}

void ATCharacter::SecondaryAttack()
{
	PlayAnimMontage(AttackAnim);
	Attack_StartTimer(SecondaryProjectileClass);
}

void ATCharacter::TertiaryAttack()
{
	PlayAnimMontage(AttackAnim);
	Attack_StartTimer(TertiaryProjectileClass);
}

void ATCharacter::Attack_StartTimer(const TSubclassOf<ATProjectile>& ProjectileClass)
{
	FTimerDelegate TimerDel;
	TimerDel.BindLambda([&]() {
		Attack_TimeElapsed(ProjectileClass);
	});
	GetWorldTimerManager().SetTimer(TimerHandle_Attack, TimerDel, 0.2f, false);
}

void ATCharacter::Attack_TimeElapsed(const TSubclassOf<ATProjectile>& ProjectileClass)
{
	const FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	FVector Target;
	ComputeAttackTarget(Target);

	const FRotator LookAtRotation = FRotationMatrix::MakeFromX(Target - HandLocation).Rotator();
	const FTransform SpawnTM = FTransform(LookAtRotation, HandLocation);

	FActorSpawnParameters SpawnParams;
	// ignore spawning the projectile
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

 	GetWorld()->SpawnActor<ATProjectile>(ProjectileClass, SpawnTM, SpawnParams);
}

// Target the first thing some distance in front of the camera. If we find nothing, target the max distance
bool ATCharacter::ComputeAttackTarget(FVector& TargetLocation)
{
	const FVector CameraLocation = CameraComp->GetComponentLocation();
	const FVector CameraForward = CameraComp->GetForwardVector();
	const FVector TraceEnd = CameraLocation + (CameraForward * MaxAttackTraceDistance);
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	
	FHitResult Hit;
	bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(
		Hit, CameraLocation, TraceEnd, ObjectQueryParams, CollisionQueryParams);

	TargetLocation = bBlockingHit ? Hit.Location : TraceEnd;

	return bBlockingHit;
}

void ATCharacter::DrawDebugArrows()
{
	const float DrawScale = 100.f;
	const float Thickness = 5.f;
	
	// Offset to the right of the pawn
	FVector LineStart = GetActorLocation() + (GetActorRightVector() * 100.f);

	// Draw Actor's Direction
	FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.f);
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow, false, 0.f, 0, Thickness);

	// Draw 'Controller' Rotation
	FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.f);
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.f, 0, Thickness);
}

