#pragma once

#include <cassert>
#include "functional.h"
#include <memory>
#include <typeinfo>

namespace fn {


////////////////////////////// functor_impl_base //////////////////////////////

template <typename _R>
struct functor_impl_base
{
	virtual ~functor_impl_base(void) {}

	virtual functor_impl_base<_R> * do_clone(void) const = 0;

	template <class U>
	static U * clone(U * pObj)
	{
		if (!pObj) return 0;
		U * pClone = static_cast<U *>(pObj->do_clone());
		assert(typeid(*pClone) == typeid(*pObj));
		return pClone;
	}

private:
	functor_impl_base & operator=(functor_impl_base const &);
};


////////////////////////////// functorx_impl //////////////////////////////

template <typename _R>
class functor0_impl : public functor_impl_base<_R>, public function0<_R>
{
public:
	virtual result_type operator()(void) = 0;
};

template <typename _R, typename _A1>
class functor1_impl : public functor_impl_base<_R>, public function1<_R, _A1>
{
public:
	virtual result_type operator()(argument1_type a1) = 0;
};

template <typename _R, typename _A1, typename _A2>
class functor2_impl : public functor_impl_base<_R>, public function2<_R, _A1, _A2>
{
public:
	virtual result_type operator()(argument1_type a1, argument2_type a2) = 0;
};

template <typename _R, typename _A1, typename _A2, typename _A3>
class functor3_impl : public functor_impl_base<_R>, public function3<_R, _A1, _A2, _A3>
{
public:
	virtual result_type operator()(argument1_type a1, argument2_type a2, argument3_type a3) = 0;
};

template <typename _R, typename _A1, typename _A2, typename _A3, typename _A4>
class functor4_impl : public functor_impl_base<_R>, public function4<_R, _A1, _A2, _A3, _A4>
{
public:
	virtual result_type operator()(argument1_type a1, argument2_type a2, argument3_type a3, argument4_type a4) = 0;
};

template <typename _R, typename _A1, typename _A2, typename _A3, typename _A4, typename _A5>
class functor5_impl : public functor_impl_base<_R>, public function5<_R, _A1, _A2, _A3, _A4, _A5>
{
public:
	virtual result_type operator()(argument1_type a1, argument2_type a2, argument3_type a3, argument4_type a4, argument5_type a5) = 0;
};


////////////////////////////// functorx_handler //////////////////////////////

#define DEFINE_CLONE_FUNCTORIMPL(Cls) \
	functor_impl_base<result_type> * do_clone(void) const { return new Cls(*this); }

template <class parent_functor, class fun>
class functor0_handler : public functor0_impl<typename parent_functor::result_type>
{
public:
	functor0_handler(fun const & f) : _m_fun(f) {}

    DEFINE_CLONE_FUNCTORIMPL(functor0_handler)

	result_type operator()(void) { return _m_fun(); }

private:
	fun _m_fun;
};

template <class parent_functor, class fun>
class functor1_handler : public functor1_impl<typename parent_functor::result_type, typename parent_functor::argument1_type>
{
public:
	functor1_handler(fun const & f) : _m_fun(f) {}

	DEFINE_CLONE_FUNCTORIMPL(functor1_handler)

	result_type operator()(argument1_type a1) { return _m_fun(a1); }

private:
	fun _m_fun;
};

template <class parent_functor, class fun>
class functor2_handler : public functor2_impl<typename parent_functor::result_type, typename parent_functor::argument1_type, typename parent_functor::argument2_type>
{
public:
	functor2_handler(fun const & f) : _m_fun(f) {}

	DEFINE_CLONE_FUNCTORIMPL(functor2_handler)

	result_type operator()(argument1_type a1, argument2_type a2) { return _m_fun(a1, a2); }

private:
	fun _m_fun;
};

template <class parent_functor, class fun>
class functor3_handler : public functor3_impl<typename parent_functor::result_type, typename parent_functor::argument1_type, typename parent_functor::argument2_type, typename parent_functor::argument3_type>
{
public:
	functor3_handler(fun const & f) : _m_fun(f) {}

