#include <chrono>
#include <functional>
#include <string>
#include <thread>
#include <iostream>

#include "gtest/gtest.h"

#include "model.h"
#include "tree.hpp"


Auto::Result test_api_1(Auto::Command& cmd) { return { 0, "NOT IMPLEMENTED" }; }
Auto::Result test_api_2(Auto::Command& cmd) { return { 0, "NOT IMPLEMENTED" }; }
Auto::Result test_api_3(Auto::Command& cmd) { std::exit(EXIT_SUCCESS); return { 0, "doesn't matter" }; }

TEST(DummyTests, TRUE_IS_TRUE)
{
	EXPECT_TRUE(true);
}

TEST(TreeTests, ValidateCommand)
{
	using namespace Auto;
	
	Command ps_list_cmd;
	ps_list_cmd.function = "ps";
	ps_list_cmd.arguments = { "list" };
	
	Command ps_info_cmd;
	ps_info_cmd.function = "ps";
	ps_info_cmd.arguments = { "info", "whatever" };
	
	Command exit_cmd;
	exit_cmd.function = "exit";
	
	auto ps_list_leaf = leaf_node("list", test_api_1);
	auto ps_info_leaf = leaf_node("$", test_api_1);
	auto ps_info = branch_node("info", { ps_info_leaf });
	auto ps = branch_node("ps", { ps_info, ps_list_leaf });
	auto exit_leaf = leaf_node("exit", test_api_3);
	auto root = node({ ps, exit_leaf });
	
	auto ps_list_res = root->validate(ps_list_cmd);
	EXPECT_TRUE(ps_list_res.status == 0);
	auto ps_info_res = root->validate(ps_info_cmd);
	EXPECT_TRUE(ps_info_res.status == 0);
	auto exit_res = root->validate(exit_cmd);
	EXPECT_TRUE(exit_res.status == 0);
}
