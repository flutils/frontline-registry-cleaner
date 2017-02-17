#pragma once

namespace fn {

/*
template <typename T>
struct type_traits { typedef T & ParameterType; };

template <typename T>
struct type_traits<T *> { typedef T * ParameterType; };

template <typename T>
struct type_traits<T &> { typedef T & ParameterType; };

template <>
struct type_traits<char> { typedef char ParameterType; };

template <>
struct type_traits<signed char> { typedef signed char ParameterType; };

template <>
struct type_traits<unsigned char> { typedef unsigned char ParameterType; };

template <>
struct type_traits<signed short> { typedef signed short ParameterType; };

template <>
struct type_traits<unsigned short> { typedef unsigned short ParameterType; };

template <>
struct type_traits<signed int> { typedef signed int ParameterType; };

template <>
struct type_traits<unsigned int> { typedef unsigned int ParameterType; };

template <>
struct type_traits<signed long> { typedef signed long ParameterType; };

template <>
struct type_traits<unsigned long> { typedef unsigned long ParameterType; };

template <>
struct type_traits<signed __int64> { typedef signed __int64 ParameterType; };

template <>
struct type_traits<unsigned __int64> { typedef unsigned __int64 ParameterType; };

template <>
struct type_traits<float> { typedef float ParameterType; };

template <>
struct type_traits<double> { typedef double ParameterType; };

template <>
struct type_traits<long double> { typedef long double ParameterType; };
*/

////////////////////////////// ref_holder //////////////////////////////

template<class T> class ref_holder
{ 
public:
	typedef T type;

	explicit ref_holder(T & t): _m_T(t) {}

	operator T & (void) const { return _m_T; }

	T & get(void) const { return _m_T; }

	T * get_pointer(void) const { return &_m_T; }

private:
	T & _m_T;
};

template<class T>
inline ref_holder<T> const ref(T & t) { return ref_holder<T>(t); }

template<class T>
inline ref_holder<T const> const cref(T const & t) { return ref_holder<T const>(t); }

class not_assignable
{
private:
	not_assignable & operator=(not_assignable const &);
};

////////////////////////////// functionx //////////////////////////////

template <typename _R>
struct function0
{
	typedef _R result_type;
};

template <typename _R, typename _A1>
struct function1
{
	typedef _R result_type;
	typedef _A1 argument1_type;
};

template <typename _R, typename _A1, typename _A2>
struct function2
{
	typedef _R result_type;
	typedef _A1 argument1_type;
	typedef _A2 argument2_type;
};

template <typename _R, typename _A1, typename _A2, typename _A3>
struct function3
{
	typedef _R result_type;
	typedef _A1 argument1_type;
	typedef _A2 argument2_type;
	typedef _A3 argument3_type;
};

template <typename _R, typename _A1, typename _A2, typename _A3, typename _A4>
struct function4
{
	typedef _R result_type;
	typedef _A1 argument1_type;
	typedef _A2 argument2_type;
	typedef _A3 argument3_type;
	typedef _A4 argument4_type;
};

template <typename _R, typename _A1, typename _A2, typename _A3, typename _A4, typename _A5>
struct function5
{
	typedef _R result_type;
	typedef _A1 argument1_type;
	typedef _A2 argument2_type;
	typedef _A3 argument3_type;
	typedef _A4 argument4_type;
	typedef _A5 argument5_type;
};


////////////////////////////// ptr_funx_t //////////////////////////////

template<typename _R>
class ptr_fun0_t : public function0<_R>, private not_assignable
{
public:
	typedef _R (* ptr_fun_type)(void);

	explicit ptr_fun0_t(ptr_fun_type pFun) : _m_pFun(pFun) {}

	_R operator()(void) const { return _m_pFun(); }

private:
	ptr_fun_type _m_pFun;
};

template<typename _R>
inline ptr_fun0_t<_R> ptr_fun(_R (* pFun)(void)) { return ptr_fun0_t<_R>(pFun); }

template<typename _R, typename _A1>
class ptr_fun1_t : public function1<_R, _A1>, private not_assignable
{
public:
	typedef _R (* ptr_fun_type)(_A1);

	explicit ptr_fun1_t(ptr_fun_type pFun) : _m_pFun(pFun) {}

