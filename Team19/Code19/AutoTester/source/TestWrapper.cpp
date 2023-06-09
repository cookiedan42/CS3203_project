#include "TestWrapper.h"


// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool TestWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program

	pkb = new PKB();
	insertFacade = new InsertFacade(*pkb);
	getFacade = new GetFacade(*pkb);
	queryManager = new QueryManager();
}


TestWrapper::~TestWrapper() {}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
	std::ifstream ifs(filename);
	std::string content((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	
	try {
		SPManager::construct(content).populatePKB(*insertFacade);
	} catch (const std::exception& exp) {
		graceful_exit = true;
	}
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results){
// call your evaluator to evaluate the query here
  // ...code to evaluate query...
	if (graceful_exit) {
		return;
	}
	results = queryManager->getQueryResponse(query, getFacade);

  // store the answers to the query in the results list (it is initially empty)
  // each result must be a string.
	

	
}
