//
// Created by sascha on 17.06.16.
//

#ifndef OOS_SQLITE_DIALECT_COMPILER_HPP
#define OOS_SQLITE_DIALECT_COMPILER_HPP

#include "sql/dialect_token.hpp"

#include "sql/basic_dialect_compiler.hpp"

namespace oos {

namespace sqlite {

class sqlite_dialect;

class sqlite_dialect_compiler : public detail::basic_dialect_compiler
{
public:
  sqlite_dialect_compiler(sqlite_dialect &dialect);

  virtual void visit(const oos::detail::select &select1) override;
  virtual void visit(const oos::detail::update &update1) override;
  virtual void visit(const oos::detail::remove &remove1) override;
  virtual void visit(const oos::detail::tablename &table1) override;

  virtual void visit(const oos::detail::where &where1) override;
  virtual void visit(const oos::detail::top &top1) override;

protected:
  virtual void on_compile_start() override;

private:
  bool is_update = false;
  bool is_delete = false;

  std::string tablename_;
  token_list_t::iterator where_;

  sqlite_dialect &dialect_;
};

}
}

#endif //OOS_SQLITE_DIALECT_COMPILER_HPP