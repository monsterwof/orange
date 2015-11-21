/*
** Copyright 2014-2015 Robert Fratto. See the LICENSE.txt file at the top-level
** directory of this distribution.
**
** Licensed under the MIT license <http://opensource.org/licenses/MIT>. This file
** may not be copied, modified, or distributed except according to those terms.
*/

#pragma once

#include <vector>
#include "ObjectBase.h"

namespace llvm {
	class ConstantFolder;

	template <bool preserveNames>
	class IRBuilderDefaultInserter;

	template <bool preserveNames, typename T, typename Inserter>
	class IRBuilder;
}

typedef llvm::IRBuilder<true, llvm::ConstantFolder,
	llvm::IRBuilderDefaultInserter<true> > IRBuilder;

class Module;
class Named;
class Type;

/**
 * ASTNode is the root node for any element in the AST.
 */
class ASTNode : public ObjectBase {
private:
	Module* m_module = nullptr;
	ASTNode* m_parent = nullptr;

	std::vector<ASTNode *> m_children;
public:
	/// Gets the module this node resides in.
	Module* getModule() const;

	/// Gets the parent of this node.
	ASTNode* getParent() const;

	std::vector<ASTNode *> getChildren() const;

	/// Gets the IR builder in the module.
	IRBuilder* IRBuilder() const;

	void addChild(ASTNode* child, bool mustExist = false);

	bool isRootNode() const;

	/// Creates a copy of this node.
	virtual ASTNode* copy() const;

	virtual void resolve();

	/// Generate code. Sets value if applicable.
	virtual void build();

	/// Finds a parent of a type T.
	template <typename T> T findParent() const
	{
		ASTNode* ptr = getParent();

		while (ptr != nullptr)
		{
			if (ptr->is<T>())
			{
				return ptr->as<T>();
			}

			ptr = ptr->getParent();
		}

		return nullptr;
	}

	/// Finds the first child, depth-first, of a type T.
	template <typename T> T findChild() const
	{
		for (auto child : getChildren())
		{
			if (child->is<T>())
			{
				return child->as<T>();
			}

			auto val = child->findChild<T>();
			if (val != nullptr)
			{
				return val;
			}
		}

		return nullptr;
	}

	/// Finds all children, depth-first, of a type T.
	template <typename T> std::vector<T> findChildren() const
	{
		std::vector<T> children;

		for (auto child : getChildren())
		{
			if (child->is<T>())
			{
				children.push_back(child->as<T>());
			}

			auto vals = child->findChildren<T>();
			children.insert(children.end(), vals.begin(), vals.end());
		}

		return children;
	}

	/**
	 * Tries to find a Named node in the AST. Searches up to this node.
	 *
	 * If multiple matching nodes are found, an exception is thrown.
	 *
	 * @param name The name of the node to look for.
	 * @param limit The child to stop searching at, if any.
	 *
	 * @return The named node, if one was found.
	 * @throws Throws an exception if multiple nodes were found.
	 */
	Named* findNamed(std::string name) const;

	/**
	 * Tries to find a Named node in the AST. Searches up to this node.
	 *
	 * This function allows for a type hint to be passed in to narrow 
	 * down results if more than one node was found by name. The hint can 
	 * be any type, including VarType and compound types (like FunctionType).
	 * VarType is used as a wildcard.
	 *
	 * @param name The name of the node to look for.
	 * @param hint The type hint to narrow down search results.
	 * @param limit The child to stop searching at, if any.
	 *
	 * @return The named node, if one was found.
	 *
	 * @throws Throws an exception if multiple nodes with the same name
	 * were found, and using hint does not narrow down results to exactly
	 * one node.
	 */
	Named* findNamed(std::string name, Type* type,
					 bool forceTypeMatch = false, bool createGeneric = true) const;

	/// Constructs a new root node with a module.
	ASTNode(Module* module);

	/// Constructs a child node with a parent.
	ASTNode(ASTNode* parent);

	/// Constructs an orphaned node.
	/// It is up to the parent to add it as a child.
	ASTNode();

	virtual ~ASTNode();
};
