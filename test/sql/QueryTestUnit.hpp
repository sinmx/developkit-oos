//
// Created by sascha on 9/7/15.
//

#ifndef OOS_SQLTEST_HPP
#define OOS_SQLTEST_HPP


#include "unit/unit_test.hpp"

#include "sql/connection.hpp"

namespace oos {
class connection;
}

class QueryTestUnit : public oos::unit_test
{
public:
  QueryTestUnit(const std::string &name, const std::string &msg, const std::string &db = "memory");

  virtual void initialize();

  void test_datatypes();
  void test_describe();
  void test_identifier();
  void test_create();
  void test_statement_insert();
  void test_statement_update();
  void test_foreign_query();
  void test_query();
  void test_query_select();
  void test_query_select_columns();
  void test_select_limit();
  void test_update_limit();

protected:
  oos::connection* create_connection();

  std::string db() const;

private:
  std::string db_;
  std::unique_ptr<oos::connection> connection_;
};


#endif //OOS_SQLTEST_HPP