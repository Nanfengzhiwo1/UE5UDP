// Fill out your copyright notice in the Description page of Project Settings.


#include "UdpSend.h"

// Sets default values
AUdpSend::AUdpSend()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SenderSocket = NULL;

}

// Called when the game starts or when spawned
void AUdpSend::BeginPlay()
{
	Super::BeginPlay();
	
}

void AUdpSend::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
		if (SenderSocket) 
		{
			SenderSocket->Close();
			ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(SenderSocket);
		}
}

// Called every frame
void AUdpSend::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AUdpSend::StartUDPSender(const FString& YourChosenSocketName, const FString& TheIP, const int32 ThePort, bool UDP)
{
	RemoteAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	bool bISValid;
	RemoteAddr->SetIp(*TheIP, bISValid);
	RemoteAddr->SetPort(ThePort);
	if (!bISValid)
	{
		UE_LOG(LogTemp, Warning, TEXT("UDP Sender >> IP address was no valid!"), *TheIP);
		return false;
	}

	SenderSocket = FUdpSocketBuilder(*YourChosenSocketName)
		.AsReusable()
		.WithBroadcast()
		.WithSendBufferSize(2 * 1024 * 1024);
	int32 SendSize = 2 * 1024 * 1024;
	SenderSocket->SetSendBufferSize(SendSize, SendSize);
	SenderSocket->SetReceiveBufferSize(SendSize, SendSize);
	if (bISValid)
	{
		bISValid = true;
	}
	return bISValid;
}

//发送字符串数据
bool AUdpSend::UDPSender_SendString(FString ToSend)
{
	if (!SenderSocket)
	{
		UE_LOG(LogTemp, Warning, TEXT("No sender socket"));
		return false;
	}


	int32 BytesSend = 0;
	FString serialized = ToSend;
	TCHAR* serializedChar = serialized.GetCharArray().GetData();
	int32 size = FCString::Strlen(serializedChar);
	int sent = 0;
	SenderSocket->SendTo((uint8*)TCHAR_TO_UTF8(serializedChar), size, BytesSend, * RemoteAddr);
	if (BytesSend<0)
	{
		const FString Str = "Socket is valid but the receiver received 0 bytes,make sure it is listening properly!";
		UE_LOG(LogTemp, Error, TEXT("%s"), *Str);
		return false;
	}
	UE_LOG(LogTemp, Warning, TEXT("UDP send success! INFO sent = %s"), *ToSend);
	return true;
}

//发送字节数据
bool AUdpSend::UDPSender_SendData(TArray<uint8>data)
{
	if (!SenderSocket)
	{
		UE_LOG(LogTemp, Warning, TEXT("No sender socket"));
		return false;
	}

	int32 BytesSend = 0;
	int32 size = 0;
	SenderSocket->SendTo(&data[0], size, BytesSend, *RemoteAddr);
	if (BytesSend<0)
	{
		const FString Str = "Socket is valid but the receiver received 0 bytes,make sure it is listening properly! ";
		UE_LOG(LogTemp, Error, TEXT("%s"), *Str);
		return false;
	}

	return true;
}