	DEFINE_CLONE_FUNCTORIMPL(functor3_handler)

	result_type operator()(argument1_type a1, argument2_type a2, argument3_type a3) { return _m_fun(a1, a2, a3); }

private:
	fun _m_fun;
};

template <class parent_functor, class fun>
class functor4_handler : public functor4_impl<typename parent_functor::result_type, typename parent_functor::argument1_type, typename parent_functor::argument2_type, typename parent_functor::argument3_type, typename parent_functor::argument4_type>
{
public:
	functor4_handler(fun const & f) : _m_fun(f) {}

	DEFINE_CLONE_FUNCTORIMPL(functor4_handler)

	result_type operator()(argument1_type a1, argument2_type a2, argument3_type a3, argument4_type a4) { return _m_fun(a1, a2, a3, a4); }

private:
	fun _m_fun;
};

template <class parent_functor, class fun>
class functor5_handler : public functor5_impl<typename parent_functor::result_type, typename parent_functor::argument1_type, typename parent_functor::argument2_type, typename parent_functor::argument3_type, typename parent_functor::argument4_type, typename parent_functor::argument5_type>
{
public:
	functor5_handler(fun const & f) : _m_fun(f) {}

	DEFINE_CLONE_FUNCTORIMPL(functor5_handler)

	result_type operator()(argument1_type a1, argument2_type a2, argument3_type a3, argument4_type a4, argument5_type a5) { return _m_fun(a1, a2, a3, a4, a5); }

private:
	fun _m_fun;
};


////////////////////////////// functorx //////////////////////////////

template <typename functor_impl>
class functor_base
{
public:
	typedef functor_impl * (std::auto_ptr<functor_impl>::*unspecified_bool_type)(void) const;

	operator unspecified_bool_type(void) const
	{
		return _m_pImpl.get() ? &std::auto_ptr<functor_impl>::get : 0;
	}

protected:
	functor_base() : _m_pImpl(0) {}

	functor_base(functor_base const & rhs) : _m_pImpl(functor_impl::clone(rhs._m_pImpl.get())) {}

	explicit functor_base(std::auto_ptr<functor_impl> pImpl) : _m_pImpl(pImpl) {}

	functor_base & operator=(functor_base const & rhs)
	{
		_m_pImpl.reset(functor_impl::clone(rhs._m_pImpl.get()));
		return *this;
	}

protected:
	std::auto_ptr<functor_impl> _m_pImpl;
};

template <typename _R>
class functor0 : public functor_base< functor0_impl<_R> >
{
public:
	typedef functor0_impl<_R> functor_impl;

	functor0() : functor_base<functor_impl>() {}

	functor0(functor0 const & rhs) : functor_base<functor_impl>(rhs) {}

	explicit functor0(std::auto_ptr<functor_impl> pImpl) : functor_base<functor_impl>(pImpl) {}

	template <class fun>
	functor0(fun const & f) : functor_base<functor_impl>(std::auto_ptr<functor_impl>(new functor0_handler<functor_impl, fun>(f))) {}

	typename functor_impl::result_type operator()(void) const { return (*_m_pImpl)(); }
};

template <typename _R, typename _A1>
class functor1 : public functor_base< functor1_impl<_R, _A1> >
{
public:
	typedef functor1_impl<_R, _A1> functor_impl;

	functor1() : functor_base<functor_impl>() {}

	functor1(functor1 const & rhs) : functor_base<functor_impl>(rhs) {}

	explicit functor1(std::auto_ptr<functor_impl> pImpl) : functor_base<functor_impl>(pImpl) {}

	template <class fun>
	functor1(fun const & f) : functor_base<functor_impl>(std::auto_ptr<functor_impl>(new functor1_handler<functor_impl, fun>(f))) {}

