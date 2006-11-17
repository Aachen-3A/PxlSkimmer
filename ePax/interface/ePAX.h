#define MERGED_PXL
#ifndef pcl_hh
#define pcl_hh

#include <string>

#include <iostream>
//----------------------------------------------------------------------
namespace pcl {
//----------------------------------------------------------------------
extern double getCpuTime();
//----------------------------------------------------------------------
class Exception {
  public: 
    Exception() {
      routine = "unspecified routine";
      message = "unspecified error"; }  

    Exception(const std::string& rout, const std::string& msg) {
      routine = rout;
      message = msg; }
       
    ~Exception() {;} 
    
    std::string routine;
    std::string message;
};
//----------------------------------------------------------------------
extern void exception(const std::string& rout, const std::string& msg);
//----------------------------------------------------------------------
class BasicLinuxIoStreamer {

  protected:
  
    // writing data
    inline void dumpMemory(std::ostream& cxxx, const char* address, const int& bytes) {
      cxxx.rdbuf()->sputn(address, bytes);
      cxxx.rdbuf()->pubsync(); }
    
    inline void storeBasicTypeChar(std::ostream& cxxx, const char& data) {
    	dumpMemory(cxxx, (const char*)&data, 1); 
    }
      
    inline void storeBasicTypeBool(std::ostream& cxxx, const bool& data) {
      if (data) dumpMemory(cxxx, "Y", 1);
      else dumpMemory(cxxx, "N", 1);
    }
      
    inline void storeBasicTypeInt(std::ostream& cxxx, const int& data) {
      dumpMemory(cxxx, (const char*)&data, 4);
    }
      
    inline void storeBasicTypeFloat(std::ostream& cxxx, const float& data) {
      dumpMemory(cxxx, (const char*)&data, 4);
    }
    	
    inline void storeBasicTypeDouble(std::ostream& cxxx, const double& data) {
      dumpMemory(cxxx, (const char*)&data, 8);
    }
    
    inline void storeBasicTypeCStr(std::ostream& cxxx, const char* address) {
      while ((*address) != '\0') {
      	cxxx.rdbuf()->sputc(*address);
      	address++; }
      cxxx.rdbuf()->sputc(*address);
    }
      
    inline void storeBasicTypeString(std::ostream& cxxx, const std::string& data) {
      const int length = data.length();
      storeBasicTypeInt(cxxx, length);
      dumpMemory(cxxx, data.c_str(), length);
    }
  
    // reading data
    inline bool redumpMemory(std::istream& cxxx, char* address, const int& bytes) {
      cxxx.read(address, bytes);
      return (bytes == cxxx.gcount());
    }
    
    inline bool restoreBasicTypeChar(std::istream& cxxx, char& data) {
      return redumpMemory(cxxx, (char*)&data, 1);
    }
      
    inline bool restoreBasicTypeBool(std::istream& cxxx, bool& data) {
      char cYesNo = ' ';
      bool success = redumpMemory(cxxx, &cYesNo, 1);
      data = (cYesNo == 'Y');
      return success;
    }
    	
    inline bool restoreBasicTypeInt(std::istream& cxxx, int& data) {
      return redumpMemory(cxxx, (char*)&data, 4);
   	}
    	
    inline bool restoreBasicTypeFloat(std::istream& cxxx, float& data) {
      return redumpMemory(cxxx, (char*)&data, 4);
    }
    
    inline bool restoreBasicTypeDouble(std::istream& cxxx, double& data) {
      return redumpMemory(cxxx, (char*)&data, 8);
    }
    
    inline bool restoreBasicTypeCStr(std::istream& cxxx, std::string& data, const int& bytes = -1) {
       int read;
       data.clear();
       cxxx.get(_operationsBuffer, 1024-1, '\0'); 
       read = cxxx.gcount();
       _operationsBuffer[read+1] = '\0';
       while (read == 1024-1) {
       	 data += _operationsBuffer;
       	 cxxx.get(_operationsBuffer, 1024-1, '\0');
       	 read = cxxx.gcount();
       	 _operationsBuffer[read+1] = '\0';
       }
       data += _operationsBuffer;
       cxxx.ignore(1);
       return true;
     }
       
    inline bool restoreBasicTypeString(std::istream& cxxx, std::string& data) {
      int length;
      restoreBasicTypeInt(cxxx, length); 
      if (length) {
	    char *buffer = new char[length];
	    cxxx.read(buffer, length);
	    data.assign(buffer, length);
	    delete [] buffer;
	  }
      else {
      	data = "";
      }   
      return 1;
    }
    
  private:
  
    char _operationsBuffer[1024];  
};
//
typedef BasicLinuxIoStreamer BasicIoStreamer;
//----------------------------------------------------------------------
} // namespace pcl
//----------------------------------------------------------------------
#endif // pcl_hh
#ifndef ptl_hh
#define ptl_hh

#ifndef MERGED_PXL
	#include "pcl.hh"
#endif

#include <vector>
#include <map>
//
//
//----------------------------------------------------------------------
// necessary ptl class declarations
//----------------------------------------------------------------------
namespace ptl {
  class WkPtrBase;
  template<class datatype, class objecttype> class WkPtrSpec;
  template<class datatype> class WkPtr;
  template<class datatype> class CowWkPtr;
  template<class datatype> class SpyWkPtr;

  class ObjectBase;
  template<class datatype> class Object;
  template<class datatype> class CowObject;
  template<class datatype> class SpyObject;
}
//
//
//----------------------------------------------------------------------
// necessary iotl class declarations
//----------------------------------------------------------------------
namespace iotl {
  class oStreamer;
  class iStreamer;
  class TypeManager;
}
//
//
//----------------------------------------------------------------------
// ptl namespace definitions
//----------------------------------------------------------------------
namespace ptl {
//
//
//----------------------------------------------------------------------
// very basic ptl stuff
//----------------------------------------------------------------------
class Get {public: Get() {;} ~Get() {;}}; 
class Set {public: Set() {;} ~Set() {;}}; 
//----------------------------------------------------------------------
static const ptl::Get get;
static const ptl::Set set;
//----------------------------------------------------------------------
typedef int MutableId;
typedef const MutableId Id;
//----------------------------------------------------------------------
//
//
//----------------------------------------------------------------------
// ptl container classes
//----------------------------------------------------------------------
template<class itemtype> class Vector {

  public:
    virtual ~Vector() {;}
  
    typedef          std::vector<itemtype> StlContainer;
    typedef typename std::vector<itemtype>::const_iterator StlConstIterator;
    typedef typename std::vector<itemtype>::iterator       StlIterator;

    inline void set(itemtype& item) {_container.push_back(item);}

    // navigation
    const StlConstIterator begin() const {return _container.begin();}
    const StlConstIterator end()   const {return _container.end();}
    
    inline StlContainer& getContainer() {return _container;}
    virtual void clearContainer() {return _container.clear();}
    
    inline int getSize() const {return _container.size();} 
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - -
    class PtlIterator {
      public:
		PtlIterator(const ptl::Vector<itemtype>::PtlIterator& original) : _iter(original._iter), _containerRef(original._containerRef) {;}
		PtlIterator(const ptl::Vector<itemtype>& vector) : _containerRef(&vector) {first();}
		
		inline void first()  {_iter = _containerRef->begin();}
		inline void next()   {_iter++;}
		inline bool isDone() {return (_iter == _containerRef->end());}
		
		inline itemtype item() {return *_iter;} 
	  private:
		PtlIterator() : _containerRef(0) {;}
		
		StlConstIterator   _iter;
		const ptl::Vector<itemtype>* _containerRef;
    };
    // - - - - - - - - - - - - - - - - - - - - - - - - - - -    

  protected:
    StlContainer _container;
  
  friend class iotl::iStreamer;
  friend class iotl::oStreamer;      
};
//----------------------------------------------------------------------
template<class keytype, class itemtype> class Map {

  public:
    virtual ~Map() {;}

    typedef          std::pair<keytype,itemtype> StlPair;
    typedef          std::map<keytype,itemtype>  StlContainer;
    typedef typename std::map<keytype,itemtype>::const_iterator StlConstIterator;
    typedef typename std::map<keytype,itemtype>::iterator       StlIterator;

    void set   (const keytype& key, const itemtype& item)    {_container.erase(key); _container.insert(StlPair(key, item));}
    void remove(const keytype& key)                          {_container.erase(key);}
    itemtype find(const keytype& key, itemtype defaultitem) const {StlConstIterator found = _container.find(key); if (found != _container.end()) return found->second; return defaultitem;}
    itemtype find(const keytype& key)                       const {StlConstIterator found = _container.find(key); if (found != _container.end()) return found->second; pcl::exception("ptl::Map::find(...)","key not found and no default item provided");throw;}

    // navigation
    const StlConstIterator begin() const {return _container.begin();}
    const StlConstIterator end() const   {return _container.end();}
    
    inline StlContainer& getContainer() {return _container;}
    virtual void clearContainer() {return _container.clear();}
    
    inline int getSize() const {return _container.size();} 

    // - - - - - - - - - - - - - - - - - - - - - - - - - - -
    class PtlIterator {
      public:
		PtlIterator(const ptl::Map<keytype, itemtype>::PtlIterator& original) : _iter(original._iter), _containerRef(original._containerRef) {;}
		PtlIterator(const ptl::Map<keytype, itemtype>& map) : _containerRef(&map) {first();}
		
		inline void first()  {_iter = _containerRef->begin();}
		inline void next()   {_iter++;}
		inline bool isDone() {return (_iter == _containerRef->end());}
		
		inline keytype  key()  {return _iter->first;} 
		inline itemtype item() {return _iter->second;} 
	  private:
		PtlIterator() : _containerRef(0) {;}
		
		StlConstIterator             _iter;
		const ptl::Map<keytype, itemtype>* _containerRef;
    };
    // - - - - - - - - - - - - - - - - - - - - - - - - - - -    
    
  protected:
    StlContainer _container;
  
  friend class iotl::iStreamer;
  friend class iotl::oStreamer;
};
//----------------------------------------------------------------------
typedef ptl::Map<ptl::Id,ptl::ObjectBase*> CopyHistory;
//----------------------------------------------------------------------
typedef ptl::Map<std::string, ptl::ObjectBase*> Index;
//----------------------------------------------------------------------
class Objects : public ptl::Vector<ptl::ObjectBase*> {

