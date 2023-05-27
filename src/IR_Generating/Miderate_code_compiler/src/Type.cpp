/*!
 *@file Type.h
 *@brief 类型接口定义文件
 *@version 1.0.0
 *@date 2022-10-04
 */
#include "Type.h"
#include "Module.h"

#include <cassert>

/**
 * @brief Construct a new Type object
 *
 * @param tid 类型
 * @param m 所属模块
 */
Type::Type(TypeID tid, Module *m) {
  tid_ = tid;
  m_ = m;
}
/**
 * @brief Get the module object，获取所属模块
 *
 * @return Module* 模块指针
 */
Module *Type::get_module() { return m_; }
/**
 * @brief 判断两个类型是否一致
 *
 * @return true 是
 * @return false 不是
 */
bool Type::is_eq_type(Type *ty1, Type *ty2) { return ty1 == ty2; }
/**
 * @brief 获取一个void类型指针
 * @param m 所属模块
 */
Type *Type::get_void_type(Module *m) { return m->get_void_type(); }
/**
 * @brief Get the label type object
 *
 * @param m 所属模块
 * @return Type* label类型指针
 */
Type *Type::get_label_type(Module *m) { return m->get_label_type(); }
/**
 * @brief Get the int1 type object
 *
 * @param m 所属模块
 * @return IntegerType* 1位整数类型指针
 */
IntegerType *Type::get_int1_type(Module *m) { return m->get_int1_type(); }
/**
 * @brief Get the int32 type object
 *
 * @param m 所属模块
 * @return IntegerType* 32位整数类型指针
 */
IntegerType *Type::get_int32_type(Module *m) { return m->get_int32_type(); }
/**
 * @brief Get the pointer type object
 *
 * @param contained 所指向的数据类型指针
 * @return PointerType* 指针类型指针
 */
PointerType *Type::get_pointer_type(Type *contained) {
  return PointerType::get(contained);
}
/**
 * @brief Get the array type object
 *
 * @param contained 数组类型
 * @param num_elements 数组元素个数
 * @return ArrayType* 数组类型指针
 */
ArrayType *Type::get_array_type(Type *contained, unsigned num_elements) {
  return ArrayType::get(contained, num_elements);
}
/**
 * @brief Get the int32 ptr type object
 *
 * @param m 所属模块
 * @return PointerType* 32位整数指针类型指针
 */
PointerType *Type::get_int32_ptr_type(Module *m) {
  return m->get_int32_ptr_type();
}
/**
 * @brief Get the float type object
 *
 * @param m 所属模块
 * @return FloatType* 浮点数据类型指针
 */
FloatType *Type::get_float_type(Module *m) { return m->get_float_type(); }
/**
 * @brief Get the float ptr type object
 *
 * @param m 所属模块
 * @return PointerType* 浮点数据指针类型指针
 */
PointerType *Type::get_float_ptr_type(Module *m) {
  return m->get_float_ptr_type();
}
/**
 * @brief Get the pointer element type object，获取指针指向数据的类型
 *
 * @return Type* 数据类型指针
 */
Type *Type::get_pointer_element_type() {
  if (this->is_pointer_type())
    return static_cast<PointerType *>(this)->get_element_type();
  else
    return nullptr;
}
/**
 * @brief Get the array element type object，获取数组元素数据类型
 *
 * @return Type* 数组元素数据类型指针
 */
Type *Type::get_array_element_type() {
  if (this->is_array_type())
    return static_cast<ArrayType *>(this)->get_element_type();
  else
    return nullptr;
}
/**
 * @brief Get the size object，获取对应类型数据大小
 *
 * @param extended 范围是否延申，即在判定数组指针时，是否计算指向的数组大小
 * @return int 大小
 * @note 如果是整数，那么返回32或1
 * @note 如果是数组，那么返回元素大小乘元素个数
 * @note 如果是指针，那么返回指针大小或是指针指向数组的大小
 */
int Type::get_size(bool extended) {
  if (this->is_integer_type()) {
    auto bits = static_cast<IntegerType *>(this)->get_num_bits() / 8;
    return bits > 0 ? bits : 1;
  }
  if (this->is_array_type()) {
    auto element_size =
        static_cast<ArrayType *>(this)->get_element_type()->get_size();
    auto num_elements = static_cast<ArrayType *>(this)->get_num_of_elements();
    return element_size * num_elements;
  }
  if (this->is_pointer_type()) {
    if (extended && this->get_pointer_element_type()->is_array_type()) {
      return this->get_pointer_element_type()->get_size();
    } else {
      return 4;
    }
  }
  return 0;
}
/**
 * @brief 打印类型
 *
 * @return std::string，字符串
 */
std::string Type::print() {
  std::string type_ir;
  switch (this->get_type_id()) {
  case VoidTyID:
    type_ir += "void";
    break;
  case LabelTyID:
    type_ir += "label";
    break;
  case IntegerTy32ID:
  case IntegerTy1ID:
    type_ir += "i";
    type_ir += std::to_string(static_cast<IntegerType *>(this)->get_num_bits());
    break;
  case FunctionTyID:
    type_ir += static_cast<FunctionType *>(this)->get_return_type()->print();
    type_ir += " (";
    for (int i = 0;
         i < (int)static_cast<FunctionType *>(this)->get_num_of_args(); i++) {
      if (i)
        type_ir += ", ";
      type_ir += static_cast<FunctionType *>(this)->get_param_type(i)->print();
    }
    type_ir += ")";
    break;
  case PointerTyID:
    type_ir += this->get_pointer_element_type()->print();
    type_ir += "*";
    break;
  case ArrayTyID:
    type_ir += "[";
    type_ir +=
        std::to_string(static_cast<ArrayType *>(this)->get_num_of_elements());
    type_ir += " x ";
    type_ir += static_cast<ArrayType *>(this)->get_element_type()->print();
    type_ir += "]";
    break;
  case FloatTyID:
    type_ir += "float";
    break;
  default:
    break;
  }
  return type_ir;
}
/**
 * @brief Construct a new Void Type object
 *
 * @param m 所属模块
 */
