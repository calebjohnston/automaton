#include "ftxui/component/animation.hpp"
#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/box.hpp"
#include "ftxui/screen/color.hpp"

namespace Auto {

using namespace ftxui;

using ftxui::Screen;

ftxui::Component AnimatedBackground(Ref<ButtonOption> option) {
	class Impl : public ComponentBase {
	public:
		Impl(Ref<ButtonOption> option) : option_(std::move(option)) {}
		
		Element Render() override {
			return emptyElement() | AnimatedColorStyle() | reflect(box_);
		}
		
		Decorator AnimatedColorStyle() {
			return color(Color::Interpolate(animation_background_, Color::Green, Color::Black));
		}
		
		void SetAnimationTarget(float target) {
			animator_background_ = animation::Animator(&animation_background_, target,
													   std::chrono::milliseconds(500),
													   animation::easing::QuadraticInOut);
		}
		
		void OnAnimation(animation::Params& p) override {
			animator_background_.OnAnimation(p);
		}
		
		void SetActiveChild(ComponentBase* child) override {
			ComponentBase::SetActiveChild(child);
			animation_background_ = 1.0f;
			SetAnimationTarget(0.0f);
		}
		
		bool Focusable() const final { return false; }
		
	private:
		Box box_;
		Ref<ButtonOption> option_;
		float animation_background_ = 0;
		animation::Animator animator_background_ =
			animation::Animator(&animation_background_);
	};
	
	return Make<Impl>(std::move(option));
}

}	// namespace Auto