  public:
    Objects();
    Objects(const ptl::Objects& original);
    virtual ~Objects() {ptl::Objects::clearContainer();}
    
    template<class objecttype> inline objecttype& create()                                       {objecttype* pitem = new objecttype;           pitem->_refObjects = this; _container.push_back(static_cast<ptl::ObjectBase*>(pitem)); return *pitem;}
    template<class objecttype> inline objecttype& create(const objecttype& original)             {objecttype* pitem = new objecttype(original); pitem->_refObjects = this; _container.push_back(static_cast<ptl::ObjectBase*>(pitem)); return *pitem;}
    template<class objecttype, class ctrtype> inline objecttype& create(const ctrtype& original) {objecttype* pitem = new objecttype(original); pitem->_refObjects = this; _container.push_back(static_cast<ptl::ObjectBase*>(pitem)); return *pitem;}
    
    void set(ptl::ObjectBase& item);
    void remove(ptl::ObjectBase& item);    
    bool has(const ptl::ObjectBase& item) const;
    
    virtual void clearContainer();
    
    template<class objecttype> inline objecttype* findObject(const std::string idx)           const {return dynamic_cast<objecttype*>(_index.find(idx, 0));}                  // goes via Index & casts
    template<class objecttype> inline objecttype* findCopyOf(const ptl::ObjectBase& original) const;  // goes via CopyHistory & casts

    inline const ptl::CopyHistory& getCopyHistory() const {return _copyHistory;}
    inline void                    clearCopyHistory() {_copyHistory.clearContainer();}
    
    inline bool setIndex(const std::string& idx, ptl::ObjectBase& obj) {if (idx == "") return false; if (!has(obj)) return false; _index.set(idx, &obj); return true;}

    inline const ptl::Index& getIndex() const {return _index;}
    inline void              removeIndex(const std::string& idx) {_index.remove(idx);}
    inline void              clearIndex() {_index.clearContainer();}
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - -
    class PtlIterator {
      public:
		PtlIterator(const ptl::Objects::PtlIterator& original) : _iter(original._iter), _containerRef(original._containerRef) {;}
		PtlIterator(const ptl::Objects& vector) : _containerRef(&vector) {first();}
		
		inline void first()  {_iter = _containerRef->begin();}
		inline void next()   {_iter++;}
		inline bool isDone() {return (_iter == _containerRef->end());}
		
		inline ptl::ObjectBase* item() {return *_iter;} 
		inline ptl::ObjectBase& object() {return *item();}
		 
	  private:
		PtlIterator() : _containerRef(0) {;}
		
		StlConstIterator   _iter;
		const ptl::Objects* _containerRef;
    };
    // - - - - - - - - - - - - - - - - - - - - - - - - - - -
    template<class objecttype> class PtlTypeIterator {
      public:
		PtlTypeIterator(const ptl::Objects::PtlTypeIterator<objecttype>& original) : _iter(original._iter), _containerRef(original._containerRef) {;}
		PtlTypeIterator(const ptl::Objects& oo) : _containerRef(&oo) {first();}
		
		inline void first()  {_iter = _containerRef->begin(); while (!tryItem()) _iter++;}
		inline void next()   {_iter++; while (!tryItem()) _iter++;}
		inline bool isDone() {return (_iter == _containerRef->end());}
		
		inline objecttype* item() {return dynamic_cast<objecttype*>(*_iter);} 
		inline objecttype& object() {return *item();}
	  private:
		PtlTypeIterator() : _containerRef(0) {;}
		
		inline bool tryItem() {if (isDone()) return true; return (dynamic_cast<objecttype*>(*_iter) != 0);}
		
		StlConstIterator  _iter;
		const ptl::Objects* _containerRef;
      };
    // - - - - - - - - - - - - - - - - - - - - - - - - - - -

   protected:
     ptl::CopyHistory _copyHistory;
     ptl::Index       _index;
    
  friend class iotl::iStreamer;
  friend class iotl::oStreamer;
};
//----------------------------------------------------------------------
class Relations : public ptl::Map<ptl::Id, ptl::WkPtrBase*> {

  public:
    Relations();
    virtual ~Relations() {ptl::Relations::clearContainer();}
    
    void set(ptl::Id id, ptl::WkPtrBase* pptr);

    void remove(ptl::ObjectBase& object);
    void remove(ptl::Id id);
    
    bool has(const ptl::ObjectBase& object) const;
    bool has(ptl::Id id) const;
    
    virtual void clearContainer();
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - -
    class PtlIterator {
      public:
		PtlIterator(const ptl::Relations::PtlIterator& original) : _iter(original._iter), _containerRef(original._containerRef) {;}
		PtlIterator(const ptl::Relations& map) : _containerRef(&map) {first();}
		
		inline void first()  {_iter = _containerRef->begin();}
		inline void next()   {_iter++;}
		inline bool isDone() {return (_iter == _containerRef->end());}
		
		inline ptl::Id  key()  {return _iter->first;} 
		inline ptl::WkPtrBase* item() {return _iter->second;} 
		inline ptl::WkPtrBase& relation() {return *item();} 

	  private:
		PtlIterator() : _containerRef(0) {;}
		
		StlConstIterator _iter;
		const ptl::Relations* _containerRef;
    };
    // - - - - - - - - - - - - - - - - - - - - - - - - - - -
    template<class objecttype> class PtlTypeIterator {
      public:
		PtlTypeIterator(ptl::Relations::PtlTypeIterator<objecttype>& original) : _iter(original._iter), _containerRef(original._containerRef) {;}
		PtlTypeIterator(ptl::Relations& oo) : _containerRef(&oo) {first();}
		
		inline void first()  {_iter = _containerRef->begin(); while (!tryItem()) _iter++;}
		inline void next()   {_iter++; while (!tryItem()) _iter++;}
		inline bool isDone() {return (_iter == _containerRef->end());}
		
		inline ptl::Id key() {return _iter->first;} 
		inline objecttype* item() {return dynamic_cast<objecttype*>(_iter->second);} 
		inline objecttype& relation() {return *item();} 

	  private:
		PtlTypeIterator() : _containerRef(0) {;}
		
		inline bool tryItem() {if (isDone()) return true; return (dynamic_cast<objecttype*>(_iter->second) != 0);}
		
		StlConstIterator _iter;
		const ptl::Relations* _containerRef;
      };
    // - - - - - - - - - - - - - - - - - - - - - - - - - - -

  private:    
    Relations(const ptl::Relations& original);
  
  friend class iotl::iStreamer;
  friend class iotl::oStreamer;
};
//----------------------------------------------------------------------
//
//
//----------------------------------------------------------------------
// ptl pointer classes
//----------------------------------------------------------------------
template<class datatype> class Ptr {

   public:
     Ptr() : _cppPtr(0) {;}
     Ptr(const ptl::Ptr<datatype>& original) : _cppPtr(original._cppPtr) {;}
     Ptr(datatype* original) : _cppPtr(original) {;}
     ~Ptr() {;}

     inline datatype* pointer() const  {return _cppPtr;}
     inline bool valid() const         {return _cppPtr != 0;}

     inline datatype& object() const   {return *access();}

     inline void operator=(const ptl::Ptr<datatype>& pptr)           {_cppPtr = pptr._cppPtr;}
     inline void operator=(datatype& data)                               {_cppPtr = &data;}
     inline void operator=(datatype* dataptr)                            {_cppPtr = dataptr;}
     
     inline datatype* operator ->() const {return _cppPtr;}

  protected:
     // safe access to object
     inline datatype* access() const {if (_cppPtr) return _cppPtr; 
										std::cerr << "ptl::Ptr::access(): FATAL: The object you intend to access does not exist!" << std::endl; 
										return 0;}

      datatype* _cppPtr;
};
// dereference operators
template<class datatype> datatype& operator *(ptl::Ptr<datatype>& ptr) {return ptr.object();}
template<class datatype> const datatype& operator *(const ptl::Ptr<datatype>& ptr) {return ptr.object();}
//----------------------------------------------------------------------
// ptl weak pointer classes
//----------------------------------------------------------------------
class WkPtrBase {
  public:
     virtual ~WkPtrBase() {connect(0);}
     
     // for deep copies:
     virtual ptl::WkPtrBase* clone() const {return new ptl::WkPtrBase(*this);}
     
     inline ptl::ObjectBase* pointer() const  {return _objectRef;}
     inline bool valid() const                {return _objectRef != 0;}

     inline ptl::ObjectBase* operator ->() const {return _objectRef;}

  protected:  
     WkPtrBase() : _notifyChainIn(0), _notifyChainOut(0), _objectRef(0) {;}
     
     void notifyDeleted();
     
     void connect(ptl::ObjectBase* pointer);

     ptl::WkPtrBase*  _notifyChainIn; 
     ptl::WkPtrBase*  _notifyChainOut;

     ptl::ObjectBase* _objectRef;

