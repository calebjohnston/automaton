//
//  tree.cpp
//
//  Created by Caleb Johnston on 8/23/2023.
//  Copyright © 2023 Caleb Johnston. All rights reserved.
//

#include <memory>
#include <string>
#include <vector>

#include "tree.h"

using namespace Auto;

Node branch_node(Nodes children)
{
	return std::make_shared<BranchNode>(std::move(children));
}

Node leaf_node(Nodes children)
{
	return std::make_shared<LeafNode>(std::move(children));
}
