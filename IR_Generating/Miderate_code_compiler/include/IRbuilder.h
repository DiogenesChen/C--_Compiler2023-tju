#ifndef SYSYC_IRBUILDER_H
#define SYSYC_IRBUILDER_H

#include "BasicBlock.h"
#include "Instruction.h"
#include "Value.h"

class IRBuilder {
private:
  BasicBlock *BB_;
  Module *m_;
  Function *curfunc;

public:
  /*!
   *@brief irbuilder的构造函数
   *@param bb 基本块
   *@param m 模块
   *@return 当前对象本身
   */
  IRBuilder(BasicBlock *bb, Module *m) : BB_(bb), m_(m){};
  /*!
   *@brief irbuilder的析构函数
   */
  ~IRBuilder() = default;
  /*!
   *@brief 设置函数指针
   *@return 函数指针
   */
  void set_curFunc(Function *f) { this->curfunc = f; };
  /*!
   *@brief 获取函数指针
   *@return 函数指针
   */
  Function *get_curFunc() { return this->curfunc; }
  /*!
   *@brief 获取模块指针
   *@return 模块指针
   */
  Module *get_module() { return m_; }
  /*!
   *@brief 获取要插入的基本块
   *@return 基本块指针
   */
  BasicBlock *get_insert_block() { return this->BB_; }
  /*!
   *@brief 更新要进行修改的基本块
   *@param bb 基本块指针
   *@note 在某个基本块中插入指令
   */
  void set_insert_point(BasicBlock *bb) {
    this->BB_ = bb;
  } //在某个基本块中插入指令
  /*!
   *@brief 创建加法指令
   *@param lhs 左值指针
   *@param rhs 右值指针
   *@return 二元操作符加法指令指针
   *@note 调用指针类的创建函数
   */
  BinaryInst *create_iadd(Value *lhs, Value *rhs) {
    return BinaryInst::create_add(lhs, rhs, this->BB_, m_);
  } //创建加法指令（以及其他算术指令）
  /*!
   *@brief 创建减法指令
   *@param lhs 左值指针
   *@param rhs 右值指针
   *@return 二元操作符减法指令指针
   *@note 调用指针类的创建函数
   */
  BinaryInst *create_isub(Value *lhs, Value *rhs) {
    return BinaryInst::create_sub(lhs, rhs, this->BB_, m_);
  }
  /*!
   *@brief 创建乘法指令
   *@param lhs 左值指针
   *@param rhs 右值指针
   *@return 二元操作符乘法指令指针
   *@note 调用指针类的创建函数
   */
  BinaryInst *create_imul(Value *lhs, Value *rhs) {
    return BinaryInst::create_mul(lhs, rhs, this->BB_, m_);
  }
  /*!
   *@brief 创建除法指令
   *@param lhs 左值指针
   *@param rhs 右值指针
   *@return 二元操作符除法指令指针
   *@note 调用指针类的创建函数
   */
  BinaryInst *create_isdiv(Value *lhs, Value *rhs) {
    return BinaryInst::create_sdiv(lhs, rhs, this->BB_, m_);
  }
  /*!
   *@brief 创建模运算指令
   *@param lhs 左值指针
   *@param rhs 右值指针
   *@return 二元操作符模运算指令指针
   *@note 调用指针类的创建函数
   */
  BinaryInst *create_irem(Value *lhs, Value *rhs) {
    return BinaryInst::create_mod(lhs, rhs, this->BB_, m_);
  }
  /*!
   *@brief 创建与运算指令
   *@param lhs 左值指针
   *@param rhs 右值指针
   *@return 二元操作符除法指令指针
   *@note 调用指针类的创建函数
   */
  BinaryInst *create_iand(Value *lhs, Value *rhs) {
    return BinaryInst::create_sdiv(lhs, rhs, this->BB_, m_);
  }
  /*!
   *@brief 创建或运算指令
   *@param lhs 左值指针
   *@param rhs 右值指针
   *@return 二元操作符除法指令指针
   *@note 调用指针类的创建函数
   */
  BinaryInst *create_ior(Value *lhs, Value *rhs) {
    return BinaryInst::create_sdiv(lhs, rhs, this->BB_, m_);
  }
  /*!
   *@brief 创建比较相等指令
   *@param lhs 左值指针
   *@param rhs 右值指针
   *@return 二元操作符相等指令指针
   *@note 调用指针类的创建函数
   */

  CmpInst *create_icmp_eq(Value *lhs, Value *rhs) {
    return CmpInst::create_cmp(CmpInst::EQ, lhs, rhs, this->BB_, m_);
  }
  /*!
   *@brief 创建比较不等指令
   *@param lhs 左值指针
   *@param rhs 右值指针
   *@return 二元操作符不等指令指针
   *@note 调用指针类的创建函数
   */
  CmpInst *create_icmp_ne(Value *lhs, Value *rhs) {
    return CmpInst::create_cmp(CmpInst::NE, lhs, rhs, this->BB_, m_);
  }
  /*!
   *@brief 创建比较大于等于指令
   *@param lhs 左值指针
   *@param rhs 右值指针
   *@return 二元操作符大于等于指令指针
   *@note 调用指针类的创建函数
   */
  CmpInst *create_icmp_gt(Value *lhs, Value *rhs) {
    return CmpInst::create_cmp(CmpInst::GT, lhs, rhs, this->BB_, m_);
  }
  /*!
   *@brief 创建比较大于指令
   *@param lhs 左值指针
   *@param rhs 右值指针
   *@return 二元操作符大于指令指针
   *@note 调用指针类的创建函数
   */