  friend class ObjectBase;
};
//----------------------------------------------------------------------
template<class datatype, class objecttype> class WkPtrSpec : public ptl::WkPtrBase {

   public:
     WkPtrSpec() : ptl::WkPtrBase() {;}
     WkPtrSpec(objecttype* ptr) : ptl::WkPtrBase() {connect(ptr);}
     WkPtrSpec(objecttype& object) : ptl::WkPtrBase() {connect(&object);}
     WkPtrSpec(const ptl::WkPtrSpec<datatype, objecttype>& original) : ptl::WkPtrBase() {connect((objecttype*) original._objectRef);}
     virtual ~WkPtrSpec() {connect(0);}

     // for deep copies:
     virtual ptl::WkPtrBase* clone() const {return new ptl::WkPtrSpec<datatype, objecttype>(*this);}
     
     inline void operator=(const ptl::WkPtrSpec<datatype, objecttype>& pptr) {connect(pptr._objectRef);}
     inline void operator=(objecttype& object)                               {connect(&object);}
     inline void operator=(objecttype* objectptr)                            {connect(objectptr);}
     
     // methods to grant object & data access
     inline objecttype& object() const  {return *access();}
     
     inline const datatype& get() const {return access()->get();}
     inline       datatype& set()       {return access()->set();}
     
     inline const datatype& operator()()                  const {return get();}
     inline const datatype& operator()(const ptl::Get&)   const {return get();}
     inline       datatype& operator()(const ptl::Set&)         {return set();}
     
     inline objecttype* operator ->() const {return access();}

  protected:
     // safe access to object
     inline objecttype* access() const {if (_objectRef) return (objecttype*)_objectRef; 
										std::cerr << "ptl::WkPtrSpec::access(): FATAL: The object you intend to access does not exist!" << std::endl; 
										return 0;}
  
  friend class iotl::iStreamer;
  friend class iotl::oStreamer;      
};
// dereference operators
template<class datatype, class objecttype> objecttype& operator *(ptl::WkPtrSpec<datatype, objecttype>& wkPtr) {return wkPtr.object();}
template<class datatype, class objecttype> const objecttype& operator *(const ptl::WkPtrSpec<datatype, objecttype>& wkPtr) {return wkPtr.object();}
//----------------------------------------------------------------------
// Derive the classes ptr & ptrbase
typedef ptl::WkPtrBase WkPtrBase;
//----------------------------------------------------------------------
template<class datatype> class WkPtr : public ptl::WkPtrSpec<datatype, ptl::Object<datatype> > {

   public:
     WkPtr() : ptl::WkPtrSpec<datatype, ptl::Object<datatype> >() {;}
     WkPtr(ptl::Object<datatype>* ptr) : ptl::WkPtrSpec<datatype, ptl::Object<datatype> >(ptr) {;}
     WkPtr(ptl::Object<datatype>& object) : ptl::WkPtrSpec<datatype, ptl::Object<datatype> >(object) {;}
     WkPtr(const ptl::WkPtrSpec<datatype, ptl::Object<datatype> >& original) : ptl::WkPtrSpec<datatype, ptl::Object<datatype> >(original) {;}
};     
// dereference operators
template<class datatype>       ptl::Object<datatype>& operator *(      ptl::WkPtr<datatype>& wkPtr) {return wkPtr.object();}
template<class datatype> const ptl::Object<datatype>& operator *(const ptl::WkPtr<datatype>& wkPtr) {return wkPtr.object();}
//
template<class datatype> class CowWkPtr : public ptl::WkPtrSpec<datatype, ptl::CowObject<datatype> > {

   public:
     CowWkPtr() : ptl::WkPtrSpec<datatype, ptl::CowObject<datatype> >() {;}
     CowWkPtr(ptl::CowObject<datatype>* ptr) : ptl::WkPtrSpec<datatype, ptl::CowObject<datatype> >(ptr) {;}
     CowWkPtr(ptl::CowObject<datatype>& object) : ptl::WkPtrSpec<datatype, ptl::CowObject<datatype> >(object) {;}
     CowWkPtr(const ptl::WkPtrSpec<datatype, ptl::CowObject<datatype> >& original) : ptl::WkPtrSpec<datatype, ptl::CowObject<datatype> >(original) {;}
};
// dereference operators
template<class datatype>       ptl::CowObject<datatype>& operator *(      ptl::CowWkPtr<datatype>& wkPtr) {return wkPtr.object();}
template<class datatype> const ptl::CowObject<datatype>& operator *(const ptl::CowWkPtr<datatype>& wkPtr) {return wkPtr.object();}
//
template<class datatype> class SpyWkPtr : public ptl::WkPtrSpec<datatype, ptl::SpyObject<datatype> > {

   public:
     SpyWkPtr() : ptl::WkPtrSpec<datatype, ptl::SpyObject<datatype> >() {;}
     SpyWkPtr(ptl::SpyObject<datatype>* ptr) : ptl::WkPtrSpec<datatype, ptl::SpyObject<datatype> >(ptr) {;}
     SpyWkPtr(ptl::SpyObject<datatype>& object) : ptl::WkPtrSpec<datatype, ptl::SpyObject<datatype> >(object) {;}
     SpyWkPtr(const ptl::WkPtrSpec<datatype, ptl::SpyObject<datatype> >& original) : ptl::WkPtrSpec<datatype, ptl::SpyObject<datatype> >(original) {;}
};
// dereference operators
template<class datatype>       ptl::SpyObject<datatype>& operator *(      ptl::SpyWkPtr<datatype>& wkPtr) {return wkPtr.object();}
template<class datatype> const ptl::SpyObject<datatype>& operator *(const ptl::SpyWkPtr<datatype>& wkPtr) {return wkPtr.object();}
//
//
//----------------------------------------------------------------------
// ptl visual properties class
//----------------------------------------------------------------------
class Layout {

  public:
	Layout() : _type(0), _style(0), _color(0), _a(0.), _b(0.), _c(0.), _d(0.) {;}
	~Layout() {;}

	inline int getType() {return _type;}
	inline int getStyle() {return _style;}
	inline int getColor() {return _color;}
	
	inline double getA() {return _a;}
	inline double getB() {return _b;}
	inline double getC() {return _c;}
	inline double getD() {return _d;}

	inline void setType(int v)  {_type = v;}
	inline void setStyle(int v) {_style = v;}
	inline void setColor(int v) {_color = v;}
	
	inline void setA(double v) {_a = v;}
	inline void setB(double v) {_b = v;}
	inline void setC(double v) {_c = v;}
	inline void setD(double v) {_d = v;}

  protected: 
	int _type;
	int _style;
	int _color;
	
	double _a;
	double _b;
	double _c;
	double _d;
	
  friend class iotl::iStreamer;
  friend class iotl::oStreamer; 

};
//
//
//----------------------------------------------------------------------
// ptl object classes
//----------------------------------------------------------------------
class ObjectBase {
  public:
     virtual ~ObjectBase() {if (_refWkPtrSpec) _refWkPtrSpec->notifyDeleted(); delete _ptrLayout;}

     inline ptl::Id id() const {const void* p = this; return int(p);}

     inline ptl::Objects* holder() const {return _refObjects;}

     virtual ptl::ObjectBase* clone() const {return new ptl::ObjectBase(*this);}
     
     inline ptl::Relations& getMotherRelations()  {return _motherRelations;}
     inline ptl::Relations& getDaughterRelations() {return _daughterRelations;}
    
     void linkMother(ptl::ObjectBase& target);
     void linkDaughter(ptl::ObjectBase& target);
     
     inline void linkMother(ptl::WkPtrBase& target)   {if (target._objectRef) linkMother(*(target._objectRef));}
     inline void linkDaughter(ptl::WkPtrBase& target) {if (target._objectRef) linkDaughter(*(target._objectRef));}
     
     void unlinkMother(ptl::ObjectBase& target);
     void unlinkDaughter(ptl::ObjectBase& target);
     
     inline void unlinkMother(ptl::WkPtrBase& target)   {if (target._objectRef) unlinkMother(*(target._objectRef));}
     inline void unlinkDaughter(ptl::WkPtrBase& target) {if (target._objectRef) unlinkDaughter(*(target._objectRef));}
     
     void unlinkMothers();
     void unlinkDaughters();
     
     inline ptl::Layout& layout() {if (!_ptrLayout) {_ptrLayout = new ptl::Layout;} return *_ptrLayout;}

     std::ostream& printDecayTree(int level = 0, std::ostream& os = std::cout, int pan = 1) const;
     virtual std::ostream&  print(int level = 0, std::ostream& os = std::cout, int pan = 0) const;

  protected:  
     ObjectBase() : _refWkPtrSpec(0), _refObjects(0), _motherRelations(), _daughterRelations(), _ptrLayout(0) {;}
     ObjectBase(const ObjectBase& original) : _refWkPtrSpec(0), _refObjects(0), _motherRelations(), _daughterRelations() {if (original._ptrLayout) {_ptrLayout = new ptl::Layout(*original._ptrLayout);} else {_ptrLayout = 0;}}
     
     virtual ptl::WkPtrBase* createSelfWkPtr() {pcl::exception("ptl::ObjectBase::createSelfPointer()","ATTENTION! Inherting class must reimplement this virtual method."); return 0;}
     virtual void storeYourSelf(iotl::oStreamer& output) const {pcl::exception("ptl::ObjectBase::storeYourSelf()","ATTENTION! Inherting class must reimplement this virtual method.");}
     
     std::ostream& printPan1st(std::ostream& os, int pan) const;
     std::ostream& printPan(std::ostream& os, int pan) const;

     ptl::WkPtrBase* _refWkPtrSpec;
     ptl::Objects*  _refObjects;
     
     ptl::Relations _motherRelations;
     ptl::Relations _daughterRelations;
     
     ptl::Layout* _ptrLayout;

  friend class WkPtrBase;
  friend class Objects;
  
