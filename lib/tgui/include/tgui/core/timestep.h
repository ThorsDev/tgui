#pragma once

namespace tgui
{
	class timestep
	{
	public:
		timestep() = default;
		timestep(float time);

		inline float get_seconds() const { return m_time; }
		inline float get_milliseconds() const { return m_time * 1000.0f; }

		operator float() { return m_time; }

	private:
		float m_time = 0.0f;
	};
}