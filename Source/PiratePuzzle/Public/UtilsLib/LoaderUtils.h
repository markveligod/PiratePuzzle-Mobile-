#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

class LoaderUtils
{
public:
    template <typename T>
    static T* LoaderSyncObject(UObject* WorldContextObject, TAssetPtr<T> Asset)
    {
        // Check we're trying to load not null asset
        if (Asset.IsNull()) return (nullptr);

        // Load asset into memory (sync)
        FStreamableManager& AssetLoader = UAssetManager::GetStreamableManager();
        const FStringAssetReference Reference = Asset.ToSoftObjectPath();
        AssetLoader.LoadSynchronous(Reference);
        T* NewObject = Cast<T>(StaticLoadObject(T::StaticClass(), WorldContextObject, *Reference.ToString()));

        // Be sure that it won't be killed by GC on this frame
        if (NewObject)
        {
            NewObject->SetFlags(RF_StrongRefOnFrame);
        }

        return (NewObject);
    }
};
