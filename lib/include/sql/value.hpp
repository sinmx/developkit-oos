/*
 * This file is part of OpenObjectStore OOS.
 *
 * OpenObjectStore OOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenObjectStore OOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY {} without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenObjectStore OOS. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef VALUE_HPP
#define VALUE_HPP

#include "sql/token.hpp"

#include "tools/varchar.hpp"
#include "tools/date.hpp"
#include "tools/time.hpp"
#include "tools/string.hpp"

#include <string>
#include <typeinfo>
#include <tools/serializer.hpp>
#include <cstring>

namespace oos {

template<class T, class Enabled = void>
struct value;

namespace detail {
/// @cond OOS_DEV

struct basic_value : public token
{
  basic_value(token::t_token tok) : token(tok) { }

  template < class T > T get() {
    oos::value<T> *v = dynamic_cast<oos::value<T>* >(this);
    if (v) {
      return v->val;
    } else {
      throw std::bad_cast();
    }
  }

  std::string value() const
  {
    return str();
  }

  virtual void serialize(const char *id, serializer &srlzr) = 0;

  virtual void accept(token_visitor &visitor);

  virtual std::string str() const = 0;
};

}

struct null_value : public detail::basic_value
{
  static std::string NULLSTR;

  null_value() : basic_value(detail::token::VALUE) { }

  virtual void serialize(const char *id, serializer &srlzr);

  std::string str() const;
};

template<class T>
struct value<T, typename std::enable_if<
  std::is_scalar<T>::value &&
  !std::is_same<char, T>::value &&
  !std::is_same<char*, T>::value>::type> : public detail::basic_value
{
  value(const T &val)
    : basic_value(detail::token::VALUE)
    , val(val)
  { }

  virtual void serialize(const char *id, serializer &srlzr)
  {
    srlzr.serialize(id, val);
  }

  std::string str() const
  {
    std::stringstream str;
    str << val;
    return str.str();
  }
  T val;
};


template<class T>
struct value<T, typename std::enable_if<
  std::is_same<std::string, T>::value ||
  std::is_base_of<oos::varchar_base, T>::value>::type> : public detail::basic_value
{
  value(const T &val)
    : basic_value(detail::token::VALUE)
    , val(val) { }

  virtual void serialize(const char *id, serializer &srlzr)
  {
    srlzr.serialize(id, val);
  }

  std::string str() const
  {
    std::stringstream str;
    str << "'" << val << "'";
    return str.str();
  }

  T val;
};

template<>
struct value<char> : public detail::basic_value
{
  value(char val)
    : basic_value(detail::token::VALUE)
    , val(val) { }

  virtual void serialize(const char *id, serializer &srlzr)
  {
    srlzr.serialize(id, val);
  }

  std::string str() const
  {
    std::stringstream str;
    str << "'" << val << "'";
    return str.str();
  }

  char val;
};

template<>
struct value<char*> : public detail::basic_value
{
  value(const char *v, size_t l)
    : basic_value(detail::token::VALUE)
    , val(v, v+l)
  {
    val.push_back('\0');
  }

  virtual void serialize(const char *id, serializer &srlzr)
  {

    char *begin = &val.front();
    srlzr.serialize(id, begin, val.size());
  }

  std::string str() const
  {
    std::stringstream str;
    str << "'" << &val.front() << "'";
    return str.str();
  }

  std::vector<char> val;
};

template<>
struct value<oos::date> : public detail::basic_value
{
  value(const oos::date &val)
    : basic_value(detail::token::VALUE)
    , val(val) { }

  virtual void serialize(const char *id, serializer &srlzr)
  {
    srlzr.serialize(id, val);
  }

  std::string str() const
  {
    std::stringstream str;
    str << "'" << oos::to_string(val) << "'";
    return str.str();
  }

  oos::date val;
};

template<>
struct value<oos::time> : public detail::basic_value
{
  value(const oos::time &val)
    : basic_value(detail::token::VALUE)
    , val(val)
  { }

  virtual void serialize(const char *id, serializer &srlzr)
  {
    srlzr.serialize(id, val);
  }

  std::string str() const
  {
    std::stringstream str;
    str << "'" << oos::to_string(val, "%F %T.%f") << "'";
    return str.str();
  }

  oos::time val;
};

template < class T >
detail::basic_value* make_value(const T &val)
{
  return new value<T>(val);
}

detail::basic_value* make_value(const char* val, size_t len);

/// @endcond

}

#endif /* VALUE_HPP */