  friend class iotl::iStreamer;
  friend class iotl::oStreamer; 
};
//----------------------------------------------------------------------
template<class datatype> class Object : public ptl::ObjectBase {

   public:
     Object() : ptl::ObjectBase(), _data() {;}
     Object(const datatype& original) : ptl::ObjectBase(), _data(original) {;}
     Object(const ptl::Object<datatype>& original) : ptl::ObjectBase(original), _data(original._data) {;}
     virtual ~Object() {;}
     
     // for deep copies:
     virtual ptl::ObjectBase* clone() const {return new ptl::Object<datatype>(*this);}
     
     inline const datatype& get() const {return _data;}
     inline       datatype& set()       {return _data;}
     
     inline const datatype& operator()()                      const {return get();}
     inline const datatype& operator()(const ptl::Get&)       const {return get();}
     inline       datatype& operator()(const ptl::Set&)             {return set();}
     
     inline ptl::Object<datatype>& operator=(const datatype& original) {_data = original; return *this;}
     inline ptl::Object<datatype>& operator=(const ptl::Object<datatype>& original) {_data = original._data; return *this;}

     virtual std::ostream&  print(int level = 0, std::ostream& os = std::cout, int pan = 0) const {os << "called by ptl::Object<...>: "; return ptl::ObjectBase::print(level, os, pan);}
     
  protected:
     virtual ptl::WkPtrBase* createSelfWkPtr() {return new ptl::WkPtr<datatype>(*this);}
     virtual void storeYourSelf(iotl::oStreamer& output) const;

     datatype _data;
  
  friend class iotl::iStreamer;
  friend class iotl::oStreamer;      
};
//----------------------------------------------------------------------
template<class datatype> class CowObject : public ptl::ObjectBase {

   private:
    // - - - - - - - - - - - - - - - - - - - - - - - - - - -
      class DataSocket {
	public:
	   DataSocket() : _references(1) {;}
	   DataSocket(const datatype& original) : _references(1), _data(original) {;}
	   DataSocket(const ptl::CowObject<datatype>::DataSocket& original) : _references(1), _data(original._data) {;}
	   virtual ~DataSocket() {;}

	   // for deep copies:
	   virtual DataSocket* clone() const {return new DataSocket(*this);}

	   // methods to grant data access
	   inline datatype& getData()                   {return _data;}
	   inline void  setData(const datatype& object) {_data = object;}

	   unsigned int _references;
	   datatype     _data;

	friend class iotl::iStreamer;
	friend class iotl::oStreamer;      
      };   
    // - - - - - - - - - - - - - - - - - - - - - - - - - - -

   public:
     CowObject() : ptl::ObjectBase() {_dataSocket = new DataSocket;}
     CowObject(const datatype& original) : ptl::ObjectBase() {_dataSocket = new DataSocket(original);}
     CowObject(const ptl::CowObject<datatype>& original) : ptl::ObjectBase(original) {_dataSocket = original._dataSocket; _dataSocket->_references++;}
     virtual ~CowObject() {dropDataSocket();}

     // for deep copies:
     virtual ptl::ObjectBase* clone() const {return new ptl::CowObject<datatype>(*this);}
     
     inline const datatype& get() const {return _dataSocket->getData();}
     inline       datatype& set() {if (_dataSocket->_references == 1) {return _dataSocket->getData();}
                                   else                               {_dataSocket->_references--; _dataSocket = new DataSocket(*_dataSocket); return _dataSocket->getData();}}
     
     // until further decision:
     inline const datatype& operator()()                      const {return get();}
     inline const datatype& operator()(const ptl::Get&)       const {return get();}
     inline       datatype& operator()(const ptl::Set&)             {return set();}

     inline ptl::CowObject<datatype>& operator=(const datatype& original)                 {dropDataSocket(); _dataSocket = new DataSocket(original); return *this;}
     inline ptl::CowObject<datatype>& operator=(const ptl::CowObject<datatype>& original) {dropDataSocket(); _dataSocket = original._dataSocket; _dataSocket->_references++; return *this;}

     virtual std::ostream&  print(int level = 0, std::ostream& os = std::cout, int pan = 0) const {os << "called by ptl::CowObject<...>: "; return ptl::ObjectBase::print(level, os, pan);}

  protected:
     CowObject(DataSocket& original) : ptl::ObjectBase() {_dataSocket = &original; _dataSocket->_references++;}

     inline void dropDataSocket() {_dataSocket->_references--; if (_dataSocket->_references > 0) return; delete _dataSocket;}

     virtual ptl::WkPtrBase* createSelfWkPtr() {return new ptl::CowWkPtr<datatype>(*this);}
     virtual void storeYourSelf(iotl::oStreamer& output) const;

     DataSocket* _dataSocket;
  
  friend class iotl::iStreamer;
  friend class iotl::oStreamer;      
};
//----------------------------------------------------------------------
template<class datatype> class SpyObject : public ptl::Object<ptl::Ptr<datatype> > {

   public:
     SpyObject()                                         : ptl::Object<ptl::Ptr<datatype> >() {;}
     SpyObject(datatype* original)                       : ptl::Object<ptl::Ptr<datatype> >(ptl::Ptr<datatype>(original)) {;}
     SpyObject(const ptl::Ptr<datatype>& original)       : ptl::Object<ptl::Ptr<datatype> >(original) {;}
     SpyObject(const ptl::SpyObject<datatype>& original) : ptl::Object<ptl::Ptr<datatype> >(original.get()) {;}
     virtual ~SpyObject() {;}
     
     // for deep copies:
     virtual ptl::ObjectBase* clone() const {return new ptl::SpyObject<datatype>(*this);}
     
     inline ptl::SpyObject<datatype>& operator=(datatype* original)                       {ptl::Object<ptl::Ptr<datatype> >::operator=(ptl::Ptr<datatype>(original)); return (*this);}
     inline ptl::SpyObject<datatype>& operator=(const ptl::Ptr<datatype>& original)       {ptl::Object<ptl::Ptr<datatype> >::operator=(original); return (*this);}
     inline ptl::SpyObject<datatype>& operator=(const ptl::SpyObject<datatype>& original) {ptl::Object<ptl::Ptr<datatype> >::operator=(original.get()); return (*this);}

     virtual std::ostream& print(int level = 0, std::ostream& os = std::cout, int pan = 0) const {os << "called by ptl::SpyObject<...>: "; return ptl::ObjectBase::print(level, os, pan);}
     
  protected:
     virtual ptl::WkPtrBase* createSelfWkPtr() {return new ptl::SpyWkPtr<datatype>(*this);}
     virtual void storeYourSelf(iotl::oStreamer& output) const;
     
  friend class iotl::iStreamer;
  friend class iotl::oStreamer;      
};
//----------------------------------------------------------------------
//template<class datatype> class SpyObject : public ptl::ObjectBase {
//
//   public:
//     SpyObject() : ptl::ObjectBase(), _dataRef(0) {;}
//     SpyObject(datatype* original) : ptl::ObjectBase(), _dataRef(original) {;}
//     SpyObject(datatype& original) : ptl::ObjectBase(), _dataRef(&original) {;}
//     SpyObject(const ptl::SpyObject<datatype>& original) : ptl::ObjectBase(original), _dataRef(original._dataRef) {;}
//     virtual ~SpyObject() {;}
//     
//     // for deep copies:
//     virtual ptl::ObjectBase* clone() const {return new ptl::SpyObject<datatype>(*this);}
//     
//     inline const datatype& get() const {return *access();}
//     inline       datatype& set()       {return *access();}
//     
//     inline const datatype& operator()()                      const {return get();}
//     inline const datatype& operator()(const ptl::Get&)       const {return get();}
//     inline       datatype& operator()(const ptl::Set&)             {return set();}
//     
//     inline datatype* pointer() const  {return _dataRef;}
//     inline bool      valid() const    {return _dataRef != 0;}
//     
//     inline ptl::SpyObject<datatype>& operator=(datatype* original) {_dataRef = original; return *this;}
//     inline ptl::SpyObject<datatype>& operator=(datatype& original) {_dataRef = &original; return *this;}
//     inline ptl::SpyObject<datatype>& operator=(const ptl::SpyObject<datatype>& original) {_dataRef = original._dataRef; return *this;}
//
//     virtual std::ostream& print(int level = 0, std::ostream& os = std::cout, int pan = 0) const {os << "called by ptl::AccObject<...>: "; return ptl::ObjectBase::print(level, os, pan);}
//     
//  protected:
//     virtual ptl::WkPtrBase* createSelfWkPtr() {return new ptl::SpyWkPtr<datatype>(*this);}
//     virtual void storeYourSelf(iotl::oStreamer& output) const;
//     
//     // safe access to object
//     inline datatype* access() const {if (_dataRef) return _dataRef; 
//                                      std::cerr << "ptl::SpyObject<>::access(): FATAL: The pointer you are about to access does not exist!" << std::endl; 
//                                      return 0;}
//     datatype* _dataRef;
//  
//  friend class iotl::iStreamer;
//  friend class iotl::oStreamer;      
//};
//----------------------------------------------------------------------
template<class objecttype> inline objecttype* Objects::findCopyOf(const ptl::ObjectBase& original) const {return dynamic_cast<objecttype*>(_copyHistory.find(original.id(), 0));}  // goes via CopyHistory & casts
//
//
//----------------------------------------------------------------------
} // namespace ptl
//----------------------------------------------------------------------
//
//
//----------------------------------------------------------------------
// ptl operators
//----------------------------------------------------------------------
std::ostream& operator << (std::ostream& cxxx, const ptl::ObjectBase& obj);
//----------------------------------------------------------------------
//
//
//
#ifndef MERGED_PXL
	#include "iotl.hh"
	#include "ptl.icc"
#endif

#endif
#ifndef iotl_hh
#define iotl_hh

#ifndef MERGED_PXL
	#include "ptl.hh"
#endif

#include <typeinfo>
#include <zlib.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <ios>
//
//
//----------------------------------------------------------------------
#define iotl__iStreamer__lengthUnzipBuffer 65536
#define iotl__eventMarker "<E>"
#define iotl__default__compressionMode '6'
//#define iotl__default__compressionMode ' '
//----------------------------------------------------------------------
#define iotl__declareDataTypeExplicit(type, id, interfacename, data, storecode, restorecode) namespace iotl { template<> const char* getIotlTypeId<type>(const type*) {return id;} template<> void oStreamer::storeData<type>(const type& data){storecode;} template<> ptl::Id iStreamer::restoreData<type>(type& data){restorecode; return 0;} TypeAgent<ptl::Object<type> > _typeInterface_ptl__Object_ ## interfacename ("\1OO"); TypeAgent<ptl::CowObject<type> > _typeInterface_ptl__CowObject_ ## interfacename ("\1CO"); }
#define iotl__declareDataType(type, data, storecode, restorecode) iotl__declareDataTypeExplicit(type, # type, type, data, storecode, restorecode)
//
#define iotl__declareSpyTypeExplicit(type, id, interfacename, ptr, storecode, restorecode) namespace iotl { template<> const char* getIotlTypeId<ptl::Ptr<type> >(const ptl::Ptr<type>*) {return id;} template<> void oStreamer::storeData<ptl::Ptr<type> >(const ptl::Ptr<type>& ptr){storecode;} template<> ptl::Id iStreamer::restoreData<ptl::Ptr<type> >(ptl::Ptr<type>& ptr){restorecode; return 0;} TypeAgent<ptl::SpyObject<type> > _typeInterface_ptl__SpyObject_ ## interfacename ("\1SO"); }
#define iotl__declareSpyType(type, ptr, storecode, restorecode) iotl__declareSpyTypeExplicit(type, # type, type, ptr, storecode, restorecode)
//
#define iotl__declareObjectTypeExplicit(type, id, interfacename) namespace iotl { TypeAgent<type> _typeInterface_ ## interfacename (id); }
#define iotl__declareObjectType(type) iotl__declareObjectTypeExplicit(type, # type, type)
//----------------------------------------------------------------------
namespace iotl {
//----------------------------------------------------------------------
class Orphan {public: Orphan() {;} ~Orphan() {;}}; 
//----------------------------------------------------------------------
template<class type> const char* getIotlTypeId(const type* = 0);
//----------------------------------------------------------------------
// Type Manager
//----------------------------------------------------------------------
class TypeAgentBase
{
  public:
    virtual ~TypeAgentBase() {;}
    virtual void        storeObject(iotl::oStreamer& output, const ptl::ObjectBase& objectbase) {;}
    virtual ptl::Id restoreObject(iotl::iStreamer& input, ptl::ObjectBase& obj) {return 0;}
    virtual ptl::Id restoreObject(iotl::iStreamer& input, ptl::ObjectBase** ppobj) {return 0;}
		