	typename functor_impl::result_type operator()(typename functor_impl::argument1_type a1) const { return (*_m_pImpl)(a1); }
};

template <typename _R, typename _A1, typename _A2>
class functor2 : public functor_base< functor2_impl<_R, _A1, _A2> >
{
public:
	typedef functor2_impl<_R, _A1, _A2> functor_impl;

	functor2() : functor_base<functor_impl>() {}

	functor2(functor2 const & rhs) : functor_base<functor_impl>(rhs) {}

	explicit functor2(std::auto_ptr<functor_impl> pImpl) : functor_base<functor_impl>(pImpl) {}

	template <class fun>
	functor2(fun const & f) : functor_base<functor_impl>(std::auto_ptr<functor_impl>(new functor2_handler<functor_impl, fun>(f))) {}

	typename functor_impl::result_type operator()(typename functor_impl::argument1_type a1, typename functor_impl::argument2_type a2) const { return (*_m_pImpl)(a1, a2); }
};

template <typename _R, typename _A1, typename _A2, typename _A3>
class functor3 : public functor_base< functor3_impl<_R, _A1, _A2, _A3> >
{
public:
	typedef functor3_impl<_R, _A1, _A2, _A3> functor_impl;

	functor3() : functor_base<functor_impl>() {}

	functor3(functor3 const & rhs) : functor_base<functor_impl>(rhs) {}

	explicit functor3(std::auto_ptr<functor_impl> pImpl) : functor_base<functor_impl>(pImpl) {}

	template <class fun>
	functor3(fun const & f) : functor_base<functor_impl>(std::auto_ptr<functor_impl>(new functor3_handler<functor_impl, fun>(f))) {}

	typename functor_impl::result_type operator()(typename functor_impl::argument1_type a1, typename functor_impl::argument2_type a2, typename functor_impl::argument3_type a3) const { return (*_m_pImpl)(a1, a2, a3); }
};

template <typename _R, typename _A1, typename _A2, typename _A3, typename _A4>
class functor4 : public functor_base< functor4_impl<_R, _A1, _A2, _A3, _A4> >
{
public:
	typedef functor4_impl<_R, _A1, _A2, _A3, _A4> functor_impl;

	functor4() : functor_base<functor_impl>() {}

	functor4(functor4 const & rhs) : functor_base<functor_impl>(rhs) {}

	explicit functor4(std::auto_ptr<functor_impl> pImpl) : functor_base<functor_impl>(pImpl) {}

	template <class fun>
	functor4(fun const & f) : functor_base<functor_impl>(std::auto_ptr<functor_impl>(new functor4_handler<functor_impl, fun>(f))) {}

	typename functor_impl::result_type operator()(typename functor_impl::argument1_type a1, typename functor_impl::argument2_type a2, typename functor_impl::argument3_type a3, typename functor_impl::argument4_type a4) const { return (*_m_pImpl)(a1, a2, a3, a4); }
};

template <typename _R, typename _A1, typename _A2, typename _A3, typename _A4, typename _A5>
class functor5 : public functor_base< functor5_impl<_R, _A1, _A2, _A3, _A4, _A5> >
{
public:
	typedef functor5_impl<_R, _A1, _A2, _A3, _A4, _A5> functor_impl;

	functor5() : functor_base<functor_impl>() {}

	functor5(functor5 const & rhs) : functor_base<functor_impl>(rhs) {}

	explicit functor5(std::auto_ptr<functor_impl> pImpl) : functor_base<functor_impl>(pImpl) {}

	template <class fun>
	functor5(fun const & f) : functor_base<functor_impl>(std::auto_ptr<functor_impl>(new functor5_handler<functor_impl, fun>(f))) {}

	typename functor_impl::result_type operator()(typename functor_impl::argument1_type a1, typename functor_impl::argument2_type a2, typename functor_impl::argument3_type a3, typename functor_impl::argument4_type a4, typename functor_impl::argument5_type a5) const { return (*_m_pImpl)(a1, a2, a3, a4, a5); }
};

}	// namespace fn
