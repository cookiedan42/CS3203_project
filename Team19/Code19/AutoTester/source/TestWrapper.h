#ifndef TESTWRAPPER_H
#define TESTWRAPPER_H

#include <string>
#include <iostream>
#include <list>
#include <unordered_set>

#include <fstream>

// include your other headers here
#include "AbstractWrapper.h"
#include "../../source/PKB/PKB.h"
#include "../../source/PKB/InsertFacade.h"
#include "../../source/PKB/GetFacade.h"
#include "../../source/QP/QueryManager.h"
#include "../../source/SourceProcessor/SPManager.h"

class TestWrapper : public AbstractWrapper {
 private:
	 PKB* pkb;
	 InsertFacade* insertFacade;
	 GetFacade* getFacade;
	 QueryManager* queryManager;
	 bool graceful_exit = false;
 public:
	 
  // default constructor
	 TestWrapper();
  
  // destructor
	 ~TestWrapper();
  
  // method for parsing the SIMPLE source
  virtual void parse(std::string filename);
  
  // method for evaluating a query
  virtual void evaluate(std::string query, std::list<std::string>& results);

};

#endif