	_R operator()(_A1 a1) const { return _m_pFun(a1); }

private:
	ptr_fun_type _m_pFun;
};

template<typename _R, typename _A1>
inline ptr_fun1_t<_R, _A1> ptr_fun(_R (* pFun)(_A1)) { return ptr_fun1_t<_R, _A1>(pFun); }

template<typename _R, typename _A1, typename _A2>
class ptr_fun2_t : public function2<_R, _A1, _A2>, private not_assignable
{
public:
	typedef _R (* ptr_fun_type)(_A1, _A2);

	explicit ptr_fun2_t(ptr_fun_type pFun) : _m_pFun(pFun) {}

	_R operator()(_A1 a1, _A2 a2) const { return _m_pFun(a1, a2); }

private:
	ptr_fun_type _m_pFun;
};

template<typename _R, typename _A1, typename _A2>
inline ptr_fun2_t<_R, _A1, _A2> ptr_fun(_R (* pFun)(_A1, _A2)) { return ptr_fun2_t<_R, _A1, _A2>(pFun); }

template<typename _R, typename _A1, typename _A2, typename _A3>
class ptr_fun3_t : public function3<_R, _A1, _A2, _A3>, private not_assignable
{
public:
	typedef _R (* ptr_fun_type)(_A1, _A2, _A3);

	explicit ptr_fun3_t(ptr_fun_type pFun) : _m_pFun(pFun) {}

	_R operator()(_A1 a1, _A2 a2, _A3 a3) const { return _m_pFun(a1, a2, a3); }

private:
	ptr_fun_type _m_pFun;
};

template<typename _R, typename _A1, typename _A2, typename _A3>
inline ptr_fun3_t<_R, _A1, _A2, _A3> ptr_fun(_R (* pFun)(_A1, _A2, _A3)) { return ptr_fun3_t<_R, _A1, _A2, _A3>(pFun); }

template<typename _R, typename _A1, typename _A2, typename _A3, typename _A4>
class ptr_fun4_t : public function4<_R, _A1, _A2, _A3, _A4>, private not_assignable
{
public:
	typedef _R (* ptr_fun_type)(_A1, _A2, _A3, _A4);

	explicit ptr_fun4_t(ptr_fun_type pFun) : _m_pFun(pFun) {}

	_R operator()(_A1 a1, _A2 a2, _A3 a3, _A4 a4) const { return _m_pFun(a1, a2, a3, a4); }

private:
	ptr_fun_type _m_pFun;
};

template<typename _R, typename _A1, typename _A2, typename _A3, typename _A4>
inline ptr_fun4_t<_R, _A1, _A2, _A3, _A4> ptr_fun(_R (* pFun)(_A1, _A2, _A3, _A4)) { return ptr_fun4_t<_R, _A1, _A2, _A3, _A4>(pFun); }

template<typename _R, typename _A1, typename _A2, typename _A3, typename _A4, typename _A5>
class ptr_fun5_t : public function5<_R, _A1, _A2, _A3, _A4, _A5>, private not_assignable
{
public:
	typedef _R (* ptr_fun_type)(_A1, _A2, _A3, _A4, _A5);

	explicit ptr_fun5_t(ptr_fun_type pFun) : _m_pFun(pFun) {}

	_R operator()(_A1 a1, _A2 a2, _A3 a3, _A4 a4, _A5 a5) const { return _m_pFun(a1, a2, a3, a4, a5); }

private:
	ptr_fun_type _m_pFun;
};

template<typename _R, typename _A1, typename _A2, typename _A3, typename _A4, typename _A5>
inline ptr_fun5_t<_R, _A1, _A2, _A3, _A4, _A5> ptr_fun(_R (* pFun)(_A1, _A2, _A3, _A4, _A5)) { return ptr_fun5_t<_R, _A1, _A2, _A3, _A4, _A5>(pFun); }


////////////////////////////// mem_fun0_t //////////////////////////////

template<typename _R, class _T>
class mem_fun0_t : public function1<_R, _T *>, private not_assignable
{
public:
	typedef _R (_T::* mem_fun_type)(void);

	explicit mem_fun0_t(mem_fun_type pFun) : _m_pMemFun(pFun) {}

