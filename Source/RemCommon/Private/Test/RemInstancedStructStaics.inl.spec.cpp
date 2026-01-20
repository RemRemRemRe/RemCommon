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
			TestEqual(TEXT("EmptyView"), Rem::Struct::FindStructView<FRemEmptyStruct>(MakeArrayView(A)).Get<0>(), EmptyView);
			TestEqual(TEXT("EmptyView"), Rem::Struct::FindStructView<FRemEmptyStruct>(MakeArrayView(B)).Get<0>(), EmptyView);
			TestEqual(TEXT("EmptyView"), Rem::Struct::FindStructView<FRemEmptyStruct>(MakeArrayView(C)).Get<0>(), EmptyView);
			TestEqual(TEXT("EmptyView"), Rem::Struct::FindStructView<FRemEmptyStruct>(MakeArrayView(D)).Get<0>(), EmptyView);
			TestEqual(TEXT("EmptyView"), Rem::Struct::FindStructView<FRemEmptyStruct>(MakeArrayView(E)).Get<0>(), EmptyView);
			TestEqual(TEXT("EmptyView"), Rem::Struct::FindStructView<FRemEmptyStruct>(MakeArrayView(F)).Get<0>(), EmptyView);
			TestEqual(TEXT("EmptyView"), Rem::Struct::FindStructView<FRemEmptyStruct>(MakeArrayView(G)).Get<0>(), EmptyView);
			TestEqual(TEXT("EmptyView"), Rem::Struct::FindStructView<FRemEmptyStruct>(MakeArrayView(H)).Get<0>(), EmptyView);

			constexpr TConstStructView<FRemEmptyStruct> ConstEmptyView{};
			TestEqual(TEXT("ConstEmptyView"), Rem::Struct::FindConstStructView<FRemEmptyStruct>(MakeConstArrayView(A)).Get<0>(), ConstEmptyView);
			TestEqual(TEXT("ConstEmptyView"), Rem::Struct::FindConstStructView<FRemEmptyStruct>(MakeConstArrayView(B)).Get<0>(), ConstEmptyView);
			TestEqual(TEXT("ConstEmptyView"), Rem::Struct::FindConstStructView<FRemEmptyStruct>(MakeConstArrayView(C)).Get<0>(), ConstEmptyView);
			TestEqual(TEXT("ConstEmptyView"), Rem::Struct::FindConstStructView<FRemEmptyStruct>(MakeConstArrayView(D)).Get<0>(), ConstEmptyView);
			TestEqual(TEXT("ConstEmptyView"), Rem::Struct::FindConstStructView<FRemEmptyStruct>(MakeConstArrayView(E)).Get<0>(), ConstEmptyView);
			TestEqual(TEXT("ConstEmptyView"), Rem::Struct::FindConstStructView<FRemEmptyStruct>(MakeConstArrayView(F)).Get<0>(), ConstEmptyView);
			TestEqual(TEXT("ConstEmptyView"), Rem::Struct::FindConstStructView<FRemEmptyStruct>(MakeConstArrayView(G)).Get<0>(), ConstEmptyView);
			TestEqual(TEXT("ConstEmptyView"), Rem::Struct::FindConstStructView<FRemEmptyStruct>(MakeConstArrayView(H)).Get<0>(), ConstEmptyView);
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
			TestNotEqual(TEXT("EmptyView"), Rem::Struct::FindStructView<FRemEmptyStruct>(MakeArrayView(B)).Get<0>(), EmptyView);
			TestNotEqual(TEXT("EmptyView"), Rem::Struct::FindStructView<FRemEmptyStruct>(MakeArrayView(C)).Get<0>(), EmptyView);
			TestNotEqual(TEXT("EmptyView"), Rem::Struct::FindStructView<FRemEmptyStruct>(MakeArrayView(D)).Get<0>(), EmptyView);
			TestNotEqual(TEXT("EmptyView"), Rem::Struct::FindStructView<FRemEmptyStruct>(MakeArrayView(E)).Get<0>(), EmptyView);
			TestNotEqual(TEXT("EmptyView"), Rem::Struct::FindStructView<FRemEmptyStruct>(MakeArrayView(F)).Get<0>(), EmptyView);
			TestNotEqual(TEXT("EmptyView"), Rem::Struct::FindStructView<FRemEmptyStruct>(MakeArrayView(G)).Get<0>(), EmptyView);
			TestNotEqual(TEXT("EmptyView"), Rem::Struct::FindStructView<FRemEmptyStruct>(MakeArrayView(H)).Get<0>(), EmptyView);

			constexpr TConstStructView<FRemEmptyStruct> ConstEmptyView{};
			//TestNotEqual(TEXT("ConstEmptyView"), Rem::Struct::FindConstStructView<FRemEmptyStruct>(MakeConstArrayView(A)), ConstEmptyView);
			TestNotEqual(TEXT("ConstEmptyView"), Rem::Struct::FindConstStructView<FRemEmptyStruct>(MakeConstArrayView(B)).Get<0>(), ConstEmptyView);
			TestNotEqual(TEXT("ConstEmptyView"), Rem::Struct::FindConstStructView<FRemEmptyStruct>(MakeConstArrayView(C)).Get<0>(), ConstEmptyView);
			TestNotEqual(TEXT("ConstEmptyView"), Rem::Struct::FindConstStructView<FRemEmptyStruct>(MakeConstArrayView(D)).Get<0>(), ConstEmptyView);
			TestNotEqual(TEXT("ConstEmptyView"), Rem::Struct::FindConstStructView<FRemEmptyStruct>(MakeConstArrayView(E)).Get<0>(), ConstEmptyView);
			TestNotEqual(TEXT("ConstEmptyView"), Rem::Struct::FindConstStructView<FRemEmptyStruct>(MakeConstArrayView(F)).Get<0>(), ConstEmptyView);
			TestNotEqual(TEXT("ConstEmptyView"), Rem::Struct::FindConstStructView<FRemEmptyStruct>(MakeConstArrayView(G)).Get<0>(), ConstEmptyView);
			TestNotEqual(TEXT("ConstEmptyView"), Rem::Struct::FindConstStructView<FRemEmptyStruct>(MakeConstArrayView(H)).Get<0>(), ConstEmptyView);
		});
	});
}

#endif
