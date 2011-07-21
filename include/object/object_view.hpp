#ifndef OBJECTVIEW_HPP
#define OBJECTVIEW_HPP

#include "object/object_store.hpp"
#include "object/object_proxy.hpp"
#include "object/object_ptr.hpp"

template < class T >
class object_view_iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
public:
  typedef object_view_iterator<T> self;	
  typedef object_ptr<T> value_type;
  typedef T* pointer;
  typedef value_type& reference ;

  object_view_iterator()
    : node_(NULL)
  {}
  object_view_iterator(object_store::prototype_node *node, object_proxy *current, object_proxy *last)
    : node_(node)
    , current_(current)
    , last_(last)
  {}

  object_view_iterator(const object_view_iterator &x)
    : node_(x.node_)
    , current_(x.current_)
    , last_(x.last_)
  {}
  object_view_iterator& operator=(const object_view_iterator &x)
  {
    node_ = x.node_;
    current_ = x.current_;
    last_ = x.last_;
    return *this;
  }

  ~object_view_iterator() {}

  bool operator==(const object_view_iterator &i) const {
    return (current_ == i.current_);
  }

  bool operator!=(const object_view_iterator &i) const {
    return (current_ != i.current_);
  }

  self& operator++() {
    increment();
    return *this;
  }

  self operator++(int) {
    object_proxy *tmp = current_;
    increment();
    return object_view_iterator(node_, tmp, last_);
  }

  self& operator--() {
    decrement();
    return *this;
  }

  self operator--(int) {
    object_proxy *tmp = current_;
    decrement();
    return object_view_iterator(node_, tmp, last_);
  }

  pointer operator->() const {
    return static_cast<pointer>(current_->obj);
  }

  value_type operator*() const { return this->optr(); }

  object_ptr<T> optr() const {
    if (current_->obj)
      return object_ptr<T>(current_->obj);
    else
      return object_ptr<T>();
  }

//private:
  void increment() {
    if (current_ != last_) {
      current_ = current_->next;
    }
  }
  void decrement() {
    if (current_ != node_->op_first) {
      current_ = current_->prev;
    }
  }

//private:
  object_store::prototype_node *node_;
  object_proxy *current_;
  object_proxy *last_;
};

template < class T >
class const_object_view_iterator : public std::iterator<std::bidirectional_iterator_tag, T, std::ptrdiff_t, const T*, const T&> {
public:
  typedef const_object_view_iterator<T> self;
  typedef object_ptr<T> value_type;
  typedef T* pointer;
  typedef value_type& reference ;

  const_object_view_iterator()
    : node_(NULL)
  {}
  const_object_view_iterator(object_store::prototype_node *node, object_proxy *current, object_proxy *last)
    : node_(node)
    , current_(current)
    , last_(last)
  {}

  const_object_view_iterator(const const_object_view_iterator &x)
    : node_(x.node_)
    , current_(x.current_)
    , last_(x.last_)
  {}
  const_object_view_iterator(const object_view_iterator<T> &x)
    : node_(x.node_)
    , current_(x.current_)
    , last_(x.last_)
  {}
  const_object_view_iterator& operator=(const const_object_view_iterator &x)
  {
    node_ = x.node_;
    current_ = x.current_;
    last_ = x.last_;
    return *this;
  }
  const_object_view_iterator& operator=(const object_view_iterator<T> &x)
  {
    node_ = x.node_;
    current_ = x.current_;
    last_ = x.last_;
    return *this;
  }

  ~const_object_view_iterator() {}

  bool operator==(const const_object_view_iterator &i) const {
    return (current_ == i.current_);
  }

  bool operator!=(const const_object_view_iterator &i) const {
    return (current_ != i.current_);
  }

  self& operator++() {
    increment();
    return *this;
  }

  self operator++(int) {
    object_proxy *tmp = current_;
    increment();
    return const_object_view_iterator(node_, tmp, last_);
  }

  self& operator--() {
    decrement();
    return *this;
  }

  self operator--(int) {
    object_proxy *tmp = current_;
    decrement();
    return const_object_view_iterator(node_, tmp, last_);
  }

  pointer operator->() const {
    return static_cast<pointer>(current_->obj);
  }

  value_type operator*() const { return this->optr(); }

  object_ptr<T> optr() const {
    if (current_->obj)
      return object_ptr<T>(current_->obj);
    else
      return object_ptr<T>();
  }

private:
  void increment() {
    if (current_ != last_) {
      current_ = current_->next;
    }
  }
  void decrement() {
    if (current_ != node_->op_first) {
      current_ = current_->prev;
    }
  }

private:
  object_store::prototype_node *node_;
  object_proxy *current_;
  object_proxy *last_;
};

template < class T >
class object_view
{
public:
  typedef object_view_iterator<T> iterator;
  typedef const_object_view_iterator<T> const_iterator;

  object_view(object_store &ostore, bool skip_siblings = false)
    : ostore_(ostore)
    , skip_siblings_(skip_siblings)
    , node_(NULL)
  {
    node_ = ostore_.find_prototype(equal_classname(typeid(T).name()));
    if (!node_) {
      // throw excpetion
    }
  }

  iterator begin() {
    if (skip_siblings_) {
      return ++iterator(node_, node_->op_first, node_->op_marker);
    } else {
      return ++iterator(node_, node_->op_first, node_->op_last);
    }
  }
  const_iterator begin() const {
    if (skip_siblings_) {
      return ++const_iterator(node_, node_->op_first, node_->op_marker);
    } else {
      return ++const_iterator(node_, node_->op_first, node_->op_last);
    }
  }
  iterator end() {
    if (skip_siblings_) {
      return iterator(node_, node_->op_marker, node_->op_marker);
    } else {
      return iterator(node_, node_->op_last, node_->op_last);
    }
  }
  const_iterator end() const {
    if (skip_siblings_) {
      return const_iterator(node_, node_->op_marker, node_->op_marker);
    } else {
      return const_iterator(node_, node_->op_last, node_->op_last);
    }
  }

  bool empty() const {
    return node_->op_first->next == node_->op_last;
  }
  void skip_siblings(bool skip) {
    skip_siblings_ = skip;
  }

private:
    object_store &ostore_;
    bool skip_siblings_;
    object_store::prototype_node *node_;
};

#endif /* OBJECTVIEW_HPP */