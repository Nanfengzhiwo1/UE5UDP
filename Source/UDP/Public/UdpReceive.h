// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Sockets/Public/Sockets.h"
#include "Sockets/Public/SocketSubsystem.h"
#include "Runtime/Networking/Public/Common/UdpSocketReceiver.h"
#include "Runtime/Networking/Public/Common/UdpSocketBuilder.h"
#include "Networking/Public/Interfaces/IPv4/IPv4Address.h"
#include "UdpReceive.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUdpByteMessageDelegate, const TArray<uint8>&, Message, const FString&, SenderIp);


UCLASS()
class UDP_API AUdpReceive : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUdpReceive();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	FSocket* ListenSocket;
	FUdpSocketReceiver* Receiver = nullptr;

public:
	UFUNCTION(BlueprintCallable,Category = "UDP")
	void StartUDPReceiver(const FString& YourChonsenSocketName,const FString& TheIP,const int32 ThePort,bool& success);

	UFUNCTION(BlueprintCallable,Category = "UDP")
	void DataRecv(FString& str,bool& success);

	UFUNCTION(BlueprintCallable,Category = "UDP")
	TArray<uint8> DataRecvBytes(bool& success);

	UPROPERTY(BlueprintAssignable,Category = "Socket|Event")
	FOnUdpByteMessageDelegate OnByteMessage;
};
