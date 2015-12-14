/*
** Copyright 2014-2015 Robert Fratto. See the LICENSE.txt file at the top-level
** directory of this distribution.
**
** Licensed under the MIT license <http://opensource.org/licenses/MIT>. This file
** may not be copied, modified, or distributed except according to those terms.
*/

#pragma once 

#include "Expression.h"
#include "Accessible.h"
#include "OString.h"

class ClassDecl;
class Valued;
class MemberVarDecl;

class MemberAccess : public Expression, public Accessible
{
private:
	const ClassDecl* m_class = nullptr;
	MemberVarDecl* m_member = nullptr;
	Valued* m_valued = nullptr;
	
	OString m_name;
public:
	virtual ASTNode* copy() const override;
	
	const ClassDecl* getClass() const;
	const Valued* getValued() const;
	const MemberVarDecl* getMember() const;
	const OString& getName() const;
	
	virtual bool hasPointer() const override;
	virtual llvm::Value* getPointer() const override;
	virtual llvm::Value* getValue() const override;
	
	virtual bool isAccessible() const override;
	virtual Expression* access(OString name, const ASTNode* hint)
		const override;
	
	virtual void findDependencies() override;
	virtual void resolve() override;
	virtual void build() override;
	
	MemberAccess(const ClassDecl* classDecl, Valued* valued,
				 const OString& name);
	
	/// Creates a member access that will look for the this
	/// parameter. Only valid for use inside of a method.
	MemberAccess(const ClassDecl* classDecl, const OString& name);
};