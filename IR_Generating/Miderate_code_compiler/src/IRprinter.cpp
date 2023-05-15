/*!
 *@file IRprinter.cpp
 *@brief 中间语言输出接口定义文件
 *@version 1.0.0
 *@date 2022-10-04
 */

#include "IRprinter.h"

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
std::string print_as_op(Value *v, bool print_ty) {
  std::string op_ir;
  if (print_ty) {
    op_ir += v->get_type()->print();
    op_ir += " ";
  }

  if (dynamic_cast<GlobalVariable *>(v)) {
    op_ir += "@" + v->get_name();
  } else if (dynamic_cast<Function *>(v)) {
    op_ir += "@" + v->get_name();
  } else if (dynamic_cast<Constant *>(v)) {
    op_ir += v->print();
  } else {
    op_ir += "%" + v->get_name();
  }

  return op_ir;
}

/*!
 *@brief 打印比较operands的名称
 *@return 字符串
 *@note
 *---------
 *
 */
std::string print_cmp_type(CmpInst::CmpOp op) {
  switch (op) {
  case CmpInst::GE:
    return "sge";
  case CmpInst::GT:
    return "sgt";
  case CmpInst::LE:
    return "sle";
  case CmpInst::LT:
    return "slt";
  case CmpInst::EQ:
    return "eq";
  case CmpInst::NE:
    return "ne";
  default:
    break;
  }
  return "wrong cmpop";
}