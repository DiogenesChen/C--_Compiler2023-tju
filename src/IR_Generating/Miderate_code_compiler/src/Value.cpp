/*!
 *@file Value.cpp
 *@brief Value类接口定义文件
 *@version 1.0.0
 *@date 2022-10-04
 */

#include <cassert>

#include "BasicBlock.h"
#include "Type.h"
#include "User.h"
#include "Value.h"
/*!
 *@brief Value的构造函数
 *@param ty 类型
 *@param name value名称
 *@return 当前对象本身
 */
Value::Value(Type *ty, const std::string &name) : type_(ty), name_(name) {}

/*!
 *@brief 添加use
 *@param val 使用该value的value
 *@param arg_no 在指令中的顺序
 *@note
 *---------
 *在use链中添加
 */
void Value::add_use(Value *val, unsigned arg_no) {
  use_list_.push_back(Use(val, arg_no));
}
/*!
 *@brief 获取value的名称
 *@return value字符串常量
 */
std::string Value::get_name() const { return name_; }
/*!
 *@brief 替换所有对于旧value的引用，改为新的
 *@param new_val value型指针
 *@note
 *--------
 *支持对于所有的value的修改，包括基本块
 *&emsp; 首先遍历所属的use_list，修改其他value中对于当前value的引用为新value
 *&emsp; 转换value类型为basicblock，修改成功即为对基本块间的类型调用修改，
 *&emsp; 依次修改前置后置的链表中对于该基本块的引用
 */
void Value::replace_all_use_with(Value *new_val) {
  for (auto use : use_list_) {
    auto val = dynamic_cast<User *>(use.val_);
    assert(val && "new_val is not a user");
    val->set_operand(use.arg_no_, new_val);
  }
  auto val = dynamic_cast<BasicBlock *>(this);
  if (val) {
    auto new_bb = dynamic_cast<BasicBlock *>(new_val);
    for (BasicBlock *pre_bb : val->get_pre_basic_blocks()) {
      pre_bb->remove_succ_basic_block(val);
      pre_bb->add_succ_basic_block(new_bb);
      new_bb->add_pre_basic_block(pre_bb);
    }
    for (BasicBlock *suc_bb : val->get_succ_basic_blocks()) {
      suc_bb->remove_pre_basic_block(val);
      suc_bb->add_pre_basic_block(new_bb);
      new_bb->add_succ_basic_block(suc_bb);
    }
  }
}

/*!
 *@brief 替换所有对于旧value的引用，改为新的
 *@param val value型指针
 *@note
 *----------
 *设置lambda函数，判定val的匹配
 *匹配成功则进行value的相关删除
 */
void Value::remove_use(Value *val) {
  auto is_val = [val](const Use &use) { return use.val_ == val; };
  use_list_.remove_if(is_val);
}