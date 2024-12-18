#pragma once

namespace test {
	class Test {
	public:
		Test() {}
		virtual ~Test() {}

		virtual void OnUpdate(float deltaTima) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	};
}
