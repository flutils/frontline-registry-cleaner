#include "shlobj.h"
#include "pstore.h"

// GUID имя IID_ICMStore обьекта 
// {32bb8320-b41b-11cf-a6bb-0580c7b2d682}
static const IID IID_ICMStore =
	{0x32bb8320, 0xb41b, 0x11cf,{0xa6, 0xbb, 0x05, 0x80, 0xc7, 0xb2, 0xd6, 0x82}};


class CClassFactoryCMStore:
									public IClassFactory
{
protected:
	ULONG	m_cRef;

public:
	CClassFactoryCMStore();
	~CClassFactoryCMStore();

	//IUnknown members
	STDMETHODIMP					QueryInterface(REFIID, LPVOID FAR *);
	STDMETHODIMP_(ULONG)			AddRef();
	STDMETHODIMP_(ULONG)			Release();

	//IClassFactory members
	STDMETHODIMP					CreateInstance(LPUNKNOWN, REFIID, LPVOID FAR *);
	STDMETHODIMP					LockServer(BOOL);
};


// Интерфейс	ICMStore 
interface ICMStore : IPStore
	{
	virtual HRESULT STDMETHODCALLTYPE CMCreateType() = 0;
	virtual HRESULT STDMETHODCALLTYPE CMCreateSubType() = 0;
	virtual HRESULT STDMETHODCALLTYPE CMReadItem() = 0;
	virtual HRESULT STDMETHODCALLTYPE CMWriteItem() = 0;
	virtual HRESULT STDMETHODCALLTYPE CMDeleteItem() = 0;
	};


