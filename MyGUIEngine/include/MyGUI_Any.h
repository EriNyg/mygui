/*!
	@file
	@author		Albert Semenov
	@date		10/2008
	@module
*/

// -- Based on boost::any, original copyright information follows --
// Copyright Kevlin Henney, 2000, 2001, 2002. All rights reserved.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompAnying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// -- End original copyright --

#ifndef __MYGUI_ANY_H__
#define __MYGUI_ANY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"
#include <algorithm>
#include <typeinfo>

namespace MyGUI
{

	/** Usage example of class Any

	void f()
	{
		// RU: �������� �����, � �������� ������ ��� ����������
		// EN: test class, with simple types all is similar
		struct Data { int value; };

		// RU: ��������� � �������������
		// EN: instance and initialization
		Data data;
		data.value = 0xDEAD;

		// RU: ��������� ����� ������ Data
		// EN: copy of class Data will be created
		MyGUI::Any any = data;
		// RU: ����� ������ Data
		// EN: copy of class Data
		Data copy_data = *any.castType<Data>();
		// RU: ������ value == 0xDEAD
		// EN: now value == 0xDEAD
		int value = copy_data.value;


		// RU: ��������� ����� ��������� �� ����� Data
		// EN: copy of pointer on class Data will be created
		any = &data;
		// RU: ����� ��������� �� ����� Data � ��������� �� ������ data
		// EN: copy of pointer on class Data and on object data
		Data * copy_ptr = *any.castType<Data*>();
		// RU: ������ data.value == 0
		// EN: now value == 0xDEAD
		copy_ptr->value = 0;

	}
	
	*/

	class _MyGUIExport Any
	{

	private:
		struct AnyEmpty { };

	public:
		static AnyEmpty Null;

	public:
		Any() :
			mContent(null)
		{
		}

		template<typename ValueType> Any(const ValueType & value) :
			mContent(new Holder<ValueType>(value))
		{
		}

		Any(const Any::AnyEmpty & value) :
			mContent(null)
		{
		}

		Any(const Any & other) :
			mContent(other.mContent ? other.mContent->clone() : null)
		{
		}

		~Any()
		{
			delete mContent;
		}

		Any & swap(Any & rhs)
		{
			std::swap(mContent, rhs.mContent);
			return *this;
		}

		template<typename ValueType> Any & operator = (const ValueType & rhs)
		{
			Any(rhs).swap(*this);
			return *this;
		}

		Any & operator = (const Any::AnyEmpty & rhs)
		{
			delete mContent;
			mContent = null;
			return *this;
		}

		Any & operator = (const Any & rhs)
		{
			Any(rhs).swap(*this);
			return *this;
		}

		bool empty() const
		{
			return !mContent;
		}

		const std::type_info & getType() const
		{
			return mContent ? mContent->getType() : typeid(void);
		}

		template<typename ValueType> ValueType * castType(bool _throw = true)
		{
			if (this->getType() == typeid(ValueType)) {
				return & static_cast<Any::Holder<ValueType> *>(this->mContent)->held;
			}
			MYGUI_ASSERT(!_throw, "Bad cast from type '" << getType().name() << "' to '" << typeid(ValueType).name() << "'");
			return 0;
		}

	private:
		class Placeholder
		{
		public:
			virtual ~Placeholder() { }

		public:
			virtual const std::type_info & getType() const = 0;
			virtual Placeholder * clone() const = 0;

		};

		template<typename ValueType> class Holder : public Placeholder
		{
		public:
			Holder(const ValueType & value) :
				held(value)
			{
			}

		public:
			virtual const std::type_info & getType() const
			{
				return typeid(ValueType);
			}

			virtual Placeholder * clone() const
			{
				return new Holder(held);
			}

		public:
			ValueType held;

		private:
			Holder & operator=(const Holder &);

		};


		private: // representation
			Placeholder * mContent;

	};

} // namespace MyGUI

#endif // __MYGUI_ANY_H__
