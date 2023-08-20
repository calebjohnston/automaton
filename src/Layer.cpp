#include <ftxui/component/event.hpp>  // for Event
#include <ftxui/dom/elements.hpp>  // for operator|, Element, center, clear_under, dbox
#include <memory>                  // for __shared_ptr_access, shared_ptr
#include <utility>                 // for move

#include "ftxui/component/component.hpp"  // for Make, Tab, ComponentDecorator, Modal
#include "ftxui/component/component_base.hpp"  // for Component, ComponentBase

namespace Auto {

ftxui::Component Layer(ftxui::Component main, ftxui::Component layer, const bool* show_layer) {
	class Impl : public ftxui::ComponentBase {
		
	public:
		explicit Impl(ftxui::Component main, ftxui::Component layer, const bool* show_layer)
		: main_(std::move(main)), layer_(std::move(layer)), show_(show_layer) {
			Add(ftxui::Container::Tab({main_, layer_}, &selector_));
		}
		
	private:
		ftxui::Element Render() override {
			using namespace ftxui;
			selector_ = *show_;
			auto document = main_->Render();
			if (*show_) {
				document = dbox({
					document,
					layer_->Render(),
				});
			}
			return document;
		}
		
		bool OnEvent(ftxui::Event event) override {
			selector_ = *show_;
			return ftxui::ComponentBase::OnEvent(event);
		}
		
		ftxui::Component main_;
		ftxui::Component layer_;
		const bool* show_;
		int selector_ = *show_;
	};
	return ftxui::Make<Impl>(main, layer, show_layer);
}

ftxui::ComponentDecorator Layer(ftxui::Component layer, const bool* show_layer) {
	return [layer, show_layer](ftxui::Component main) {
		return Layer(std::move(main), layer, show_layer);
	};
}

}	// namespace Auto
