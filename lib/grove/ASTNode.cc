/*
** Copyright 2014-2015 Robert Fratto. See the LICENSE.txt file at the top-level
** directory of this distribution.
**
** Licensed under the MIT license <http://opensource.org/licenses/MIT>. This file
** may not be copied, modified, or distributed except according to those terms.
*/

#include <stdexcept>
#include <grove/ASTNode.h>
#include <grove/Module.h>
#include <grove/Block.h>

Module* ASTNode::getModule() const
{
	if (m_module == nullptr && getParent() != nullptr)
	{
		return getParent()->getModule();
	}
	
	return m_module;
}

ASTNode* ASTNode::getParent() const
{
	return m_parent;
}

IRBuilder* ASTNode::IRBuilder() const
{
	if (getModule() == nullptr)
	{
		return nullptr;
	}
	
	return getModule()->getIRBuilder();
}

std::vector<ASTNode *> ASTNode::getChildren() const
{
	return m_children;
}

void ASTNode::addChild(ASTNode *child, bool mustExist)
{
	if (child == nullptr)
	{
		if (mustExist == true)
		{
    		throw std::invalid_argument("child must not be nullptr");
		}
		else
		{
			return;
		}
	}
	
	m_children.push_back(child);
	
	child->m_parent = this;
	child->m_module = getModule();
}

bool ASTNode::isRootNode() const
{
	return getParent() == nullptr;
}

ASTNode* ASTNode::copy() const
{
	throw std::runtime_error("ASTNode::copy() not overridden");
}

void ASTNode::resolve()
{
	// Do nothing
}

void ASTNode::build()
{
	// Do nothing 
}

Named* ASTNode::findNamed(std::string name) const
{
	std::vector<Type *> candidate_list;
	return findNamed(name, candidate_list);
}

Named* ASTNode::findNamed(std::string name, std::vector<Type *> candidates) const
{
	auto ptr = this;
	
	while (ptr != nullptr)
	{
		// Find the nearest block from this pointer.
		auto block = ptr->findParent<Block *>();
		
		if (block == nullptr)
		{
			return nullptr;
		}
		
		// Find closest node whose parent is that block.
		auto limit = ptr;
		while (limit != nullptr)
		{
			if (limit->getParent() == block)
			{
				break;
			}
			
			limit = limit->getParent();
		}
		
		auto named = block->getNamed(name, candidates, limit);
		if (named != nullptr)
		{
			return named;
		}
		
		// If we didn't find it, start looking from the block.
		ptr = block;
	}
	
	return nullptr;
}

ASTNode::ASTNode(Module* module)
{
	if (module == nullptr)
	{
		throw std::invalid_argument("module cannot be null.");
	}
	
	m_module = module;
}

ASTNode::ASTNode(ASTNode* parent)
{
	if (parent == nullptr)
	{
		throw std::invalid_argument("parent cannot be null.");
	}
	
	m_parent = parent;
	m_module = getParent()->getModule();
	
	getParent()->addChild(this);
}

ASTNode::ASTNode()
{
	// Do nothing.
}

ASTNode::~ASTNode()
{
	// Do nothing 
}