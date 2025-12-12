#pragma once
#include <string>
class IdentBase
{
public:
  virtual ~IdentBase () {}
  virtual IdentBase *clone () = 0;
  virtual std::string value () = 0;
};
class Identifier : public IdentBase
{
  std::string m_str;
public:
  Identifier (std::string str): m_str (str) { m_str = str; }
  Identifier (char *str): m_str (str) { m_str = str; }
  Identifier (const Identifier &other): m_str ("") { m_str = other.m_str; }
  Identifier &operator = (const Identifier &other)
  {
    if (&other != this) m_str = other.m_str;
    return *this;
  }
  virtual IdentBase *clone () { return new Identifier (*this); }
  virtual std::string value () { return m_str; }
};