VoidType::VoidType(Module *m) : Type(Type::VoidTyID, m) {}
/**
 * @brief Construct a new Integer Type object
 *
 * @param num_bits 位数：32/1
 * @param m 所属模块
 */
IntegerType::IntegerType(unsigned num_bits, Module *m)
    : Type(num_bits == 1 ? Type::IntegerTy1ID : Type::IntegerTy32ID, m),
      num_bits_(num_bits) {}
/**
 * @brief 获取一个构建好的整数类型指针
 *
 * @param num_bits 位数
 * @param m 所属模块
 * @return IntegerType*
 */
IntegerType *IntegerType::get(unsigned num_bits, Module *m) {
  return new IntegerType(num_bits, m);
}
/**
 * @brief Get the num bits object，获取整数类型对应的位数
 *
 * @return unsigned 位数结果
 */
unsigned IntegerType::get_num_bits() { return num_bits_; }
/**
 * @brief Construct a new Function Type object
 *
 * @param result 返回参数类型
 * @param params 参数类型列表
 *
 * @return 创建对象本身
 */
FunctionType::FunctionType(Type *result, std::vector<Type *> params)
    : Type(Type::FunctionTyID, nullptr) {
  assert(is_valid_return_type(result) && "Invalid return type for function!");
  result_ = result;

  for (auto p : params) {
    // assert(is_valid_argument_type(p) && "Not a valid type for function
    // argument!");
    args_.push_back(p);
  }
}
/**
 * @brief 判断返回类型是否有效
 *
 * @param ty 待判定的返回类型指针
 * @return true
 * @return false
 */
bool FunctionType::is_valid_return_type(Type *ty) {
  return ty->is_integer_type() || ty->is_void_type() || ty->is_float_type();
}
/**
 * @brief 判断参数类型是否有效
 *
 * @param ty 待判定的参数类型指针
 * @return true
 * @return false
 */
bool FunctionType::is_valid_argument_type(Type *ty) {
  return ty->is_integer_type() || ty->is_pointer_type() || ty->is_float_type();
}
/**
 * @brief 创建一个函数类型指针
 *
 * @param result 返回参数类型指针
 * @param params 参数类型指针数组
 * @return FunctionType* 函数类型指针
 */
FunctionType *FunctionType::get(Type *result, std::vector<Type *> params) {
  return new FunctionType(result, params);
}
/**
 * @brief Get the num of args object，获取参数个数
 *
 * @return unsigned 参数数量
 */
unsigned FunctionType::get_num_of_args() const { return args_.size(); }
/**
 * @brief Get the param type object，获取参数类型
 *
 * @param i 要获取的参数索引
 * @return Type* 参数类型结果
 */
Type *FunctionType::get_param_type(unsigned i) const { return args_[i]; }
/**
 * @brief Get the return type object，获取返回类型的指针
 *
 * @return Type* 返回类型结果指针
 */
Type *FunctionType::get_return_type() const { return result_; }
/**
 * @brief Construct a new Array Type object，创建一个数组类型对象
 *
 * @param contained 数组类型
 * @param num_elements 数组元素个数
 */
ArrayType::ArrayType(Type *contained, unsigned num_elements)
    : Type(Type::ArrayTyID, contained->get_module()),
      num_elements_(num_elements) {
  assert(is_valid_element_type(contained) &&
         "Not a valid type for array element!");
  contained_ = contained;
}
/**
 * @brief 判断元素类型是否有效
 *
 * @param ty 元素类型
 * @return true
 * @return false
 */
bool ArrayType::is_valid_element_type(Type *ty) {
  return ty->is_integer_type() || ty->is_array_type() || ty->is_float_type();
}
/**
 * @brief 创建一个数组类型，并返回指针
 *
 * @param contained 数组元素类型
 * @param num_elements 数组元素个数
 * @return ArrayType* 数组元素类型
 */
ArrayType *ArrayType::get(Type *contained, unsigned num_elements) {
  return contained->get_module()->get_array_type(contained, num_elements);
}
/**
 * @brief Construct a new Pointer Type object
 *
 * @param contained 指针指向元素的类型
 */
PointerType::PointerType(Type *contained)
    : Type(Type::PointerTyID, contained->get_module()), contained_(contained) {}
/**
 * @brief 创建一个指针类型
 *
 * @param contained 指向元素的类型
 * @return PointerType* 指针类型指针
 */
PointerType *PointerType::get(Type *contained) {
  return contained->get_module()->get_pointer_type(contained);
}
/**
 * @brief Construct a new Float Type object，构建浮点类型
 *
 * @param m 所属模块
 */
FloatType::FloatType(Module *m) : Type(Type::FloatTyID, m) {}
