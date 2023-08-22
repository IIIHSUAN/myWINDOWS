#pragma once

struct Easing
{
	/*** credit: https://easings.net/ ***/

	// swing
	static inline float easeInOutQuad(const float& x) { return float(x < 0.5 ? 2 * x * x : 1 - pow(-2 * x + 2, 2) / 2); }
	static inline float easeInOutCubic(const float& x) { return float(x < 0.5 ? 4 * x * x * x : 1 - pow(-2 * x + 2, 3) / 2); }
	static inline float easeInOutQuint(const float& x) { return float(x < 0.5 ? 16 * x * x * x * x * x : 1 - pow(-2 * x + 2, 5) / 2); }

	// accelerate
	static inline float easeInQuad(const float& x) { return float(x * x); }
	static inline float easeInCubic(const float& x) { return float(x * x * x); }
	static inline float easeInQuint(const float& x) { return float(x * x * x * x * x); }
	static inline float easeInCirc(const float& x) { return float(1 - sqrt(1 - pow(x, 2))); }

	// decelerate
	static inline float easeOutQuad(const float& x) { return float(1 - (1 - x) * (1 - x)); }
	static inline float easeOutCubic(const float& x) { return float(1 - pow(1 - x, 3)); }
	static inline float easeOutQuint(const float& x) { return float(1 - pow(1 - x, 5)); }
	static inline float easeOutCirc(const float& x) { return float(sqrt(1 - pow(x - 1, 2))); }

	// bounce
	static inline float easeInElastic(const float& x) { return float(x == 0 ? 0 : x == 1 ? 1 : -pow(2, 10 * x - 10) * sin((x * 10 - 10.75) * 2.094)); }
	static inline float easeOutElastic(const float& x) { return float(x == 0 ? 0 : x == 1 ? 1 : pow(2, -10 * x) * sin((x * 10 - 0.75) *  2.094) + 1); }
	static inline float easeInOutBack(const float& x) { return float(x < 0.5 ? (pow(2 * x, 2) * ((2.594 + 1) * 2 * x - 2.594)) / 2 : (pow(2 * x - 2, 2) * ((2.594 + 1) * (x * 2 - 2) + 2.594) + 2) / 2); }
	static inline float easeInOutElastic(const float& x) { return float(x == 0 ? 0 : x == 1 ? 1 : x < 0.5 ? -(pow(2, 20 * x - 10) * sin((20 * x - 11.125) * 1.396)) / 2 : (pow(2, -20 * x + 10) * sin((20 * x - 11.125) * 1.396)) / 2 + 1); }
};

struct Animate
{
	Pos startPos, endPos;
	Pos4 startPos4, endPos4;
	Size startSize, endSize;
	Size2 startSize2, endSize2;
	bool isPosAnim = false, isSizeAnim = false, isSetCanvasPos4 = false;
	float duration = 0.0f, time = 0.0f, dt = 0.0f, sleepTime = 0.0f;
	std::function<float(const float&)> easingFunc;
	enum Status { play, pause };
	Status status = play;
	std::function<void()> callback = nullptr;

	float x, y, w, h;
	Pos lastPos;
	Size lastSize;

	inline void sleep(float time) { sleepTime += time; }

	Animate() {}
	Animate(Pos4 pos4, Size2 size2, float duration, std::function<float(const float&)> easingFunc = nullptr)
		: endPos4(pos4), endSize2(size2), duration(duration), easingFunc(easingFunc), isPosAnim(true), isSizeAnim(true) {}
	Animate(Pos4 pos4, float duration, std::function<float(const float&)> easingFunc = nullptr)
		: endPos4(pos4), duration(duration), easingFunc(easingFunc), isPosAnim(true) {}
	Animate(Size2 size2, float duration, std::function<float(const float&)> easingFunc = nullptr)
		: endSize2(size2), duration(duration), easingFunc(easingFunc), isSizeAnim(true) {}
};
