#ifndef object_ptr_H
#define object_ptr_H

#include "object/object_proxy.hpp"

#include <memory>
#include <iostream>
#include <typeinfo>

class object;

class base_object_ptr {
protected:
	explicit base_object_ptr(bool is_ref);
	base_object_ptr(const base_object_ptr &x);
	base_object_ptr& operator=(const base_object_ptr &x);
	base_object_ptr(object* o, bool is_ref);
	base_object_ptr(object_proxy *op, bool is_ref);
	virtual ~base_object_ptr();

public:
	bool operator==(const base_object_ptr &x) const;
	bool operator!=(const base_object_ptr &x) const;

	void reset(object *o);

	bool is_loaded() const;

	long id() const;

	object* ptr() const;

	object* lookup_object();

  bool delete_object();

  bool is_reference() const;

protected:
	friend class object_atomizer;
  template < class T > friend class object_ref;

	long id_;
  object_proxy *proxy_;
  bool is_reference_;
};

template < class T >
class object_ptr : public base_object_ptr
{
public:
	object_ptr() : base_object_ptr(true) {}
	//  object_ptr(const object_ptr &x) {}
	template < class Y >
	object_ptr(const object_ptr<Y> &x) {}
	//  object_ptr& operator=(const object_ptr &x) { return *this; }
	template < class Y >
	object_ptr& operator=(const object_ptr<Y> &x) { return *this; }
	object_ptr(object* o) : base_object_ptr(o, false) {}

	T* operator->() {
	  if (proxy_) {
	    return dynamic_cast<T*>(lookup_object());
	  }
	  return NULL;
	}
	T& operator*() {
		if (proxy_) {
      return *dynamic_cast<T*>(lookup_object());
 		}
    return NULL;
	}
	T* get() {
		if (proxy_) {
      return dynamic_cast<T*>(lookup_object());
 		}
    return NULL;
	}
  //friend class object_ref;
};

template < class T >
class object_ref : public base_object_ptr
{
public:
	object_ref() : base_object_ptr(false) {}
	//  object_ptr(const object_ptr &x) {}
	template < class Y >
	object_ref(const object_ref<Y> &x) {}
	template < class Y >
  object_ref(const object_ptr<Y> &x) : base_object_ptr(x.proxy_, true) {}
	//  object_ptr& operator=(const object_ptr &x) { return *this; }
	template < class Y >
	object_ref& operator=(const object_ref<Y> &x) { return *this; }
	object_ref(object* o) : base_object_ptr(o, false) {}

	T* operator->() {
	  if (proxy_) {
	    return dynamic_cast<T*>(lookup_object());
	  }
	  return NULL;
	}
	T& operator*() {
		if (proxy_) {
      return *dynamic_cast<T*>(lookup_object());
 		}
    return NULL;
	}
	T* get() {
		if (proxy_) {
      return dynamic_cast<T*>(lookup_object());
 		}
    return NULL;
	}
};

#endif /* object_ptr_H */