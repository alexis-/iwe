#ifndef IWE_COMMON_H
# define IWE_COMMON_H


template <typename T>
class CIWEPoint
{
public:
	CIWEPoint()
	{
		x = y = (T)0;
	}

	CIWEPoint(T nx, T ny)
	{
		x = nx;
		y = ny;
	}

public:
  inline bool operator==(const CIWEPoint& other) const
  {
    return x == other.x && y == other.y;
  }

  inline CIWEPoint& operator=(const CIWEPoint& other)
  {
    x = other.x;
    y = other.y;

    return *this;
  }

public:
	T					x;
	T					y;
};



enum												e_inputMouseButton
{
	IPT_NONE									= 0,
	IPT_MOUSE_LEFT						= 1,
	IPT_MOUSE_RIGHT						= 2,
	IPT_MOUSE_MIDDLE					= 3,
	IPT_MOUSE_WHEEL						= 4,
	IPT_MOUSE_MOVE						= 5
};



enum												e_inputMouseAction
{
	IPT_MOUSE_UP							= 0,
	IPT_MOUSE_DOWN						= 1,
};



typedef struct							s_inputDatas
{
	e_inputMouseAction				m_MouseAction;
  e_inputMouseButton        m_MouseButton;
	CIWEPoint<int>						m_Pt;
	short											m_sDelta;
}														s_inputDatas;



struct s_IWEImageData
{
  ~s_IWEImageData() { if (pData) delete pData; }

  uint          width;
  uint          height;
  uchar         bpp;
  void*         pData;
};



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

enum IWEVariantType
{
  IWEVAR_BOOL,
  IWEVAR_INT,
  IWEVAR_FLOAT,
  IWEVAR_STR,
  IWEVAR_VEC,
  IWEVAR_PTR,
};

class IWEVariant
{
public:
  bool                          getBool() {
    return data.bVal;
  }

  int                           getInt() {
    return data.iVal;
  }

  float                         getFloat() {
    return data.fVal;
  }

  std::vector<IWEVariant>&      getVector() {
    type = IWEVAR_VEC;
    return vecVal;
  }

  std::string&                  getString() {
    type = IWEVAR_STR;
    return sVal;
  }
  

public:
  operator bool () { return getBool(); }
  operator int () { return getInt(); }
  operator float () { return getFloat(); }
  operator std::vector<IWEVariant>& () { return getVector(); }
  operator std::string& () { return getString(); }


public:
  void                          setBool(bool val) {
    type = IWEVAR_BOOL;
    data.bVal = val;
  }

  void                          setInt(int val) {
    type = IWEVAR_INT;
    data.iVal = val;
  }

  void                          setFloat(float val) {
    type = IWEVAR_FLOAT;
    data.fVal = val;
  }


public:
  IWEVariant& operator=(bool val) { setBool(val); return *this; }
  IWEVariant& operator=(int val) { setInt(val); return *this; }
  IWEVariant& operator=(float val) { setFloat(val); return *this; }
  IWEVariant& operator=(const std::string& val) { sVal = val; type = IWEVAR_STR; return *this; }
  IWEVariant& operator=(const std::vector<IWEVariant>& val) { vecVal = val; type = IWEVAR_VEC; return *this; }


private:
  union u_data
  {
    bool                        bVal;
    int                         iVal;
    float                       fVal;
  };
  
  std::string                   sVal;
  std::vector<IWEVariant>       vecVal;

  u_data                        data;
  IWEVariantType                type;


public:
  u_data                        min;
  u_data                        max;
};

class IWEVariantSetting
{
public:
  IWEVariantSetting() {
    m_bReadOnly = true;
    m_sName = "NoName";
    m_sDesc = "";
  }
  
  IWEVariantSetting(const std::string& sName, const std::string& sDesc, bool readonly = true) {
    m_bReadOnly = readonly;
    m_sName = sName;
    m_sDesc = sDesc;
  }


public:
  operator IWEVariant& () { return m_data; }
  IWEVariantSetting& operator=(IWEVariant& variant) { m_data = variant; }


public:
  bool                          m_bReadOnly;
  IWEVariant                    m_data;
  std::string                   m_sDesc;
  std::string                   m_sName;
};



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

class BasePropVar
{
public:
  Event0<>          eventChanged;
};


// This is taken from Source SDK
template<class Type>
class CPropVar : public BasePropVar
{
public:
	Type& Get()
	{
		return m_Value;
	}

  void Update()
  {
    eventChanged();
  }

	template< class C >
	const Type& operator=(const C &val ) 
	{ 
		return Set((const Type )val ); 
	}
	
	template< class C >
	const Type& operator=(const CPropVar<C> &val ) 
	{ 
		return Set((const Type )val.m_Value ); 
	}
	
	const Type& Set(const Type &val )
	{
		if (memcmp(&m_Value, &val, sizeof(Type) ) )
		{
			m_Value = val;
		}

    eventChanged();

		return m_Value;
	}

	template< class C >
	const Type& operator+=(const C &val ) 
	{
		return Set(m_Value + (const Type )val ); 
	}

	template< class C >
	const Type& operator-=(const C &val ) 
	{
		return Set(m_Value - (const Type )val ); 
	}
	
	template< class C >
	const Type& operator/=(const C &val ) 
	{
		return Set(m_Value / (const Type )val ); 
	}
	
	template< class C >
	const Type& operator*=(const C &val ) 
	{
		return Set(m_Value * (const Type )val ); 
	}
	
	template< class C >
	const Type& operator^=(const C &val ) 
	{
		return Set(m_Value ^ (const Type )val ); 
	}

	template< class C >
	const Type& operator|=(const C &val ) 
	{
		return Set(m_Value | (const Type )val ); 
	}

	const Type& operator++()
	{
		return (*this += 1);
	}

	Type operator--()
	{
		return (*this -= 1);
	}
	
	Type operator++(int ) // postfix version..
	{
		Type val = m_Value;
		(*this += 1);
		return val;
	}

	Type operator--(int ) // postfix version..
	{
		Type val = m_Value;
		(*this -= 1);
		return val;
	}
	
	// For some reason the compiler only generates type conversion warnings for this operator when used like 
	// CNetworkVarBase<unsigned char> = 0x1
	// (it warns about converting from an int to an unsigned char).
	template< class C >
	const Type& operator&=(const C &val ) 
	{	
		return Set(m_Value & (const Type )val ); 
	}

	operator const Type&() const 
	{
		return m_Value; 
	}
	
	//const Type& Get() const 
	//{
	//	return m_Value; 
	//}
	
	Type* operator->() const 
	{
		return &m_Value; 
	}

private:
	Type m_Value;
};

#endif // !IWE_COMMON_H