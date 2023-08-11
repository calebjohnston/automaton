#include <algorithm>  // for min
#include <memory>     // for make_shared
#include <string>     // for string, wstring
#include <utility>    // for move
#include <vector>     // for vector

#include "ftxui/component/animation.hpp"  // for Animator, Params (ptr only)
#include "ftxui/component/captured_mouse.hpp"  // for CapturedMouse
#include "ftxui/component/component.hpp"       // for Make, Button
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/component_options.hpp"  // for ButtonOption, AnimatedColorOption, AnimatedColorsOption, EntryState
#include "ftxui/component/event.hpp"  // for Event, Event::Return
#include "ftxui/component/mouse.hpp"  // for Mouse, Mouse::Left, Mouse::Pressed
#include "ftxui/component/screen_interactive.hpp"  // for Component
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/box.hpp"    // for Box
#include "ftxui/screen/color.hpp"  // for Color
#include "ftxui/util/ref.hpp"      // for Ref, ConstStringRef

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

} // Auto
