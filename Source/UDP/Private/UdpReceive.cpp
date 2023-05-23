// Fill out your copyright notice in the Description page of Project Settings.


#include "UdpReceive.h"

// Sets default values
AUdpReceive::AUdpReceive()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUdpReceive::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUdpReceive::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AUdpReceive::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (this->Receiver != nullptr)
	{
		this->Receiver->Stop();
		delete this->Receiver;
		this->Receiver = nullptr;
	}
	if (ListenSocket)
	{
		ListenSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ListenSocket);
	}
}

void AUdpReceive::StartUDPReceiver(const FString& YourChonsenSocketName, const FString& TheIP, const int32 ThePort, bool& success)
{
	TSharedRef<FInternetAddr> targetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	FIPv4Address Addr;
	FIPv4Address::Parse(TheIP, Addr);
	FIPv4Endpoint Endpoint(FIPv4Address::Any, ThePort);

	ListenSocket = FUdpSocketBuilder(*YourChonsenSocketName)
		.AsNonBlocking()
		.AsReusable()
		.BoundToEndpoint(Endpoint)
		.WithReceiveBufferSize(2 * 1024 * 1024);
	
	int32 BufferSize = 2 * 1024 * 1024;
	ListenSocket->SetSendBufferSize(BufferSize, BufferSize);
	ListenSocket->SetReceiveBufferSize(BufferSize, BufferSize);

	if (!ListenSocket)
	{
		UE_LOG(LogTemp, Warning, TEXT("No sockets"));
		success = false;
	}
	if (ListenSocket)
	{
		UE_LOG(LogTemp, Warning, TEXT("The receiver is initialized"));
		success = true;
		this->Receiver = new FUdpSocketReceiver(ListenSocket, FTimespan::FromMilliseconds(100), TEXT("DEFAULT"));

// 		this->Receiver->OnDataReceived().BindLambda([this](const FArrayReaderPtr& Data, const FIPv4Endpoint& From)
// 			{
// 				TArray<uint8> ReceiveBytes;
// 				ReceiveBytes.Append(Data->GetData(), Data->Num());
// 				FMemory::Memcpy(ReceiveBytes.GetData(), Data->GetData(), Data->Num() * sizeof(uint8));
// 				if (this->OnByteMessage.IsBound())
// 				{
// 					this->OnByteMessage.Broadcast(ReceiveBytes, From.ToString());
// 
// 				}
// 
// 			});
// 		this->Receiver->Start();
	}
}

void AUdpReceive::DataRecv(FString& str, bool& success)
{
	if (!ListenSocket)
	{
		UE_LOG(LogTemp, Warning, TEXT("No send sockets"));
		success = false;
	}
	TSharedRef<FInternetAddr> targetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	TArray<uint8> ReceivedData;
	uint32 Size;
		if (ListenSocket->HasPendingData(Size))
		{
			success = true; str = "";
			uint8* Recv = new uint8[Size];
			int32 BytesRead = 0;

			ReceivedData.SetNumUninitialized(FMath::Min(Size, 65507u));
			ListenSocket->RecvFrom(ReceivedData.GetData(), ReceivedData.Num(), BytesRead, *targetAddr);
			char ansiiData[1024];
			memcpy(ansiiData, ReceivedData.GetData(), BytesRead);
			ansiiData[BytesRead] = 0;
			FString debugData = ANSI_TO_TCHAR(ansiiData);
			str = debugData;

		}
		else
		{
			success = false;
		}

}

TArray<uint8> AUdpReceive::DataRecvBytes(bool& success)
{
	if (!ListenSocket)
	{
		UE_LOG(LogTemp, Warning, TEXT("No send sockets"));
		success = false;
	}
	TSharedRef<FInternetAddr> targetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	TArray<uint8> ReceivedData;
	uint32 Size;
	if (ListenSocket->HasPendingData(Size)) 
	{
		success = true;
		uint8* Recv = new uint8[Size];
		int32 BytesRead = 0;
		ReceivedData.SetNumUninitialized(FMath::Min(Size, 65507u));
		ListenSocket->RecvFrom(ReceivedData.GetData(), ReceivedData.Num(), BytesRead, *targetAddr);
		ReceivedData.SetNum(BytesRead);
		return ReceivedData;
	}
	else
	{
		success = false;
	}
	return TArray<uint8>();
}

