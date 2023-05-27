/*!
 *@file IRprinter.h
 *@brief 中间语言输出接口头文件
 *@version 1.0.0
 *@date 2022-10-04
 */

#include "BasicBlock.h"
#include "Constant.h"
#include "Function.h"
#include "GlobalVariable.h"
#include "Instruction.h"
#include "Module.h"
#include "Type.h"
#include "User.h"
#include "Value.h"

/*!
 *@brief 打印operands的名称
 *@return 字符串
 *@note
 *---------
 *初始化字符串
 *&emsp; **if** 如果打印类型，打印类型数值
 *&emsp; **if** 如果属于全局变量，打印@+对应名称
 *&emsp;&emsp; **else if** 如果属于函数量，打印@+对应名称
 *&emsp;&emsp; **else if** 如果属于常量，打印对应名称
 *&emsp;&emsp; **else** 如果属于普通变量，打印%+对应名称
 *&emsp; 返回字符串
 */
std::string print_as_op(Value *v, bool print_ty);

/*!
 *@brief 打印比较operands的名称
 *@return 字符串
 *@note
 *---------
 *
 */
std::string print_cmp_type(CmpInst::CmpOp op);