	_R operator()(_T * pObj) const { return (pObj->*_m_pMemFun)(); }

private:
	mem_fun_type _m_pMemFun;
};

template<typename _R, class _T>
inline mem_fun0_t<_R, _T> mem_fun(_R (_T::* pFun)(void))
{
	return mem_fun0_t<_R, _T>(pFun);
}

template<typename _R, class _T>
class const_mem_fun0_t : public function1<_R, _T const *>, private not_assignable
{
public:
	typedef _R (_T::* mem_fun_type)(void) const;

	explicit const_mem_fun0_t(mem_fun_type pFun) : _m_pMemFun(pFun) {}

	_R operator()(_T const * pObj) const { return (pObj->*_m_pMemFun)(); }

private:
	mem_fun_type _m_pMemFun;
};

template<typename _R, class _T>
inline const_mem_fun0_t<_R, _T> mem_fun(_R (_T::* pFun)(void) const)
{
	return const_mem_fun0_t<_R, _T>(pFun);
}

template<typename _R, class _T>
class mem_fun0_ref_t : public function1<_R, _T>, private not_assignable
{
public:
	typedef _R (_T::* mem_fun_type)(void);

	explicit mem_fun0_ref_t(mem_fun_type pFun) : _m_pMemFun(pFun) {}

	_R operator()(_T & Obj) const { return (Obj.*_m_pMemFun)(); }

private:
	mem_fun_type _m_pMemFun;
};

template<typename _R, class _T>
inline mem_fun0_ref_t<_R, _T> mem_fun_ref(_R (_T::* pFun)(void))
{
	return mem_fun0_ref_t<_R, _T>(pFun);
}

template<typename _R, class _T>
class const_mem_fun0_ref_t : public function1<_R, _T>, private not_assignable
{
public:
	typedef _R (_T::* mem_fun_type)(void) const;

	explicit const_mem_fun0_ref_t(mem_fun_type pFun) : _m_pMemFun(pFun) {}

	_R operator()(_T const & Obj) const { return (Obj.*_m_pMemFun)(); }

private:
	mem_fun_type _m_pMemFun;
};

template<typename _R, class _T>
inline const_mem_fun0_ref_t<_R, _T> mem_fun_ref(_R (_T::* pFun)(void) const)
{
	return const_mem_fun0_ref_t<_R, _T>(pFun);
}


////////////////////////////// mem_fun1_t //////////////////////////////

template<typename _R, class _T, typename _A1>
class mem_fun1_t : public function2<_R, _T *, _A1>, private not_assignable
{
public:
	typedef _R (_T::* mem_fun_type)(_A1);

	explicit mem_fun1_t(mem_fun_type pFun) : _m_pMemFun(pFun) {}

	_R operator()(_T * pObj, _A1 a1) const { return (pObj->*_m_pMemFun)(a1); }

private:
	mem_fun_type _m_pMemFun;
};

template<typename _R, class _T, typename _A1>
inline mem_fun1_t<_R, _T, _A1> mem_fun(_R (_T::* pFun)(_A1))
{
	return mem_fun1_t<_R, _T, _A1>(pFun);
}

template<typename _R, class _T, typename _A1>
class const_mem_fun1_t : public function2<_R, _T const *, _A1>, private not_assignable
{
public:
	typedef _R (_T::* mem_fun_type)(_A1) const;

	explicit const_mem_fun1_t(mem_fun_type pFun) : _m_pMemFun(pFun) {}

	_R operator()(_T const * pObj, _A1 a1) const { return (pObj->*_m_pMemFun)(a1); }

private:
	mem_fun_type _m_pMemFun;
};

template<typename _R, class _T, typename _A1>
inline const_mem_fun1_t<_R, _T, _A1> mem_fun(_R (_T::* pFun)(_A1) const)
{
	return const_mem_fun1_t<_R, _T, _A1>(pFun);
}

template<typename _R, class _T, typename _A1>
class mem_fun1_ref_t : public function2<_R, _T, _A1>, private not_assignable
{
public:
	typedef _R (_T::* mem_fun_type)(_A1);

	explicit mem_fun1_ref_t(mem_fun_type pFun) : _m_pMemFun(pFun) {}

	_R operator()(_T & Obj, _A1 a1) const { return (Obj.*_m_pMemFun)(a1); }

private:
	mem_fun_type _m_pMemFun;
};

template<typename _R, class _T, typename _A1>
inline mem_fun1_ref_t<_R, _T, _A1> mem_fun_ref(_R (_T::* pFun)(_A1))
{
	return mem_fun1_ref_t<_R, _T, _A1>(pFun);
}

template<typename _R, class _T, typename _A1>
class const_mem_fun1_ref_t : public function2<_R, _T, _A1>, private not_assignable
{
public:
	typedef _R (_T::* mem_fun_type)(_A1) const;