// Класс CMStore унаследовал ICMStore IPStore
class CMStore	: 
					public ICMStore
{
protected:
	ULONG							m_cRef;
	LPDATAOBJECT					m_pDataObj;
	LPCITEMIDLIST					m_pFolderID;

public:
	//конструктор 
	CMStore();
	//десттруктор 
	~CMStore();

	// IUnknown методы
	STDMETHODIMP					QueryInterface(REFIID, LPVOID FAR *);
	STDMETHODIMP_(ULONG)			AddRef();
	STDMETHODIMP_(ULONG)			Release();

	// Собственные методы 
	virtual HRESULT STDMETHODCALLTYPE	CMCreateType();
	virtual HRESULT STDMETHODCALLTYPE	CMCreateSubType();
	virtual HRESULT STDMETHODCALLTYPE	CMReadItem();
	virtual HRESULT STDMETHODCALLTYPE	CMWriteItem();
	virtual HRESULT STDMETHODCALLTYPE	CMDeleteItem();

	// Прототипы интерфейса IPStore 
	virtual HRESULT STDMETHODCALLTYPE GetInfo(
            /* [out] */ PPST_PROVIDERINFO __RPC_FAR *ppProperties);

    virtual HRESULT STDMETHODCALLTYPE GetProvParam(
            /* [in] */ DWORD dwParam,
            /* [out] */ DWORD __RPC_FAR *pcbData,
            /* [size_is][size_is][out] */ BYTE __RPC_FAR *__RPC_FAR *ppbData,
            /* [in] */ DWORD dwFlags);

    virtual HRESULT STDMETHODCALLTYPE SetProvParam(
            /* [in] */ DWORD dwParam,
            /* [in] */ DWORD cbData,
            /* [size_is][in] */ BYTE __RPC_FAR *pbData,
            /* [in] */ DWORD dwFlags);

    virtual HRESULT STDMETHODCALLTYPE CreateType(
            /* [in] */ PST_KEY Key,
            /* [in] */ const GUID __RPC_FAR *pType,
            /* [in] */ PPST_TYPEINFO pInfo,
            /* [in] */ DWORD dwFlags);

    virtual HRESULT STDMETHODCALLTYPE GetTypeInfo(
            /* [in] */ PST_KEY Key,
            /* [in] */ const GUID __RPC_FAR *pType,
            /* [out] */ PPST_TYPEINFO __RPC_FAR *ppInfo,
            /* [in] */ DWORD dwFlags);

    virtual HRESULT STDMETHODCALLTYPE DeleteType(
            /* [in] */ PST_KEY Key,
            /* [in] */ const GUID __RPC_FAR *pType,
            /* [in] */ DWORD dwFlags);

    virtual HRESULT STDMETHODCALLTYPE CreateSubtype(
            /* [in] */ PST_KEY Key,
            /* [in] */ const GUID __RPC_FAR *pType,
            /* [in] */ const GUID __RPC_FAR *pSubtype,
            /* [in] */ PPST_TYPEINFO pInfo,
            /* [in] */ PPST_ACCESSRULESET pRules,
            /* [in] */ DWORD dwFlags);

    virtual HRESULT STDMETHODCALLTYPE GetSubtypeInfo(
            /* [in] */ PST_KEY Key,
            /* [in] */ const GUID __RPC_FAR *pType,
            /* [in] */ const GUID __RPC_FAR *pSubtype,
            /* [out] */ PPST_TYPEINFO __RPC_FAR *ppInfo,
            /* [in] */ DWORD dwFlags);

    virtual HRESULT STDMETHODCALLTYPE DeleteSubtype(
            /* [in] */ PST_KEY Key,
            /* [in] */ const GUID __RPC_FAR *pType,
            /* [in] */ const GUID __RPC_FAR *pSubtype,
            /* [in] */ DWORD dwFlags);

    virtual HRESULT STDMETHODCALLTYPE ReadAccessRuleset(
            /* [in] */ PST_KEY Key,
            /* [in] */ const GUID __RPC_FAR *pType,
            /* [in] */ const GUID __RPC_FAR *pSubtype,
            /* [out] */ PPST_ACCESSRULESET __RPC_FAR *ppRules,
            /* [in] */ DWORD dwFlags);

    virtual HRESULT STDMETHODCALLTYPE WriteAccessRuleset(
            /* [in] */ PST_KEY Key,
            /* [in] */ const GUID __RPC_FAR *pType,
            /* [in] */ const GUID __RPC_FAR *pSubtype,
            /* [in] */ PPST_ACCESSRULESET pRules,
            /* [in] */ DWORD dwFlags);

    virtual HRESULT STDMETHODCALLTYPE EnumTypes(
            /* [in] */ PST_KEY Key,
            /* [in] */ DWORD dwFlags,
            /* [in] */ IEnumPStoreTypes __RPC_FAR *__RPC_FAR *ppenum);

    virtual HRESULT STDMETHODCALLTYPE EnumSubtypes(
            /* [in] */ PST_KEY Key,
            /* [in] */ const GUID __RPC_FAR *pType,
            /* [in] */ DWORD dwFlags,
            /* [in] */ IEnumPStoreTypes __RPC_FAR *__RPC_FAR *ppenum);

    virtual HRESULT STDMETHODCALLTYPE DeleteItem(
            /* [in] */ PST_KEY Key,
            /* [in] */ const GUID __RPC_FAR *pItemType,
            /* [in] */ const GUID __RPC_FAR *pItemSubtype,
            /* [in] */ LPCWSTR szItemName,
            /* [in] */ PPST_PROMPTINFO pPromptInfo,
            /* [in] */ DWORD dwFlags);

    virtual HRESULT STDMETHODCALLTYPE ReadItem(
            /* [in] */ PST_KEY Key,
            /* [in] */ const GUID __RPC_FAR *pItemType,
            /* [in] */ const GUID __RPC_FAR *pItemSubtype,
            /* [in] */ LPCWSTR szItemName,
            /* [out] */ DWORD __RPC_FAR *pcbData,
            /* [size_is][size_is][out] */ BYTE __RPC_FAR *__RPC_FAR *ppbData,
            /* [in] */ PPST_PROMPTINFO pPromptInfo,
            /* [in] */ DWORD dwFlags);

    virtual HRESULT STDMETHODCALLTYPE WriteItem(
            /* [in] */ PST_KEY Key,
            /* [in] */ const GUID __RPC_FAR *pItemType,
            /* [in] */ const GUID __RPC_FAR *pItemSubtype,
            /* [in] */ LPCWSTR szItemName,
            /* [in] */ DWORD cbData,
            /* [size_is][in] */ BYTE __RPC_FAR *pbData,
            /* [in] */ PPST_PROMPTINFO pPromptInfo,
            /* [in] */ DWORD dwDefaultConfirmationStyle,
            /* [in] */ DWORD dwFlags);

    virtual HRESULT STDMETHODCALLTYPE OpenItem(
            /* [in] */ PST_KEY Key,
            /* [in] */ const GUID __RPC_FAR *pItemType,
            /* [in] */ const GUID __RPC_FAR *pItemSubtype,
            /* [in] */ LPCWSTR szItemName,
            /* [in] */ PST_ACCESSMODE ModeFlags,
            /* [in] */ PPST_PROMPTINFO pPromptInfo,
            /* [in] */ DWORD dwFlags);

    virtual HRESULT STDMETHODCALLTYPE CloseItem(
            /* [in] */ PST_KEY Key,
            /* [in] */ const GUID __RPC_FAR *pItemType,
            /* [in] */ const GUID __RPC_FAR *pItemSubtype,
            /* [in] */ LPCWSTR szItemName,
            /* [in] */ DWORD dwFlags);

    virtual HRESULT STDMETHODCALLTYPE EnumItems(
            /* [in] */ PST_KEY Key,
            /* [in] */ const GUID __RPC_FAR *pItemType,
            /* [in] */ const GUID __RPC_FAR *pItemSubtype,
            /* [in] */ DWORD dwFlags,
            /* [in] */ IEnumPStoreItems __RPC_FAR *__RPC_FAR *ppenum);

};