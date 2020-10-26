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
		class USpringArmComponent* SpringArm;						// ���嵯�ɱ�

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UCameraComponent* TppCamera;							// ���������

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USkeletalMeshComponent* GunWeapon;							// ��������

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool isHip = true;																			// �����Ƿ���׼

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* FireMontage;												// ����Montage

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundBase* FireSound;														// ���忪������

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Score = 0;														// ����÷�
	
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