  CmpInst *create_icmp_ge(Value *lhs, Value *rhs) {
    return CmpInst::create_cmp(CmpInst::GE, lhs, rhs, this->BB_, m_);
  }
  /*!
   *@brief 创建比较小于指令
   *@param lhs 左值指针
   *@param rhs 右值指针
   *@return 二元操作符比较小于指令指针
   *@note 调用指针类的创建函数
   */
  CmpInst *create_icmp_lt(Value *lhs, Value *rhs) {
    return CmpInst::create_cmp(CmpInst::LT, lhs, rhs, this->BB_, m_);
  }
  /*!
   *@brief 创建比较小于等于指令
   *@param lhs 左值指针
   *@param rhs 右值指针
   *@return 二元操作符比较小于指令指针
   *@note 调用指针类的创建函数
   */
  CmpInst *create_icmp_le(Value *lhs, Value *rhs) {
    return CmpInst::create_cmp(CmpInst::LE, lhs, rhs, this->BB_, m_);
  }
  /*!
   *@brief 创建调用指令
   *@param func 函数指针
   *@param args 函数参数value指针数组
   *@return 调用指令指针
   *@note 调用指针类的创建函数
   */
  CallInst *create_call(Value *func, std::vector<Value *> args) {
    assert(dynamic_cast<Function *>(func) && "func must be Function * type");
    return CallInst::create(static_cast<Function *>(func), args, this->BB_);
  }
  /*!
   *@brief 创建无条件跳转指令
   *@param if_true 基本块指针
   *@return 无条件跳转指令指针
   *@note 调用指针类的创建函数
   */
  BranchInst *create_br(BasicBlock *if_true) {
    return BranchInst::create_br(if_true, this->BB_);
  }

  /*!
   *@brief 创建有条件跳转指令
   *@param cond 条件
   *@param if_ture 条件为真的基本块指针
   *@param if_false 条件为假的基本块指针
   *@return 有条件跳转指令指针
   *@note 调用指针类的创建函数
   */
  BranchInst *create_cond_br(Value *cond, BasicBlock *if_true,
                             BasicBlock *if_false) {
    return BranchInst::create_cond_br(cond, if_true, if_false, this->BB_);
  }
  /*!
   *@brief 创建返回指令
   *@param val 返回值
   *@return 返回指令指针
   *@note 调用指针类的创建函数
   */
  ReturnInst *create_ret(Value *val) {
    return ReturnInst::create_ret(val, this->BB_);
  }
  /*!
   *@brief 创建空值返回指令
   *@return 无值返回指令指针
   *@note 调用指针类的创建函数
   */
  ReturnInst *create_void_ret() {
    return ReturnInst::create_void_ret(this->BB_);
  }
  /*!
   *@brief 创建指针指令
   *@param ptr 指令指针
   *@param rhs value指针数组
   *@return 元素指针指令指针
   *@note 调用指针类的创建函数
   */
  GetElementPtrInst *create_gep(Value *ptr, std::vector<Value *> idxs) {
    return GetElementPtrInst::create_gep(ptr, idxs, this->BB_);
  }
  /*!
   *@brief 创建存储指令
   *@param val value值指针
   *@param ptr 指针，指向存值的地址
   *@return 存储指令指针
   *@note 调用指针类的创建函数
   */
  StoreInst *create_store(Value *val, Value *ptr) {
    return StoreInst::create_store(val, ptr, this->BB_);
  }

  /*!
   *@brief 创建加载指令
   *@param ty 类型
   *@param ptr 指针，指向存值的地址
   *@return 加载指令指针
   *@note 调用指针类的创建函数
   */
  LoadInst *create_load(Type *ty, Value *ptr) {
    return LoadInst::create_load(ty, ptr, this->BB_);
  }

  /*!
   *@brief 创建加载指令
   *@param ptr value类型指针
   *@return 加载指令指针
   *@note 调用指针类的创建函数
   */
  LoadInst *create_load(Value *ptr) {
    assert(ptr->get_type()->is_pointer_type() && "ptr must be pointer type");
    return LoadInst::create_load(ptr->get_type()->get_pointer_element_type(),
                                 ptr, this->BB_);
  }
  /*!
   *@brief 创建申请内容指令
   *@param ty 类型
   *@return 申请内容指令指针
   *@note 调用指针类的创建函数
   */
  AllocaInst *create_alloca(Type *ty) {
    return AllocaInst::create_alloca(ty, this->BB_);
  }

  /*!
   *@brief 创建扩展指令
   *@param value value值指针
   *@param ty 新类型
   *@return 扩展指令指针
   *@note 调用指针类的创建函数
   */
  ZextInst *create_zext(Value *val, Type *ty) {
    return ZextInst::create_zext(val, ty, this->BB_);
  }
};

#endif // SYSYC_IRBUILDER_H