    inline const std::string& getObjectTypeId() {return _objectTypeId;}		
    inline const std::string& getDataTypeId() {return _dataTypeId;} 
    inline const std::string& getCppTypeId() {return _cppTypeId;} 
		
  protected:
    std::string _objectTypeId;
    std::string _dataTypeId; 
    std::string _cppTypeId; 
};
//
class TypeIdKey {
  public:
    TypeIdKey(const std::string& objectTypeId, const std::string& dataTypeId) : _objectTypeId(objectTypeId), _dataTypeId(dataTypeId) {;}
    const std::string& _objectTypeId;
    const std::string& _dataTypeId;

  private:
    TypeIdKey(); 
};
//
template<class objecttype> class TypeAgent : public TypeAgentBase {
  public: 
    TypeAgent(const std::string& objectTypeId); 
    virtual ~TypeAgent() {;}
    
    virtual void storeObject(iotl::oStreamer& output, const ptl::ObjectBase& obj);
    virtual ptl::Id restoreObject(iotl::iStreamer& input, ptl::ObjectBase& obj);
    virtual ptl::Id restoreObject(iotl::iStreamer& input, ptl::ObjectBase** ppobj);
};
//
inline bool operator < (const TypeIdKey& a, const TypeIdKey& b) {
	if (a._dataTypeId == b._dataTypeId) return a._objectTypeId < b._objectTypeId;
	else                                return a._dataTypeId   < b._dataTypeId;
}
//	
class TypeManager {
  public:
    static TypeManager& instance();  
  
    void  registerAgent(iotl::TypeAgentBase* interface);

    void          storeObject(iotl::oStreamer& output, const ptl::ObjectBase& obj, const std::string& cppTypeId) const;
    ptl::Id restoreObject(iotl::iStreamer& input,  ptl::ObjectBase& obj,  const std::string& cppTypeId) const;
    ptl::Id restoreObject(iotl::iStreamer& input,  ptl::ObjectBase** ppobj,  const std::string& objectTypeId, const std::string& dataTypeId) const;

  protected:
    TypeManager() {;} // locked for singletons
    ~TypeManager() {;} // locked for singletons
    
    //ptl::Map<std::string, TypeAgentBase*> _agentsByIotlTypeId;
    ptl::Map<TypeIdKey, TypeAgentBase*>   _agentsByIotlTypeId;
    ptl::Map<std::string, TypeAgentBase*> _agentsByCppTypeId;

  private:
    TypeManager(const TypeManager&); // locked for singletons
    TypeManager& operator=(const TypeManager&); // locked for singletons
    
    static TypeManager* _instance;
}; 
//
//
//
//----------------------------------------------------------------------
// oStreamer
//----------------------------------------------------------------------
class oStreamer : public pcl::BasicIoStreamer {

  public:
    oStreamer() : pcl::BasicIoStreamer(), _buffer() {;}
    ~oStreamer() {;}
  
    void getEvent(std::ostream& cxxx, char compressionMode = iotl__default__compressionMode);

    template<class objecttype> inline void storeObject(const objecttype& obj) {iotl::TypeManager::instance().storeObject(*this, obj, typeid(obj).name());} 

    template<class datatype>                void storeData(const datatype& data); //{pcl::exception("iotl::oStreamer::storeData<>()",std::string("No output storage scheme implemented for: ") + typeid(data).name());}
    template<class itemtype>                void storeData(const ptl::Vector<itemtype>& vector);
    template<class keytype, class itemtype> void storeData(const ptl::Map<keytype, itemtype>& map);

    inline void storeTypeId(const char* typeId)                     {pcl::BasicIoStreamer::storeBasicTypeCStr(_buffer, typeId);}
    inline void storeTypeId(std::ostream& cxxx, const char* typeId) {pcl::BasicIoStreamer::storeBasicTypeCStr(cxxx, typeId);}
    
  protected:    
    inline void storeAbstractObject(const ptl::ObjectBase& obj) {obj.storeYourSelf(*this);} 
    inline void storeId(ptl::Id& id) {storeMemory((const char*)&id, 4);}
    inline void storeMemory(const char* address, const int& bytes) {pcl::BasicIoStreamer::dumpMemory(_buffer, address, bytes);}
    
    std::stringstream _buffer;
  private:
};
//
//
//
//----------------------------------------------------------------------
class oFile {
  public:
      virtual ~oFile() {;}

      virtual bool open(const std::string& filename, const bool& append = false) {return false;}
      virtual void close() {;}
      virtual void writeEvent(char compressionMode = iotl__default__compressionMode) {;}
  protected:
      oFile() {;}
  private:      
      oFile(const iotl::oFile&) {;}
};  
//
//
//
//----------------------------------------------------------------------
template<class ostreamertype> class oDiskFileVx : public iotl::oFile, public ostreamertype {

  public:
      oDiskFileVx() : iotl::oFile(), ostreamertype() {;}
      virtual ~oDiskFileVx() {close();}
      virtual bool open(const std::string& filename, const bool& append = false) {
					      close();
					      if (append) {_file.open( filename.c_str(), std::fstream::app | std::fstream::out | std::fstream::binary );}
					      else        {_file.open( filename.c_str(), std::fstream::trunc | std::fstream::out | std::fstream::binary );}
					      return _file.good();
					      }
      virtual void close() {if(!_file.is_open()) return; _file.rdbuf()->pubsync(); _file.close();}
      virtual void writeEvent(char compressionMode = iotl__default__compressionMode) {if(_file.is_open()) {ostreamertype::getEvent(_file, compressionMode);} else {pcl::exception("iotl::oDiskFileVx<>::writeEvent()","No file open.");}}
      
  protected:
      std::fstream _file;
      
  private:      
      oDiskFileVx(const iotl::oDiskFileVx<ostreamertype>&) {;}
  
};  
//----------------------------------------------------------------------
typedef iotl::oDiskFileVx<iotl::oStreamer> oDiskFile;
//
//
//----------------------------------------------------------------------
// iStreamer
//----------------------------------------------------------------------
class iStreamer : public pcl::BasicIoStreamer {

  public:
    iStreamer() : pcl::BasicIoStreamer(), _buffer() {;}
    virtual ~iStreamer() {;}
  
    inline bool putEvent(std::istream& cxxx) {return putEvent(cxxx, false);}
    inline bool next(std::istream& cxxx)     {return putEvent(cxxx, true);}
    bool previous(std::istream& cxxx);
    bool endOfEvent() {return _buffer.peek() == EOF;}

    template<class objecttype> ptl::Id restoreObject(objecttype& obj) {return iotl::TypeManager::instance().restoreObject(*this, obj, typeid(obj).name());}

    template<class datatype>                ptl::Id restoreData(datatype& data); //{pcl::exception("iotl::iStreamer::restoreData<>()",std::string("No input storage scheme implemented for: ") + typeid(data).name()); return 0;}
    template<class itemtype>                ptl::Id restoreData(ptl::Vector<itemtype>& vector);
    template<class keytype, class itemtype> ptl::Id restoreData(ptl::Map<keytype, itemtype>& map);
    
    inline void restoreTypeId(const char* expectedTypeId)                      {std::string read; pcl::BasicIoStreamer::restoreBasicTypeCStr(_buffer, read); if (read != expectedTypeId) {pcl::exception("iotl::iStreamer::restoreTypeId()",std::string("Unexpected object type: ")+read);}}
    inline void restoreTypeId(std::istream& cxxx, const char* expectedTypeId)  {std::string read; pcl::BasicIoStreamer::restoreBasicTypeCStr(cxxx, read);    if (read != expectedTypeId) {pcl::exception("iotl::iStreamer::restoreTypeId()",std::string("Unexpected object type: ")+read);}}
    
  protected:
    bool putEvent(std::istream& cxxx, bool ignore);
    ptl::Id restoreAbstractObject(ptl::ObjectBase** ppobj);
    inline void restoreId(ptl::MutableId& persistentId)        {restoreMemory((char*)&persistentId, 4); persistentId *= -1;}
    inline void restoreMemory(char* address, const int& bytes) {pcl::BasicIoStreamer::redumpMemory(_buffer, address, bytes);}
    
    std::stringstream _buffer;
  private:
    int unzipEventData( std::istream &in, int nBytes);
    
    unsigned char _inputBuffer[iotl__iStreamer__lengthUnzipBuffer];
    unsigned char _outputBuffer[iotl__iStreamer__lengthUnzipBuffer];
};
//
//
//
//----------------------------------------------------------------------
class iFile {
  public:
      virtual ~iFile() {;}