	explicit const_mem_fun1_ref_t(mem_fun_type pFun) : _m_pMemFun(pFun) {}

	_R operator()(_T const & Obj, _A1 a1) const { return (Obj.*_m_pMemFun)(a1); }

private:
	mem_fun_type _m_pMemFun;
};

template<typename _R, class _T, typename _A1>
inline const_mem_fun1_ref_t<_R, _T, _A1> mem_fun_ref(_R (_T::* pFun)(_A1) const)
{
	return const_mem_fun1_ref_t<_R, _T, _A1>(pFun);
}


////////////////////////////// mem_fun2_t //////////////////////////////

template<typename _R, class _T, typename _A1, typename _A2>
class mem_fun2_t : public function3<_R, _T *, _A1, _A2>, private not_assignable
{
public:
	typedef _R (_T::* mem_fun_type)(_A1, _A2);

	explicit mem_fun2_t(mem_fun_type pFun) : _m_pMemFun(pFun) {}

	_R operator()(_T * pObj, _A1 a1, _A2 a2) const { return (pObj->*_m_pMemFun)(a1, a2); }

private:
	mem_fun_type _m_pMemFun;
};

template<typename _R, class _T, typename _A1, typename _A2>
inline mem_fun2_t<_R, _T, _A1, _A2> mem_fun(_R (_T::* pFun)(_A1, _A2))
{
	return mem_fun2_t<_R, _T, _A1, _A2>(pFun);
}

template<typename _R, class _T, typename _A1, typename _A2>
class const_mem_fun2_t : public function3<_R, _T const *, _A1, _A2>, private not_assignable
{
public:
	typedef _R (_T::* mem_fun_type)(_A1, _A2) const;

	explicit const_mem_fun2_t(mem_fun_type pFun) : _m_pMemFun(pFun) {}

	_R operator()(_T const * pObj, _A1 a1, _A2 a2) const { return (pObj->*_m_pMemFun)(a1, a2); }

private:
	mem_fun_type _m_pMemFun;
};

template<typename _R, class _T, typename _A1, typename _A2>
inline const_mem_fun2_t<_R, _T, _A1, _A2> mem_fun(_R (_T::* pFun)(_A1, _A2) const)
{
	return const_mem_fun2_t<_R, _T, _A1, _A2>(pFun);
}

template<typename _R, class _T, typename _A1, typename _A2>
class mem_fun2_ref_t : public function3<_R, _T, _A1, _A2>, private not_assignable
{
public:
	typedef _R (_T::* mem_fun_type)(_A1, _A2);

	explicit mem_fun2_ref_t(mem_fun_type pFun) : _m_pMemFun(pFun) {}

	_R operator()(_T & Obj, _A1 a1, _A2 a2) const { return (Obj.*_m_pMemFun)(a1, a2); }

private:
	mem_fun_type _m_pMemFun;
};

template<typename _R, class _T, typename _A1, typename _A2>
inline mem_fun2_ref_t<_R, _T, _A1, _A2> mem_fun_ref(_R (_T::* pFun)(_A1, _A2))
{
	return mem_fun2_ref_t<_R, _T, _A1, _A2>(pFun);
}

template<typename _R, class _T, typename _A1, typename _A2>
class const_mem_fun2_ref_t : public function3<_R, _T, _A1, _A2>, private not_assignable
{
public:
	typedef _R (_T::* mem_fun_type)(_A1, _A2) const;

	explicit const_mem_fun2_ref_t(mem_fun_type pFun) : _m_pMemFun(pFun) {}

	_R operator()(_T const & Obj, _A1 a1, _A2 a2) const { return (Obj.*_m_pMemFun)(a1, a2); }

private:
	mem_fun_type _m_pMemFun;
};

template<typename _R, class _T, typename _A1, typename _A2>
inline const_mem_fun2_ref_t<_R, _T, _A1, _A2> mem_fun_ref(_R (_T::* pFun)(_A1, _A2) const)
{
	return const_mem_fun2_ref_t<_R, _T, _A1, _A2>(pFun);
}


////////////////////////////// mem_fun3_t //////////////////////////////

template<typename _R, class _T, typename _A1, typename _A2, typename _A3>
class mem_fun3_t : public function4<_R, _T *, _A1, _A2, _A3>, private not_assignable
{
public:
	typedef _R (_T::* mem_fun_type)(_A1, _A2, _A3);

