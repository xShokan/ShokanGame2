// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Sound/SoundBase.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "Components/SceneComponent.h"
#include "TimerManager.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->bUsePawnControlRotation = true;						// 镜头上下移动
	SpringArm->bEnableCameraLag = true;

	TppCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TppCamera"));
	TppCamera->SetupAttachment(SpringArm);

	GunWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunWeapon"));
	GunWeapon->SetupAttachment(GetMesh(), TEXT("weapon"));

	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);  // 镜头移动速度
	GetCharacterMovement()->bOrientRotationToMovement = true;  // 移动以键盘朝向为准
	bUseControllerRotationYaw = false;   // 旋转镜头人物不旋转

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;  // 允许人物蹲
	GetCharacterMovement()->MaxWalkSpeed = 270;  // 最大走路速度
	GetCharacterMovement()->MaxWalkSpeedCrouched = 170;  // 最大蹲走速度

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!isHip)
	{
		FVector FireStart = TppCamera->GetComponentLocation();
		FVector FireEnd = TppCamera->GetForwardVector() * 5000 + FireStart;

		TArray<AActor*> ActorsToIgnore;
		bool ishit = UKismetSystemLibrary::LineTraceSingle(this, FireStart, FireEnd, ETraceTypeQuery::TraceTypeQuery2, false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, OutHit, true);
		
		// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, *OutHit.GetComponent()->GetName());
		
	
	}
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this , &AMyCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this , &AMyCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this , &AMyCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed ,this , &AMyCharacter::CrouchDown);
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Released , this , &AMyCharacter::CrouchUp);
	PlayerInputComponent->BindAction(TEXT("Ironsight"), IE_Pressed, this, &AMyCharacter::IronsightDown);
	PlayerInputComponent->BindAction(TEXT("Ironsight"), IE_Released, this, &AMyCharacter::IronsightUp);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AMyCharacter::FireDown);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &AMyCharacter::FireUp);
}

void AMyCharacter::MoveForward(float Value)
{
	AddMovementInput(UKismetMathLibrary::GetForwardVector(GetControlRotation()), Value);
}

void AMyCharacter::MoveRight(float Value)
{
	AddMovementInput(UKismetMathLibrary::GetRightVector(GetControlRotation()), Value);
}

void AMyCharacter::CrouchDown()
{
	Crouch();
}

void AMyCharacter::CrouchUp()
{
	UnCrouch();
}

void AMyCharacter::IronsightDown()
{
	isHip = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;  // 移动以人物朝向为准
	GetCharacterMovement()->bUseControllerDesiredRotation = true;  // 人物随镜头延迟移动
	SpringArm->SetRelativeLocation(FVector(10, 100, 70));  // 设置弹簧臂位置
	SpringArm->TargetArmLength = 200; // 设置相机镜头
}

void AMyCharacter::IronsightUp()
{
	isHip = true;	
	GetCharacterMovement()->bOrientRotationToMovement = true;  // 移动以键盘朝向为准
	GetCharacterMovement()->bUseControllerDesiredRotation = false;  // 人物随镜头延迟移动
	SpringArm->SetRelativeLocation(FVector(10, 10, 70));  // 设置弹簧臂位置
	SpringArm->TargetArmLength = 300; // 设置相机镜头
}

void AMyCharacter::Fire()
{
	if (!isHip)
	{
		PlayAnimMontage(FireMontage);
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());

		/*FVector FireStart = GunWeapon->GetSocketLocation(TEXT("Muzzle"));
		FVector FireEnd = TppCamera->GetForwardVector() * 5000 + FireStart;
		FHitResult OutHit;
		TArray<AActor*> ActorsToIgnore;
		bool ishit = UKismetSystemLibrary::LineTraceSingle(this, FireStart, FireEnd, ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHit, true);*/

		if (OutHit.GetComponent()->GetName() == "Cube")
		{
			// GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, FString::FromInt(Score));
			Score++;
		}

	}
}

void AMyCharacter::FireDown()
{
	GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &AMyCharacter::Fire, 0.1f, true);
	// Fire();
}

void AMyCharacter::FireUp()
{
	GetWorld()->GetTimerManager().ClearTimer(FireTimer);
}
