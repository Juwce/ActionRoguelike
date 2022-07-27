// Fill out your copyright notice in the Description page of Project Settings.


#include "TCharacter.h"

#include "DrawDebugHelpers.h"
#include "TAttributeComponent.h"
#include "TProjectile_Dash.h"
#include "TInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
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

	ActionComponent = CreateDefaultSubobject<UTActionComponent>("ActionComponent");

	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	// Don't apply up/down rotation to the character when the camera rotates
	bUseControllerRotationYaw = false;

	HandSocketName = "Muzzle_01";
	TimeToHitMaterialParamName = "TimeToHit";
	HitFlashSpeedMaterialParamName = "HitFlashSpeed";
	HitFlashColorMaterialParamName = "HitFlashColor";
	
	ActionName_Sprint = "Sprint";
	ActionName_PrimaryAttack = "Attack_MagicProjectile";
	ActionName_SecondaryAttack = "Attack_Dash";
	ActionName_TertiaryAttack = "Attack_BlackHole";

	LifeSpanOnDeathSeconds = 5.f;
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
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this,  &ATCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this,  &ATCharacter::StopSprint);
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

void ATCharacter::StartSprint()
{
	ActionComponent->StartActionByName(this, ActionName_Sprint);
}

void ATCharacter::StopSprint()
{
	ActionComponent->StopActionByName(this, ActionName_Sprint);
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
	ActionComponent->StartActionByName(this, ActionName_PrimaryAttack);
}

void ATCharacter::SecondaryAttack()
{
	ActionComponent->StartActionByName(this, ActionName_SecondaryAttack);
}

void ATCharacter::TertiaryAttack()
{
	ActionComponent->StartActionByName(this, ActionName_TertiaryAttack);
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
	// die
	if (NewHealth <= 0.f && Delta < 0.f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
		SetLifeSpan(LifeSpanOnDeathSeconds);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	}

	if (Delta < 0.f)
	{
		TriggerHitFlashEffect();
	}
}

void ATCharacter::CheatHealSelf(float Amount)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}

void ATCharacter::CheatInvincible(bool Value)
{
	AttributeComp->bCheat_TakeNoDamage = Value;
	AttributeComp->bCheat_TakeAlmostNoDamage = false;
}

void ATCharacter::CheatInvincibleAlmost(bool Value)
{
	AttributeComp->bCheat_TakeAlmostNoDamage = Value;
	AttributeComp->bCheat_TakeNoDamage = false;
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

