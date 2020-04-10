#pragma once

#include "Test.h"

namespace test {
	class Lesson2_1: public Test
	{
	public:
		Lesson2_1();
		~Lesson2_1();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	
	};
}