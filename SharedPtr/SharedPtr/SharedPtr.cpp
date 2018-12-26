/*
	https://stepik.org/lesson/4650/step/5?unit=1055

	Для ScopedPtr мы запретили копирование, однако, копирование можно и разрешить. Это позволит реализовать более продвинутый умный указатель — SharedPtr. 
	Он отличается от ScopedPtr тем, что кроме хранения указателя на объект, он хранит еще и счетчик ссылок (количество объектов SharedPtr, которые хранят один 
	и тот же указатель).

	Имея такой счетчик, мы можем определить момент, когда на объект, выделенный в куче, не останется больше ссылок (когда счетчик ссылок станет равным 0), 
	и освободить память.

	Поддержка счетчика ссылок состоит из нескольких частей:

    в конструкторе SharedPtr от ненулевого указателя мы инициализируем счетчик ссылок в 1 (конструктор создает первый SharedPtr, который хранит указатель),
    в конструкторе копирования мы увеличиваем счетчик ссылок на 1, если копируемый SharedPtr содержит ненулевой указатель (конструктор копирования создает еще 
	один SharedPtr с указателем на тот же самый объект), в деструкторе мы уменьшаем значение счетчика на 1, если в объекте SharedPtr хранится ненулевой указатель 
	(мы удаляем один SharedPtr, который указывает на объект в куче), оператор присваивания уменьшает счетчик ссылок левого операнда на 1, если внутри левого 
	SharedPtr хранится ненулевой указатель, увеличивает счетчик правого SharedPtr на 1, если в правом SharedPtr хранится ненулевой указатель (обычное дело для 
	оператора присваивания — сначала освобождаем старые ресурсы, потом выделяем новые, но при этом нужно быть особенно внимательным с присваиванием самому себе).

	Для класса SharedPtr могут оказаться полезными следующие методы (кроме операторов * и ->, конструктора копирования, оператора присваивания, деструктора и конструктора):

    метод get, как и в случае со ScopedPtr,
    метод reset — аналогичен reset у ScopedPtr, но освобождает память, только если счетчик ссылок после декремента равен 0.

	Реализуйте класс SharedPtr
*/

#include <iostream>
#include <memory>

#define ENABLE_DEBUG_PRINT

#ifdef ENABLE_DEBUG_PRINT
#define DEBUG_PRINT(msg, ptr)  if (!ptr) { std::cout << msg << " nullptr" << std::endl; } else { std::cout << msg << *ptr << std::endl; }
#else
#define DEBUG_PRINT(msg, ptr)	42;
#endif  

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


struct SharedPtr
{
	// реализуйте следующие методы:
	// устанавливаем значение *ptr_ и счетчик ссылок = 1
	explicit SharedPtr(Expression *ptr = 0)
		: ptr_(ptr)
		, ref_count_(nullptr)
	{
		if (ptr_)
			ref_count_ = new size_t(1);

		DEBUG_PRINT("Called object constructor, ref_count = ", ref_count_);
	};

	// в конструкторе копирования мы увеличиваем счетчик ссылок на 1, если копируемый SharedPtr содержит ненулевой указатель(конструктор копирования создает еще 
	// один SharedPtr с указателем на тот же самый объект),
	SharedPtr(SharedPtr& sptr) {
		ptr_ = sptr.ptr_;
		ref_count_ = sptr.ref_count_;
		if (ptr_) 
			++(*ref_count_);

		DEBUG_PRINT("Called copy constructor, ref_count = ", ref_count_); 		
	};

	// оператор присваивания уменьшает счетчик ссылок левого операнда на 1, если внутри левого SharedPtr хранится ненулевой указатель, увеличивает счетчик правого 
	// SharedPtr на 1, если в правом SharedPtr хранится ненулевой указатель (обычное дело для оператора присваивания — сначала освобождаем старые ресурсы, потом выделяем 
	// новые, но при этом нужно быть особенно внимательным с присваиванием самому себе).
	SharedPtr& operator=(const SharedPtr& right)
	{
		if (this == &right)
			return *this;

		if (ptr_)
			if (!--(*ref_count_))
			{
				delete ptr_; ptr_ = nullptr;
				delete ref_count_; ref_count_ = nullptr;
			}

		ptr_ = right.ptr_;
		ref_count_ = right.ref_count_;

		if (right.ptr_)
			++(*ref_count_);
		

		DEBUG_PRINT("Called operator =, ref_count = ", ref_count_);
		
		return *this;
	};

	// в деструкторе мы уменьшаем значение счетчика на 1, если в объекте SharedPtr хранится ненулевой указатель(мы удаляем один SharedPtr, который указывает на объект в куче)
	~SharedPtr() {
		if (ptr_)
		{
			if  (!--(*ref_count_))
			{
				delete ptr_; ptr_ = nullptr;
				delete ref_count_; ref_count_ = nullptr;
			}

		}
		DEBUG_PRINT("Called object destructor, ref_count = ", ref_count_);
	};

	// возвращает указатель, сохраненный внутри SharedPtr(например, чтобы передать его в какую - то функцию);
	Expression* get() const {
		return ptr_;
	};

	// Освобождает память, только если счетчик ссылок после декремента равен 0.
	void reset(Expression *ptr = 0) {
		if (ptr_)
			if (!--(*ref_count_))
			{
				delete ptr_;
				delete ref_count_;
			}

		ptr_ = ptr;
		ref_count_ = nullptr;
		if (ptr)
			ref_count_ = new size_t(1);
	};

	Expression& operator*() const {
		return *ptr_;
	};

	Expression* operator->() const {
		return ptr_;
	};

private:
	Expression *ptr_;
	size_t *ref_count_;
};

int main()
{
	// создаем shared pointer для выражения
	SharedPtr sp_expr(new BinaryOperation(new Number(3), '+', new BinaryOperation(new Number(4.5), '*', new Number(5))));

	// копирование
	SharedPtr sp_copy = sp_expr;
	SharedPtr sp_null(0);
	
	// самоприсваивание
	sp_copy = sp_copy;

	sp_null = sp_copy;

	return 0;
}