      virtual bool open(const std::string& filename) {return false;}
      virtual void close()     {;}
      virtual bool readEvent() {return false;}
              int  skipEvent() {return skipEvents(1);}
      virtual int  skipEvents(int n) {return 0;}
      virtual bool endOfFile() {return true;}
  protected:
      iFile() {;}
  private:      
      iFile(const iotl::iFile&) {;}
};  
//
//
//
//----------------------------------------------------------------------
template<class istreamertype> class iDiskFileVx : public iotl::iFile, public istreamertype {

  public:
      iDiskFileVx() : iotl::iFile(), istreamertype() {;}
      virtual ~iDiskFileVx() {close();}
      virtual bool open(const std::string& filename) {
					      close();
					      _file.open( filename.c_str(), std::fstream::in | std::fstream::binary );
					      return _file.is_open();
					      }
      virtual void close()     {if(!_file.is_open()) return; _file.close();}
      virtual bool readEvent() {if(_file.is_open()) {return istreamertype::putEvent(_file);} else {pcl::exception("iotl::iDiskFileVx<>::readEvent()","No file open."); return false;}}
      virtual int  skipEvents(int n) {int s = 0;
      	                       while (n < 0) {if (!istreamertype::previous(_file)) break; n++; s--;}
      	                       while (n > 0) {if (!istreamertype::next(_file)) break; n--; s++;}
      	                       return s;
                               }
      virtual bool endOfFile() {return _file.peek() == EOF;}
      
  protected:
      std::fstream _file;
      
  private:      
      iDiskFileVx(const iotl::iDiskFileVx<istreamertype>&) {;}
  
};  
//----------------------------------------------------------------------
typedef iotl::iDiskFileVx<iotl::iStreamer> iDiskFile;
//----------------------------------------------------------------------
//
//
//----------------------------------------------------------------------
} // namespace iotl
//----------------------------------------------------------------------

#ifndef MERGED_PXL
	#include "iotl.icc"
#endif

#endif
//
//
//----------------------------------------------------------------------
// iotl
//----------------------------------------------------------------------
// oStreamer:
//----------------------------------------------------------------------
template<class itemtype> void iotl::oStreamer::storeData(const ptl::Vector<itemtype>& vector) {

  typedef typename ptl::Vector<itemtype>::StlConstIterator PtlVectorConstIterator;
    
  storeData<int>(vector.getSize());
  for (PtlVectorConstIterator iter = vector.begin(); iter != vector.end(); iter++) {storeData<itemtype>(*iter);}

}
//----------------------------------------------------------------------
template<class keytype, class itemtype> void iotl::oStreamer::storeData(const ptl::Map<keytype, itemtype>& map) {

  typedef typename ptl::Map<keytype, itemtype>::StlConstIterator PtlMapConstIterator;

  storeData<int>(map.getSize());
  for (PtlMapConstIterator iter = map.begin(); iter != map.end(); iter++) {storeData<keytype>(iter->first);storeData<itemtype>(iter->second);}

}
//----------------------------------------------------------------------
// iStreamer
//----------------------------------------------------------------------
template<class itemtype> ptl::Id iotl::iStreamer::restoreData(ptl::Vector<itemtype>& vector) {

  vector.clearContainer();
  
  int size; iotl::iStreamer::restoreData<int>(size);
  for (int i=0; i<size; i++) {itemtype item; iotl::iStreamer::restoreData<itemtype>(item); vector.set(item);}
  return 0;

}
//----------------------------------------------------------------------
template<class keytype, class itemtype> ptl::Id iotl::iStreamer::restoreData(ptl::Map<keytype, itemtype>& map) {

  map.clearContainer();
  
  int size; restoreData<int>(size);
  for (int i=0; i<size; i++) {
      keytype key; restoreData<keytype>(key); 
      itemtype item; restoreData<itemtype>(item); 
      map.set(key, item);
      }
  return 0;
}
//----------------------------------------------------------------------
// TypeManager
//----------------------------------------------------------------------
template<class objecttype> iotl::TypeAgent<objecttype>::TypeAgent(const std::string& objectTypeId) {
	
  objecttype obj; 

  _objectTypeId = objectTypeId;
  _dataTypeId   = iotl::getIotlTypeId(&obj.get()); 
  _cppTypeId    = std::string(typeid(obj).name());
 
  iotl::TypeManager::instance().registerAgent(this);
}
//----------------------------------------------------------------------
template<class objecttype> void iotl::TypeAgent<objecttype>::storeObject(iotl::oStreamer& output, const ptl::ObjectBase& obj) {
  const objecttype* ptr = dynamic_cast<const objecttype*>(&obj);
  output.storeTypeId(_objectTypeId.c_str()); 
  output.storeTypeId(_dataTypeId.c_str()); 
  output.storeData(ptr->get()); 
  output.storeData(obj);
}
//----------------------------------------------------------------------
template<class objecttype> ptl::Id iotl::TypeAgent<objecttype>::restoreObject(iotl::iStreamer& input, ptl::ObjectBase& obj) {
  objecttype* ptr = dynamic_cast<objecttype*>(&obj);
  input.restoreTypeId(_objectTypeId.c_str()); 
  input.restoreTypeId(_dataTypeId.c_str()); 
  input.restoreData(ptr->set()); 
  return input.restoreData(obj);
}
//----------------------------------------------------------------------
template<class objecttype> ptl::Id iotl::TypeAgent<objecttype>::restoreObject(iotl::iStreamer& input, ptl::ObjectBase** ppobj) {
  objecttype* ptr = new objecttype;
  // please notice: object & data type ids have been read already! 
  input.restoreData(ptr->set()); 
  (*ppobj) = dynamic_cast<ptl::ObjectBase*>(ptr);
  return input.restoreData(**ppobj);
}
//----------------------------------------------------------------------
//
//
//----------------------------------------------------------------------
// ptl
//----------------------------------------------------------------------
template<class datatype> void ptl::Object<datatype>::storeYourSelf(iotl::oStreamer& output) const {output.storeObject(*this);}
//----------------------------------------------------------------------
template<class datatype> void ptl::CowObject<datatype>::storeYourSelf(iotl::oStreamer& output) const {output.storeObject(*this);}
//----------------------------------------------------------------------
template<class datatype> void ptl::SpyObject<datatype>::storeYourSelf(iotl::oStreamer& output) const {output.storeObject(*this);}
//----------------------------------------------------------------------
#ifndef pol_hh
#define pol_hh

#include <cmath>

#ifndef MERGED_PXL
	#include "ptl.hh"
#endif

//
//
//----------------------------------------------------------------------
namespace pol {
//----------------------------------------------------------------------
static const ptl::Get get;
static const ptl::Set set;
//----------------------------------------------------------------------
typedef ptl::CopyHistory CopyHistory;
typedef CopyHistory& CopyHistoryRef;
typedef const CopyHistory& CopyHistoryConstRef;
//----------------------------------------------------------------------
typedef ptl::Index Index;
typedef Index& IndexRef;
typedef const Index& IndexConstRef;
//----------------------------------------------------------------------
typedef ptl::Objects Objects;
typedef Objects& ObjectsRef;
typedef const Objects& ObjectsConstRef;
//----------------------------------------------------------------------
typedef ptl::Relations Relations;
typedef Relations& RelationsRef;
typedef const Relations& RelationsConstRef;
//----------------------------------------------------------------------
typedef ptl::Map<std::string, double > UserRecords;
typedef UserRecords& UserRecordsRef;
typedef const UserRecords& UserRecordsConstRef;
//----------------------------------------------------------------------
typedef ptl::Map<std::string, void* > CppPointers;
typedef CppPointers& CppPointersRef;
typedef const CppPointers& CppPointersConstRef;
//----------------------------------------------------------------------
class Basic4VectorData {
    private:
    double _x;
    double _y;
    double _z;
    double _t;

    public:
    Basic4VectorData() {_t=_x=_y=_z=0.;}
    Basic4VectorData(double x, double y, double z, double t) {_t=t; _x=x; _y=y; _z=z;}

    inline void           setPx( double px ) {_x = px;}
    inline void           setPy( double py ) {_y = py;}
    inline void           setPz( double pz ) {_z = pz;}
    inline void           setE ( double e  ) {_t = e;}
    inline void           setMass( double m ){_t = sqrt(m*m+_x*_x+_y*_y+_z*_z);}

    inline void           setX ( double x )  {_x = x;}
    inline void           setY ( double y )  {_y = y;}
    inline void           setZ ( double z )  {_z = z;}
    inline void           setT ( double t )  {_t = t;}

    inline double         getPx()    const {return _x;}
    inline double         getPy()    const {return _y;}
    inline double         getPz()    const {return _z;}
    inline double         getE ()    const {return _t;}

    inline double         getX()     const {return _x;}
    inline double         getY()     const {return _y;}
    inline double         getZ()     const {return _z;}
    inline double         getT()     const {return _t;}

    inline double         getMass()  const {return sqrt(_t*_t-_x*_x-_y*_y-_z*_z);}
    inline double         getPerp()  const {return sqrt(_x*_x + _y*_y);}
    inline double         getPt()    const {return getPerp();}
    inline double         getPhi()   const {return (_x == 0.0 && _y == 0.0) ? 0.0 : atan2(_y,_x);}
    inline double         getTheta() const {return (_x == 0.0 && _y == 0.0 && _z == 0.0) ? 0.0 : atan2(getPerp(),_z);}
    inline double         getEta()   const {return -log(tan(getTheta()/2.));}

    inline const pol::Basic4VectorData& operator+=( const pol::Basic4VectorData& vec) {_x += vec._x; _y += vec._y; _z += vec._z; _t += vec._t; return (*this);}
    inline const pol::Basic4VectorData& operator-=( const pol::Basic4VectorData& vec) {_x -= vec._x; _y -= vec._y; _z -= vec._z; _t -= vec._t; return (*this);}

  friend class iotl::iStreamer;
  friend class iotl::oStreamer;      
  };
// non-member operators
bool const operator==(const pol::Basic4VectorData& obj1, const pol::Basic4VectorData& obj2);
bool const operator!=(const pol::Basic4VectorData& obj1, const pol::Basic4VectorData& obj2);
//----------------------------------------------------------------------
class Basic3VectorData {
    private:
    double _x;
    double _y;
    double _z;

    public:
    Basic3VectorData() {_x=_y=_z=0.;}
    Basic3VectorData(double x, double y, double z) {_x=x; _y=y; _z=z;}

    inline void           setX ( double x )  {_x = x;}
    inline void           setY ( double y )  {_y = y;}
    inline void           setZ ( double z )  {_z = z;}

    inline double         getX()     const {return _x;}
    inline double         getY()     const {return _y;}
    inline double         getZ()     const {return _z;}

    inline const pol::Basic3VectorData& operator+=( const pol::Basic3VectorData& vec) {_x += vec._x; _y += vec._y; _z += vec._z; return (*this);}
    inline const pol::Basic3VectorData& operator-=( const pol::Basic3VectorData& vec) {_x -= vec._x; _y -= vec._y; _z -= vec._z; return (*this);}

