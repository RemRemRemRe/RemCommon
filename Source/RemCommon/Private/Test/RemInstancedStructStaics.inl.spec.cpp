// Copyright RemRemRemRe. 2025. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Object/RemEmptyStruct.h"
#include "Struct/RemInstancedStructStaics.inl"
#include "StructUtils/StructView.h"

#if WITH_DEV_AUTOMATION_TESTS

DEFINE_SPEC(FRemInstancedStructStaicsInlineTest, "Rem.Struct.Statics.Inline",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);


void FRemInstancedStructStaicsInlineTest::Define()
{
	Describe(TEXT("Test API FindStructView"), [this]
	{
		It(TEXT("should pass for empty array of struct utils type"), [this]
		{
			TArray<TSharedStruct<FRemEmptyStruct>> A{};
			TArray<TInstancedStruct<FRemEmptyStruct>> B{};
			TArray<TStructView<FRemEmptyStruct>> C{};
			TArray<TConstStructView<FRemEmptyStruct>> D{};

			TArray<FSharedStruct> E{};
			TArray<FInstancedStruct> F{};
			TArray<FStructView> G{};
			TArray<FConstStructView> H{};

			constexpr TStructView<FRemEmptyStruct> EmptyView{};
			TestEqual(TEXT("EmptyView"), Rem::Struct::FindStructView<FRemEmptyStruct>(MakeArrayView(A)), EmptyView);
			TestEqual(TEXT("EmptyView"), Rem::Struct::FindStructView<FRemEmptyStruct>(MakeArrayView(B)), EmptyView);
			TestEqual(TEXT("EmptyView"), Rem::Struct::FindStructView<FRemEmptyStruct>(MakeArrayView(C)), EmptyView);
			TestEqual(TEXT("EmptyView"), Rem::Struct::FindStructView<FRemEmptyStruct>(MakeArrayView(D)), EmptyView);
			TestEqual(TEXT("EmptyView"), Rem::Struct::FindStructView<FRemEmptyStruct>(MakeArrayView(E)), EmptyView);
			TestEqual(TEXT("EmptyView"), Rem::Struct::FindStructView<FRemEmptyStruct>(MakeArrayView(F)), EmptyView);
			TestEqual(TEXT("EmptyView"), Rem::Struct::FindStructView<FRemEmptyStruct>(MakeArrayView(G)), EmptyView);
			TestEqual(TEXT("EmptyView"), Rem::Struct::FindStructView<FRemEmptyStruct>(MakeArrayView(H)), EmptyView);

			constexpr TConstStructView<FRemEmptyStruct> ConstEmptyView{};
			TestEqual(TEXT("ConstEmptyView"), Rem::Struct::FindConstStructView<FRemEmptyStruct>(MakeConstArrayView(A)), ConstEmptyView);
			TestEqual(TEXT("ConstEmptyView"), Rem::Struct::FindConstStructView<FRemEmptyStruct>(MakeConstArrayView(B)), ConstEmptyView);
			TestEqual(TEXT("ConstEmptyView"), Rem::Struct::FindConstStructView<FRemEmptyStruct>(MakeConstArrayView(C)), ConstEmptyView);
			TestEqual(TEXT("ConstEmptyView"), Rem::Struct::FindConstStructView<FRemEmptyStruct>(MakeConstArrayView(D)), ConstEmptyView);
			TestEqual(TEXT("ConstEmptyView"), Rem::Struct::FindConstStructView<FRemEmptyStruct>(MakeConstArrayView(E)), ConstEmptyView);
			TestEqual(TEXT("ConstEmptyView"), Rem::Struct::FindConstStructView<FRemEmptyStruct>(MakeConstArrayView(F)), ConstEmptyView);
			TestEqual(TEXT("ConstEmptyView"), Rem::Struct::FindConstStructView<FRemEmptyStruct>(MakeConstArrayView(G)), ConstEmptyView);
			TestEqual(TEXT("ConstEmptyView"), Rem::Struct::FindConstStructView<FRemEmptyStruct>(MakeConstArrayView(H)), ConstEmptyView);
		});

		It(TEXT("should pass for non empty array of struct utils type"), [this]
		{
			FRemEmptyStruct EmptyStruct;

			//TArray<TSharedStruct<FRemEmptyStruct>> A{EmptyStruct};
			TArray<TInstancedStruct<FRemEmptyStruct>> B{TInstancedStruct<FRemEmptyStruct>::Make(EmptyStruct)};
			TArray<TStructView<FRemEmptyStruct>> C{EmptyStruct};
			TArray<TConstStructView<FRemEmptyStruct>> D{EmptyStruct};

			TArray E{FSharedStruct::Make<FRemEmptyStruct>(EmptyStruct)};
			TArray F{FInstancedStruct::Make<FRemEmptyStruct>(EmptyStruct)};
			TArray G{FStructView::Make<FRemEmptyStruct>(EmptyStruct)};
			TArray H{FConstStructView::Make<FRemEmptyStruct>(EmptyStruct)};

			constexpr TStructView<FRemEmptyStruct> EmptyView{};
			//TestNotEqual(TEXT("EmptyView"), Rem::Struct::FindStructView<FRemEmptyStruct>(MakeArrayView(A)), EmptyView);
			TestNotEqual(TEXT("EmptyView"), Rem::Struct::FindStructView<FRemEmptyStruct>(MakeArrayView(B)), EmptyView);
			TestNotEqual(TEXT("EmptyView"), Rem::Struct::FindStructView<FRemEmptyStruct>(MakeArrayView(C)), EmptyView);
			TestNotEqual(TEXT("EmptyView"), Rem::Struct::FindStructView<FRemEmptyStruct>(MakeArrayView(D)), EmptyView);
			TestNotEqual(TEXT("EmptyView"), Rem::Struct::FindStructView<FRemEmptyStruct>(MakeArrayView(E)), EmptyView);
			TestNotEqual(TEXT("EmptyView"), Rem::Struct::FindStructView<FRemEmptyStruct>(MakeArrayView(F)), EmptyView);
			TestNotEqual(TEXT("EmptyView"), Rem::Struct::FindStructView<FRemEmptyStruct>(MakeArrayView(G)), EmptyView);
			TestNotEqual(TEXT("EmptyView"), Rem::Struct::FindStructView<FRemEmptyStruct>(MakeArrayView(H)), EmptyView);

			constexpr TConstStructView<FRemEmptyStruct> ConstEmptyView{};
			//TestNotEqual(TEXT("ConstEmptyView"), Rem::Struct::FindConstStructView<FRemEmptyStruct>(MakeConstArrayView(A)), ConstEmptyView);
			TestNotEqual(TEXT("ConstEmptyView"), Rem::Struct::FindConstStructView<FRemEmptyStruct>(MakeConstArrayView(B)), ConstEmptyView);
			TestNotEqual(TEXT("ConstEmptyView"), Rem::Struct::FindConstStructView<FRemEmptyStruct>(MakeConstArrayView(C)), ConstEmptyView);
			TestNotEqual(TEXT("ConstEmptyView"), Rem::Struct::FindConstStructView<FRemEmptyStruct>(MakeConstArrayView(D)), ConstEmptyView);
			TestNotEqual(TEXT("ConstEmptyView"), Rem::Struct::FindConstStructView<FRemEmptyStruct>(MakeConstArrayView(E)), ConstEmptyView);
			TestNotEqual(TEXT("ConstEmptyView"), Rem::Struct::FindConstStructView<FRemEmptyStruct>(MakeConstArrayView(F)), ConstEmptyView);
			TestNotEqual(TEXT("ConstEmptyView"), Rem::Struct::FindConstStructView<FRemEmptyStruct>(MakeConstArrayView(G)), ConstEmptyView);
			TestNotEqual(TEXT("ConstEmptyView"), Rem::Struct::FindConstStructView<FRemEmptyStruct>(MakeConstArrayView(H)), ConstEmptyView);
		});
	});
}

#endif
