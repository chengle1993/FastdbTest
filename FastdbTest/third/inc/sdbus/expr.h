
#ifndef __SDBUS_EXPR_H
#define __SDBUS_EXPR_H 1

#include "sdbus/compile.h"
#include "sdbus/type.h"
#include "sdbus/string.h"
#include "sdbus/exprex.h"

namespace sdbus{

/*
    比较友好的条件设置，类似于SQL的WHERE 条件。但是有个限制是同名之间是OR，不同名字之间是AND。如下：
    (Name = "ABC" OR Name = "BCD") AND Yield > 0.5
*/
class SDBUSAPI ExprNode{
public:
    ExprNode() ;
    virtual ~ExprNode() ;

    ExprNode(const ExprNode& node) ;
    ExprNode& operator=(const ExprNode& node) ;

    ExprNode * Parent ;
    ExprNode * Left ;
    ExprNode * Right ;

    int Type ;

    //单词类型
    static const int kNodeUnknown   =   0 ;
    static const int kNodeLogical   =   1 ;     //逻辑运算符 OR 、AND
    static const int kNodeArith     =   2 ;     //算术运算符 > < = ..
    static const int kNodeDeguacus  =   3 ;     //小括号

    void Free() ;

    bool operator==(const ExprNode& node) const ;
    bool operator!=(const ExprNode& node) const ;

    virtual int Scan(const char * str , int len) ;
    virtual const sdbus::string ToString() const ;

    static const ExprNode EMPTY ;

private:
    void Assign(const ExprNode& node) ;
} ;

class SDBUSAPI ArithNode : public ExprNode{
public:
    ArithNode() ;
    virtual ~ArithNode() ;

    int      Tag ;
    sdbus::string Name ;
    sdbus::string Operator ;
    sdbus::string Value ;

    virtual int Scan(const char * str , int len) ;

    virtual const sdbus::string ToString() const ;
} ;

class SDBUSAPI LogicalNode : public ExprNode{
public:
    LogicalNode() ;
    virtual ~LogicalNode() ;

    sdbus::string Operator ;

    virtual const sdbus::string ToString() const ;
} ;

class SDBUSAPI DeguacusNode : public ExprNode{
public:
    DeguacusNode() ;
    virtual ~DeguacusNode() ;

    virtual int Scan(const char * str , int len) ;

    //扫描小括号之间的内容范围
    int ScanCont(const char * str , int len , const char *&dstr , int&dlen) ;

    //解析小括号之间的内容
    int ParseCont(const char * str , int slen) ;

    sdbus::string Operator ;

    virtual const sdbus::string ToString() const ;
private:
    bool AddArith(ArithNode * arith) ;
    bool AddLogical(LogicalNode * logical) ;
} ;


class SDBUSAPI Expr{
public:
    Expr() ;
    ~Expr() ;

    bool Parse(const char * expr) ;

    void Clear() ;

    /*
        后序遍历
    */
    const ExprNode* First() const ;
    const ExprNode* Next(const ExprNode* node) const ;

     static int SkipSpace(const char * str , int slen) ;

     /*
        以[A-Z , a-z , _]开头，后续跟着[A-Z , a-z , _ , 0-9]，如果是OR , AND，那么将被变成大写。

        2014-06-06
        由于需要支持FID作为名字，因此需要在原来Name的基础上，加上全数字的支持。
     */
     static int ScanName(const char * str , int slen , const char *& name , int& nlen) ;


     static bool CheckLogical(const char * str , int slen , sdbus::string& upper) ;

     static bool CheckDigit(const char * str , int slen) ;

     /*
        扫描关系操作符号
     */
     static int ScanRelation(const char * str , int slen , const char *& oper , int& olen) ;

     /*
        扫描值，如果以""为界，那么取引号内部的内容，如果没有，则以空格为界
     */
     static int ScanValue(const char * str , int slen , const char *& value , int& vlen) ;

     /*
        扫描逻辑运算符，只能是"OR"或者是"AND"
     */
     static int ScanLogical(const char * str , int slen , sdbus::string& logical) ;
private:
    ExprNode * root_ ;

    const ExprNode * Left(const ExprNode * node) const ; 

    bool AddExpr(ExprNode * node) ;
    bool AddLogical(LogicalNode * node) ;
} ;

}

#endif  /**__SDBUS_EXPR_H  */

