// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Sockets/Public/Sockets.h"
#include "Sockets/Public/SocketSubsystem.h"
#include "Runtime/Networking/Public/Common/UdpSocketBuilder.h"
#include "UdpSend.generated.h"

UCLASS()
class UDP_API AUdpSend : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUdpSend();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FSocket* SenderSocket;

	//‘∂≥Ãµÿ÷∑
	TSharedPtr<FInternetAddr> RemoteAddr;

	UFUNCTION(BlueprintCallable,Category = "UDP")
	bool StartUDPSender(const FString& YourChosenSocketName,const FString& TheIP,const int32 ThePort,bool UDP);
	bool IsUDP;

	UFUNCTION(BlueprintCallable,Category = "UDP")
	bool UDPSender_SendString(FString ToSend);

	UFUNCTION(BlueprintCallable,Category = "UDP")
	bool UDPSender_SendData(TArray<uint8>data);


};
