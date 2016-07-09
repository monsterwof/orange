//
// Copyright 2014-2016 Robert Fratto. See the LICENSE.txt file at the top-level
// directory of this distribution.
//
// Licensed under the MIT license <http://opensource.org/licenses/MIT>. This file
// may not be copied, modified, or distributed except according to those terms.
//

#pragma once

#include "ast.h"

namespace orange { namespace ast {
	bool IsArithBinOp(BinOp binop);
	bool IsAssignBinOp(BinOp binop);
	bool IsCompareBinOp(BinOp binop);
	bool IsLogicalBinOp(BinOp binop);
}}

