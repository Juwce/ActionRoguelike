// Fill out your copyright notice in the Description page of Project Settings.


#include "TCharacter.h"

#include "DrawDebugHelpers.h"
#include "TAttributeComponent.h"
#include "TProjectile_Dash.h"
#include "TInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ATCharacter::ATCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bDrawDebugArrows = false;

	/*
	 * Camera Setup
	 */
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);
	
	InteractionComp = CreateDefaultSubobject<UTInteractionComponent>("InteractionComp");

	AttributeComp = CreateDefaultSubobject<UTAttributeComponent>("AttributeComp");

	MaxAttackTraceDistance = 3000.f;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	// Don't apply up/down rotation to the character when the camera rotates
	bUseControllerRotationYaw = false;

	HandSocketName = "Muzzle_01";
	TimeToHitMaterialParamName = "TimeToHit";
	HitFlashSpeedMaterialParamName = "HitFlashSpeed";
	HitFlashColorMaterialParamName = "HitFlashColor";
}

// Called every frame
void ATCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bDrawDebugArrows) { DrawDebugArrows(); }
}

void ATCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ATCharacter::OnHealthChanged);
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
	Attack_StartTimer(PrimaryProjectileClass);
}

void ATCharacter::SecondaryAttack()
{
	Attack_StartTimer(SecondaryProjectileClass);
}

void ATCharacter::TertiaryAttack()
{
	Attack_StartTimer(TertiaryProjectileClass);
}

void ATCharacter::Attack_StartTimer(const TSubclassOf<ATProjectileBase>& ProjectileClass)
{
	PlayAnimMontage(AttackAnim);
	
	FTimerDelegate TimerDel;
	TimerDel.BindLambda([&]() { Attack_TimeElapsed(ProjectileClass); });
	GetWorldTimerManager().SetTimer(TimerHandle_Attack, TimerDel, 0.2f, false);
}

void ATCharacter::Attack_TimeElapsed(const TSubclassOf<ATProjectileBase>& ProjectileClass)
{
	if (ensure(ProjectileClass))
	{
		const FVector HandLocation = GetMesh()->GetSocketLocation(HandSocketName);

		FVector Target;
		ComputeAttackTarget(Target);

		const FRotator LookAtRotation = FRotationMatrix::MakeFromX(Target - HandLocation).Rotator();
		const FTransform SpawnTM = FTransform(LookAtRotation, HandLocation);

		FActorSpawnParameters SpawnParams;
		// ignore collision checks when spawning
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		GetWorld()->SpawnActor<ATProjectileBase>(ProjectileClass, SpawnTM, SpawnParams);
		UGameplayStatics::SpawnEmitterAttached(SpellCastVFX, GetMesh(), HandSocketName);
	}
}

// Target the first thing some distance in front of the camera. If we find nothing, target the max distance
bool ATCharacter::ComputeAttackTarget(FVector& TargetLocation)
{
	const FVector CameraLocation = CameraComp->GetComponentLocation();
	const FVector ControlRotation = GetControlRotation().Vector();
	const FVector TraceEnd = CameraLocation + (ControlRotation * MaxAttackTraceDistance);
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	
	FHitResult Hit;
	bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(
		Hit, CameraLocation, TraceEnd, ObjectQueryParams, CollisionQueryParams);

	TargetLocation = bBlockingHit ? Hit.Location : TraceEnd;
	return bBlockingHit;
}

void ATCharacter::TriggerHitFlashEffect()
{
	GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitMaterialParamName, GetWorld()->TimeSeconds);
	GetMesh()->SetScalarParameterValueOnMaterials(HitFlashSpeedMaterialParamName, HitFlashSpeed);
	GetMesh()->SetVectorParameterValueOnMaterials(HitFlashColorMaterialParamName,
	                                              FVector(HitFlashColor.R, HitFlashColor.G, HitFlashColor.B));
}

void ATCharacter::OnHealthChanged(AActor* InstigatorActor, UTAttributeComponent* OwningComp, float NewHealth,
                                  float Delta)
{
	if (NewHealth <= 0.f && Delta < 0.f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
	}

	if (Delta < 0.f)
	{
		TriggerHitFlashEffect();
	}
}

void ATCharacter::DrawDebugArrows() const
{
	const float DrawScale = 100.f;
	const float Thickness = 3.f;
	
	// Offset to the right of the pawn
	const FVector LineStart = GetActorLocation() + FVector(100.f, 0.f, -30);

	// Draw Actor's Direction
	const FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 150.f);
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow, false, 0.f, 0, Thickness);

	// Draw 'Controller' Rotation
	const FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 150.f);
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.f, 0, Thickness);
}

