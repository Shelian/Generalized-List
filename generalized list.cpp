#define _CRT_SECURE_NO_WARNINGS 1

#include <iostream>
#include<assert.h>
using namespace std;

enum Type
{
	HEAD,
	VALUE,
	SUB,
};

struct GeneralizedNode
{
	Type _type;				//类型
	GeneralizedNode* _next;	//指向同层下一个结点的指针

	GeneralizedNode()
		:_next(NULL)
	{}

	GeneralizedNode(Type type = HEAD, int value = 0)
		:_type(type)
		, _next(NULL)
	{
		if (type == SUB)
		{
			_subLink = NULL;
		}
		if (type == VALUE)
		{
			_value = value;
		}
	}

	union
	{
		int _value;
		GeneralizedNode* _subLink;      //指向子表的指针
	};

};

class Generalized  //广义表
{
public:
	Generalized()
		:_head(NULL)
	{}

	Generalized(const char* str)
		:_head(NULL)
	{
		_head = _CreatList(str);
	}

	Generalized(const Generalized& g)
	{
		_head = _Copy(g._head);
	}

	Generalized& operator=(Generalized g)
	{
		swap(_head, g._head);

		return *this;
	}

	~Generalized()
	{
		Destroy(_head);
	}

	void Print()
	{
		_Print(_head);
	}
	
	size_t Size()
	{
		return 	_Size(_head);
		
	}
	size_t Depth()
	{
		return _Depth(_head);
	}

protected:
	
	size_t _Depth(GeneralizedNode* head)
	{
		GeneralizedNode* cur = head;
		size_t depth = 1;
		while (cur)
		{
			if (cur->_type == HEAD)
			{
				cur = cur->_next;
			}

			else if (cur->_type == SUB)
			{
				GeneralizedNode* SubNode = cur;
				cur = cur->_next;
				size_t depth_ = _Depth(SubNode->_subLink) + 1;
				if (depth < depth_)
				{
					depth = depth_;
				}
			}

			else if (cur->_type == VALUE)
			{
				cur = cur->_next;
			}
		}
		return depth;
	}

	size_t _Size(GeneralizedNode* head)
	{
		GeneralizedNode* cur = head;
		size_t size = 0;
		while (cur)
		{
			if (cur->_type == HEAD)
			{
				cur = cur->_next;
			}

			else if (cur->_type == SUB)
			{
				GeneralizedNode* SubNode = cur;
				cur = cur->_next;
				size += _Size(SubNode->_subLink);
			}
			else if (cur->_type == VALUE)
			{
				size++;
				cur = cur->_next;
			}
		}
		return size;
	}

	void Destroy(GeneralizedNode* head)
	{
		GeneralizedNode* cur = head;
		GeneralizedNode* del;
		while (cur)
		{
			if (cur->_type == HEAD)
			{
				del = cur;
				cur = cur->_next;
				delete del;
			}
			else if (cur->_type == SUB)
			{
				Destroy(cur->_subLink);
				del = cur;
				cur = cur->_next;
				delete del;
			}

			else if (cur->_type == VALUE)
			{
				del = cur;
				cur = cur->_next;
				delete del;
			}
		}
	}

	GeneralizedNode* _Copy(GeneralizedNode* head)
	{
		GeneralizedNode* src = head;
		GeneralizedNode* head_ = new GeneralizedNode(HEAD);
		GeneralizedNode* dst = head_;
		src=src->_next;
		while (src)
		{
			if (src->_type == HEAD)
			{
				dst = new GeneralizedNode(HEAD);
				dst = dst->_next;
			}
			else if (src->_type == SUB)
			{
				GeneralizedNode* SubNode = new GeneralizedNode(SUB);
				dst->_next = SubNode;
				dst = dst->_next;
				dst->_subLink = _Copy(src->_subLink);
			}
			else if (src->_type == VALUE)
			{
				dst->_next = new GeneralizedNode(VALUE, src->_value);
				dst = dst->_next;
			}
			src = src->_next;
		}
		return head_;
	}

	void _Print(GeneralizedNode* head)
	{
		GeneralizedNode* cur = head;
		while (cur)
		{
			if (cur->_type == HEAD)
			{
				cout << '(';
			}
			else if (cur->_type == SUB)
			{
				_Print(cur->_subLink);
				if (cur->_next != NULL)
				{
					cout << ',';
				}
			}
			else if (cur->_type == VALUE)
			{
				cout << cur->_value;
				if (cur->_next!=NULL)
				cout << ',';
			}
			if (cur->_next == NULL)
			{
				cout << ')';
			}
			cur = cur->_next;
		}
	}

	GeneralizedNode* _CreatList(const char*& str) //创建广义表
	{
		assert(*str == '(');
		++str;
	    GeneralizedNode* head = new GeneralizedNode(HEAD);
		GeneralizedNode* cur = head;
		while (*str)
		{
			if (*str == '(')
			{
				GeneralizedNode* SubNode = new GeneralizedNode(SUB);
				cur->_next = SubNode;
				cur = cur->_next;
				SubNode->_subLink = _CreatList(str);
			}

			else if ((*str >= '0' && *str <= '9') ||
					 (*str >= 'a' && *str <= 'z') ||
					 (*str >= 'A' && *str <= 'Z'))
			{
				cur->_next = new GeneralizedNode(VALUE,*str);
				cur = cur->_next;
				str++;
			}

			else if (*str == ')')
			{
				str++;
				return head;
			}
			else
			{
				str++;
			}
		}
		return head;
	}
protected:
	GeneralizedNode* _head;
};




void test()
{
	Generalized gl("()");
	Generalized g2("(a, b)");
	Generalized g3("(a, b, (c, d))");
	Generalized g4("(a, b, (c, d), (e, (f), h))");
	g2.Print();
	cout << endl;

	g3.Print();
	cout << endl;

	g4.Print();
	cout << endl;

	Generalized g5 = g4;
	g5.Print();
	cout << endl;

	g4 = g3;
	g4.Print();

	cout << g5.Size() << endl;
	cout << g3.Depth() << endl;


}

int main()
{
	test();
	system("pause");
	return 0;
}