	explicit mem_fun3_t(mem_fun_type pFun) : _m_pMemFun(pFun) {}

	_R operator()(_T * pObj, _A1 a1, _A2 a2, _A3 a3) const { return (pObj->*_m_pMemFun)(a1, a2, a3); }

private:
	mem_fun_type _m_pMemFun;
};

template<typename _R, class _T, typename _A1, typename _A2, typename _A3>
inline mem_fun3_t<_R, _T, _A1, _A2, _A3> mem_fun(_R (_T::* pFun)(_A1, _A2, _A3))
{
	return mem_fun3_t<_R, _T, _A1, _A2, _A3>(pFun);
}

template<typename _R, class _T, typename _A1, typename _A2, typename _A3>
class const_mem_fun3_t : public function4<_R, _T const *, _A1, _A2, _A3>, private not_assignable
{
public:
	typedef _R (_T::* mem_fun_type)(_A1, _A2, _A3) const;

	explicit const_mem_fun3_t(mem_fun_type pFun) : _m_pMemFun(pFun) {}

	_R operator()(_T const * pObj, _A1 a1, _A2 a2, _A3 a3) const { return (pObj->*_m_pMemFun)(a1, a2, a3); }

private:
	mem_fun_type _m_pMemFun;
};

template<typename _R, class _T, typename _A1, typename _A2, typename _A3>
inline const_mem_fun3_t<_R, _T, _A1, _A2, _A3> mem_fun(_R (_T::* pFun)(_A1, _A2, _A3) const)
{
	return const_mem_fun3_t<_R, _T, _A1, _A2, _A3>(pFun);
}

template<typename _R, class _T, typename _A1, typename _A2, typename _A3>
class mem_fun3_ref_t : public function4<_R, _T, _A1, _A2, _A3>, private not_assignable
{
public:
	typedef _R (_T::* mem_fun_type)(_A1, _A2, _A3);

	explicit mem_fun3_ref_t(mem_fun_type pFun) : _m_pMemFun(pFun) {}

	_R operator()(_T & Obj, _A1 a1, _A2 a2, _A3 a3) const { return (Obj.*_m_pMemFun)(a1, a2, a3); }

private:
	mem_fun_type _m_pMemFun;
};

template<typename _R, class _T, typename _A1, typename _A2, typename _A3>
inline mem_fun3_ref_t<_R, _T, _A1, _A2, _A3> mem_fun_ref(_R (_T::* pFun)(_A1, _A2, _A3))
{
	return mem_fun3_ref_t<_R, _T, _A1, _A2, _A3>(pFun);
}

template<typename _R, class _T, typename _A1, typename _A2, typename _A3>
class const_mem_fun3_ref_t : public function4<_R, _T, _A1, _A2, _A3>, private not_assignable
{
public:
	typedef _R (_T::* mem_fun_type)(_A1, _A2, _A3) const;

	explicit const_mem_fun3_ref_t(mem_fun_type pFun) : _m_pMemFun(pFun) {}

	_R operator()(_T const & Obj, _A1 a1, _A2 a2, _A3 a3) const { return (Obj.*_m_pMemFun)(a1, a2, a3); }

private:
	mem_fun_type _m_pMemFun;
};

template<typename _R, class _T, typename _A1, typename _A2, typename _A3>
inline const_mem_fun3_ref_t<_R, _T, _A1, _A2, _A3> mem_fun_ref(_R (_T::* pFun)(_A1, _A2, _A3) const)
{
	return const_mem_fun3_ref_t<_R, _T, _A1, _A2, _A3>(pFun);
}


////////////////////////////// mem_fun4_t //////////////////////////////

template<typename _R, class _T, typename _A1, typename _A2, typename _A3, typename _A4>
class mem_fun4_t : public function5<_R, _T *, _A1, _A2, _A3, _A4>, private not_assignable
{
public:
	typedef _R (_T::* mem_fun_type)(_A1, _A2, _A3, _A4);

