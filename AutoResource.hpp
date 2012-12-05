#pragma once

#pragma warning(push)
#pragma warning(disable: 4995)
#include <stdexcept>
#pragma warning(pop)
#include <algorithm>	// for std::swap

template <typename T>
class AutoResource
{
	class DeleterBase
	{
	public:
		virtual ~DeleterBase() {}
		virtual void Delete(void*) = 0;
	};

	template <typename D>
	class DeleterImpl
		: public DeleterBase
	{
		D deleter_;

	public:
		DeleterImpl(D deleter)
			: deleter_(deleter)
		{
		} 

		virtual ~DeleterImpl()
		{
		}

		void Delete(void* param)
		{
			deleter_(static_cast<T>(param));
		}
	};

private:
	DeleterBase* pDeleter_;

protected:
	T res_;

public:
	template <typename D>
	AutoResource(T res, D deleter)
		: pDeleter_(new DeleterImpl<D>(deleter))
		, res_(res)
	{
	}

	AutoResource(AutoResource& other)
		: pDeleter_(other.pDeleter_)
		, res_(other.res_)
	{
		other.pDeleter_ = 0;
		other.res_ = 0;
	}

	const AutoResource& operator=(AutoResource& rhs)
	{
		AutoResource<T> temp(rhs);
		Close();
		std::swap(this->pDeleter_, temp.pDeleter_);
		std::swap(this->res_, temp.res_);

		return *this;
	}

	const AutoResource& operator=(T res)
	{
		Close();
		res_ = res;

		return *this;
	}

	virtual ~AutoResource()
	{
		try
		{
			if (res_ && pDeleter_)
			{
				pDeleter_->Delete(res_);
			}
		}
		catch (...) {}

		if (pDeleter_)
		{
			delete pDeleter_;
		}
	}

	T* operator&()
	{
		return &res_;
	}

	T operator->()
	{
		return res_;
	}

	operator T() const
	{
		return res_;
	}

	void Close()
	{
		if (IsValid())
		{
			if (res_ && !pDeleter_)
			{
				throw std::logic_error("cannot release because of no closer");
			}
			pDeleter_->Delete(res_);
			res_ = 0;
		}
	}

	virtual bool IsValid() const
	{
		return (res_ != 0 && res_ != INVALID_HANDLE_VALUE);
	}
};
