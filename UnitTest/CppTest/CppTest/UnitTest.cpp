#include "stdafx.h"
#ifdef _TEST  // �����������������Debug��Releaseʱ����֡�ȱ�� .net �Ķ�̬�⡱����ʾ 
#include "CppUnitTest.h" 
#include "cpptest.h" // �˴������޸�  
using namespace Microsoft::VisualStudio::CppUnitTestFramework; 
namespace MyTest 
{   
	TEST_CLASS(MyTests)   
	{   
		public:       
		TEST_METHOD(MyTestMethod)
		{           
			Assert::AreEqual(add(2, 3), 5); // �˴�����Ҫ�޸�      
		}
		TEST_METHOD(MyTestMethod_2)       
		{           
			Assert::AreEqual(add(2,5), 9); // �˴�����Ҫ�޸�       
		} // ��   
	}; 
}
#endif // �˴������