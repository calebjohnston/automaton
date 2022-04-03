//
//  controller.h
//
//  Created by Caleb Johnston on 4/3/2022.
//  Copyright © 2022 Caleb Johnston. All rights reserved.
//

#pragma once

#include <memory>

#include "node.h"

using KernelRef = std::shared_ptr<class Kernel>;
using ComputerRef = std::shared_ptr<class Computer>;
typedef std::shared_ptr<class Controller> ControllerRef;

class Controller : public GraphNode {
public:
	static ControllerRef create(ComponentRef component, std::string name = "");

	virtual void connect(GridNodeRef grid) override;

	virtual int add(GraphNodeRef child) override;

	virtual bool remove(size_t index) override;

protected:
	Controller(ComponentRef component, const std::string& name);

	int _capacity;
};

class ProgramController : public Controller {
public:
	static ProgramControllerRef create(ComponentRef component, std::string name = "");

	bool create();
	bool read();
	bool update();
	bool delete();

	bool load(ProgramRef program) { return add(program) >= 0; }
	bool unload(size_t index) { return remove(index); }
	std::vector<ProgramRef> loaded() const;
	bool loaded(ProgramRef program) const;

protected:
	ProgramController(ComponentRef component, const std::string& name);

};
