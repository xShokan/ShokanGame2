// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class SHOKAN_GAME2_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USpringArmComponent* SpringArm;						// 定义弹簧臂

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UCameraComponent* TppCamera;							// 定义摄像机

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USkeletalMeshComponent* GunWeapon;							// 定义武器

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool isHip = true;																			// 定义是否瞄准

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* FireMontage;												// 定义Montage

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundBase* FireSound;														// 定义开火声音

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Score = 0;														// 定义得分
	
	void MoveForward(float Value);
	void MoveRight(float Value);

	void CrouchDown();
	void CrouchUp();

	void IronsightDown();
	void IronsightUp();

	void Fire();
	void FireDown();
	void FireUp();

	FHitResult OutHit;

	FTimerHandle FireTimer;
};
