// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FindSessionsCallbackProxy.h"
#include "Engine/GameInstance.h"
#include "FHGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDele_SessionResult,
	bool, IsFind, const TArray<FBlueprintSessionResult>&, SessionResult);

USTRUCT(BlueprintType)
struct FPlayerStats
{
	GENERATED_USTRUCT_BODY()

public:
	FPlayerStats()
		: Health(1000), Stamina(1000), Attack(0),
		AttackSpeed(0.f), Critcal(0.f), Defence(0)
	{
	}

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Status)
	int32 Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Status)
	int32 Stamina;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Status)
	int32 Attack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Status)
	float AttackSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Status)
	float Critcal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Status)
	int32 Defence;
};

UCLASS()
class FALLINGHELLGATE_API UFHGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
protected:
	UFHGameInstance(const FObjectInitializer& ObjectInitializer);

public:
	//test
	UFUNCTION(BlueprintCallable, Category = Test)
	void TESTss();

protected:
	virtual void Init() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = Data)
	TSubclassOf<class UItemDataManager> ItemDataManagerClass;

	UPROPERTY()
	class UItemDataManager* ItemDataManager;

public:
	UFUNCTION(BLueprintCallable)
	FORCEINLINE class UItemDataManager* GetItemDataManager() const { return ItemDataManager; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Data)
	int32 CurrentDungeonID;

public:
	UFUNCTION(BlueprintCallable, Category = Data)
	FORCEINLINE void SetCurrentDungeonID(const int32& NewDungeonID) { CurrentDungeonID = NewDungeonID; }

	UFUNCTION(BlueprintCallable, Category = Data)
	FORCEINLINE int32 GetCurrentDungeonID() const { return CurrentDungeonID; }

protected:
	// TMap<ItemData, ItemAmount>
	UPROPERTY()
	TMap<class UItemData*, int32> InventoryItems;

	// TMap<QuickSlotIdx, ItemData>
	UPROPERTY()
	TMap<int32, class UItemData*> QuickSlotItems;

	// TArray<ItemData>
	UPROPERTY()
	TArray<class UItemData*> EquipmentItems;

public:
	FORCEINLINE TMap<class UItemData*, int32>* GetInventoryItems() { return &InventoryItems; }

	FORCEINLINE TMap<int32, class UItemData*>* GetQuickSlotItems() { return &QuickSlotItems; }

	FORCEINLINE TArray<class UItemData*>* GetEquipments() { return &EquipmentItems; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Data)
	FText PlayerName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Status)
	int32 Money;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Status)
	FPlayerStats DefaultPlayerStats;

public:
	UFUNCTION(BlueprintCallable, Category = Data)
	FORCEINLINE void SetPlayerName(const FText& NewPlayerName) { PlayerName = NewPlayerName; }

	UFUNCTION(BlueprintCallable, Category = Data)
	FORCEINLINE FText& GetPlayerName() { return PlayerName; }

	UFUNCTION(BlueprintCallable, Category = Status)
	FORCEINLINE int32& GetPlayerMoney() { return Money; }

	UFUNCTION(BlueprintCallable, Category = Status)
	FORCEINLINE FPlayerStats& GetDefaultPlayerStats() { return DefaultPlayerStats; }

	//--------------------------------------------------------------[ Start - Create Session Code ]------------------------------------------------------------------//
public:
	/**
	*	Function to host a game!
	*
	*	@Param		UserID			User that started the request
	*	@Param		SessionName		Name of the Session
	*	@Param		bIsLAN			Is this is LAN Game?
	*	@Param		bIsPresence		"Is the Session to create a presence Session"
	*	@Param		MaxNumPlayers	        Number of Maximum allowed players on this "Session" (Server)
	*/
	bool HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers);

	/* Delegate called when session created */
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	/* Delegate called when session started */
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;

	/** Handles to registered delegates for creating/starting a session */
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;

	TSharedPtr<class FOnlineSessionSettings> SessionSettings;
	// In our UNWGameInstance.h:

	/**
	*	Function fired when a session create request has completed
	*
	*	@param SessionName the name of the session this callback is for
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	/**
	*	Function fired when a session start request has completed
	*
	*	@param SessionName the name of the session this callback is for
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);

	//--------------------------------------------------------------[ End - Create Session Code ]------------------------------------------------------------------//


	//--------------------------------------------------------------[ Start - Find Session Code ]------------------------------------------------------------------//

	/**
	*	Find an online session
	*
	*	@param UserId user that initiated the request
	*	@param bIsLAN are we searching LAN matches
	*	@param bIsPresence are we searching presence sessions
	*/
	void FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence);

	/** Delegate for searching for sessions */
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;

	/** Handle to registered delegate for searching a session */
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	/**
	*	Delegate fired when a session search query has completed
	*
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	void OnFindSessionsComplete(bool bWasSuccessful);

	//--------------------------------------------------------------[ End - Find Session Code ]------------------------------------------------------------------//

	//--------------------------------------------------------------[ Start - Join Session Code ]------------------------------------------------------------------//

	/**
	*	Joins a session via a search result
	*
	*	@param SessionName name of session
	*	@param SearchResult Session to join
	*
	*	@return bool true if successful, false otherwise
	*/
	bool JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);

	/** Delegate for joining a session */
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;

	/** Handle to registered delegate for joining a session */
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;

	/**
	*	Delegate fired when a session join request has completed
	*
	*	@param SessionName the name of the session this callback is for
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	//--------------------------------------------------------------[ End - Join Session Code ]------------------------------------------------------------------//


	//--------------------------------------------------------------[ Start - Destroy Session Code ]------------------------------------------------------------------//

	/** Delegate for destroying a session */
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;

	/** Handle to registered delegate for destroying a session */
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;

	/**
	*	Delegate fired when a destroying an online session has completed
	*
	*	@param SessionName the name of the session this callback is for
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	virtual void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	//--------------------------------------------------------------[ End - Destroy Session Code ]------------------------------------------------------------------//

	//--------------------------------------------------------------[ Start - Session Test Code ]------------------------------------------------------------------//

	UFUNCTION(BlueprintCallable, Category = "Network|Test")
	void StartOnlineGame(bool bIsLAN, int MaxNumPlayers, FString PartyName, FString LevelName);

	UFUNCTION(BlueprintCallable, Category = "Network|Test")
	void FindOnlineGames();

	UFUNCTION(BlueprintCallable, Category = "Network|Test")
	void JoinOnlineGame(FBlueprintSessionResult SessionResult, FString Name);

	UFUNCTION(BlueprintCallable, Category = "Network|Test")
	void DestroySessionAndLeaveGame();

	//--------------------------------------------------------------[ End - Session Test Code ]------------------------------------------------------------------//

	/** virtual function to allow custom GameInstances an opportunity to do cleanup when shutting down */
	virtual void Shutdown() override;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnUpdateSessionResult(const TArray<FBlueprintSessionResult>& SessionResults);

	void OnUpdateSessionResult_Implementation(const TArray<FBlueprintSessionResult>& SessionResults);

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
	FDele_SessionResult Fuc_Dele_SessionResult;

};