	explicit mem_fun4_t(mem_fun_type pFun) : _m_pMemFun(pFun) {}

	_R operator()(_T * pObj, _A1 a1, _A2 a2, _A3 a3, _A4 a4) const { return (pObj->*_m_pMemFun)(a1, a2, a3, a4); }

private:
	mem_fun_type _m_pMemFun;
};

template<typename _R, class _T, typename _A1, typename _A2, typename _A3, typename _A4>
inline mem_fun4_t<_R, _T, _A1, _A2, _A3, _A4> mem_fun(_R (_T::* pFun)(_A1, _A2, _A3, _A4))
{
	return mem_fun4_t<_R, _T, _A1, _A2, _A3, _A4>(pFun);
}

template<typename _R, class _T, typename _A1, typename _A2, typename _A3, typename _A4>
class const_mem_fun4_t : public function5<_R, _T const *, _A1, _A2, _A3, _A4>, private not_assignable
{
public:
	typedef _R (_T::* mem_fun_type)(_A1, _A2, _A3, _A4) const;

	explicit const_mem_fun4_t(mem_fun_type pFun) : _m_pMemFun(pFun) {}

	_R operator()(_T const * pObj, _A1 a1, _A2 a2, _A3 a3, _A4 a4) const { return (pObj->*_m_pMemFun)(a1, a2, a3, a4); }

private:
	mem_fun_type _m_pMemFun;
};

template<typename _R, class _T, typename _A1, typename _A2, typename _A3, typename _A4>
inline const_mem_fun4_t<_R, _T, _A1, _A2, _A3, _A4> mem_fun(_R (_T::* pFun)(_A1, _A2, _A3, _A4) const)
{
	return const_mem_fun4_t<_R, _T, _A1, _A2, _A3, _A4>(pFun);
}

template<typename _R, class _T, typename _A1, typename _A2, typename _A3, typename _A4>
class mem_fun4_ref_t : public function5<_R, _T, _A1, _A2, _A3, _A4>, private not_assignable
{
public:
	typedef _R (_T::* mem_fun_type)(_A1, _A2, _A3, _A4);

	explicit mem_fun4_ref_t(mem_fun_type pFun) : _m_pMemFun(pFun) {}

	_R operator()(_T & Obj, _A1 a1, _A2 a2, _A3 a3, _A4 a4) const { return (Obj.*_m_pMemFun)(a1, a2, a3, a4); }

private:
	mem_fun_type _m_pMemFun;
};

template<typename _R, class _T, typename _A1, typename _A2, typename _A3, typename _A4>
inline mem_fun4_ref_t<_R, _T, _A1, _A2, _A3, _A4> mem_fun_ref(_R (_T::* pFun)(_A1, _A2, _A3, _A4))
{
	return mem_fun4_ref_t<_R, _T, _A1, _A2, _A3, _A4>(pFun);
}

template<typename _R, class _T, typename _A1, typename _A2, typename _A3, typename _A4>
class const_mem_fun4_ref_t : public function5<_R, _T, _A1, _A2, _A3, _A4>, private not_assignable
{
public:
	typedef _R (_T::* mem_fun_type)(_A1, _A2, _A3, _A4) const;

	explicit const_mem_fun4_ref_t(mem_fun_type pFun) : _m_pMemFun(pFun) {}

	_R operator()(_T const & Obj, _A1 a1, _A2 a2, _A3 a3, _A4 a4) const { return (Obj.*_m_pMemFun)(a1, a2, a3, a4); }

private:
	mem_fun_type _m_pMemFun;
};

template<typename _R, class _T, typename _A1, typename _A2, typename _A3, typename _A4>
inline const_mem_fun4_ref_t<_R, _T, _A1, _A2, _A3, _A4> mem_fun_ref(_R (_T::* pFun)(_A1, _A2, _A3, _A4) const)
{
	return const_mem_fun4_ref_t<_R, _T, _A1, _A2, _A3, _A4>(pFun);
}


////////////////////////////// binder1stx //////////////////////////////

template <class _Bfn>
class binder1st0 : public function0<typename _Bfn::result_type>, private not_assignable
{
public:
	binder1st0(_Bfn const & bfn, typename _Bfn::argument1_type const & arg)
		: op(bfn), value(arg) {}

