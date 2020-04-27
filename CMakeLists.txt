cmake_minimum_required(VERSION 3.12)
project(tree)

set(CMAKE_CXX_STANDARD 17)

add_executable(tree main.cpp Tree.h Order.h BinaryTreeNode.h TreapNode.h Node.h)