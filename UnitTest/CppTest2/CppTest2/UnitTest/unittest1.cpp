#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\CppTest2\CppTest2.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
	TEST_CLASS(CppTest2UT)
	{
	public:
		CppTest2UT()
		{
			m_pCppTest = new CppTest2();
		}
		~CppTest2UT()
		{
			delete m_pCppTest;
		}
		TEST_METHOD(AddTest)
		{
			// TODO: Your test code here
			Assert::AreEqual(2, m_pCppTest->Add(1, 1), 0);
		}
		TEST_METHOD(AddTest)
		{
			// TODO: Your test code here
			Assert::AreEqual(2, m_pCppTest->Add(1, 1), 0);
		}
		
		CppTest2 * m_pCppTest;
	};
}