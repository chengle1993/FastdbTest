/***************************************************************************************************
  *Copyright(C),2010-2017,Sumscope
  *FileName	:  SqlExpression.h
  *Author	:  scofined.qi
  *Version	:  1.0
  *Date		:  2018/08/02
  *Desc		:  解析sql语句中where后面的表达式,用以替换sdbus中的Expr类
  *Relation :  
  *Others	:  
  *Function :  
  *History	:  
***************************************************************************************************/
#ifndef SQLEXPRESSION_H_9203E9B7_F512_4E3C_B7BD_B1E6E1E2847E
#define SQLEXPRESSION_H_9203E9B7_F512_4E3C_B7BD_B1E6E1E2847E
#include "sdbus/string.h"
#include <vector>

#pragma warning(disable:4251)
namespace sdbus{
#define SQL_API SDBUSAPI 
	typedef string StringType;
	enum kExprLink{
		kExprLinkOr,					// or
		kExprLinkAnd,					// and
		kExprLinkCount,					// 
	};
	enum kExprType{
		kExprTypeNone,
		kExprTypeAtom,
		kExprTypeGroup
	};
	class AtomExpression;
	class GroupExpression;
	class SQL_API Expression{
	protected:
		kExprLink				m_link;		// 前向连接,or,and
		kExprType				m_type;		//
		GroupExpression*		m_parent;	//
	public:
		Expression(GroupExpression* parent, kExprLink link = kExprLinkOr) :m_link(link), m_parent(parent), m_type(kExprTypeNone){}
		virtual ~Expression(){}
		void SetLink(kExprLink link){ m_link = link; }
		kExprLink GetLink()const{ return m_link; }
		GroupExpression* GetParent()const{ return m_parent; }
		GroupExpression* GetRoot()const;
		kExprType GetType()const{ return m_type; }
	};
	class SQL_API AtomExpression : public Expression{
		StringType		m_comp;
		StringType		m_name;
		StringType		m_value;	// 保留首尾的引号
	public:
		AtomExpression(GroupExpression* parent, const StringType& comp) :Expression(parent), m_comp(comp){ m_type = kExprTypeAtom; }
		virtual ~AtomExpression(){}
		const StringType& GetComp()const{ return m_comp; }
		const StringType& GetName()const{ return m_name; }
		const StringType& GetValue()const{ return m_value; }
		void SetName(const char* str, int len){ m_name = (str && len > 0) ? StringType(str, len) : ""; }
		void SetValue(const char* str, int len){ m_value = (str && len > 0) ? StringType(str, len) : ""; }
		void SetValue(StringType&& str){ std::swap(m_value, str); }
	};
	class SQL_API GroupExpression : public Expression{
		std::vector<Expression*>		m_children;
	public:
		GroupExpression(GroupExpression* parent, kExprLink link) :Expression(parent, link){ m_type = kExprTypeGroup; }
		virtual ~GroupExpression()
		{
			for (int i = 0; i < (int)m_children.size(); i++)
			{
				delete m_children[i];
			}
			m_children.clear();
		}
		Expression* GetTail()const{ return m_children.size() > 0 ? m_children.at(m_children.size() - 1) : 0; }
		int size()const{ return (int)m_children.size(); }
		Expression* at(int index)const{ return m_children.at(index); }
		void AddChild(Expression* child);
	};

	class SqlExpressionImpl;
	class SQL_API SqlExpression {
		SqlExpressionImpl*	m_impl;
	private:
		SqlExpression(const SqlExpression&) = delete;
		SqlExpression& operator=(const SqlExpression& expr) = delete;
	public:
		SqlExpression();
		~SqlExpression();
		const GroupExpression* Parse(const char* str, int len);
		const AtomExpression* ParseFirstName(const char* str, int len, const char* szName);
		const std::string& GetLastError();
		static bool IsValid(const char* str, int len=0);
		static bool IsEqual(const char* str1, const char* str2);
	};
}

#endif
