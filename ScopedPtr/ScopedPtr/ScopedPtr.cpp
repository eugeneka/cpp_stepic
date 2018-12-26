/*
	https://stepik.org/lesson/4650/step/3?unit=1055

	Реализуйте ScopedPtr, который будет работать с указателями на базовый класс Expression. В этом задании вам требуется реализовать
	методы get, release и reset, операторы * и -> так, как это было описано в предыдущем степе. А также реализуйте конструктор ScopedPtr
	от указателя на Expression.

*/

#include <iostream>

struct Expression
{
	virtual double evaluate() const = 0;
	virtual ~Expression() {};
};

struct Number : Expression
{
	Number(double value)
		: value(value)
	{}

	~Number() {};

	double evaluate() const { return value; };
private:
	double value;
};

struct BinaryOperation : Expression
{
	/*
	  Здесь op это один из 4 символов: '+', '-', '*' или '/', соответствующих операциям,
	  которые вам нужно реализовать.
	 */
	BinaryOperation(Expression const * left, char op, Expression const * right)
		: left(left), op(op), right(right)
	{ }

	~BinaryOperation()
	{
		if (left) delete left;
		if (right) delete right;
	}

	double evaluate() const
	{
		switch (op)
		{
		case '+':
			return left->evaluate() + right->evaluate();
		case '-':
			return left->evaluate() - right->evaluate();
		case '*':
			return left->evaluate() * right->evaluate();
		case '/':
			return left->evaluate() / right->evaluate();
		default:
			return 0;
		}
	}

private:
	Expression const * left;
	Expression const * right;
	char op;
};


struct ScopedPtr
{
	// реализуйте следующие методы:
	//
	explicit ScopedPtr(Expression *ptr = 0)
		: ptr_(ptr)
	{};

	~ScopedPtr() {
		delete ptr_;
	};

	// возвращает указатель, сохраненный внутри ScopedPtr(например, чтобы передать его в какую - то функцию);
	Expression* get() const {
		return ptr_;
	};

	// забирает указатель у ScopedPtr и возвращает значение этого указателя, после вызова release ScopedPtr 
	// не должен освобождать память(например, чтобы вернуть этот указатель из функции);
	Expression* release() {
		Expression* pEx = ptr_;
		ptr_ = 0;
		return pEx;
	};

	// метод заставляет ScopedPtr освободить старый указатель, а вместо него захватить новый(например, чтобы переиспользовать
	// ScopedPtr, так как оператор присваивания запрещен).
	void reset(Expression *ptr = 0) {
		delete ptr_;
		ptr_ = ptr;
	};

	Expression& operator*() const {
		return *ptr_;
	};

	Expression* operator->() const {
		return ptr_;
	};


private:
	// запрещаем копирование ScopedPtr
	ScopedPtr(const ScopedPtr&);
	ScopedPtr& operator=(const ScopedPtr&);

	Expression *ptr_;
};

int main()
{
	// создаем scoped pointer для выражения
	ScopedPtr sp_expr(new BinaryOperation(new Number(3), '+', new BinaryOperation(new Number(4.5), '*', new Number(5))));

	// вычисляем и выводим результат: 25.5
	std::cout << sp_expr->evaluate() << std::endl;

	return 0;
}