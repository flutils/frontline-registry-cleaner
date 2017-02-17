#ifndef SCOPEGUARD_H_
#define SCOPEGUARD_H_

#pragma once

namespace sg {

class ScopeGuardImplBase
{
protected:
	ScopeGuardImplBase(void) {}
	~ScopeGuardImplBase(void) {}
private:
	ScopeGuardImplBase & operator=(ScopeGuardImplBase const &);
};

typedef ScopeGuardImplBase const & ScopeGuard;

template <typename PFun>
class ScopeGuardImpl0 : public ScopeGuardImplBase
{
public:
	ScopeGuardImpl0(PFun pFun) : pFun_(pFun) {}
	~ScopeGuardImpl0(void) { (*pFun_)(); }
private:
	PFun pFun_;
};

template <typename PFun>
inline ScopeGuardImpl0<PFun> MakeScopeGuard(PFun pFun)
{
	return ScopeGuardImpl0<PFun>(pFun);
}

template <typename PFun, typename P1>
class ScopeGuardImpl1 : public ScopeGuardImplBase
{
public:
	ScopeGuardImpl1(PFun pFun, P1 p1) : pFun_(pFun), p1_(p1) {}
	~ScopeGuardImpl1(void) { (*pFun_)(p1_); }
private:
	PFun pFun_;
	P1 const p1_;
};

template <typename PFun, typename P1>
inline ScopeGuardImpl1<PFun, P1> MakeScopeGuard(PFun pFun, P1 p1)
{
	return ScopeGuardImpl1<PFun, P1>(pFun, p1);
}

template <typename PFun, typename P1, typename P2>
class ScopeGuardImpl2 : public ScopeGuardImplBase
{
public:
	ScopeGuardImpl2(PFun pFun, P1 p1, P2 p2) : pFun_(pFun), p1_(p1), p2_(p2) {}
	~ScopeGuardImpl2(void) { (*pFun_)(p1_, p2_); }
private:
	PFun pFun_;
	P1 const p1_;
	P2 const p2_;
};

template <typename PFun, typename P1, typename P2>
inline ScopeGuardImpl2<PFun, P1, P2> MakeScopeGuard(PFun pFun, P1 p1, P2 p2)
{
	return ScopeGuardImpl2<PFun, P1, P2>(pFun, p1, p2);
}

template <typename PFun, typename P1, typename P2, typename P3>
class ScopeGuardImpl3 : public ScopeGuardImplBase
{
public:
	ScopeGuardImpl3(PFun pFun, P1 p1, P2 p2, P3 p3) : pFun_(pFun), p1_(p1), p2_(p2), p3_(p3) {}
	~ScopeGuardImpl3(void) { (*pFun_)(p1_, p2_, p3); }
private:
	PFun pFun_;
	P1 const p1_;
	P2 const p2_;
	P3 const p3_;
};

template <typename PFun, typename P1, typename P2, typename P3>
inline ScopeGuardImpl3<PFun, P1, P2, P3> MakeScopeGuard(PFun pFun, P1 p1, P2 p2, P3 p3)
{
	return ScopeGuardImpl3<PFun, P1, P2, P3>(pFun, p1, p2, p3);
}

//************************************************************

template <class Obj, typename PMemFun>
class ObjScopeGuardImpl0 : public ScopeGuardImplBase
{
public:
	ObjScopeGuardImpl0(Obj & obj, PMemFun pMemFun)
		: obj_(obj), pMemFun_(pMemFun) {}
	~ObjScopeGuardImpl0(void) { (obj_.*pMemFun_)(); }
private:
	Obj & obj_;
	PMemFun pMemFun_;
};

template <class Obj, typename pMemFun>
inline ObjScopeGuardImpl0<Obj, pMemFun> MakeObjScopeGuard(Obj & obj, pMemFun pmemFun)
{
	return ObjScopeGuardImpl0<Obj, pMemFun>(obj, pmemFun);
}

template <class Obj, typename PMemFun, typename P1>
class ObjScopeGuardImpl1 : public ScopeGuardImplBase
{
public:
	ObjScopeGuardImpl1(Obj & obj, PMemFun pMemFun, P1 p1)
		: obj_(obj), pMemFun_(pMemFun), p1_(p1) {}
	~ObjScopeGuardImpl1(void) { (obj_.*pMemFun_)(p1_); }
private:
	Obj & obj_;
	PMemFun pMemFun_;
	P1 const p1_;
};

template <class Obj, typename pMemFun, typename P1>
inline ObjScopeGuardImpl1<Obj, pMemFun, P1> MakeObjScopeGuard(Obj & obj, pMemFun pmemFun, P1 p1)
{
	return ObjScopeGuardImpl1<Obj, pMemFun, P1>(obj, pmemFun, p1);
}

template <class Obj, typename PMemFun, typename P1, typename P2>
class ObjScopeGuardImpl2 : public ScopeGuardImplBase
{
public:
	ObjScopeGuardImpl2(Obj & obj, PMemFun pMemFun, P1 p1, P2 p2)
		: obj_(obj), pMemFun_(pMemFun), p1_(p1), p2_(p2) {}
	~ObjScopeGuardImpl2(void) { (obj_.*pMemFun_)(p1_, p2_); }
private:
	Obj & obj_;
	PMemFun pMemFun_;
	P1 const p1_;
	P2 const p2_;
};

template <class Obj, typename pMemFun, typename P1, typename P2>
inline ObjScopeGuardImpl2<Obj, pMemFun, P1, P2> MakeObjScopeGuard(Obj & obj, pMemFun pmemFun, P1 p1, P2 p2)
{
	return ObjScopeGuardImpl2<Obj, pMemFun, P1, P2>(obj, pmemFun, p1, p2);
}

template <class Obj, typename PMemFun>
class PObjScopeGuardImpl0 : public ScopeGuardImplBase
{
public:
	PObjScopeGuardImpl0(Obj * pObj, PMemFun pMemFun)
		: pObj_(pObj), pMemFun_(pMemFun) {}
	~PObjScopeGuardImpl0(void) { (pObj_->*pMemFun_)(); }
private:
	Obj * pObj_;
	PMemFun pMemFun_;
};

template <class Obj, typename pMemFun>
inline PObjScopeGuardImpl0<Obj, pMemFun> MakePObjScopeGuard(Obj * pObj, pMemFun pmemFun)
{
	return PObjScopeGuardImpl0<Obj, pMemFun>(pObj, pmemFun);
}

template <class Obj, typename PMemFun, typename P1>
class PObjScopeGuardImpl1 : public ScopeGuardImplBase
{
public:
	PObjScopeGuardImpl1(Obj * pObj, PMemFun pMemFun, P1 p1)
		: pObj_(pObj), pMemFun_(pMemFun), p1_(p1) {}
	~PObjScopeGuardImpl1(void) { (pObj_->*pMemFun_)(p1_); }
private:
	Obj * pObj_;
	PMemFun pMemFun_;
	P1 const p1_;
};

template <class Obj, typename pMemFun, typename P1>
inline PObjScopeGuardImpl1<Obj, pMemFun, P1> MakePObjScopeGuard(Obj * pObj, pMemFun pmemFun, P1 p1)
{
	return PObjScopeGuardImpl1<Obj, pMemFun, P1>(pObj, pmemFun, p1);
}

}	// namespace sg

#define CONCATENATE_DIRECT(s1, s2) s1##s2
#define CONCATENATE(s1, s2) CONCATENATE_DIRECT(s1, s2)
#define ANONYMOUS_VARIABLE(str) CONCATENATE(str, __COUNTER__)

#define ON_BLOCK_EXIT sg::ScopeGuard ANONYMOUS_VARIABLE(scopeGuard) = sg::MakeScopeGuard
#define ON_BLOCK_EXIT_OBJ sg::ScopeGuard ANONYMOUS_VARIABLE(scopeGuard) = sg::MakeObjScopeGuard
#define ON_BLOCK_EXIT_POBJ sg::ScopeGuard ANONYMOUS_VARIABLE(scopeGuard) = sg::MakePObjScopeGuard

#endif //SCOPEGUARD_H_
