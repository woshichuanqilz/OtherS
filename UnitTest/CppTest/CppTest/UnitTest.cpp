#include "stdafx.h"
#ifdef _TEST  // 必须加入此项，否则运行Debug或Release时会出现“缺少 .net 的动态库”的提示 
#include "CppUnitTest.h" 
#include "cpptest.h" // 此处按需修改  
using namespace Microsoft::VisualStudio::CppUnitTestFramework; 
namespace MyTest 
{   
	TEST_CLASS(MyTests)   
	{   
		public:       
		TEST_METHOD(MyTestMethod)
		{           
			Assert::AreEqual(add(2, 3), 5); // 此处按需要修改      
		}
		TEST_METHOD(MyTestMethod_2)       
		{           
			Assert::AreEqual(add(2,5), 9); // 此处按需要修改       
		} // …   
	}; 
}
#endif // 此处必须加