  friend class iotl::iStreamer;
  friend class iotl::oStreamer;      
  };
// non-member operators
bool const operator==(const pol::Basic3VectorData& obj1, const pol::Basic3VectorData& obj2);
bool const operator!=(const pol::Basic3VectorData& obj1, const pol::Basic3VectorData& obj2);
//----------------------------------------------------------------------
class BasicObjectData {
  public: 
    BasicObjectData() : _locked(0), _monteCarloMode(0), _name("default"), _status(0), _workflag(0), _userRecords(), _cppPointers() {;}    
    
    inline bool        getLocked()   const {return _locked;}
    inline int         getMonteCarloMode() const {return _monteCarloMode;}
    inline std::string getName()     const {return _name;}
    inline int         getStatus()   const {return _status;}
    inline int         getWorkflag() const {return _workflag;}

    inline void setLocked(bool v)      {_locked = v ;}
    inline void setMonteCarloMode(int v) {_monteCarloMode = v ;}
    inline void setName(std::string v) {_name = v ;}
    inline void setStatus(int v)       {_status = v ;}
    inline void setWorkflag(int v)     {_workflag = v ;}
    
    inline const pol::UserRecords& getUserRecords() const {return _userRecords;}
    inline const pol::CppPointers& getCppPointers() const {return _cppPointers;}

    inline void setUserRecord(const std::string& key, double item) {_userRecords.set(key, item);}
    inline void setCppPointer(const std::string& key, void* cptr)  {_cppPointers.set(key, cptr);}
    
    inline void removeUserRecord(const std::string& key) {_userRecords.remove(key);}
    inline void removeCppPointer(const std::string& key) {_cppPointers.remove(key);}
    
    inline double findUserRecord(const std::string& key, double defaultitem) const {return _userRecords.find(key, defaultitem);}
    inline double findUserRecord(const std::string& key)                     const {return _userRecords.find(key);}
    template<class datatype> inline datatype* findCppPointer(const std::string& key) const {return dynamic_cast<datatype*>(_cppPointers.find(key, 0));}
    
  protected:
    bool _locked;
    int _monteCarloMode;
    std::string _name;
    int _status;
    int _workflag;
    
    pol::UserRecords _userRecords;
    pol::CppPointers _cppPointers;

  friend class iotl::iStreamer;
  friend class iotl::oStreamer;      
};
typedef ptl::CowObject<pol::BasicObjectData> BasicObject;
typedef ptl::WkPtrSpec<pol::BasicObjectData, pol::BasicObject> BasicObjectWkPtr;
typedef BasicObject& BasicObjectRef;
typedef const BasicObject& BasicObjectConstRef;
//----------------------------------------------------------------------
class BasicObjectManagerData : public pol::BasicObjectData {
  public: 
    BasicObjectManagerData() : BasicObjectData(), _objects() {;}
    BasicObjectManagerData(const pol::BasicObjectManagerData& original) : BasicObjectData(original), _objects(original._objects) {;}
    
    template<class datatype>                    datatype& create()                       {return _objects.create<datatype>();}
    template<class datatype, class ctrdatatype> datatype& create(const ctrdatatype& ori) {return _objects.create<datatype,ctrdatatype>(ori);}
    
    template<class datatype>                    datatype& createIndexed(const std::string& idx)                         {datatype& obj = _objects.create<datatype>(); setIndex(idx, obj); return obj;}
    template<class datatype, class ctrdatatype> datatype& createIndexed(const ctrdatatype& ori, const std::string& idx) {datatype& obj = _objects.create<datatype,ctrdatatype>(ori); setIndex(idx, obj); return obj;}
    
    inline void setObject(ptl::ObjectBase& obj, const std::string idx) {_objects.set(obj); setIndex(idx, obj);}
    inline bool setIndex(const std::string& idx, ptl::ObjectBase& obj) {return _objects.setIndex(idx, obj);}
    
    inline const pol::Objects&                    getObjects() const {return _objects;}
    inline void                                   removeObject(ptl::ObjectBase& obj) {_objects.remove(obj);}
    inline void                                   clearObjects() {_objects.clearContainer();}
    template<class objecttype> inline objecttype* findObject(const std::string idx)           const {return _objects.findObject<objecttype>(idx);}
    template<class objecttype> inline objecttype* findCopyOf(const ptl::ObjectBase& original) const {return _objects.findCopyOf<objecttype>(original);}

    inline const pol::CopyHistory& getCopyHistory() const {return _objects.getCopyHistory();}
    inline void                    clearCopyHistory() {_objects.clearCopyHistory();}
    
    inline const pol::Index& getIndex() const {return _objects.getIndex();}
    inline void              removeIndex(const std::string& idx) {_objects.removeIndex(idx);}
    inline void              clearIndex() {_objects.clearIndex();}

  protected:
    pol::Objects     _objects;

  friend class iotl::iStreamer;
  friend class iotl::oStreamer;      
};
typedef ptl::Object<pol::BasicObjectManagerData> BasicObjectManager; // (NOTICE: BasicObjectManagers can not be managed by Copy On Write!)
typedef ptl::WkPtrSpec<pol::BasicObjectManagerData, pol::BasicObjectManager> BasicObjectManagerWkPtr;
typedef BasicObjectManager& BasicObjectManagerRef;
typedef const BasicObjectManager& BasicObjectManagerConstRef;
//----------------------------------------------------------------------
class ParticleData : public pol::BasicObjectData {
  public: 
    ParticleData() : BasicObjectData(), _vector(), _charge(0), _particleId(0) {;}
    
    inline const pol::Basic4VectorData& vector()                const {return _vector;}
    inline const pol::Basic4VectorData& vector(const ptl::Get&) const {return _vector;}
    inline       pol::Basic4VectorData& vector(const ptl::Set&)       {return _vector;}

    inline double getCharge() const {return _charge;}
    inline void setCharge(double v)  {_charge = v;}

    inline int getParticleId() const {return _particleId;}
    inline void setParticleId(int v)  {_particleId = v;}

    inline const pol::ParticleData& operator+=( const pol::ParticleData& vec) {this->vector(pol::set)+= vec.vector(); _charge += vec._charge; return (*this);}
    inline const pol::ParticleData& operator-=( const pol::ParticleData& vec) {this->vector(pol::set)-= vec.vector(); _charge += vec._charge; return (*this);}

  protected:
    pol::Basic4VectorData _vector;
    double _charge;
    int _particleId;
    
  friend class iotl::iStreamer;
  friend class iotl::oStreamer;      
};    
// non-member operators
bool const operator==(const pol::ParticleData& obj1, const pol::ParticleData& obj2);
bool const operator!=(const pol::ParticleData& obj1, const pol::ParticleData& obj2);
// typedefs
/// data=>pol::ParticleData
typedef ptl::CowObject<pol::ParticleData> Particle;
typedef ptl::WkPtrSpec<pol::ParticleData, pol::Particle> ParticleWkPtr;
typedef Particle& ParticleRef;
typedef const Particle& ParticleConstRef;
//----------------------------------------------------------------------
class VertexData : public pol::BasicObjectData {
  public: 
    inline const pol::Basic3VectorData& vector()                const {return _vector;}
    inline const pol::Basic3VectorData& vector(const ptl::Get&) const {return _vector;}
    inline       pol::Basic3VectorData& vector(const ptl::Set&)       {return _vector;}
    
    inline const pol::VertexData& operator+=( const pol::VertexData& vec) {this->vector(pol::set) += vec.vector(); return (*this);}
    inline const pol::VertexData& operator-=( const pol::VertexData& vec) {this->vector(pol::set) -= vec.vector(); return (*this);}

  protected:
    pol::Basic3VectorData _vector;
      
  friend class iotl::iStreamer;
  friend class iotl::oStreamer;
};
// non-member operators
bool const operator==(const pol::VertexData& obj1, const pol::VertexData& obj2);
bool const operator!=(const pol::VertexData& obj1, const pol::VertexData& obj2);
// typedefs
/// data=>pol::VertexData
typedef ptl::CowObject<pol::VertexData> Vertex;
typedef ptl::WkPtrSpec<pol::VertexData, pol::Vertex> VertexWkPtr;
typedef Vertex& VertexRef;
typedef const Vertex& VertexConstRef;
//----------------------------------------------------------------------
typedef pol::BasicObjectData CollisionData;
// typedefs
/// data=>pol::CollisionData
typedef ptl::CowObject<pol::CollisionData> Collision;
typedef ptl::WkPtrSpec<pol::CollisionData, pol::Collision> CollisionWkPtr;
typedef Collision& CollisionRef;
typedef const Collision& CollisionConstRef;
//----------------------------------------------------------------------
class EventViewData : public pol::BasicObjectManagerData {
  public: 
    EventViewData() : BasicObjectManagerData() {;}
    EventViewData(const pol::EventViewData& original) : BasicObjectManagerData(original) {;}

  friend class iotl::iStreamer;
  friend class iotl::oStreamer;      
};
/// data=>pol::EventViewData
typedef ptl::Object<pol::EventViewData> EventView; // (NOTICE: EventViews can not be managed by Copy On Write!)
typedef ptl::WkPtrSpec<pol::EventViewData, pol::EventView> EventViewWkPtr;
typedef EventView& EventViewRef;
typedef const EventView& EventViewConstRef;
//----------------------------------------------------------------------
class AnalysisProcessData : public pol::BasicObjectManagerData {
  public: 
    AnalysisProcessData() : BasicObjectManagerData() {;}
    AnalysisProcessData(const pol::AnalysisProcessData& original) : BasicObjectManagerData(original) {;}

  friend class iotl::iStreamer;
  friend class iotl::oStreamer;      
};
/// data=>pol::AnalysisProcessData
class AnalysisProcess : public ptl::Object<pol::AnalysisProcessData> { // (NOTICE: AnalysisProcesses can not be managed by Copy On Write!)
  public: 
    AnalysisProcess() : ptl::Object<pol::AnalysisProcessData>() {;}
    AnalysisProcess(const pol::AnalysisProcess& original) : ptl::Object<pol::AnalysisProcessData>(original) {;}
    virtual ~AnalysisProcess() {;}
    
