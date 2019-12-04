
#include "ICE_Search.h"
#include "ICE_ButtonImage.h"
#include "ICE_ActorFunction.h"
#include "ICE_BasicFunction.h"

void UICE_Search::NativeConstruct()
{
	GetSearchText()->OnTextChanged.AddDynamic(this, &UICE_Search::OnTextChanged);
}

void UICE_Search::OnTextChanged(const FText& text)
{
	UpdateValues();
}

void UICE_Search::UpdateValues()
{
	GetHolderScroll()->ClearChildren();
	for (int i = 0; i < selector->GetOptionCount(); i++) {
		if (GetSearchText()->GetText().ToString() == "" || selector->GetOptionAtIndex(i).Contains(GetSearchText()->GetText().ToString())) {
			//UTexture2D* texture = Cast<UTexture2D>(textures2D[i]);
			UICE_ButtonImage* imageOption = CreateWidget<UICE_ButtonImage>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_ButtonImage_BP.ICE_ButtonImage_BP_C'")));
			imageOption->m_data = EButtonTypeInfo::E_Select;
			if (basicFun) {
				imageOption->m_dataOther = EFunctionButtonInfo::E_Basic;
				imageOption->basicFun = basicFun;
			}
			else if (actorFun) {
				imageOption->m_dataOther = EFunctionButtonInfo::E_Complex;
				imageOption->actorFun = actorFun;
			}
			imageOption->index = i;
			imageOption->selector = selector;
			imageOption->expandable = GetExpandableArea();
			imageOption->expandableName = GetExpandableText();
			imageOption->searchText = GetSearchText();
			if (hasImages) {
				imageOption->GetImage()->Visibility = ESlateVisibility::Visible;
				//imageOption->GetImage()->SetBrushFromTexture(texture);
			}
			else {
				imageOption->GetImage()->Visibility = ESlateVisibility::Collapsed;
			}
			imageOption->GetTextBlock()->SetText(FText::FromString(selector->GetOptionAtIndex(i)));
			//imageOption->SetToolTipText(FText::FromString(texture->GetPathName()));
			GetHolderScroll()->AddChild(imageOption);
		}
	}
	GetExpandableText()->SetText(FText::FromString(selector->GetSelectedOption()));
}