	result_type operator()(void) const { return op(value); }

private:
	_Bfn op;
	typename _Bfn::argument1_type value;
};

template<class _Bfn, typename _A>
inline binder1st0<_Bfn> bind1st0(_Bfn const & op, _A const & arg)
{
	return binder1st0<_Bfn>(op, _Bfn::argument1_type(arg));
}

template <class _Bfn>
class binder1st1 : public function1<typename _Bfn::result_type, typename _Bfn::argument2_type>, private not_assignable
{
public:
	binder1st1(_Bfn const & bfn, typename _Bfn::argument1_type arg)
		: op(bfn), value(arg) {}

	result_type operator()(typename _Bfn::argument2_type a2) const { return op(value, a2); }

private:
	binder1st1 & operator=(binder1st1 const &);

private:
	_Bfn op;
	typename _Bfn::argument1_type value;
};

template<class _Bfn, typename _A>
inline binder1st1<_Bfn> bind1st1(_Bfn const & op, _A const & arg)
{
	return binder1st1<_Bfn>(op, _Bfn::argument1_type(arg));
}

template <class _Bfn>
class binder2nd1 : public function1<typename _Bfn::result_type, typename _Bfn::argument1_type>, private not_assignable
{
public:
	binder2nd1(_Bfn const & bfn, typename _Bfn::argument2_type arg)
		: op(bfn), value(arg) {}

	result_type operator()(typename _Bfn::argument1_type a1) const { return op(a1, value); }

private:
	_Bfn op;
	typename _Bfn::argument2_type value;
};

template<class _Bfn, typename _A>
inline binder2nd1<_Bfn> bind2nd1(_Bfn const & op, _A const & arg)
{
	return binder2nd1<_Bfn>(op, _Bfn::argument2_type(arg));
}

template <class _Bfn>
class binder1st2 : public function2<typename _Bfn::result_type, typename _Bfn::argument2_type, typename _Bfn::argument3_type>, private not_assignable
{
public:
	binder1st2(_Bfn const & bfn, typename _Bfn::argument1_type arg)
		: op(bfn), value(arg) {}

	result_type operator()(typename _Bfn::argument2_type a2,
		typename _Bfn::argument3_type a3) const { return op(value, a2, a3); }

private:
	_Bfn op;
	typename _Bfn::argument1_type value;
};

template<class _Bfn, typename _A>
inline binder1st2<_Bfn> bind1st2(_Bfn const & op, _A const & arg)
{
	return binder1st2<_Bfn>(op, _Bfn::argument1_type(arg));
}

template <class _Bfn>
class binder1st3 : public function3<typename _Bfn::result_type, typename _Bfn::argument2_type, typename _Bfn::argument3_type, typename _Bfn::argument4_type>, private not_assignable
{
public:
	binder1st3(_Bfn const & bfn, typename _Bfn::argument1_type arg)
		: op(bfn), value(arg) {}

	result_type operator()(typename _Bfn::argument2_type a2,
		typename _Bfn::argument3_type a3, typename _Bfn::argument4_type a4) const
	{ return op(value, a2, a3, a4); }

private:
	_Bfn op;
	typename _Bfn::argument1_type value;
};

template<class _Bfn, typename _A>
inline binder1st3<_Bfn> bind1st3(_Bfn const & op, _A const & arg)
{
	return binder1st3<_Bfn>(op, _Bfn::argument1_type(arg));
}

template <class _Bfn>
class binder1st4 : public function4<typename _Bfn::result_type, typename _Bfn::argument2_type, typename _Bfn::argument3_type, typename _Bfn::argument4_type, typename _Bfn::argument5_type>, private not_assignable
{
public:
	binder1st4(_Bfn const & bfn, typename _Bfn::argument1_type arg)
		: op(bfn), value(arg) {}

	result_type operator()(typename _Bfn::argument2_type a2,
		typename _Bfn::argument3_type a3, typename _Bfn::argument4_type a4,
		typename _Bfn::argument5_type a5) const
	{ return op(value, a2, a3, a4, a5); }

private:
	_Bfn op;
	typename _Bfn::argument1_type value;
};

template<class _Bfn, typename _A>
inline binder1st4<_Bfn> bind1st4(_Bfn const & op, _A const & arg)
{
	return binder1st4<_Bfn>(op, _Bfn::argument1_type(arg));
}

}	// namespace fn