    virtual void buildTemplate(int mode = 0) {;}

    virtual void beginJob(const pol::Objects*input = 0) {;}
    virtual void beginRun(const pol::Objects*input = 0) {;}
    virtual void analyseEvent(const pol::Objects*input = 0) {;}
    virtual void finishEvent(const pol::Objects*input = 0)  {set().clearObjects();}
    virtual void endRun(const pol::Objects*input = 0) {;}
    virtual void endJob(const pol::Objects*input = 0) {;}

    template<class objecttype> const objecttype& castInput(const pol::Objects*input) {if (!dynamic_cast<const objecttype*>(input)) std::cerr << "pol::AnalysisProcess::castInput(): FATAL: The pointer you intend to cast does not exist!" << std::endl; return *dynamic_cast<const objecttype*>(input);}

    virtual ptl::ObjectBase* clone() const;
  protected:    
    virtual ptl::WkPtrBase* createSelfWkPtr();
    virtual void storeYourSelf(iotl::oStreamer& output) const;
};
typedef ptl::WkPtrSpec<pol::AnalysisProcessData, pol::AnalysisProcess> AnalysisProcessWkPtr;
typedef AnalysisProcess& AnalysisProcessRef;
typedef const AnalysisProcess& AnalysisProcessConstRef;
//----------------------------------------------------------------------
class AnalysisForkData : public pol::BasicObjectManagerData {
  public: 
    AnalysisForkData() : BasicObjectManagerData() {;}
    AnalysisForkData(const pol::AnalysisForkData& original) : BasicObjectManagerData(original) {;}

  friend class iotl::iStreamer;
  friend class iotl::oStreamer;      
};
/// data=>pol::AnalysisForkData
class AnalysisFork : public ptl::Object<pol::AnalysisForkData> {  // (NOTICE: AnalysisForks can not be managed by Copy On Write!)
  public: 
    AnalysisFork() : ptl::Object<pol::AnalysisForkData>() {;}
    AnalysisFork(const pol::AnalysisFork& original) : ptl::Object<pol::AnalysisForkData>(original) {;}
    virtual ~AnalysisFork() {;}
    
    virtual void buildTemplate(int mode = 0);
    
    virtual void beginJob(const pol::Objects*input = 0);
    virtual void beginRun(const pol::Objects*input = 0);
    virtual void analyseEvent(const pol::Objects*input = 0);
    virtual void finishEvent(const pol::Objects*input = 0);
    virtual void endRun(const pol::Objects*input = 0);
    virtual void endJob(const pol::Objects*input = 0);

    virtual ptl::ObjectBase* clone() const;
  protected:    
    virtual ptl::WkPtrBase* createSelfWkPtr();
    virtual void storeYourSelf(iotl::oStreamer& output) const;
    
};
typedef ptl::WkPtrSpec<pol::AnalysisForkData, pol::AnalysisFork> AnalysisForkWkPtr;
typedef AnalysisFork& AnalysisForkRef;
typedef const AnalysisFork& AnalysisForkConstRef;
//----------------------------------------------------------------------
} // namespace pol
//----------------------------------------------------------------------
#endif
#ifndef epax_hh
#define epax_hh

#ifndef MERGED_PXL
	#include <pxl.hh>
#endif	
//
//
//----------------------------------------------------------------------
namespace ePax {
//----------------------------------------------------------------------
static const ptl::Get get;
static const ptl::Set set;
//----------------------------------------------------------------------
typedef pol::Objects ePaxObjects;
typedef ePaxObjects& ePaxObjectsRef;
typedef const ePaxObjects& ePaxObjectsConstRef;
//----------------------------------------------------------------------
typedef pol::Relations ePaxRelations;
typedef ePaxRelations& ePaxRelationsRef;
typedef const ePaxRelations& ePaxRelationsConstRef;
//----------------------------------------------------------------------
typedef pol::CopyHistory ePaxCopyHistory;
typedef ePaxCopyHistory& ePaxCopyHistoryRef;
typedef const ePaxCopyHistory& ePaxCopyHistoryConstRef;
//----------------------------------------------------------------------
typedef pol::Index ePaxIndex;
typedef ePaxIndex& ePaxIndexRef;
typedef const ePaxIndex& ePaxIndexConstRef;
//----------------------------------------------------------------------
typedef pol::UserRecords ePaxUserRecords;
typedef ePaxUserRecords& ePaxUserRecordsRef;
typedef const ePaxUserRecords& ePaxUserRecordsConstRef;
//----------------------------------------------------------------------
typedef pol::CppPointers ePaxCppPointers;
typedef ePaxCppPointers& ePaxCppPointersRef;
typedef const ePaxCppPointers& ePaxCppPointersConstRef;
//----------------------------------------------------------------------
// PAXify pol::AnalysisFork
/// object=>pol::AnalysisFork data=>pol::AnalysisForkData
class ePaxAnalysisFork : public pol::AnalysisFork {

  public: 
    // Gero-style service methods go here...
//     template<class datatype> datatype& create(const std::string idx)                                                 {return set().create<datatype>(idx);}
//     template<class datatype> datatype& create(const datatype& original, const std::string idx)                       {return set().create<datatype>(original, idx);}
//     template<class datatype, class ctrdatatype> datatype& create(const ctrdatatype& original, const std::string idx) {return set().create<datatype,ctrdatatype>(original, idx);}
//     
//     inline void setUserRecord(const std::string& key, double item) {set().setUserRecord(key, item);}
//     inline void setCppPointer(const std::string& key, void* cptr)  {set().setCppPointer(key, cptr);}
//     ...


    virtual ptl::ObjectBase* clone() const;
    virtual std::ostream&  print(int level = 0, std::ostream& os = std::cout, int pan = 0) const;
  protected:    
    virtual ptl::WkPtrBase* createSelfWkPtr();
    virtual void storeYourSelf(iotl::oStreamer& output) const;
};
typedef ptl::WkPtrSpec<pol::AnalysisForkData, ePaxAnalysisFork> ePaxAnalysisForkWkPtr;
typedef ePaxAnalysisFork& ePaxAnalysisForkRef;
typedef const ePaxAnalysisFork& ePaxAnalysisForkConstRef;
//----------------------------------------------------------------------
// PAXify pol::AnalysisProcess
/// object=>pol::AnalysisProcess data=>pol::AnalysisProcessData
class ePaxAnalysisProcess : public pol::AnalysisProcess {

  public: 
    // Gero-style service methods go here...
    // ...

    virtual ptl::ObjectBase* clone() const;
    virtual std::ostream&  print(int level = 0, std::ostream& os = std::cout, int pan = 0) const;
  protected:    
    virtual ptl::WkPtrBase* createSelfWkPtr();
    virtual void storeYourSelf(iotl::oStreamer& output) const;
};
typedef ptl::WkPtrSpec<pol::AnalysisProcessData, ePaxAnalysisProcess> ePaxAnalysisProcessWkPtr;
typedef ePaxAnalysisProcess& ePaxAnalysisProcessRef;
typedef const ePaxAnalysisProcess& ePaxAnalysisProcessConstRef;
//----------------------------------------------------------------------
// PAXify pol::EventView
/// object=>pol::EventView data=>pol::EventViewData
class ePaxEventView : public pol::EventView {

  public: 
    // Gero-style service methods go here...
    // ...

    virtual ptl::ObjectBase* clone() const;
    virtual std::ostream&  print(int level = 0, std::ostream& os = std::cout, int pan = 0) const;
  protected:    
    virtual ptl::WkPtrBase* createSelfWkPtr();
    virtual void storeYourSelf(iotl::oStreamer& output) const;
};
typedef ptl::WkPtrSpec<pol::EventViewData, ePaxEventView> ePaxEventViewWkPtr;
typedef ePaxEventView& ePaxEventViewRef;
typedef const ePaxEventView& ePaxEventViewConstRef;
//----------------------------------------------------------------------
// PAXify pol::Particle
/// object=>pol::Particle data=>pol::ParticleData
class ePaxParticle : public pol::Particle {

  public: 
    // Gero-style service methods go here...
    // ...

    virtual ptl::ObjectBase* clone() const;
    virtual std::ostream&  print(int level = 0, std::ostream& os = std::cout, int pan = 0) const;
  protected:    
    virtual ptl::WkPtrBase* createSelfWkPtr();
    virtual void storeYourSelf(iotl::oStreamer& output) const;
};
typedef ptl::WkPtrSpec<pol::ParticleData, ePaxParticle> ePaxParticleWkPtr;
typedef ePaxParticle& ePaxParticleRef;
typedef const ePaxParticle& ePaxParticleConstRef;
//----------------------------------------------------------------------
// PAXify pol::Vertex
/// object=>pol::Vertex data=>pol::VertexData
class ePaxVertex : public pol::Vertex {

  public: 
    // Gero-style service methods go here...
    // ...

    virtual ptl::ObjectBase* clone() const;
    virtual std::ostream&  print(int level = 0, std::ostream& os = std::cout, int pan = 0) const;
  protected:    
    virtual ptl::WkPtrBase* createSelfWkPtr();
    virtual void storeYourSelf(iotl::oStreamer& output) const;
};
typedef ptl::WkPtrSpec<pol::VertexData, ePaxVertex> ePaxVertexWkPtr;
typedef ePaxVertex& ePaxVertexRef;
typedef const ePaxVertex& ePaxVertexConstRef;
//----------------------------------------------------------------------
// PAXify pol::Collision
/// object=>pol::Collision data=>pol::CollisionData
class ePaxCollision : public pol::Collision {

  public: 
    // Gero-style service methods go here...
    // ...

    virtual ptl::ObjectBase* clone() const;
    virtual std::ostream&  print(int level = 0, std::ostream& os = std::cout, int pan = 0) const;
  protected:    
    virtual ptl::WkPtrBase* createSelfWkPtr();
    virtual void storeYourSelf(iotl::oStreamer& output) const;
};
typedef ptl::WkPtrSpec<pol::CollisionData, ePaxCollision> ePaxCollisionWkPtr;
typedef ePaxCollision& ePaxCollisionRef;
typedef const ePaxCollision& ePaxCollisionConstRef;
//----------------------------------------------------------------------
} // namespace ePax
//----------------------------------------------------------------------

#endif
