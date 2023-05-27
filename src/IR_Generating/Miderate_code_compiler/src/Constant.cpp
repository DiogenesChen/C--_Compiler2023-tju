/*!
 *@file Constant.h
 *@brief 常量接口定义文件
 *@version 1.0.0
 *@date 2022-10-04
 */
#include "Constant.h"
#include "Module.h"
#include <iostream>
#include <sstream>
/*!
 *@brief 常量整数类32位创建函数
 *@param val 常量值
 *@param m 所属模块
 *@return 常量类对象指针
 */
ConstantInt *ConstantInt::get(int val, Module *m) {
  return new ConstantInt(Type::get_int32_type(m), val);
}
/*!
 *@brief 常量整数类1位创建函数
 *@param val 常量值
 *@param m 所属模块
 *@return 常量类对象指针
 */
ConstantInt *ConstantInt::get(bool val, Module *m) {
  return new ConstantInt(Type::get_int1_type(m), val ? 1 : 0);
}
/*!
 *@brief 打印常量类变量
 *@return 字符串
 *@note
 *---------
 *获取常量类型
 *&emsp; **if** 判定为整数常量类型并且为1为的布尔类型，
 *&emsp;&emsp; 添加true/flase字符串
 *&emsp; 判定为32位类型
 *&emsp; 将其数组转换为字符串输出
 *&emsp; 返回字符串
 */
std::string ConstantInt::print() {
  std::string const_ir;
  Type *ty = this->get_type();
  if (ty->is_integer_type() &&
      static_cast<IntegerType *>(ty)->get_num_bits() == 1) {
    // int1
    const_ir += (this->get_value() == 0) ? "false" : "true";
  } else {
    // int32
    const_ir += std::to_string(this->get_value());
  }
  return const_ir;
}

/*!
 *@brief 常量整数类构造函数
 *@param ty 常量类型
 *@param val 常量数值类型数组
 *@return 自身类对象
 *constant int array
 */
ConstantArray::ConstantArray(ArrayType *ty, const std::vector<Constant *> &val)
    : Constant(ty, "", val.size()) {
  for (int i = 0; i < (int)val.size(); i++)
    set_operand(i, val[i]);
  this->const_array.assign(val.begin(), val.end());
}
/*!
 *@brief 获取常量数组指定索引的常量数值
 *@param index 索引
 *@return 常量类指针
 *constant int array
 */
Constant *ConstantArray::get_element_value(int index) {
  return this->const_array[index];
}
/*!
 *@brief 常量数组类的创建函数
 *@param ty 数组元素的类型
 *@param val 常量类数组
 *@return 常量数组类指针
 *constant int array
 */
ConstantArray *ConstantArray::get(ArrayType *ty,
                                  const std::vector<Constant *> &val) {
  return new ConstantArray(ty, val);
}
/*!
 *@brief 常量数组类打印函数
 *@return 字符串
 *constant int array
 */
std::string ConstantArray::print() {
  std::string const_ir;
  const_ir += "[";
  for (int i = 0; i < static_cast<int>(this->get_size_of_array()); i++) {
    const_ir += get_element_value(i)->get_type()->print();
    const_ir += " ";
    const_ir += get_element_value(i)->print();
    const_ir += ", ";
  }
  const_ir.pop_back();
  const_ir.pop_back();
  const_ir += "]";
  return const_ir;
}
/*!
 *@brief 常量整数类构造函数
 *@param ty 常量类型
 *@param val 常量数值类型数组
 *@return 自身类对象
 *constant int zero
 */
ConstantZero *ConstantZero::get(Type *ty, Module *m) {
  return new ConstantZero(ty);
}
/*!
 *@brief 打印常量零值
 *@return 字符串
 *constant int zero
 */
std::string ConstantZero::print() { return "zeroinitializer"; }
