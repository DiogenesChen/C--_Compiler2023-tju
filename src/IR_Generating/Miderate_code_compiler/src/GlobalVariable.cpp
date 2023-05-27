/*!
 *@file GlobalVariable.h
 *@brief 全局变量接口定义文件
 *@version 1.0.0
 *@date 2022-10-04
 */
#include "GlobalVariable.h"
#include "IRprinter.h"

/*!
 *@brief 全局变量的构造函数
 *@param name 全局变量名称
 *@param m 所从属模块
 *@param ty 变量的类型
 *@param is_const 是否为常量
 *@param init 常量指针
 *@return 当前对象本身
 *@note
 *---------
 *在所属模块m中添加全局变量
 *全局变量如果有初始值，进行初始赋值，操作符第一个位置为init
 */
GlobalVariable::GlobalVariable(std::string name, Module *m, Type *ty,
                               bool is_const, Constant *init)
    : User(ty, name, init != nullptr), is_const_(is_const), init_val_(init) {
  m->add_global_variable(this);
  if (init) {
    this->set_operand(0, init);
  }
} // global操作数为initval

/*!
 *@brief 全局变量的创建函数
 *@param name 全局变量名称
 *@param m 所从属模块
 *@param ty 变量的类型
 *@param is_const 是否为常量
 *@param init 常量指针
 *@return 当前对象本身
 *@note
 *-------
 *全局变量的创建
 *默认变量类型为指针类型，无初值
 */
GlobalVariable *GlobalVariable::create(std::string name, Module *m, Type *ty,
                                       bool is_const,
                                       Constant *init = nullptr) {
  return new GlobalVariable(name, m, PointerType::get(ty), is_const, init);
}

/*!
 *@brief 打印全局变量
 *@return 字符串
 *@note
 *--------
 *初始化字符串
 *添加名称
 *添加常量类型
 *添加数据指针所指向数据的类型
 *添加变量初值
 */
std::string GlobalVariable::print() {
  std::string global_val_ir;
  global_val_ir += print_as_op(this, false);
  global_val_ir += " = ";
  global_val_ir += (this->is_const() ? "constant " : "global ");
  global_val_ir += this->get_type()->get_pointer_element_type()->print();
  global_val_ir += " ";
  global_val_ir += this->get_init()->print();
  return global_val_ir;
}