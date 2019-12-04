#include "ICE_Capsule.h"
#include "InteractCore.h"
#include <UnrealType.h>
#include "ICE_Stage.h"

void UICE_Capsule::NativeConstruct()
{
	int maxE = (int)GETENUMMAX(TEXT("EOutlineColor"));
	for (int fooInt = 0; fooInt != maxE; fooInt++)
	{
		EOutlineColor foo = static_cast<EOutlineColor>(fooInt);
		FString value = GETENUMSTRING(TEXT("EOutlineColor"), foo);
		GetOutlineColor()->AddOption(value.Replace(TEXT("E_"), TEXT("")));
	}

	GetOutlineColor()->SetSelectedOption("Blue");
}

void UICE_Capsule::RemoveOutlineColorOption()
{
	GetOutlineColorBlock()->RemoveFromParent();
}

void UICE_Capsule::GetData()
{
}

void UICE_Capsule::UpdateData()
{
}
