#include <algorithm>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "ftxui/component/animation.hpp"
#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/event.hpp"
#include "ftxui/component/mouse.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/box.hpp"
#include "ftxui/screen/color.hpp"
#include "ftxui/util/ref.hpp"

namespace Auto {

using namespace ftxui;

using ftxui::Screen;

ftxui::Component AnimatedText(ConstStringRef label,
							  Ref<ButtonOption> option) {
	class Impl : public ComponentBase {
	public:
		Impl(ConstStringRef label, Ref<ButtonOption> option) : label_(std::move(label)), label_cache_(*label_), option_(std::move(option)) {}
		
		// Component implementation:
		Element Render() override {
			changed_ = label_cache_ != *label_;
			label_cache_ = *label_;
			float target = changed_ ? 1.F : 0.F;
			if (changed_) {
				animation_background_ = 0.0f;
				SetAnimationTarget(1.0f);
			}
			
			return text(*label_) | AnimatedColorStyle() | reflect(box_);
		}
		
		Decorator AnimatedColorStyle() {
			return bgcolor(Color::Interpolate(animation_background_, Color::White, Color::Green));
		}
		
		void SetAnimationTarget(float target) {
			animator_background_ = animation::Animator(&animation_background_, target,
													   std::chrono::milliseconds(500),
													   animation::easing::QuadraticInOut);
		}
		
		void OnAnimation(animation::Params& p) override {
			animator_background_.OnAnimation(p);
		}
		/*
		void OnClick() {
			on_click_();
			animation_background_ = 0.5F;  // NOLINT
			animation_foreground_ = 0.5F;  // NOLINT
			SetAnimationTarget(1.F);       // NOLINT
		}
		 
		
		bool OnEvent(Event event) override {
			if (event.is_mouse()) {
				return OnMouseEvent(event);
			}
			
			if (event == Event::Return) {
				OnClick();
				return true;
			}
			return false;
		}
		
		bool OnMouseEvent(Event event) {
			mouse_hover_ =
			box_.Contain(event.mouse().x, event.mouse().y) && CaptureMouse(event);
			
			if (!mouse_hover_) {
				return false;
			}
			
			if (event.mouse().button == Mouse::Left &&
				event.mouse().motion == Mouse::Pressed) {
				TakeFocus();
				OnClick();
				return true;
			}
			
			return false;
		}
		
		
		 */
		bool Focusable() const final { return false; }
		
	private:
		ConstStringRef label_;
		std::string label_cache_;
		bool changed_ = false;
		Box box_;
		Ref<ButtonOption> option_;
		float animation_background_ = 0;
		float animation_foreground_ = 0;
		animation::Animator animator_background_ =
			animation::Animator(&animation_background_);
		animation::Animator animator_foreground_ =
			animation::Animator(&animation_foreground_);
	};
	
	return Make<Impl>(std::move(label), std::move(option));
}

}	// namespace Auto
